#pragma once
#include <enarm/assessment/RubricDefinition.h>
#include <enarm/assessment/CompetencyEvaluator.h>
#include <enarm/assessment/ScoreCalculator.h>
#include <string>
#include <vector>
#include <chrono>
#include <functional>

namespace enarm::assessment {

// ----------------------------------------------------------------
// AssessmentEngine: orquestador principal
// Uso:
//   engine.loadRubricForCase("case_001_iam");
//   engine.onConversationTurn(studentMsg, patientResponse, elapsed);
//   engine.onStudyOrdered("ECG", "RS con BCRDHH");
//   engine.onDiagnosisSubmitted("IAM CEST", "por ECG y troponinas");
//   auto report = engine.generateReport();
// ----------------------------------------------------------------
class AssessmentEngine {
public:
    AssessmentEngine();

    // ---- Configuracion ----
    void loadRubric(const CaseRubric& rubric);
    void loadRubricForCase(const std::string& caseId);
    void reset();

    // ---- Tracking en tiempo real ----
    void onConversationTurn(const std::string& studentMessage,
                            const std::string& patientResponse,
                            float elapsedSeconds);

    void onPhysicalExamRequested(const std::string& examName,
                                  const std::string& findings,
                                  float elapsedSeconds = -1.0f);

    void onStudyOrdered(const std::string& studyName,
                        const std::string& results,
                        float elapsedSeconds = -1.0f);

    void onDiagnosisSubmitted(const std::string& diagnosis,
                               const std::string& justification,
                               float elapsedSeconds = -1.0f);

    void onTreatmentOrdered(const std::string& treatment,
                             const std::string& dosage,
                             const std::string& rationale,
                             float elapsedSeconds = -1.0f);

    void onFollowUpAction(const std::string& action,
                           const std::string& rationale,
                           float elapsedSeconds = -1.0f);

    // ---- Score en tiempo real ----
    float getCurrentScorePercent() const;
    int   getCurrentScorePoints()  const;
    std::vector<DimensionScore> getCurrentDimensionScores() const;

    // ---- Reporte final ----
    PerformanceReport generateReport(const std::string& sessionId   = "",
                                     const std::string& studentId   = "",
                                     float totalTimeSeconds = 0.0f);

    // ---- Rubricas predefinidas (10 casos ENARM) ----
    // Sprint 7A original (3 casos)
    static CaseRubric createIAMRubric();          // case_001
    static CaseRubric createSepsisRubric();       // case_002
    static CaseRubric createACVRubric();          // case_003

    // Sprint 7A.2 (7 casos nuevos)
    static CaseRubric createNeumoniaRubric();     // case_004 - Neumologia
    static CaseRubric createPreeclampsiaRubric(); // case_005 - Gineco-Obstetricia
    static CaseRubric createApendicitisRubric();  // case_006 - Cirugia Pediatrica
    static CaseRubric createTCERubric();          // case_007 - Urgencias / Trauma
    static CaseRubric createCADRubric();          // case_008 - Endocrinologia Pediatrica
    static CaseRubric createLRARubric();          // case_009 - Nefrologia
    static CaseRubric createBronquiolitisRubric(); // case_010 - Pediatria

private:
    CaseRubric              m_rubric;
    CompetencyEvaluator     m_evaluator;
    std::vector<StudentAction> m_actions;
    float                   m_lastElapsed = 0.0f;
    bool                    m_rubricLoaded = false;

    std::unordered_map<CompetencyDimension, float> m_dimStartTime;
    std::unordered_map<CompetencyDimension, float> m_dimEndTime;

    void recordAction(StudentAction action);
    float timeForDimension(CompetencyDimension d) const;
};

} // namespace enarm::assessment