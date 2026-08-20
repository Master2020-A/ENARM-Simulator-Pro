// ═══════════════════════════════════════════════════════════════
//   ENARM.Simulation - SimulationRoom (Sprint 18)
//   Sala de simulacion multi-estudiante con roles
//   Rol 1: Estudiante principal (interroga y trata)
//   Rol 2: Enfermero/a (adminstra medicamentos)
//   Rol 3: Observador (evalua sin interactuar)
// ═══════════════════════════════════════════════════════════════
#pragma once

#include "ENARM/Simulation/PatientSimulator.h"
#include "ENARM/Simulation/EmergencyCode.h"

#include <string>
#include <vector>
#include <unordered_map>

namespace ENARM::Simulation {

    enum class RoomRole : uint8_t {
        LeadStudent,    // Interroga y decide tratamiento
        Nurse,          // Administra medicamentos
        Observer        // Solo observa y evalua
    };

    struct RoomMember {
        std::string memberId;
        std::string displayName;
        RoomRole role{RoomRole::Observer};
        bool connected{false};
        int contributions{0};   // Acciones realizadas
        int correctActions{0};
    };

    // Evento de la sala (visible para todos)
    struct RoomEvent {
        std::string memberId;
        std::string message;
        double simTimeSeconds{0.0};
        bool critical{false};
    };

    class SimulationRoom {
    public:
        SimulationRoom();

        // ─── Gestion de miembros ───
        void AddMember(const std::string& memberId,
                       const std::string& displayName,
                       RoomRole role);
        void RemoveMember(const std::string& memberId);
        [[nodiscard]] const std::vector<RoomMember>& Members() const noexcept { return m_members; }
        [[nodiscard]] size_t MemberCount() const noexcept { return m_members.size(); }

        // ─── Sesion compartida ───
        void StartScenario(ScenarioType type, const std::string& caseTitle = "");
        void Update(double dt_s);
        void Step(double dt_s);

        // Acciones de los miembros (se registran con quien la hizo)
        bool MemberGiveBolus(const std::string& memberId,
                             const std::string& drugName, double dose_mg);
        bool MemberStartInfusion(const std::string& memberId,
                                 const std::string& drugName, double rate);
        bool MemberStopInfusion(const std::string& memberId,
                                const std::string& drugName);

        // ─── Consultas ───
        [[nodiscard]] const PatientSimulator& GetPatient() const noexcept { return m_patient; }
        [[nodiscard]] PatientSimulator& GetPatient() noexcept { return m_patient; }
        [[nodiscard]] const std::vector<RoomEvent>& GetEvents() const noexcept { return m_events; }
        [[nodiscard]] const std::string& GetCaseTitle() const noexcept { return m_caseTitle; }
        [[nodiscard]] bool IsActive() const noexcept { return m_active; }

    private:
        void LogEvent(const std::string& memberId, const std::string& msg, bool critical = false);
        [[nodiscard]] bool IsMemberPresent(const std::string& memberId) const;
        void RecordContribution(const std::string& memberId, bool correct);

        PatientSimulator m_patient;
        std::vector<RoomMember> m_members;
        std::vector<RoomEvent> m_events;
        std::string m_caseTitle;
        bool m_active{false};
    };

}