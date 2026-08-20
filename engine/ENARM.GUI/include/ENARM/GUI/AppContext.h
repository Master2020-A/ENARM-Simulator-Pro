#pragma once

#include "ENARM/GUI/GuiTypes.h"
#include "ENARM/Data/DatabaseManager.h"
#include "ENARM/Data/Repositories/CaseRepository.h"
#include "ENARM/Data/Repositories/SessionRepository.h"
#include "ENARM/Data/Repositories/MCQRepository.h"
#include "ENARM/Data/Repositories/GuidelineRepository.h"
#include "ENARM/Simulation/EmergencyCode.h"
#include "ENARM/Simulation/SurgicalProcedure.h"

#include <spdlog/spdlog.h>
#include <algorithm>
#include <cstdio>
#include "ENARM/Common/SpecialtyMapper.h"
#include "ENARM/Assessment/GuidelineAdherenceScorer.h"
#include <unordered_set>
#include "ENARM/AI/OllamaClient.h"
#include "ENARM/AI/LLMRouter.h"
#include "ENARM/AI/PatientAIController.h"
#include "ENARM/AI/PubMedClient.h"

// ---- Sprint 6: Assessment ----
#include <enarm/assessment/AssessmentEngine.h>
#include <enarm/assessment/RubricDefinition.h>

#include <memory>
#include <optional>
#include <string>
#include <vector>
#include <deque>
#include <unordered_map>
#include <future>

namespace ENARM::GUI {

    // ============================================================
    // ChatMessage (ORIGINAL - con isDoctor bool)
    // ============================================================
    struct ChatMessage {
        bool        isDoctor  = false;
        std::string content;
        int         latencyMs = 0;
    };

    // ============================================================
    // TreatmentEntry (Sprint 6)
    // ============================================================
    struct TreatmentEntry {
        std::string medication;
        std::string dose;
        std::string rationale;
    };

    // ============================================================
    // AppContext - ORIGINAL restaurado + Sprint 6 Assessment
    // ============================================================
    struct AppContext {

        // ---- Vista y navegacion ----
        AppView             view{AppView::Loading};
        std::deque<AppView> viewHistory;

        // ---- Servicios ----
        std::shared_ptr<Data::DatabaseManager> db;
        std::shared_ptr<AI::OllamaClient>       ollama;
        std::shared_ptr<AI::LLMRouter>          router;
        std::shared_ptr<AI::PubMedClient>       pubmed;

        // Referencias PubMed del caso activo (cache UI)
        std::vector<AI::PubMedArticle> pubmedRefs;
        bool pubmedLoaded{false};

        std::unique_ptr<Data::CaseRepository>    caseRepo;
        std::unique_ptr<Data::SessionRepository> sessionRepo;
        std::unique_ptr<Data::MCQRepository>     mcqRepo;
        std::unique_ptr<Data::GuidelineRepository> guidelineRepo;

        // ---- Casos ----
        std::vector<Data::ClinicalCaseRecord>   casesList;
        std::optional<Data::ClinicalCaseRecord> activeCase;

        // ---- IA paciente ----
        std::unique_ptr<AI::PatientAIController> patient;
        std::optional<std::string>               activeSessionId;

        // ---- Chat ----
        std::vector<ChatMessage> chatMessages;
        char                     chatInputBuffer[512]{0};
        bool                     waitingForResponse{false};

        // ---- Score original (6 dimensiones) ----
        int interviewPoints{0};
        int examPoints{0};
        int studiesPoints{0};
        int diagnosisPoints{0};
        int treatmentPoints{0};
        int followUpPoints{0};

        // ---- UI flags ----
        bool showDemoWindow{false};
        bool showAboutWindow{false};
        bool showSettings{false};

        // ---- Estado de conexion ----
        bool        dbConnected{false};
        bool        ollamaConnected{false};
        std::string statusMessage;

        // ---- Estadisticas ----
        size_t totalCases{0};
        size_t totalMcqs{0};
        size_t totalGuidelines{0};

