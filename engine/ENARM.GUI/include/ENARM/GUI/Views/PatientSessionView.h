#pragma once
#include "ENARM/GUI/Views/ScoringPanelView.h"

namespace ENARM::GUI {
    struct AppContext;

    class PatientSessionView {
    public:
        void render(AppContext& ctx);
        void RenderGuidelinesTab(AppContext& ctx);  // Sprint 7B
        void RenderReferencesTab(AppContext& ctx);  // Sprint 17 PubMed

    private:
        ScoringPanelView m_scoringPanel;

        void renderSessionHeader(AppContext& ctx);
        void renderCompactVitals(AppContext& ctx);

        void renderInterviewTab(AppContext& ctx);
        void renderPhysicalExamTab(AppContext& ctx);
        void renderStudiesTab(AppContext& ctx);
        void renderDiagnosisTab(AppContext& ctx);
        void renderTreatmentTab(AppContext& ctx);
        void renderFollowUpTab(AppContext& ctx);

        void renderChatMessages(AppContext& ctx);
        void renderChatInput(AppContext& ctx);

        void renderReportModal(AppContext& ctx);

        bool m_diagSubmitted = false;
        char m_diagBuffer[512]      = {};
        char m_justBuffer[1024]     = {};
        char m_diffBuffer[256]      = {};
        char m_medBuffer[256]       = {};
        char m_doseBuffer[128]      = {};
        char m_rationaleBuffer[512] = {};
        char m_fuBuffer[512]        = {};
    };
}
