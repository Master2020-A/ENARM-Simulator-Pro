// ═══════════════════════════════════════════════════════════════
//   ENARM.GUI - HelpView Implementation
// ═══════════════════════════════════════════════════════════════
#include "ENARM/GUI/Views/HelpView.h"
#include "ENARM/GUI/Theme.h"

namespace ENARM::GUI::Views {

    using namespace Theme::Colors;

    void HelpView::Render(AppContext& ctx,
                          ImFont* fontHuge, ImFont* fontLarge,
                          ImFont* fontMedium, ImFont* fontRegular,
                          ImFont* fontSmall) {
        if (fontHuge) ImGui::PushFont(fontHuge);
        ImGui::TextColored(Accent, "Ayuda");
        if (fontHuge) ImGui::PopFont();
        ImGui::Spacing();

        // ─── Como usar ───
        if (fontMedium) ImGui::PushFont(fontMedium);
        ImGui::Text("Como usar el simulador");
        if (fontMedium) ImGui::PopFont();
        ImGui::Separator();
        ImGui::Spacing();

        if (fontRegular) ImGui::PushFont(fontRegular);
        ImGui::BulletText("1. Selecciona un caso clinico en 'Casos Clinicos'");
        ImGui::BulletText("2. Interroga al paciente en la pestana 'Interrogatorio'");
        ImGui::BulletText("3. Solicita exploracion fisica y estudios");
        ImGui::BulletText("4. Establece diagnostico y tratamiento");
        ImGui::BulletText("5. Revisa las guias clinicas aplicables");
        ImGui::BulletText("6. Consulta referencias PubMed del caso");
        ImGui::BulletText("7. Practica codigos de emergencia y procedimientos quirurgicos");
        ImGui::Spacing();

        // ─── Atajos ───
        if (fontMedium) ImGui::PushFont(fontMedium);
        ImGui::Text("Atajos de teclado");
        if (fontMedium) ImGui::PopFont();
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::BulletText("Ctrl+N  - Nueva sesion");
        ImGui::BulletText("Ctrl+H  - Dashboard");
        ImGui::BulletText("Ctrl+C  - Casos clinicos");
        ImGui::BulletText("Ctrl+G  - Guias clinicas");
        ImGui::BulletText("Alt+F4  - Salir");
        ImGui::Spacing();

        // ─── Acerca de ───
        if (fontMedium) ImGui::PushFont(fontMedium);
        ImGui::Text("Acerca de");
        if (fontMedium) ImGui::PopFont();
        ImGui::Separator();
        ImGui::Spacing();

        if (fontRegular) ImGui::PushFont(fontRegular);
        ImGui::Text("ENARM Simulator Pro v0.1.0");
        ImGui::Text("Simulador clinico profesional para preparacion ENARM");
        ImGui::Text("C++20 + Dear ImGui + PostgreSQL 18 + Ollama + UE5");
        ImGui::Spacing();
        ImGui::TextColored(ImVec4(0.6f, 0.6f, 0.6f, 1.0f),
            "Estado: %s", ctx.statusMessage.c_str());
        if (fontRegular) ImGui::PopFont();
    }

} // namespace ENARM::GUI::Views