        // Sprint 7A.5: Guidelines
        std::vector<Data::Guideline> guidelinesList;
        std::optional<Data::Guideline> activeGuideline;

        // Sprint 7B: Guias aplicables a la sesion activa
        std::vector<Data::Guideline> sessionGuidelines;
        // Sprint 7B Epica 2: relevancia por id (1=P 2=S 3=T)
        std::unordered_map<std::string,int> sessionGuidelineRelevance;
        std::unordered_map<std::string,std::string> sessionGuidelineRationale;
        std::unordered_set<std::string> consultedGuidelineIds;
        std::vector<Assessment::GuidelineAdherenceResult> currentAdherence;
        std::string guidelineSearchQuery;
        std::string guidelineSpecialtyFilter;  // "" = todas
        std::vector<std::string> guidelineSpecialties;  // para dropdown

        // Sprint 7B Epica 3: filtros y orden de GPCs en sesion
        bool guideFilterPrimary{true};
        bool guideFilterSecondary{true};
        bool guideFilterTertiary{true};
        bool guideFilterOnlyPending{false};
        int  guideSortMode{0};  // 0=Relevancia 1=ID 2=Adherencia 3=Titulo

        // =========================================================
        // NAVEGACION (original)
        // =========================================================
        void NavigateTo(AppView v) {
            if (view != v) {
                viewHistory.push_back(view);
                view = v;
                if (viewHistory.size() > 20) viewHistory.pop_front();
            }
        }

        void NavigateBack() {
            if (!viewHistory.empty()) {
                view = viewHistory.back();
                viewHistory.pop_back();
            }
        }

        [[nodiscard]] bool CanGoBack() const { return !viewHistory.empty(); }

        // Sprint 7A.5.9: Carga guidelines desde el repo (API real: FindAll + GetAllSpecialties)
        void RefreshGuidelinesList() {
            if (!guidelineRepo) return;
            guidelinesList = guidelineRepo->FindAll();
            guidelineSpecialties = guidelineRepo->GetAllSpecialties();
        }

        // Sprint 7B: Carga guias aplicables a la sesion (por especialidad del caso)
                void LoadSessionGuidelines() {
            sessionGuidelines.clear();
            sessionGuidelineRelevance.clear();
            sessionGuidelineRationale.clear();
            consultedGuidelineIds.clear();
            if (!guidelineRepo || !activeCase.has_value()) return;

            // -------- Intento 1: vinculos explicitos ----------------
            auto linked = guidelineRepo->FindLinkedGuidelines(activeCase->id);
            if (!linked.empty()) {
                sessionGuidelines.reserve(linked.size());
                for (auto& lg : linked) {
                    sessionGuidelineRelevance[lg.guideline.id] = lg.relevance;
                    sessionGuidelineRationale[lg.guideline.id] = lg.rationale;
                    // LOG_TOOLTIP_DEBUG_V4: usar logger 'core' explicito + stderr
                    {
                        auto __rat_preview = lg.rationale.substr(0, std::min<size_t>(60, lg.rationale.size()));
                        auto __logger = spdlog::get("core");
                        if (__logger) {
                            __logger->info("[Tooltip DBG] id={} rel={} rationale_len={} text='{}'",
                                lg.guideline.id, (int)lg.relevance, lg.rationale.size(), __rat_preview);
                            __logger->flush();
                        }
                        // Backup: escribir a stderr para verlo en consola de VS (Output window)
                        std::fprintf(stderr, "[Tooltip DBG] id=%s rel=%d rationale_len=%zu text='%s'\n",
                            lg.guideline.id.c_str(), (int)lg.relevance, lg.rationale.size(),
                            __rat_preview.c_str());
                        std::fflush(stderr);
                    }
                    sessionGuidelines.push_back(std::move(lg.guideline));
                }
                return;
            }

            // -------- Intento 2: fallback por especialidad ----------
            const std::string gpcSpec = ENARM::Common::SpecialtyMapper::CaseToGuideline(
                activeCase->specialty
            );
            sessionGuidelines = guidelineRepo->FindBySpecialty(gpcSpec);
            // Sin relevancia asignada: se pintaran neutrales
        }

