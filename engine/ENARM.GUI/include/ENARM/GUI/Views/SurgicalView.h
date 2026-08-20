// ═══════════════════════════════════════════════════════════════
//   ENARM.GUI - SurgicalView (Sprint 15)
//   Simulador quirurgico: pasos, instrumentos, arbol de decision
// ═══════════════════════════════════════════════════════════════
#pragma once

#include "ENARM/GUI/AppContext.h"
#include "ENARM/Simulation/SurgicalProcedure.h"

#include <imgui.h>

namespace ENARM::GUI::Views {

    class SurgicalView {
    public:
        static void Render(AppContext& ctx,
                           ImFont* fontHuge, ImFont* fontLarge,
                           ImFont* fontMedium, ImFont* fontRegular,
                           ImFont* fontSmall);

    private:
        static void RenderProcedureSelector(AppContext& ctx,
                                            Simulation::SurgicalProcedure& proc,
                                            ImFont* fontRegular);
        static void RenderStepPanel(const Simulation::ProcedureStatus& st,
                                    ImFont* fontLarge, ImFont* fontRegular);
        static void RenderActionButtons(AppContext& ctx,
                                        Simulation::SurgicalProcedure& proc,
                                        ImFont* fontRegular);
        static void RenderInstruments(const Simulation::ProcedureStatus& st,
                                      ImFont* fontSmall);
        static void RenderComplications(const Simulation::ProcedureStatus& st,
                                        ImFont* fontSmall);
    };

}