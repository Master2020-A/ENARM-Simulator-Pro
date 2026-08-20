// ═══════════════════════════════════════════════════════════════
//   ENARM.GUI - SurgicalView Implementation
// ═══════════════════════════════════════════════════════════════
#include "ENARM/GUI/Views/SurgicalView.h"
#include "ENARM/GUI/Theme.h"

#include <cstdio>

namespace ENARM::GUI::Views {

    using namespace Theme::Colors;

    void SurgicalView::Render(AppContext& ctx,
                              ImFont* fontHuge, ImFont* fontLarge,
                              ImFont* fontMedium, ImFont* fontRegular,
                              ImFont* fontSmall) {
        if (!ctx.surgicalProcedure) {
            ctx.surgicalProcedure = std::make_unique<Simulation::SurgicalProcedure>();
        }
        auto& proc = *ctx.surgicalProcedure;

        if (!proc.IsRunning()) {
            RenderProcedureSelector(ctx, proc, fontRegular);
            return;
        }

        const auto& st = proc.GetStatus();

        // Header
        if (fontHuge) ImGui::PushFont(fontHuge);
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.30f, 0.75f, 0.60f, 1.0f));
        ImGui::Text("QUIROFANO");
        ImGui::PopStyleColor();
        if (fontHuge) ImGui::PopFont();

        if (fontLarge) ImGui::PushFont(fontLarge);
        ImGui::Text("%s", st.procedureName.c_str());
        if (fontLarge) ImGui::PopFont();

        // Timer
        double remaining = st.timeLimitSeconds - st.elapsedSeconds;
        bool critical = remaining < 120.0;
        ImGui::PushStyleColor(ImGuiCol_Text,
            critical ? ImVec4(0.95f, 0.35f, 0.30f, 1.0f)
                     : ImVec4(0.90f, 0.90f, 0.95f, 1.0f));
        ImGui::Text("Tiempo: %.0f s | Puntaje: %d/%d",
                    remaining, st.totalPoints, st.maxPoints);
        ImGui::PopStyleColor();

        proc.Update(ImGui::GetIO().DeltaTime);

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        RenderStepPanel(st, fontLarge, fontRegular);
        ImGui::Spacing();
        RenderActionButtons(ctx, proc, fontRegular);
        ImGui::Spacing();
        RenderInstruments(st, fontSmall);
        RenderComplications(st, fontSmall);

        if (st.completed) {
            ImGui::Spacing();
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.95f, 0.7f, 0.2f, 1.0f));
            ImGui::Text("PROCEDIMIENTO TERMINADO - Puntaje: %d/%d (%d%%)",
                        st.totalPoints, st.maxPoints,
                        st.maxPoints > 0 ? (100 * st.totalPoints / st.maxPoints) : 0);
            ImGui::PopStyleColor();
            if (ImGui::Button("Finalizar", ImVec2(200, 40))) {
                ctx.surgicalRunning = false;
                ctx.view = AppView::Dashboard;
            }
        }
    }

    void SurgicalView::RenderProcedureSelector(AppContext& ctx,
                                               Simulation::SurgicalProcedure& proc,
                                               ImFont* fontRegular) {
        if (fontRegular) ImGui::PushFont(fontRegular);
        ImGui::TextUnformatted("Selecciona un procedimiento quirurgico:");
        if (fontRegular) ImGui::PopFont();
        ImGui::Spacing();

        const char* procedures[] = {
            "Apendicectomia",
            "Colecistectomia laparoscopica",
            "Cesarea",
            "Toracostomia (drenaje pleural)",
            "Traqueostomia",
            "Sutura de heridas",
            "Drenaje de abscesos",
            "RCP Avanzado"
        };

        float btnW = (ImGui::GetContentRegionAvail().x - 30.0f) / 2.0f;
        for (int i = 0; i < 8; ++i) {
            if (i > 0 && (i % 2) != 0) ImGui::SameLine(0, 15.0f);
            if (ImGui::Button(procedures[i], ImVec2(btnW, 48.0f))) {
                proc.Start(static_cast<Simulation::ProcedureType>(i));
                ctx.surgicalRunning = true;
            }
        }
    }

    void SurgicalView::RenderStepPanel(const Simulation::ProcedureStatus& st,
                                       ImFont* fontLarge, ImFont* fontRegular) {
        if (st.IsFinished()) return;

        const auto& step = st.steps[st.currentStep];

        if (fontLarge) ImGui::PushFont(fontLarge);
        ImGui::TextColored(Accent, "Paso %zu/%zu: %s",
                           st.currentStep + 1, st.steps.size(), step.name.c_str());
        if (fontLarge) ImGui::PopFont();

        if (fontRegular) ImGui::PushFont(fontRegular);
        ImGui::TextWrapped("%s", step.description.c_str());
        ImGui::TextColored(ImVec4(0.6f, 0.6f, 0.6f, 1.0f),
                           "Instrumento: %s | Puntos: %d%s",
                           step.instrument.c_str(), step.points,
                           step.isCritical ? " (CRITICO)" : "");
        if (fontRegular) ImGui::PopFont();
    }

    void SurgicalView::RenderActionButtons(AppContext& ctx,
                                           Simulation::SurgicalProcedure& proc,
                                           ImFont* fontRegular) {
        if (proc.GetStatus().IsFinished()) return;

        const char* actions[] = {
            "asepsia", "incision", "localizar apendice", "ligadura",
            "cierre sutura", "neumoperitoneo", "trocar", "calot",
            "clip", "extraccion", "pfann", "aponeurosis", "peritoneo",
            "histerotomia", "neonato", "placenta", "5to axilar", "dedo",
            "tubo", "sello", "oxigeno", "cervical", "musculos", "traquea",
            "cannula", "fijar", "suero", "anestes", "punto", "cultivo",
            "gasas", "compresiones", "ambu", "dae", "epinefrina"
        };

        ImGui::Text("ACCIONES:");
        ImGui::Spacing();

        float btnW = (ImGui::GetContentRegionAvail().x - 30.0f) / 3.0f;
        for (int i = 0; i < 35; ++i) {
            if (i > 0 && (i % 3) != 0) ImGui::SameLine(0, 15.0f);
            bool clicked = ImGui::Button(actions[i], ImVec2(btnW, 36.0f));
            if (clicked) {
                bool ok = proc.PerformAction(actions[i]);
                if (ok) {
                    proc.NextStep();
                }
            }
        }
    }

    void SurgicalView::RenderInstruments(const Simulation::ProcedureStatus& st,
                                         ImFont* fontSmall) {
        if (fontSmall) ImGui::PushFont(fontSmall);
        ImGui::TextColored(ImVec4(0.6f, 0.6f, 0.6f, 1.0f),
                           "Instrumentos disponibles: %s",
                           st.steps.empty() ? "-" : st.steps[st.currentStep].instrument.c_str());
        if (fontSmall) ImGui::PopFont();
    }

    void SurgicalView::RenderComplications(const Simulation::ProcedureStatus& st,
                                           ImFont* fontSmall) {
        if (st.complications.empty()) return;
        if (fontSmall) ImGui::PushFont(fontSmall);
        ImGui::Spacing();
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.95f, 0.35f, 0.30f, 1.0f));
        ImGui::Text("COMPLICACIONES:");
        for (const auto& c : st.complications) {
            ImGui::BulletText("%s", c.c_str());
        }
        ImGui::PopStyleColor();
        if (fontSmall) ImGui::PopFont();
    }

} // namespace ENARM::GUI::Views
