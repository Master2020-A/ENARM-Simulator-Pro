// ═══════════════════════════════════════════════════════════════
//   ENARM.Simulation - DiseaseProgressionEngine
//   Evolucion temporal del paciente: empeora si no se trata,
//   mejora si recibe tratamiento adecuado
// ═══════════════════════════════════════════════════════════════
#pragma once

#include "ENARM/Physiology/HemodynamicModel.h"
#include "ENARM/Simulation/ClinicalScenarios.h"

#include <string>
#include <vector>

namespace ENARM::Simulation {

    // Estado de progresion de la enfermedad
    enum class ProgressionPhase : uint8_t {
        Stable,          // Fisiologia normal
        Compensated,     // Mecanismos compensatorios activos
        Decompensating,  // Deterioro progresivo
        Critical         // Riesgo inminente de paro
    };

    // Objetivos terapeuticos por escenario
    struct TreatmentTargets {
        double mapMin{65.0};        // MAP minimo aceptable
        double spO2Min{90.0};       // SpO2 minima
        double hrMax{130.0};        // FC maxima
        double respRateMax{35.0};   // FR maxima
        bool   needsAntibiotics{false};
        bool   needsFluids{false};
        bool   needsVasopressors{false};
        bool   needsReperfusion{false};
        std::string diseaseName;
    };

    class DiseaseProgressionEngine {
    public:
        DiseaseProgressionEngine();

        // Configurar escenario de base
        void Configure(ScenarioType type);

        // Evaluar el estado actual del paciente y actualizar la fase
        void Update(double dt_s,
                    const Physiology::HemodynamicModel& heart,
                    bool fluidResuscitationGiven,
                    bool vasopressorsActive,
                    bool antibioticsGiven,
                    bool reperfusionGiven);

        // Consultas
        [[nodiscard]] ProgressionPhase GetPhase() const noexcept { return m_phase; }
        [[nodiscard]] double GetDeteriorationScore() const noexcept { return m_deterioration; }
        [[nodiscard]] double GetImprovementScore() const noexcept { return m_improvement; }
        [[nodiscard]] const TreatmentTargets& GetTargets() const noexcept { return m_targets; }
        [[nodiscard]] std::string PhaseToString() const;

        // Diagnostico rapido de tratamiento faltante
        [[nodiscard]] std::vector<std::string> MissingTreatments() const;

    private:
        void UpdatePhase();

        ScenarioType m_type{ScenarioType::HealthyBaseline};
        TreatmentTargets m_targets;
        ProgressionPhase m_phase{ProgressionPhase::Stable};
        double m_deterioration{0.0};
        double m_improvement{0.0};
        double m_lastMap{93.0};
        double m_lastSpO2{98.0};
    };

}