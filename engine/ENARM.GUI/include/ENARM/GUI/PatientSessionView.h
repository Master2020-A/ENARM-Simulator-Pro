#pragma once
#include <enarm/gui/AppContext.h>
#include <enarm/gui/PatientChatView.h>
#include <enarm/gui/ScoringPanelView.h>
#include <string>
#include <vector>

namespace enarm::gui {

class PatientSessionView {
public:
    void render(AppContext& ctx);
        void RenderGuidelinesTab(AppContext& ctx);  // Sprint 7B

private:
    PatientChatView   m_chatView;
    ScoringPanelView  m_scoringPanel;

    // Sub-tabs
    void renderSessionHeader(AppContext& ctx);
    void renderPhysicalExamTab(AppContext& ctx);
    void renderStudiesTab(AppContext& ctx);
    void renderDiagnosisTab(AppContext& ctx);
    void renderTreatmentTab(AppContext& ctx);
    void renderFollowUpTab(AppContext& ctx);
    void renderCompactVitals(AppContext& ctx);
    void renderReportModal(AppContext& ctx);

    // Estado de tabs
    int  m_activeTab     = 0;
    bool m_diagSubmitted = false;

    // Buffers de diagnostico y tratamiento
    char m_diagBuffer[512]     = {};
    char m_justBuffer[1024]    = {};
    char m_diffBuffer[256]     = {};
    char m_medBuffer[256]      = {};
    char m_doseBuffer[128]     = {};
    char m_rationaleBuffer[512]= {};
    char m_fuBuffer[512]       = {};
};

} // namespace enarm::gui
