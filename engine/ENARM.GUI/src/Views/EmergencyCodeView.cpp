// ═══════════════════════════════════════════════════════════════
//   ENARM.GUI - EmergencyCodeView Implementation
// ═══════════════════════════════════════════════════════════════
#include "ENARM/GUI/Views/EmergencyCodeView.h"
#include "ENARM/GUI/Theme.h"

#include <algorithm>
#include <cstdio>

namespace ENARM::GUI::Views {

    using namespace Theme::Colors;

    void EmergencyCodeView::Render(AppContext& ctx,
                                   ImFont* fontHuge, ImFont* fontLarge,
                                   ImFont* fontMedium, ImFont* fontRegular,
                                   ImFont* fontSmall) {
        if (!ctx.emergencyCode) {
            ctx.emergencyCode = std::make_unique<Simulation::EmergencyCode>();
        }
        auto& code = *ctx.emergencyCode;

        // Si no hay codigo corriendo, mostrar selector
        if (!code.IsRunning()) {
            RenderScenarioSelector(ctx, code, fontRegular);
            return;
        }

        const auto& st = code.GetStatus();
        RenderHeader(st, fontHuge, fontLarge);
        ImGui::Spacing();
        RenderTimer(st, fontLarge, fontRegular);
        ImGui::Spacing();

        // Actualizar el codigo con el delta time del frame
        code.Update(ImGui::GetIO().DeltaTime);

        ImGui::Separator();
        ImGui::Spacing();

        RenderActionButtons(ctx, code, fontRegular);

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        RenderActionsLog(st, fontSmall);

        // Errores graves
        auto errors = code.SeriousErrors();
        if (!errors.empty()) {
            ImGui::Spacing();
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.95f, 0.35f, 0.30f, 1.0f));
            ImGui::Text("ERRORES GRAVES:");
            for (const auto& e : errors) {
                ImGui::BulletText("%s", e.c_str());
            }
            ImGui::PopStyleColor();
        }

        // Codigo terminado
        if (st.codeEnded) {
            ImGui::Spacing();
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.95f, 0.7f, 0.2f, 1.0f));
            ImGui::Text("CODIGO TERMINADO - Puntaje: %d/%d (%d%%)",
                        st.totalPoints, st.maxPoints,
                        st.maxPoints > 0 ? (100 * st.totalPoints / st.maxPoints) : 0);
            ImGui::PopStyleColor();
            if (ImGui::Button("Finalizar codigo", ImVec2(200, 40))) {
                ctx.emergencyCodeRunning = false;
                ctx.view = AppView::Dashboard;
            }
        }
    }

    void EmergencyCodeView::RenderHeader(const Simulation::CodeStatus& st,
                                         ImFont* fontHuge, ImFont* fontLarge) {
        ImVec4 col = (st.type == Simulation::EmergencyCodeType::CodeBlue)
                     ? ImVec4(0.95f, 0.35f, 0.30f, 1.0f)
                     : ImVec4(0.95f, 0.70f, 0.20f, 1.0f);
        ImGui::PushStyleColor(ImGuiCol_Text, col);
        if (fontHuge) ImGui::PushFont(fontHuge);
        ImGui::Text("EMERGENCIA");
        if (fontHuge) ImGui::PopFont();
        ImGui::PopStyleColor();

        if (fontLarge) ImGui::PushFont(fontLarge);
        ImGui::Text("%s", st.codeName.c_str());
        if (fontLarge) ImGui::PopFont();
    }

    void EmergencyCodeView::RenderTimer(const Simulation::CodeStatus& st,
                                        ImFont* fontLarge, ImFont* fontRegular) {
        double remaining = st.TimeRemaining();
        bool critical = remaining < 60.0;

        if (fontLarge) ImGui::PushFont(fontLarge);
        ImGui::PushStyleColor(ImGuiCol_Text,
            critical ? ImVec4(0.95f, 0.35f, 0.30f, 1.0f)
                     : ImVec4(0.90f, 0.90f, 0.95f, 1.0f));
        ImGui::Text("Tiempo restante: %.0f s", remaining);
        ImGui::PopStyleColor();
        if (fontLarge) ImGui::PopFont();

        if (fontRegular) ImGui::PushFont(fontRegular);
        ImGui::Text("Puntaje: %d/%d", st.totalPoints, st.maxPoints);
        if (fontRegular) ImGui::PopFont();
    }

    void EmergencyCodeView::RenderActionButtons(AppContext& ctx,
                                                Simulation::EmergencyCode& code,
                                                ImFont* fontRegular) {
        const char* actions[][3] = {
            {"RCP / Compresiones", "reanimacion", "15"},
            {"Desfibrilacion", "desfibrilacion", "20"},
            {"Epinefrina IV", "epinefrina", "10"},
            {"Amiodarona", "amiodarona", "10"},
            {"ECG 12 derivaciones", "ecg", "15"},
            {"Aspirina", "aspirina", "10"},
            {"Reperfusion (IAMCEST)", "reperfusion hemodinamia", "25"},
            {"TAC craneo", "tac tomografia", "20"},
            {"Alteplasa (Stroke)", "alteplasa trombolisis", "25"},
            {"Hemocultivos", "hemocultivo", "15"},
            {"Lactato serico", "lactato", "10"},
            {"Antibiotico empirico", "antibiotico", "20"},
            {"Fluidos IV (30mL/kg)", "fluidos cristaloides ringer", "15"},
            {"Vasopresores (NA)", "vasopresor norepinefrina", "15"},
            {"Via aerea / intubacion", "via aerea intubar", "15"},
            {"Toracostomia", "toracostomia drenaje", "15"}
        };

        ImGui::Text("ACCIONES:");
        ImGui::Spacing();

        float btnW = (ImGui::GetContentRegionAvail().x - 30.0f) / 3.0f;
        int perRow = 3;

        for (int i = 0; i < 16; ++i) {
            if (i > 0 && (i % perRow) != 0) ImGui::SameLine(0, 15.0f);
            bool clicked = ImGui::Button(actions[i][0], ImVec2(btnW, 44.0f));
            if (clicked) {
                auto result = code.PerformAction(actions[i][1]);
                // Mostrar feedback en un tooltip persistente
                ImGui::SetTooltip("%s", result.feedback.c_str());
            }
        }
    }

    void EmergencyCodeView::RenderActionsLog(const Simulation::CodeStatus& st,
                                             ImFont* fontSmall) {
        if (fontSmall) ImGui::PushFont(fontSmall);
        ImGui::Text("HISTORIAL DE ACCIONES (%zu):", st.actions.size());
        ImGui::Spacing();

        // Mostrar ultimas 8 acciones
        size_t start = st.actions.size() > 8 ? st.actions.size() - 8 : 0;
        for (size_t i = start; i < st.actions.size(); ++i) {
            const auto& a = st.actions[i];
            ImVec4 col = a.correct ? ImVec4(0.30f, 0.80f, 0.40f, 1.0f)
                                  : ImVec4(0.60f, 0.60f, 0.60f, 1.0f);
            ImGui::PushStyleColor(ImGuiCol_Text, col);
            ImGui::BulletText("%5.1fs | %s", a.timeSeconds, a.name.c_str());
            ImGui::PopStyleColor();
        }
        if (fontSmall) ImGui::PopFont();
    }

    void EmergencyCodeView::RenderScenarioSelector(AppContext& ctx,
                                                   Simulation::EmergencyCode& code,
                                                   ImFont* fontRegular) {
        if (fontRegular) ImGui::PushFont(fontRegular);
        ImGui::TextUnformatted("Selecciona un codigo de emergencia para iniciar:");
        if (fontRegular) ImGui::PopFont();
        ImGui::Spacing();

        const char* scenarios[] = {
            "Codigo Azul (Paro Cardiorespiratorio)",
            "Codigo STEMI (IAM)",
            "Codigo Stroke (ACV)",
            "Codigo Trauma (Politraumatizado)",
            "Codigo Sepsis"
        };

        float btnW = (ImGui::GetContentRegionAvail().x - 40.0f) / 2.0f;
        for (int i = 0; i < 5; ++i) {
            if (i > 0 && (i % 2) != 0) ImGui::SameLine(0, 15.0f);
            if (ImGui::Button(scenarios[i], ImVec2(btnW, 56.0f))) {
                code.Start(static_cast<Simulation::EmergencyCodeType>(i));
                ctx.emergencyCodeRunning = true;
            }
        }
    }

} // namespace ENARM::GUI::Views
