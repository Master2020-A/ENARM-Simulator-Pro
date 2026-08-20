// ═══════════════════════════════════════════════════════════════
//   ENARM.Simulation - PatientSimulator Implementation
// ═══════════════════════════════════════════════════════════════
#include "ENARM/Simulation/PatientSimulator.h"
#include "ENARM/Simulation/PhysiologyPharmacologyBridge.h"
#include "ENARM/Physiology/Constants.h"

#include <sstream>
#include <iomanip>
#include <cmath>

namespace ENARM::Simulation {

    PatientSimulator::PatientSimulator(double weight_kg)
        : m_weight_kg(weight_kg)
        , m_drugs(weight_kg)
    {
        m_events.Add(0.0, EventType::Info, "system",
                     "PatientSimulator inicializado (peso: " +
                     std::to_string((int)weight_kg) + " kg)");
    }

    void PatientSimulator::Step(double dt_s) {
        // Actualizar farmacos (PK)
        m_drugs.Update(dt_s);

        // Aplicar efectos farmacologicos a la fisiologia
        PhysiologyPharmacologyBridge::ApplyPharmacologicalEffects(
            m_heart, m_lungs, m_drugs);

        // Actualizar fisiologia
        m_heart.Update(dt_s);
        m_lungs.Update(dt_s);

        m_simTime_s += dt_s;

        // Detectar cambios clinicos cada 5s
        static double lastDetection = 0.0;
        if (m_simTime_s - lastDetection >= 5.0) {
            DetectAndLogClinicalChanges();
            lastDetection = m_simTime_s;
        }
    }

    void PatientSimulator::Simulate(double seconds) {
        const double dt = Physiology::Constants::kDefaultDt;
        const int steps = (int)(seconds / dt);
        for (int i = 0; i < steps; ++i) Step(dt);
    }

    void PatientSimulator::Simulate(std::chrono::seconds d) {
        Simulate((double)d.count());
    }

    void PatientSimulator::Simulate(std::chrono::minutes d) {
        Simulate((double)d.count() * 60.0);
    }

    void PatientSimulator::ApplyScenario(ScenarioType type) {
        ClinicalScenarios::Apply(*this, type);
        m_events.Add(m_simTime_s, EventType::Scenario, "scenario",
                     "Escenario aplicado: " + ClinicalScenarios::GetName(type), 2);
    }

    bool PatientSimulator::GiveBolus(const std::string& drug, double dose_mg) {
        bool ok = m_drugs.GiveBolus(drug, dose_mg);
        if (ok) {
            std::ostringstream oss;
            oss << "Bolo IV: " << drug << " " << dose_mg << " mg";
            m_events.Add(m_simTime_s, EventType::Medication, "drug", oss.str(), 1);
        }
        return ok;
    }

    bool PatientSimulator::StartInfusion(const std::string& drug, double rate) {
        bool ok = m_drugs.StartInfusion(drug, rate);
        if (ok) {
            std::ostringstream oss;
            oss << "Infusion iniciada: " << drug << " " << rate << " mcg/kg/min";
            m_events.Add(m_simTime_s, EventType::Medication, "drug", oss.str(), 1);
        }
        return ok;
    }

    bool PatientSimulator::StopInfusion(const std::string& drug) {
        bool ok = m_drugs.StopInfusion(drug);
        if (ok) {
            m_events.Add(m_simTime_s, EventType::Medication, "drug",
                         "Infusion detenida: " + drug, 1);
        }
        return ok;
    }

    PatientSnapshot PatientSimulator::GetSnapshot() const {
        PatientSnapshot s;
        const auto& hs = m_heart.GetState();
        const auto& rs = m_lungs.GetState();

        s.systolicBP    = hs.systolicBP_mmHg;
        s.diastolicBP   = hs.diastolicBP_mmHg;
        s.MAP           = hs.meanArterialP_mmHg;
        s.heartRate     = m_heart.GetParameters().heartRate_bpm;
        s.cardiacOutput = hs.cardiacOutput_Lmin;

        s.respRate      = m_lungs.GetParameters().respRate_rpm;
        s.SpO2          = rs.SpO2_pct;
        s.PaO2          = rs.PaO2_mmHg;
        s.PaCO2         = rs.PaCO2_mmHg;
        s.etCO2         = rs.etCO2_mmHg;

        s.simTime_s     = m_simTime_s;
        s.activeDrugsCount = (int)m_drugs.GetActiveDrugs().size();
        return s;
    }

