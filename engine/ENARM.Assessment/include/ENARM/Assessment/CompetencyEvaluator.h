#pragma once
#include <enarm/assessment/RubricDefinition.h>
#include <string>
#include <vector>

namespace enarm::assessment {

// Datos de accion del estudiante que llegan al evaluador
struct StudentAction {
    enum class Type {
        ConversationTurn,   // Mensaje en el chat
        PhysicalExam,       // Exploración solicitada
        StudyOrdered,       // Estudio paraclínico
        DiagnosisSubmitted, // Diagnóstico enviado
        TreatmentOrdered,   // Medicamento/intervención
        FollowUpAction      // Acción de seguimiento
    };
    Type        type;
    std::string primary;    // Texto principal (pregunta, examen, etc.)
    std::string secondary;  // Hallazgo / resultado / justificación
    float       timestampSeconds = 0.0f;
};

// ----------------------------------------------------------------
// CompetencyEvaluator: evalua cada dimension de la rubrica
// ----------------------------------------------------------------
class CompetencyEvaluator {
public:
    // Evalua cada dimension con las acciones acumuladas
    DimensionScore evaluateDimension(
        const DimensionRubric&           rubric,
        const std::vector<StudentAction>& actions,
        float                            timeSpentSeconds) const;

    // Evalua criterio individual
    CriterionScore evaluateCriterion(
        const RubricCriterion&           criterion,
        const std::vector<StudentAction>& actions) const;

    // Matching semantico (directo + sinonimos)
    bool containsSemanticMatch(
        const std::string& text,
        const std::string& conceptName,
        const std::vector<std::string>& synonyms = {}) const;

    // Nivel de desempeno segun porcentaje
    static std::string getPerformanceLevel(float percentage);

private:
    // Normaliza texto: minusculas, sin tildes, sin puntuacion extra
    std::string normalize(const std::string& text) const;

    // Extrae todo el texto relevante de las acciones segun dimension
    std::vector<std::string> extractTextsForDimension(
        const std::vector<StudentAction>& actions,
        CompetencyDimension dimension) const;

    // Tabla de sinonimos medicos comunes
    static const std::unordered_map<std::string, std::vector<std::string>>& getSynonymTable();

    std::string generateCriterionJustification(
        const RubricCriterion&            criterion,
        float                             coverage,
        const std::vector<std::string>&   found,
        const std::vector<std::string>&   missed) const;
};

} // namespace enarm::assessment
