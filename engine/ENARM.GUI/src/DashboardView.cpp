#include <enarm/gui/DashboardView.h>
#include <enarm/gui/AppContext.h>
#include <imgui.h>
#include <cstdio>
#include <string>
#include <vector>
#include <functional>

namespace enarm::gui {

// ================================================================
// Card definition
// ================================================================
struct CardDef {
    const char* title;
    const char* subtitle;
    const char* icon;      // 2 letras
    ImU32       accent;
    AppView     target;
};

// ================================================================
// renderStatBox — cajita del header con numero grande
// ================================================================
static void renderStatBox(const char* label, const char* value, ImU32 accent) {
    float boxW = 130.0f;
    float boxH = 82.0f;

    ImVec2 pos = ImGui::GetCursorScreenPos();
    ImVec2 end = ImVec2(pos.x + boxW, pos.y + boxH);

    ImDrawList* dl = ImGui::GetWindowDrawList();

    // Fondo semi-transparente sobre el hero
    dl->AddRectFilled(pos, end, IM_COL32(255,255,255,40), 8.0f);
    dl->AddRect(pos, end, IM_COL32(255,255,255,80), 8.0f, 0, 1.0f);

    // Label pequeno arriba
    dl->AddText(
        ImVec2(pos.x + 10.0f, pos.y + 8.0f),
        IM_COL32(255,255,255,180),
        label);

    // Numero grande
    dl->AddText(
        ImGui::GetFont(), 32.0f,
        ImVec2(pos.x + 10.0f, pos.y + 28.0f),
        IM_COL32(255,255,255,255),
        value);

    ImGui::Dummy(ImVec2(boxW, boxH));
}

// ================================================================
// renderHero — bandera azul superior
// ================================================================
static void renderHero(AppContext& ctx) {
    float heroH  = 180.0f;
    float availW = ImGui::GetContentRegionAvail().x;

    ImVec2 pos = ImGui::GetCursorScreenPos();
    ImVec2 end = ImVec2(pos.x + availW, pos.y + heroH);

    ImDrawList* dl = ImGui::GetWindowDrawList();

    // Gradiente azul horizontal
    dl->AddRectFilledMultiColor(
        pos, end,
        IM_COL32( 30,144,220,255),   // top-left
        IM_COL32( 60,180,230,255),   // top-right
        IM_COL32( 40,160,220,255),   // bottom-right
        IM_COL32( 25,130,210,255));  // bottom-left

    // Padding interior
    float pad = 32.0f;

    // Lado izquierdo: texto
    dl->AddText(
        ImVec2(pos.x + pad, pos.y + pad),
        IM_COL32(255,255,255,180),
        "ENARM SIMULATOR PRO");

    dl->AddText(
        ImGui::GetFont(), 32.0f,
        ImVec2(pos.x + pad, pos.y + pad + 22.0f),
        IM_COL32(255,255,255,255),
        "Bienvenido, Doctor");

    dl->AddText(
        ImVec2(pos.x + pad, pos.y + heroH - pad - 8.0f),
        IM_COL32(255,255,255,200),
        "Simulador clinico profesional con IA para preparacion de residencia medica");

    // Lado derecho: dos stat boxes
    float boxW = 130.0f;
    float boxGap = 12.0f;
    ImGui::SetCursorScreenPos(
        ImVec2(pos.x + availW - pad - boxW*2 - boxGap,
               pos.y + pad));
    renderStatBox("CASOS CLINICOS", "6", IM_COL32(255,255,255,255));

    ImGui::SetCursorScreenPos(
        ImVec2(pos.x + availW - pad - boxW,
               pos.y + pad));
    renderStatBox("PREGUNTAS MCQ", "9", IM_COL32(255,255,255,255));

    // Avanzar cursor debajo del hero
    ImGui::SetCursorScreenPos(ImVec2(pos.x, pos.y + heroH + 24.0f));
}

// ================================================================
// renderCard — una tarjeta clickeable
// ================================================================
static bool renderCard(const CardDef& c) {
    const float cardW = 250.0f;
    const float cardH = 150.0f;

    ImGui::PushID(c.title);

    ImVec2 pos = ImGui::GetCursorScreenPos();
    ImVec2 end = ImVec2(pos.x + cardW, pos.y + cardH);

    bool hovered = ImGui::IsMouseHoveringRect(pos, end);
    bool clicked = false;

    ImDrawList* dl = ImGui::GetWindowDrawList();

    // Fondo card
    ImU32 bg = hovered
        ? IM_COL32(52, 52, 78, 255)
        : IM_COL32(42, 42, 65, 255);
    dl->AddRectFilled(pos, end, bg, 10.0f);

    // Barra accent superior
    dl->AddRectFilled(
        pos,
        ImVec2(end.x, pos.y + 4.0f),
        c.accent,
        10.0f,
        ImDrawFlags_RoundCornersTop);

    // Glow hover
    if (hovered) {
        dl->AddRect(pos, end, c.accent, 10.0f, 0, 2.0f);
    }

    // Icono (esquina superior izquierda)
    float pad = 16.0f;
    ImVec2 iconBox = ImVec2(pos.x + pad, pos.y + pad + 6.0f);
    ImVec2 iconEnd = ImVec2(iconBox.x + 46.0f, iconBox.y + 46.0f);
    dl->AddRectFilled(iconBox, iconEnd,
        IM_COL32(
            (c.accent >>  0) & 0xFF,
            (c.accent >>  8) & 0xFF,
            (c.accent >> 16) & 0xFF,
            60), 8.0f);
    dl->AddText(
        ImGui::GetFont(), 22.0f,
        ImVec2(iconBox.x + 12.0f, iconBox.y + 10.0f),
        c.accent,
        c.icon);

    // ACCEDER (esquina superior derecha)
    const char* acceder = "ACCEDER";
    ImVec2 accSize = ImGui::CalcTextSize(acceder);
    dl->AddText(
        ImVec2(end.x - pad - accSize.x, pos.y + pad + 8.0f),
        IM_COL32(200,200,220,180),
        acceder);

    // Titulo (abajo, arriba del subtitle)
    dl->AddText(
        ImGui::GetFont(), 18.0f,
        ImVec2(pos.x + pad, pos.y + cardH - pad - 42.0f),
        IM_COL32(255,255,255,255),
        c.title);

    // Linea decorativa
    dl->AddLine(
        ImVec2(pos.x + pad,        pos.y + cardH - pad - 22.0f),
        ImVec2(pos.x + pad + 24.0f, pos.y + cardH - pad - 22.0f),
        IM_COL32(255,255,255,120), 1.5f);

    // Subtitle
    dl->AddText(
        ImVec2(pos.x + pad, pos.y + cardH - pad - 14.0f),
        IM_COL32(180,180,200,220),
        c.subtitle);

    // Boton invisible para click
    ImGui::SetCursorScreenPos(pos);
    if (ImGui::InvisibleButton("##card", ImVec2(cardW, cardH))) {
        clicked = true;
    }

    ImGui::PopID();
    return clicked;
}

// ================================================================
// DashboardView::render
// ================================================================
void DashboardView::render(AppContext& ctx) {
    // Padding general de la ventana
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(28.0f, 20.0f));

