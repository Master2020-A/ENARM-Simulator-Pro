// ═══════════════════════════════════════════════════════════════
//   ENARM.GUI - MCQPracticeView (Sprint 8)
//   Practica de preguntas estilo ENARM con retroalimentacion
// ═══════════════════════════════════════════════════════════════
#pragma once

#include "ENARM/GUI/AppContext.h"
#include "ENARM/Data/MCQEngine.h"

#include <imgui.h>

namespace ENARM::GUI::Views {

    class MCQPracticeView {
    public:
        static void Render(AppContext& ctx,
                           ImFont* fontHuge, ImFont* fontLarge,
                           ImFont* fontMedium, ImFont* fontRegular,
                           ImFont* fontSmall);

    private:
        static void RenderQuestion(const Data::MCQuestion& q,
                                   ImFont* fontLarge, ImFont* fontRegular);
        static void RenderOptions(AppContext& ctx, const Data::MCQuestion& q,
                                  ImFont* fontRegular);
        static void RenderFeedback(const Data::MCQuestion& q, char selected,
                                   ImFont* fontRegular);
        static void RenderSessionStats(const Data::MCQSession& s,
                                       ImFont* fontMedium);
    };

}