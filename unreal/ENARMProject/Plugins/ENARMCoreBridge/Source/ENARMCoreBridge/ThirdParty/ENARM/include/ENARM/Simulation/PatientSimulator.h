// ═══════════════════════════════════════════════════════════════
//   ENARM.Simulation - PatientSimulator (orquestador)
// ═══════════════════════════════════════════════════════════════
#pragma once

#include "ENARM/Physiology/HemodynamicModel.h"
#include "ENARM/Physiology/RespiratoryModel.h"
#include "ENARM/Pharmacology/DrugAdministration.h"
#include "ENARM/Simulation/EventLog.h"
#include "ENARM/Simulation/ClinicalScenarios.h"

#include <chrono>
#include <string>

namespace ENARM::Simulation {

    // Snapshot completo del paciente en un momento dado
    struct PatientSnapshot {
        // Hemodinamica
        double systolicBP{0.0};
        double diastolicBP{0.0};
        double MAP{0.0};
        double heartRate{0.0};
        double cardiacOutput{0.0};

        // Respiratorio
        double respRate{0.0};
        double SpO2{0.0};
        double PaO2{0.0};
        double PaCO2{0.0};
        double etCO2{0.0};

        // Meta
        double simTime_s{0.0};
        int activeDrugsCount{0};
    };

    // ═══════════════════════════════════════════════════════════════
    class PatientSimulator {
    public:
        explicit PatientSimulator(double weight_kg = 70.0);

        // ─── Simulacion ───

        // Avanza la simulacion N segundos (usando dt interno)
        void Simulate(double seconds);

        // Alternativa con chrono
        void Simulate(std::chrono::seconds duration);
        void Simulate(std::chrono::minutes duration);

        // Un solo tick (para control fino)
        void Step(double dt_s);

        // ─── Escenarios ───
        void ApplyScenario(ScenarioType type);

        // ─── Farmacos ───
        bool GiveBolus(const std::string& drugName, double dose_mg);
        bool StartInfusion(const std::string& drugName, double rate_mcg_kg_min);
        bool StopInfusion(const std::string& drugName);

        // ─── Consultas ───
        [[nodiscard]] PatientSnapshot GetSnapshot() const;
        [[nodiscard]] std::string GetVitalsString() const;
        [[nodiscard]] std::string GetFullStatusReport() const;
        [[nodiscard]] double GetSimTime() const noexcept { return m_simTime_s; }

        // ─── Acceso a componentes ───
        Physiology::HemodynamicModel& GetHeart() noexcept { return m_heart; }
        Physiology::RespiratoryModel& GetLungs() noexcept { return m_lungs; }
        Pharmacology::DrugAdministration& GetDrugs() noexcept { return m_drugs; }
        EventLog& GetEventLog() noexcept { return m_events; }

        [[nodiscard]] const EventLog& GetEventLog() const noexcept { return m_events; }

    private:
        // Detecta cambios significativos y los registra
        void DetectAndLogClinicalChanges();

        double m_weight_kg;
        double m_simTime_s{0.0};

        Physiology::HemodynamicModel m_heart;
        Physiology::RespiratoryModel m_lungs;
        Pharmacology::DrugAdministration m_drugs;
        EventLog m_events;

        // Ultimos valores para detectar cambios
        double m_lastMAP{93.0};
        double m_lastSpO2{98.0};
        bool m_mapObjectiveReached{false};
    };

}