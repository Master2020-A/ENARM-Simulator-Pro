// ═══════════════════════════════════════════════════════════════
//   ENARM.GUI - MCQPracticeView Implementation
// ═══════════════════════════════════════════════════════════════
#include "ENARM/GUI/Views/MCQPracticeView.h"
#include "ENARM/GUI/Theme.h"
#include "ENARM/Data/MCQEngine.h"

#include <cstdio>

namespace ENARM::GUI::Views {

    using namespace Theme::Colors;

    // Estado estatico de la sesion MCQ (persiste entre frames)
    static Data::MCQSession s_session;
    static bool s_sessionActive = false;
    static char s_selected = '?';
    static bool s_answered = false;

    void MCQPracticeView::Render(AppContext& ctx,
                                 ImFont* fontHuge, ImFont* fontLarge,
                                 ImFont* fontMedium, ImFont* fontRegular,
                                 ImFont* fontSmall) {
        // Iniciar sesion si no hay una activa
        if (!s_sessionActive) {
            if (ctx.db) {
                s_session = Data::MCQEngine(ctx.db).StartPractice("", "", 10);
                s_sessionActive = true;
                s_selected = '?';
                s_answered = false;
            } else {
                ImGui::TextColored(ImVec4(0.9f, 0.4f, 0.3f, 1.0f),
                    "Base de datos no disponible. Verifica PostgreSQL.");
                return;
            }
        }

        // Header
        if (fontHuge) ImGui::PushFont(fontHuge);
        ImGui::TextColored(Accent, "Banco MCQ");
        if (fontHuge) ImGui::PopFont();

        RenderSessionStats(s_session, fontMedium);
        ImGui::Separator();
        ImGui::Spacing();

        if (s_session.IsFinished()) {
            ImGui::Spacing();
            if (fontLarge) ImGui::PushFont(fontLarge);
            ImGui::TextColored(ImVec4(0.3f, 0.8f, 0.4f, 1.0f),
                "SESION COMPLETADA - Aciertos: %d/%d (%.0f%%)",
                s_session.correct, s_session.answered, s_session.Percent());
            if (fontLarge) ImGui::PopFont();
            ImGui::Spacing();
            if (ImGui::Button("Nueva sesion", ImVec2(200, 40))) {
                s_sessionActive = false;
            }
            return;
        }

        const auto& q = s_session.questions[s_session.current];

        RenderQuestion(q, fontLarge, fontRegular);
        ImGui::Spacing();

        if (!s_answered) {
            RenderOptions(ctx, q, fontRegular);
        } else {
            RenderFeedback(q, s_selected, fontRegular);
            ImGui::Spacing();
            if (ImGui::Button("Siguiente pregunta", ImVec2(220, 40))) {
                Data::MCQEngine engine(ctx.db);
                engine.Next(s_session);
                s_selected = '?';
                s_answered = false;
            }
        }
    }

    void MCQPracticeView::RenderQuestion(const Data::MCQuestion& q,
                                         ImFont* fontLarge, ImFont* fontRegular) {
        if (fontLarge) ImGui::PushFont(fontLarge);
        ImGui::TextWrapped("%s", q.vignette.c_str());
        ImGui::PopFont();
        ImGui::Spacing();
        if (fontRegular) ImGui::PushFont(fontRegular);
        ImGui::TextWrapped("%s", q.question.c_str());
        ImGui::PopFont();
        ImGui::Spacing();
    }

    void MCQPracticeView::RenderOptions(AppContext& ctx, const Data::MCQuestion& q,
                                        ImFont* fontRegular) {
        if (fontRegular) ImGui::PushFont(fontRegular);
        const char* labels[] = {"A", "B", "C", "D", "E"};
        for (int i = 0; i < 5 && i < (int)q.options.size(); ++i) {
            std::string opt = q.options[i].get<std::string>();
            std::string btn = std::string(labels[i]) + ") " + opt;
            if (ImGui::Button(btn.c_str(), ImVec2(ImGui::GetContentRegionAvail().x, 36.0f))) {
                Data::MCQEngine engine(ctx.db);
                engine.Answer(s_session, labels[i][0], 10.0);
                s_selected = labels[i][0];
                s_answered = true;
            }
            ImGui::Spacing();
        }
        if (fontRegular) ImGui::PopFont();
    }

    void MCQPracticeView::RenderFeedback(const Data::MCQuestion& q, char selected,
                                         ImFont* fontRegular) {
        bool correct = (selected == q.correctAnswer);
        ImGui::PushStyleColor(ImGuiCol_Text,
            correct ? ImVec4(0.3f, 0.8f, 0.4f, 1.0f) : ImVec4(0.9f, 0.4f, 0.3f, 1.0f));
        if (fontRegular) ImGui::PushFont(fontRegular);
        ImGui::TextWrapped("%s", correct ? "CORRECTO" : "INCORRECTO");
        ImGui::PopStyleColor();
        ImGui::TextWrapped("Respuesta correcta: %c", q.correctAnswer);
        if (q.explanationCorrect.has_value()) {
            ImGui::TextWrapped("%s", q.explanationCorrect->c_str());
        }
        if (fontRegular) ImGui::PopFont();
    }

    void MCQPracticeView::RenderSessionStats(const Data::MCQSession& s,
                                             ImFont* fontMedium) {
        if (fontMedium) ImGui::PushFont(fontMedium);
        ImGui::Text("Pregunta %zu/%zu | Aciertos: %d | %%: %.0f",
                    s.current + 1, s.questions.size(), s.correct, s.Percent());
        if (fontMedium) ImGui::PopFont();
    }

} // namespace ENARM::GUI::Views
