// ═══════════════════════════════════════════════════════════════
//   ENARM.GUI - SettingsView
//   Configuracion del sistema: conexiones, modelo LLM, tema
// ═══════════════════════════════════════════════════════════════
#pragma once

#include "ENARM/GUI/AppContext.h"

#include <imgui.h>

namespace ENARM::GUI::Views {

    class SettingsView {
    public:
        static void Render(AppContext& ctx,
                           ImFont* fontHuge, ImFont* fontLarge,
                           ImFont* fontMedium, ImFont* fontRegular,
                           ImFont* fontSmall);
    };

}