// ═══════════════════════════════════════════════════════════════
//   ENARM.Domain - Sintoma
// ═══════════════════════════════════════════════════════════════
#pragma once

#include <string>
#include <chrono>
#include <vector>

namespace ENARM::Domain {

    enum class SymptomSeverity {
        Mild,
        Moderate,
        Severe,
        Extreme
    };

    enum class SymptomOnset {
        Sudden,
        Acute,
        Subacute,
        Chronic
    };

    struct Symptom {
        std::string name;
        std::string location;
        std::string quality;
        SymptomSeverity severity{SymptomSeverity::Moderate};
        SymptomOnset onset{SymptomOnset::Acute};
        std::chrono::minutes duration{0};
        std::vector<std::string> triggers;
        std::vector<std::string> reliefFactors;
        std::vector<std::string> associatedSymptoms;
        std::vector<std::string> radiation;
        std::string patientDescription;
    };

}