    std::string PatientSimulator::GetVitalsString() const {
        auto s = GetSnapshot();
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(0);
        oss << "PA:" << s.systolicBP << "/" << s.diastolicBP
            << " MAP:" << s.MAP
            << " FC:" << s.heartRate
            << " FR:" << s.respRate
            << " SpO2:" << std::setprecision(1) << s.SpO2 << "%"
            << " EtCO2:" << std::setprecision(0) << s.etCO2;
        return oss.str();
    }

    std::string PatientSimulator::GetFullStatusReport() const {
        auto s = GetSnapshot();
        std::ostringstream oss;
        oss << std::fixed;

        oss << "\n================ ESTADO DEL PACIENTE ================\n";
        oss << " Tiempo simulacion: " << FormatSimTime(s.simTime_s) << "\n";
        oss << " Peso: " << (int)m_weight_kg << " kg\n";
        oss << "-----------------------------------------------------\n";
        oss << " HEMODINAMICA:\n";
        oss << "   PA:  " << std::setprecision(0) << s.systolicBP << "/" << s.diastolicBP << " mmHg\n";
        oss << "   MAP: " << s.MAP << " mmHg\n";
        oss << "   FC:  " << s.heartRate << " lpm\n";
        oss << "   GC:  " << std::setprecision(2) << s.cardiacOutput << " L/min\n";
        oss << "-----------------------------------------------------\n";
        oss << " RESPIRATORIO:\n";
        oss << "   FR:    " << std::setprecision(0) << s.respRate << " rpm\n";
        oss << "   SpO2:  " << std::setprecision(1) << s.SpO2 << "%\n";
        oss << "   PaO2:  " << std::setprecision(0) << s.PaO2 << " mmHg\n";
        oss << "   PaCO2: " << s.PaCO2 << " mmHg\n";
        oss << "   EtCO2: " << s.etCO2 << " mmHg\n";
        oss << "-----------------------------------------------------\n";
        oss << " FARMACOS ACTIVOS: " << s.activeDrugsCount << "\n";
        for (const auto& d : m_drugs.GetActiveDrugs()) {
            oss << "   * " << d << "\n";
        }
        oss << "=====================================================\n";
        return oss.str();
    }

    void PatientSimulator::DetectAndLogClinicalChanges() {
        auto s = GetSnapshot();

        // MAP objetivo (>=65)
        if (s.MAP >= 65.0 && !m_mapObjectiveReached) {
            m_mapObjectiveReached = true;
            m_events.Add(m_simTime_s, EventType::Improvement, "hemodynamic",
                         "MAP objetivo alcanzado (>=65 mmHg)", 1);
        }
        if (s.MAP < 65.0 && m_mapObjectiveReached) {
            m_mapObjectiveReached = false;
            m_events.Add(m_simTime_s, EventType::Deterioration, "hemodynamic",
                         "MAP cayo <65 mmHg", 2);
        }

        // Hipoxemia significativa
        if (s.SpO2 < 90.0 && m_lastSpO2 >= 90.0) {
            m_events.Add(m_simTime_s, EventType::Alarm, "respiratory",
                         "SpO2 <90% (hipoxemia significativa)", 2);
        }
        if (s.SpO2 >= 95.0 && m_lastSpO2 < 90.0) {
            m_events.Add(m_simTime_s, EventType::Improvement, "respiratory",
                         "SpO2 recuperada >=95%", 1);
        }

        // Hipotension critica
        if (s.systolicBP < 80.0 && m_lastMAP >= 80.0) {
            m_events.Add(m_simTime_s, EventType::Alarm, "hemodynamic",
                         "PA sistolica <80 mmHg", 3);
        }

        // Taquicardia extrema
        if (s.heartRate > 150.0) {
            m_events.Add(m_simTime_s, EventType::Alarm, "hemodynamic",
                         "Taquicardia extrema >150 lpm", 2);
        }

        m_lastMAP = s.MAP;
        m_lastSpO2 = s.SpO2;
    }

}