        // Sprint 7B Epica 3: Evaluar adherencia a GPCs en tiempo real
        void EvaluateAdherence() {
            std::vector<Assessment::TreatmentAction> txs;
            for (const auto& t : treatments) {
                txs.push_back({t.medication, t.dose, t.rationale});
            }
            std::vector<std::string> studyNames;
            for (const auto& kv : studyResults) {
                studyNames.push_back(kv.first);
            }
            currentAdherence = Assessment::GuidelineAdherenceScorer::EvaluateSession(
                sessionGuidelines, consultedGuidelineIds,
                txs, studyNames, submittedDiagnosis
            );
        }

        // =========================================================
        // SCORING (original)
        // =========================================================
        [[nodiscard]] int TotalScore() const {
            return interviewPoints + examPoints + studiesPoints +
                   diagnosisPoints + treatmentPoints + followUpPoints;
        }

        [[nodiscard]] int TotalMax() const { return 185; }

        [[nodiscard]] double ScorePercent() const {
            return TotalMax() > 0 ? (100.0 * TotalScore() / TotalMax()) : 0.0;
        }

        void RefreshCasesList() {
            if (caseRepo) {
                casesList = caseRepo->FindAll(false, 100);
            }
        }

        void StartSession(const Data::ClinicalCaseRecord& c);
        void EndSession();

        // =========================================================
        // ============  SPRINT 6: ASSESSMENT ENGINE  ==============
        // =========================================================

        // Assessment moderno (motor con rubrica + sinonimos)
        std::unique_ptr<enarm::assessment::AssessmentEngine>  assessmentEngine;
        std::unique_ptr<enarm::assessment::PerformanceReport> lastReport;
        bool showReportModal{false};

        // Estado de sesion nuevo
        float sessionElapsedSeconds{0.0f};
        bool  sessionActive{false};

        // Exploracion Fisica
        std::vector<std::string>                    requestedExams;
        std::unordered_map<std::string,std::string> examFindings;

        // Estudios Paraclinicos
        std::vector<std::string>                    requestedStudies;
        std::unordered_map<std::string,std::string> studyResults;

        // Diagnostico
        std::string              submittedDiagnosis;
        std::string              diagnosisJustification;
        std::vector<std::string> differentials;

        // Tratamiento
        std::vector<TreatmentEntry> treatments;

        // Seguimiento
        std::vector<std::string> followUpActions;

        // Chat state para tabs Sprint 6
        bool        chatScrollToBottom{false};
        bool        aiProcessing{false};
        std::string pendingAIResponse;
        int         pendingAILatencyMs{0};

        // Fonts opacos (para nuevas vistas)
        void* headerFont{nullptr};
        void* subtitleFont{nullptr};
        void* bodyFont{nullptr};

        // ---- Sprint 16: Codigo de emergencia activo ----
        std::unique_ptr<Simulation::EmergencyCode> emergencyCode;
        bool emergencyCodeRunning{false};

        // ---- Sprint 15: Procedimiento quirurgico activo ----
        std::unique_ptr<Simulation::SurgicalProcedure> surgicalProcedure;
        bool surgicalRunning{false};

        // ---- Helpers Sprint 6 ----
        bool        IsExamRequested(const std::string& name) const;
        std::string GetPhysicalExamFindings(const std::string& name);
        bool        IsStudyRequested(const std::string& name) const;
        std::string GetStudyResults(const std::string& name);

        // Titulo del caso activo (helper)
        std::string CurrentCaseTitle() const {
            return activeCase.has_value() ? activeCase->title : std::string{};
        }
        std::string CurrentCaseId() const {
            return activeCase.has_value() ? activeCase->id : std::string{};
        }
    };

} // namespace ENARM::GUI
