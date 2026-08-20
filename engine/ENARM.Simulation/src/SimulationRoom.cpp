// ═══════════════════════════════════════════════════════════════
//   ENARM.Simulation - SimulationRoom Implementation
// ═══════════════════════════════════════════════════════════════
#include "ENARM/Simulation/SimulationRoom.h"

#include <algorithm>

namespace ENARM::Simulation {

    SimulationRoom::SimulationRoom() = default;

    void SimulationRoom::AddMember(const std::string& memberId,
                                   const std::string& displayName,
                                   RoomRole role) {
        for (auto& m : m_members) {
            if (m.memberId == memberId) {
                m.connected = true;
                return;
            }
        }
        RoomMember m;
        m.memberId = memberId;
        m.displayName = displayName;
        m.role = role;
        m.connected = true;
        m_members.push_back(std::move(m));
        LogEvent(memberId, displayName + " se unio a la sala");
    }

    void SimulationRoom::RemoveMember(const std::string& memberId) {
        auto it = std::remove_if(m_members.begin(), m_members.end(),
            [&](const RoomMember& m) { return m.memberId == memberId; });
        if (it != m_members.end()) {
            LogEvent(memberId, "Miembro abandono la sala");
            m_members.erase(it, m_members.end());
        }
    }

    bool SimulationRoom::IsMemberPresent(const std::string& memberId) const {
        return std::any_of(m_members.begin(), m_members.end(),
            [&](const RoomMember& m) { return m.memberId == memberId && m.connected; });
    }

    void SimulationRoom::RecordContribution(const std::string& memberId, bool correct) {
        for (auto& m : m_members) {
            if (m.memberId == memberId) {
                ++m.contributions;
                if (correct) ++m.correctActions;
                return;
            }
        }
    }

    void SimulationRoom::LogEvent(const std::string& memberId,
                                  const std::string& msg, bool critical) {
        RoomEvent e;
        e.memberId = memberId;
        e.message = msg;
        e.simTimeSeconds = m_patient.GetSimTime();
        e.critical = critical;
        m_events.push_back(std::move(e));
        if (m_events.size() > 500) m_events.erase(m_events.begin());
    }

    void SimulationRoom::StartScenario(ScenarioType type, const std::string& caseTitle) {
        m_caseTitle = caseTitle.empty() ? "Sesion clinica" : caseTitle;
        m_patient = PatientSimulator(70.0);
        m_patient.ApplyScenario(type);
        m_events.clear();
        m_active = true;
        LogEvent("", "Escenario iniciado: " + m_caseTitle);
    }

    void SimulationRoom::Update(double dt_s) {
        if (!m_active) return;
        m_patient.Simulate(dt_s);
    }

    void SimulationRoom::Step(double dt_s) {
        if (!m_active) return;
        m_patient.Step(dt_s);
    }

    bool SimulationRoom::MemberGiveBolus(const std::string& memberId,
                                         const std::string& drugName, double dose_mg) {
        if (!IsMemberPresent(memberId) || !m_active) return false;
        const bool ok = m_patient.GiveBolus(drugName, dose_mg);
        RecordContribution(memberId, ok);
        if (ok) {
            LogEvent(memberId, "Administro bolo de " + drugName + " " +
                               std::to_string(dose_mg) + " mg");
        } else {
            LogEvent(memberId, "INTENTO FALLIDO: bolo de " + drugName, true);
        }
        return ok;
    }

    bool SimulationRoom::MemberStartInfusion(const std::string& memberId,
                                             const std::string& drugName, double rate) {
        if (!IsMemberPresent(memberId) || !m_active) return false;
        const bool ok = m_patient.StartInfusion(drugName, rate);
        RecordContribution(memberId, ok);
        if (ok) {
            LogEvent(memberId, "Inicio infusion de " + drugName + " a " +
                               std::to_string(rate) + " mcg/kg/min");
        }
        return ok;
    }

    bool SimulationRoom::MemberStopInfusion(const std::string& memberId,
                                            const std::string& drugName) {
        if (!IsMemberPresent(memberId) || !m_active) return false;
        const bool ok = m_patient.StopInfusion(drugName);
        RecordContribution(memberId, ok);
        if (ok) LogEvent(memberId, "Detuvo infusion de " + drugName);
        return ok;
    }

}