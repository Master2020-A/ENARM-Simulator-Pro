// ═══════════════════════════════════════════════════════════════
//   ENARM.GUI - EmergencyCodeView (Sprint 16)
//   UI para Codigos criticos: Azul, STEMI, Stroke, Trauma, Sepsis
// ═══════════════════════════════════════════════════════════════
#pragma once

#include "ENARM/GUI/AppContext.h"
#include "ENARM/Simulation/EmergencyCode.h"

#include <imgui.h>

namespace ENARM::GUI::Views {

    class EmergencyCodeView {
    public:
        // Renderiza la vista del codigo activo
        static void Render(AppContext& ctx,
                           ImFont* fontHuge, ImFont* fontLarge,
                           ImFont* fontMedium, ImFont* fontRegular,
                           ImFont* fontSmall);

    private:
        static void RenderHeader(const Simulation::CodeStatus& st,
                                 ImFont* fontHuge, ImFont* fontLarge);
        static void RenderTimer(const Simulation::CodeStatus& st,
                                ImFont* fontLarge, ImFont* fontRegular);
        static void RenderActionButtons(AppContext& ctx,
                                        Simulation::EmergencyCode& code,
                                        ImFont* fontRegular);
        static void RenderActionsLog(const Simulation::CodeStatus& st,
                                     ImFont* fontSmall);
        static void RenderScenarioSelector(AppContext& ctx,
                                           Simulation::EmergencyCode& code,
                                           ImFont* fontRegular);
    };

}