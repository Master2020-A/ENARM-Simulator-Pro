// =====================================================================
//   ENARM.GUI - Vista lista de guias clinicas (Sprint 7A.5)
// =====================================================================
#pragma once

#include "ENARM/GUI/AppContext.h"
#include <imgui.h>

namespace ENARM::GUI::Views {
    class GuidelinesListView {
    public:
        static void Render(AppContext& ctx,
                            ImFont* fontHuge, ImFont* fontLarge,
                            ImFont* fontMedium, ImFont* fontRegular,
                            ImFont* fontSmall);
    };

    class GuidelineDetailView {
    public:
        static void Render(AppContext& ctx,
                            ImFont* fontHuge, ImFont* fontLarge,
                            ImFont* fontMedium, ImFont* fontRegular,
                            ImFont* fontSmall);
    };
}