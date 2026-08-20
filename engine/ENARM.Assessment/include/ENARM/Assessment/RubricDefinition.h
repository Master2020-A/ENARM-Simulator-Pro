#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <nlohmann/json.hpp>

namespace enarm::assessment {

// ----------------------------------------------------------------
// Dimensiones de competencia ENARM (6 categorias, 185 pts total)
// ----------------------------------------------------------------
enum class CompetencyDimension {
    ClinicalInterview     = 0,   // Interrogatorio     40 pts
    PhysicalExamination   = 1,   // Exploracion fisica 25 pts
    DiagnosticStudies     = 2,   // Paraclínicos       30 pts
    ClinicalDiagnosis     = 3,   // Diagnostico        30 pts
    TreatmentPlan         = 4,   // Tratamiento        40 pts
    FollowUpMonitoring    = 5    // Seguimiento        20 pts
};

inline std::string dimensionToString(CompetencyDimension d) {
    switch (d) {
        case CompetencyDimension::ClinicalInterview:   return "Interrogatorio Clinico";
        case CompetencyDimension::PhysicalExamination: return "Exploracion Fisica";
        case CompetencyDimension::DiagnosticStudies:   return "Estudios Paraclinicos";
        case CompetencyDimension::ClinicalDiagnosis:   return "Diagnostico Clinico";
        case CompetencyDimension::TreatmentPlan:       return "Plan Terapeutico";
        case CompetencyDimension::FollowUpMonitoring:  return "Seguimiento";
        default:                                        return "Desconocido";
    }
}

inline std::string dimensionShortName(CompetencyDimension d) {
    switch (d) {
        case CompetencyDimension::ClinicalInterview:   return "Interrogat.";
        case CompetencyDimension::PhysicalExamination: return "Exploracion";
        case CompetencyDimension::DiagnosticStudies:   return "Estudios";
        case CompetencyDimension::ClinicalDiagnosis:   return "Diagnostico";
        case CompetencyDimension::TreatmentPlan:       return "Tratamiento";
        case CompetencyDimension::FollowUpMonitoring:  return "Seguimiento";
        default:                                        return "N/A";
    }
}

// ----------------------------------------------------------------
// Criterio individual dentro de una dimension
// ----------------------------------------------------------------
struct RubricCriterion {
    std::string id;
    std::string description;
    CompetencyDimension dimension = CompetencyDimension::ClinicalInterview;
    int maxPoints = 0;

    std::string excellentDescription;     // 90-100 %
    std::string goodDescription;          // 70-89 %
    std::string adequateDescription;      // 50-69 %
    std::string insufficientDescription;  // < 50 %

    // Palabras/frases clave que el estudiante debe mencionar
    std::vector<std::string> keyElements;
    // Sinonimos aceptables por elemento (indice paralelo a keyElements)
    std::vector<std::vector<std::string>> synonymGroups;

    float weight = 1.0f;  // Peso relativo dentro de la dimension
};

// ----------------------------------------------------------------
// Rubrica de una dimension completa
// ----------------------------------------------------------------
struct DimensionRubric {
    CompetencyDimension dimension = CompetencyDimension::ClinicalInterview;
    std::string name;
    std::string description;
    int maxPoints = 0;
    std::vector<RubricCriterion> criteria;
};

// ----------------------------------------------------------------
// Rubrica completa de un caso clinico
// ----------------------------------------------------------------
struct CaseRubric {
    std::string caseId;
    std::string caseName;
    int totalMaxPoints = 185;
    std::vector<DimensionRubric> dimensions;

    // Tiempo esperado por fase (minutos)
    std::unordered_map<CompetencyDimension, float> expectedTimeMinutes;

    // Helpers
    const DimensionRubric* getDimension(CompetencyDimension d) const {
        for (const auto& dim : dimensions)
            if (dim.dimension == d) return &dim;
        return nullptr;
    }

    int maxPointsForDimension(CompetencyDimension d) const {
        const auto* dim = getDimension(d);
        return dim ? dim->maxPoints : 0;
    }
};

// ----------------------------------------------------------------
// Resultados de evaluacion
// ----------------------------------------------------------------
struct CriterionScore {
    std::string criterionId;
    int pointsEarned = 0;
    int maxPoints    = 0;
    float percentage = 0.0f;
    std::string justification;
    std::vector<std::string> elementsIdentified;
    std::vector<std::string> elementsMissed;

    nlohmann::json toJson() const;
};

struct DimensionScore {
    CompetencyDimension dimension = CompetencyDimension::ClinicalInterview;
    int pointsEarned   = 0;
    int maxPoints      = 0;
    float percentage   = 0.0f;
    std::string performanceLevel;   // Excelente / Bueno / Adecuado / Insuficiente
    std::vector<CriterionScore> criterionScores;
    float timeSpentMinutes   = 0.0f;
    float expectedTimeMinutes= 0.0f;

    nlohmann::json toJson() const;
};

struct PerformanceReport {
    std::string sessionId;
    std::string caseId;
    std::string studentId;

    std::vector<DimensionScore> dimensionScores;

    int   totalPointsEarned  = 0;
    int   totalMaxPoints     = 185;
    float globalPercentage   = 0.0f;
    std::string globalPerformanceLevel;

    float totalTimeMinutes = 0.0f;
    bool  timeEfficient    = false;

    std::vector<std::string> strengths;
    std::vector<std::string> weaknesses;
    std::vector<std::string> recommendations;
    std::string narrativeFeedback;

    float percentileRank = -1.0f;
    std::string completedAt;

    nlohmann::json toJson() const;
    static PerformanceReport fromJson(const nlohmann::json& j);
};

} // namespace enarm::assessment
