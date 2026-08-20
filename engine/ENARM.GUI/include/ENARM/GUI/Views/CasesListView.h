// ═══════════════════════════════════════════════════════════════
//   ENARM.GUI - Vista de lista de casos clinicos
// ═══════════════════════════════════════════════════════════════
#pragma once

#include "ENARM/GUI/AppContext.h"
#include <imgui.h>

namespace ENARM::GUI::Views {
    class CasesListView {
    public:
        // Renderiza dentro del content area del dashboard
        static void Render(AppContext& ctx,
                            ImFont* fontHuge, ImFont* fontLarge,
                            ImFont* fontMedium, ImFont* fontRegular,
                            ImFont* fontSmall);
    };
}