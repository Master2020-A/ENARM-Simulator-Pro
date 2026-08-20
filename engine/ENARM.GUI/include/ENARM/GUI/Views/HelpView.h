// ═══════════════════════════════════════════════════════════════
//   ENARM.GUI - HelpView
//   Manual y atajos de la aplicacion
// ═══════════════════════════════════════════════════════════════
#pragma once

#include "ENARM/GUI/AppContext.h"

#include <imgui.h>

namespace ENARM::GUI::Views {

    class HelpView {
    public:
        static void Render(AppContext& ctx,
                           ImFont* fontHuge, ImFont* fontLarge,
                           ImFont* fontMedium, ImFont* fontRegular,
                           ImFont* fontSmall);
    };

}