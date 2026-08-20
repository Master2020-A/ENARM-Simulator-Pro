// ═══════════════════════════════════════════════════════════════
//   ENARM.GUI - ECGMonitorView (Sprint 12.5)
//   Monitor ECG en tiempo real con ECGSynthesizer + ImPlot
//   Muestra 12 derivaciones y permite cambiar ritmo
// ═══════════════════════════════════════════════════════════════
#pragma once

#include "ENARM/GUI/AppContext.h"
#include "ENARM/Physiology/ECGSynthesizer.h"

#include <imgui.h>
#include <implot.h>

#include <array>
#include <deque>

namespace ENARM::GUI::Views {

    class ECGMonitorView {
    public:
        ECGMonitorView();

        static void Render(AppContext& ctx,
                           ImFont* fontHuge, ImFont* fontLarge,
                           ImFont* fontMedium, ImFont* fontRegular,
                           ImFont* fontSmall);

    private:
        void RenderRhythmSelector();
        void Render12LeadGrid();

        const char* m_leadNames[12] = {
            "I", "II", "III", "aVR", "aVL", "aVF",
            "V1", "V2", "V3", "V4", "V5", "V6"
        };
    };

}