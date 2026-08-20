#include "ENARM/GUI/AppContext.h"
#include "ENARM/GUI/CaseFindingsProvider.h"
#include "ENARM/AI/PromptTemplates/PatientPrompts.h"
#include <nlohmann/json.hpp>

namespace ENARM::GUI {

// Sprint 7B Fase 0: Student por defecto (DEMO-001)
// Corresponde al registro real en tabla 'students'.
static constexpr const char* DEFAULT_STUDENT_ID = "654e1196-e438-463c-bb6f-d4e0535159d7";

// ================================================================
// StartSession
// ================================================================
void AppContext::StartSession(const Data::ClinicalCaseRecord& c) {
    activeCase = c;

    chatMessages.clear();
    chatInputBuffer[0] = '\0';
    waitingForResponse = false;

    chatScrollToBottom = false;
    aiProcessing       = false;
    pendingAIResponse.clear();
    pendingAILatencyMs = 0;

    requestedExams.clear();
    examFindings.clear();
    requestedStudies.clear();
    studyResults.clear();
    submittedDiagnosis.clear();
    diagnosisJustification.clear();
    differentials.clear();
    treatments.clear();
    followUpActions.clear();

    interviewPoints = examPoints = studiesPoints = 0;
    diagnosisPoints = treatmentPoints = followUpPoints = 0;

    sessionElapsedSeconds = 0.0f;
    sessionActive         = true;
    showReportModal       = false;
    lastReport.reset();

    assessmentEngine = std::make_unique<enarm::assessment::AssessmentEngine>();
    assessmentEngine->loadRubricForCase(c.id);

    patient.reset();
    if (router) {
        AI::PromptTemplates::PatientPersonaData persona;
        persona.fullName = c.patient.value("full_name", "Paciente");
        persona.age = c.patient.value("age", 0);
        persona.sex = c.patient.value("sex", "Unknown");
        persona.chiefComplaint = c.chiefComplaint;
        persona.historyPresentIllness = c.historyPresentIllness;
        persona.symptoms = c.symptoms;
        persona.medicalHistory = c.patient.value("medical_history", nlohmann::json::object());
        persona.vitalSigns = c.patient.value("vital_signs", nlohmann::json::object());
        persona.familyHistory = c.patient.value("family_history", nlohmann::json::object());
        persona.lifestyle = c.patient.value("lifestyle", nlohmann::json::object());
        patient = std::make_unique<AI::PatientAIController>(router, persona);
    }

    // Sprint 7B Fase 0: Persistir sesion en BD
    activeSessionId.reset();
    if (sessionRepo) {
        auto result = sessionRepo->Start(DEFAULT_STUDENT_ID, "case", c.id);
        if (result.IsOk()) {
            activeSessionId = result.Value();
        }
    }
}

// ================================================================
// EndSession
// ================================================================
void AppContext::EndSession() {
    sessionActive = false;
    if (assessmentEngine) {
        lastReport = std::make_unique<enarm::assessment::PerformanceReport>(
            assessmentEngine->generateReport("", "", sessionElapsedSeconds));
        showReportModal = true;
    }

    // Sprint 7B Fase 0: Completar sesion en BD
    if (sessionRepo && activeSessionId.has_value()) {
        double total = static_cast<double>(TotalScore());
        double maxp  = static_cast<double>(TotalMax());
        nlohmann::json scores = {
            {"interview",   interviewPoints},
            {"exam",        examPoints},
            {"studies",     studiesPoints},
            {"diagnosis",   diagnosisPoints},
            {"treatment",   treatmentPoints},
            {"followUp",    followUpPoints}
        };
        sessionRepo->Complete(*activeSessionId, total, maxp, scores);
    }
}

// ================================================================
// Helpers Sprint 6 (FIX 7A.2.5: usan CaseFindingsProvider)
// ================================================================
bool AppContext::IsExamRequested(const std::string& name) const {
    for (const auto& e : requestedExams) if (e == name) return true;
    return false;
}

std::string AppContext::GetPhysicalExamFindings(const std::string& name) {
    // FIX Sprint 7A.2.5: usar findings del caso ACTIVO
    // Combinar id + title + specialty para maxima robustez de matching
    std::string caseKey = "";
    if (activeCase.has_value()) {
        caseKey = activeCase->id + " " + activeCase->title + " " + activeCase->slug + " " + activeCase->chiefComplaint;
    }
    return CaseFindingsProvider::GetExamFinding(caseKey, name);
}

bool AppContext::IsStudyRequested(const std::string& name) const {
    for (const auto& s : requestedStudies) if (s == name) return true;
    return false;
}

std::string AppContext::GetStudyResults(const std::string& name) {
    // FIX Sprint 7A.2.5: usar resultados del caso ACTIVO
    std::string caseKey = "";
    if (activeCase.has_value()) {
        caseKey = activeCase->id + " " + activeCase->title + " " + activeCase->slug + " " + activeCase->chiefComplaint;
    }
    return CaseFindingsProvider::GetStudyResult(caseKey, name);
}

} // namespace ENARM::GUI