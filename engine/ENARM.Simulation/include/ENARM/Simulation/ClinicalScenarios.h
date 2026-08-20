// ═══════════════════════════════════════════════════════════════
//   ENARM.Simulation - Escenarios clinicos preprogramados
// ═══════════════════════════════════════════════════════════════
#pragma once

#include <string>
#include <vector>

namespace ENARM::Simulation {

    class PatientSimulator;   // forward decl

    enum class ScenarioType : uint8_t {
        HealthyBaseline,          // Adulto sano
        SepticShock,              // Shock septico
        CardiogenicShock,         // Shock cardiogenico
        HypovolemicShock,         // Shock hipovolemico
        AcuteMI_STEMI,            // IAM ST elevado
        AcutePulmonaryEdema,      // Edema pulmonar
        SevereAsthma,             // Crisis asmatica
        SevereARDS,               // SDRA severo
        COPDExacerbation,         // EPOC reagudizado
        MassivePE,                // TEP masivo
        DKA,                      // Cetoacidosis diabetica
        HypoglycemicComa,         // Coma hipoglucemico
        OpioidOverdose,           // Sobredosis opioides
        CardiacArrest_VF,         // Paro FV
        SevereTBI                 // Trauma craneal
    };

    struct ScenarioInfo {
        ScenarioType type;
        std::string name;
        std::string description;
        std::string expectedPresentation;
        std::vector<std::string> keyDiagnostics;
        std::vector<std::string> firstLineTreatment;
        std::string difficultyLevel;
    };

    class ClinicalScenarios {
    public:
        // Aplica un escenario al simulador
        static void Apply(PatientSimulator& patient, ScenarioType type);

        // Informacion del escenario (para UI/educacion)
        static ScenarioInfo GetInfo(ScenarioType type);

        // Lista todos los escenarios disponibles
        static std::vector<ScenarioInfo> ListAll();

        // Nombre legible
        static std::string GetName(ScenarioType type);
    };

}