    // Hero superior
    renderHero(ctx);

    // Titulo "Acciones rapidas"
    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(40,40,60,255));
    ImGui::SetWindowFontScale(1.5f);
    ImGui::Text("Acciones rapidas");
    ImGui::SetWindowFontScale(1.0f);
    ImGui::PopStyleColor();

    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(120,120,140,255));
    ImGui::Text("Atajos operativos del expediente");
    ImGui::PopStyleColor();

    ImGui::Dummy(ImVec2(0.0f, 16.0f));

    // Cards definitions
    static const std::vector<CardDef> cards = {
        { "Casos Clinicos",  "Pacientes virtuales con IA",   "CS",
          IM_COL32( 99,102,241,255), AppView::CasesList },
        { "Banco de MCQs",   "Preguntas tipo ENARM",         "MQ",
          IM_COL32(168, 85,247,255), AppView::MCQPractice },
        { "Guias Clinicas",  "GPCs IMSS actualizadas",       "GC",
          IM_COL32(236, 72,153,255), AppView::Guidelines },
        { "Mi Progreso",     "Estadisticas y debilidades",   "PR",
          IM_COL32( 34,197, 94,255), AppView::Progress },
        { "Simulacion ICU",  "Motor fisiologico en vivo",    "IC",
          IM_COL32(251,146, 60,255), AppView::ICUSimulation },
        { "Configuracion",   "Opciones del sistema",         "CF",
          IM_COL32(148,163,184,255), AppView::Settings },
    };

    // Layout responsivo: calcular columnas
    float availW  = ImGui::GetContentRegionAvail().x;
    float cardW   = 250.0f;
    float gap     = 16.0f;
    int   columns = static_cast<int>((availW + gap) / (cardW + gap));
    if (columns < 1) columns = 1;
    if (columns > 4) columns = 4;

    // Renderizar grid
    for (size_t i = 0; i < cards.size(); ++i) {
        if (i > 0 && (i % columns) != 0) {
            ImGui::SameLine(0.0f, gap);
        } else if (i > 0) {
            ImGui::Dummy(ImVec2(0.0f, gap));
        }

        if (renderCard(cards[i])) {
            // Click en la card
            if (cards[i].target == AppView::CasesList) {
                ctx.navigateTo(AppView::CasesList);
            } else {
                // Por ahora otras vistas no implementadas -> ir a CasesList
                ctx.navigateTo(AppView::CasesList);
            }
        }
    }

    ImGui::PopStyleVar();
}

} // namespace enarm::gui
