// ═══════════════════════════════════════════════════════════════
//   ENARM.GUI - SettingsView Implementation
// ═══════════════════════════════════════════════════════════════
#include "ENARM/GUI/Views/SettingsView.h"
#include "ENARM/GUI/Theme.h"

namespace ENARM::GUI::Views {

    using namespace Theme::Colors;

    void SettingsView::Render(AppContext& ctx,
                              ImFont* fontHuge, ImFont* fontLarge,
                              ImFont* fontMedium, ImFont* fontRegular,
                              ImFont* fontSmall) {
        if (fontHuge) ImGui::PushFont(fontHuge);
        ImGui::TextColored(Accent, "Configuracion");
        if (fontHuge) ImGui::PopFont();
        ImGui::Spacing();

        // ─── Conexiones ───
        if (fontMedium) ImGui::PushFont(fontMedium);
        ImGui::Text("Conexiones");
        if (fontMedium) ImGui::PopFont();
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::Text("Base de datos (PostgreSQL):");
        ImGui::SameLine();
        ImGui::TextColored(ctx.dbConnected ? ImVec4(0.3f, 0.8f, 0.4f, 1.0f)
                                           : ImVec4(0.9f, 0.4f, 0.3f, 1.0f),
                           ctx.dbConnected ? "CONECTADO" : "DESCONECTADO");
        ImGui::Text("Ollama (LLM local):");
        ImGui::SameLine();
        ImGui::TextColored(ctx.ollamaConnected ? ImVec4(0.3f, 0.8f, 0.4f, 1.0f)
                                               : ImVec4(0.9f, 0.4f, 0.3f, 1.0f),
                           ctx.ollamaConnected ? "CONECTADO" : "DESCONECTADO");
        ImGui::Text("PubMed (E-utilities):");
        ImGui::SameLine();
        ImGui::TextColored(ctx.pubmed ? ImVec4(0.3f, 0.8f, 0.4f, 1.0f)
                                      : ImVec4(0.9f, 0.4f, 0.3f, 1.0f),
                           ctx.pubmed ? "DISPONIBLE" : "NO DISPONIBLE");
        ImGui::Spacing();

        // ─── Estadisticas ───
        if (fontMedium) ImGui::PushFont(fontMedium);
        ImGui::Text("Estadisticas");
        if (fontMedium) ImGui::PopFont();
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::Text("Casos clinicos: %zu", ctx.totalCases);
        ImGui::Text("Preguntas MCQ:  %zu", ctx.totalMcqs);
        ImGui::Text("Guias clinicas: %zu", ctx.totalGuidelines);
        ImGui::Spacing();

        // ─── Preferencias ───
        if (fontMedium) ImGui::PushFont(fontMedium);
        ImGui::Text("Preferencias");
        if (fontMedium) ImGui::PopFont();
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::Checkbox("Ventana ImGui Demo", &ctx.showDemoWindow);
        ImGui::Checkbox("Ventana Acerca de", &ctx.showAboutWindow);
        ImGui::Spacing();

        // ─── Estado ───
        if (fontSmall) ImGui::PushFont(fontSmall);
        ImGui::TextColored(ImVec4(0.6f, 0.6f, 0.6f, 1.0f),
                           "Estado: %s", ctx.statusMessage.c_str());
        if (fontSmall) ImGui::PopFont();
    }

} // namespace ENARM::GUI::Views
