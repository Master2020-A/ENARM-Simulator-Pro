// =====================================================================
//   ENARM.GUI - GuidelinesListView + GuidelineDetailView (Sprint 7A.5)
// =====================================================================
#include "ENARM/GUI/Views/GuidelinesListView.h"
#include "ENARM/GUI/Theme.h"

#include <imgui.h>
#include <cstring>
#include <cstdio>
#include <algorithm>

namespace ENARM::GUI::Views {

using namespace Theme::Colors;

// ============================================================
// Helper: color por nivel de recomendacion A/B/C
// ============================================================
static ImU32 ColorForLevel(const std::string& level) {
    if (level == "A") return IM_COL32(76, 175, 80, 255);   // Verde
    if (level == "B") return IM_COL32(255, 152, 0, 255);   // Naranja
    if (level == "C") return IM_COL32(158, 158, 158, 255); // Gris
    return IM_COL32(100, 100, 100, 255);
}

static const char* LevelDescription(const std::string& level) {
    if (level == "A") return "Evidencia alta";
    if (level == "B") return "Evidencia moderada";
    if (level == "C") return "Consenso";
    return "";
}

// ============================================================
// Helper: color por especialidad
// ============================================================
static ImU32 ColorForSpecialty(const std::string& sp) {
    if (sp == "Cardiologia")                return IM_COL32(233, 30, 99, 255);   // Pink
    if (sp == "Infectologia")               return IM_COL32(255, 87, 34, 255);   // Deep orange
    if (sp == "Neurologia")                 return IM_COL32(156, 39, 176, 255);  // Purple
    if (sp == "Neumologia")                 return IM_COL32(33, 150, 243, 255);  // Blue
    if (sp == "Gineco-Obstetricia")         return IM_COL32(236, 64, 122, 255);  // Pink
    if (sp == "Cirugia Pediatrica")         return IM_COL32(76, 175, 80, 255);   // Green
    if (sp == "Cirugia General")            return IM_COL32(0, 150, 136, 255);   // Teal
    if (sp == "Urgencias")                  return IM_COL32(255, 87, 34, 255);   // Deep orange
    if (sp == "Endocrinologia")             return IM_COL32(255, 193, 7, 255);   // Amber
    if (sp == "Endocrinologia Pediatrica")  return IM_COL32(255, 193, 7, 255);   // Amber
    if (sp == "Pediatria")                  return IM_COL32(3, 169, 244, 255);   // Light blue
    if (sp == "Nefrologia")                 return IM_COL32(103, 58, 183, 255);  // Deep purple
    return IM_COL32(96, 125, 139, 255);  // Blue grey default
}

// ============================================================
// GuidelinesListView::Render
// ============================================================
void GuidelinesListView::Render(AppContext& ctx,
                                  ImFont* fontHuge, ImFont* fontLarge,
                                  ImFont* fontMedium, ImFont* fontRegular,
                                  ImFont* fontSmall) {

    // Cargar guidelines si no estan cargadas
    if (ctx.guidelinesList.empty() && ctx.guidelineRepo) {
        ctx.guidelinesList = ctx.guidelineRepo->FindAll();
        ctx.guidelineSpecialties = ctx.guidelineRepo->GetAllSpecialties();
    }

    // ---- Titulo ----
    if (fontLarge) ImGui::PushFont(fontLarge);
    ImGui::PushStyleColor(ImGuiCol_Text, TextTitle);
    ImGui::Text("Guias de Practica Clinica");
    ImGui::PopStyleColor();
    if (fontLarge) ImGui::PopFont();

    if (fontRegular) ImGui::PushFont(fontRegular);
    ImGui::PushStyleColor(ImGuiCol_Text, TextSecondary);
    ImGui::Text("Consulta las guias CENETEC/IMSS  |  %zu disponibles",
                ctx.guidelinesList.size());
    ImGui::PopStyleColor();
    if (fontRegular) ImGui::PopFont();

    ImGui::Dummy(ImVec2(0, 12));

    // ---- Barra de busqueda + filtro ----
    ImGui::PushStyleColor(ImGuiCol_FrameBg, BgSubtle);
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(12, 10));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 8.0f);

    static char searchBuf[128] = "";
    ImGui::SetNextItemWidth(400.0f);
    if (ImGui::InputTextWithHint("##search", "Buscar por titulo, ID o tag...",
                                   searchBuf, sizeof(searchBuf))) {
        ctx.guidelineSearchQuery = searchBuf;
        if (ctx.guidelineRepo) {
            ctx.guidelinesList = ctx.guidelineRepo->Search(ctx.guidelineSearchQuery);
        }
    }

    ImGui::SameLine();
    ImGui::SetNextItemWidth(280.0f);
    const char* currentSpec = ctx.guidelineSpecialtyFilter.empty()
                                ? "Todas las especialidades"
                                : ctx.guidelineSpecialtyFilter.c_str();
    if (ImGui::BeginCombo("##specfilter", currentSpec)) {
        if (ImGui::Selectable("Todas las especialidades",
                                ctx.guidelineSpecialtyFilter.empty())) {
            ctx.guidelineSpecialtyFilter.clear();
            if (ctx.guidelineRepo) ctx.guidelinesList = ctx.guidelineRepo->FindAll();
        }
        for (const auto& sp : ctx.guidelineSpecialties) {
            bool selected = (ctx.guidelineSpecialtyFilter == sp);
            if (ImGui::Selectable(sp.c_str(), selected)) {
                ctx.guidelineSpecialtyFilter = sp;
                if (ctx.guidelineRepo) {
                    ctx.guidelinesList = ctx.guidelineRepo->FindBySpecialty(sp);
                }
            }
        }
        ImGui::EndCombo();
    }

    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor();

    ImGui::Dummy(ImVec2(0, 16));

    // ---- Lista de GPCs (cards) ----
    ImGui::BeginChild("##gpclist", ImVec2(0, -1), false);

    for (size_t i = 0; i < ctx.guidelinesList.size(); ++i) {
        const auto& g = ctx.guidelinesList[i];

        ImDrawList* dl = ImGui::GetWindowDrawList();
        ImVec2 pos = ImGui::GetCursorScreenPos();
        float availW = ImGui::GetContentRegionAvail().x;
        float cardH = 110.0f;

        ImVec2 cardMin = pos;
        ImVec2 cardMax = ImVec2(pos.x + availW, pos.y + cardH);

        // Card background
        dl->AddRectFilled(cardMin, cardMax, IM_COL32(255, 255, 255, 255), 8.0f);
        dl->AddRect(cardMin, cardMax, IM_COL32(230, 230, 230, 255), 8.0f, 0, 1.0f);

        // Barra lateral color de especialidad
        std::string sp = g.specialty.value_or("");
        ImU32 spColor = ColorForSpecialty(sp);
        dl->AddRectFilled(cardMin,
                            ImVec2(cardMin.x + 5.0f, cardMax.y),
                            spColor, 8.0f, ImDrawFlags_RoundCornersLeft);

        // Contenido
        ImGui::SetCursorScreenPos(ImVec2(pos.x + 20.0f, pos.y + 12.0f));
        ImGui::BeginGroup();

        // Linea 1: ID + especialidad + ano
        char meta[256];
        snprintf(meta, sizeof(meta), "%s  |  %s  |  %d",
                    g.id.c_str(),
                    sp.empty() ? "General" : sp.c_str(),
                    g.year.value_or(0));

        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(120, 120, 120, 255));
        if (fontSmall) ImGui::PushFont(fontSmall);
        ImGui::Text("%s", meta);
        if (fontSmall) ImGui::PopFont();
        ImGui::PopStyleColor();

        // Titulo
        if (fontMedium) ImGui::PushFont(fontMedium);
        ImGui::PushStyleColor(ImGuiCol_Text, TextTitle);
        ImGui::PushTextWrapPos(pos.x + availW - 180.0f);
        ImGui::Text("%s", g.title.c_str());
        ImGui::PopTextWrapPos();
        ImGui::PopStyleColor();
        if (fontMedium) ImGui::PopFont();

        // Tags
        ImGui::Dummy(ImVec2(0, 4));
        std::string tagStr;
        for (size_t t = 0; t < g.tags.size() && t < 5; ++t) {
            if (t > 0) tagStr += "  ";
            tagStr += "#" + g.tags[t];
        }
        if (!tagStr.empty()) {
            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(140, 140, 140, 255));
            if (fontSmall) ImGui::PushFont(fontSmall);
            ImGui::Text("%s", tagStr.c_str());
            if (fontSmall) ImGui::PopFont();
            ImGui::PopStyleColor();
        }

        ImGui::EndGroup();

        // Boton "Ver detalles" a la derecha
        ImGui::SetCursorScreenPos(ImVec2(pos.x + availW - 160.0f, pos.y + cardH/2 - 18.0f));
        ImGui::PushStyleColor(ImGuiCol_Button, spColor);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
            IM_COL32(((spColor >> IM_COL32_R_SHIFT) & 0xFF) * 0.9f,
                        ((spColor >> IM_COL32_G_SHIFT) & 0xFF) * 0.9f,
                        ((spColor >> IM_COL32_B_SHIFT) & 0xFF) * 0.9f, 255));
        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 255, 255));
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 6.0f);

        char btnId[64];
        snprintf(btnId, sizeof(btnId), "Ver detalles  >##btn%zu", i);
        if (ImGui::Button(btnId, ImVec2(140, 36))) {
            ctx.activeGuideline = g;
            ctx.NavigateTo(AppView::GuidelineDetail);
        }

        ImGui::PopStyleVar();
        ImGui::PopStyleColor(3);

        // Avanzar cursor
        ImGui::SetCursorScreenPos(ImVec2(pos.x, pos.y + cardH + 12.0f));
    }

    if (ctx.guidelinesList.empty()) {
        ImGui::Dummy(ImVec2(0, 40));
        ImGui::PushStyleColor(ImGuiCol_Text, TextSecondary);
        ImGui::TextWrapped("Sin resultados. Prueba con otro filtro.");
        ImGui::PopStyleColor();
    }

    ImGui::EndChild();
}

// ============================================================
// GuidelineDetailView::Render
// ============================================================
void GuidelineDetailView::Render(AppContext& ctx,
                                    ImFont* fontHuge, ImFont* fontLarge,
                                    ImFont* fontMedium, ImFont* fontRegular,
                                    ImFont* fontSmall) {

    if (!ctx.activeGuideline.has_value()) {
        ImGui::Text("No hay guia seleccionada");
        return;
    }

    const auto& g = ctx.activeGuideline.value();

    // ---- Header con boton volver ----
    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(100, 181, 246, 255));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(66, 165, 245, 255));
    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 255, 255));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 8.0f);
    if (ImGui::Button("< Volver a Guias", ImVec2(160, 36))) {
        ctx.NavigateBack();
    }
    ImGui::PopStyleVar();
    ImGui::PopStyleColor(3);

    ImGui::Dummy(ImVec2(0, 12));

    // ---- ID + especialidad + ano ----
    std::string sp = g.specialty.value_or("General");
    ImU32 spColor = ColorForSpecialty(sp);

    if (fontSmall) ImGui::PushFont(fontSmall);
    ImGui::PushStyleColor(ImGuiCol_Text, spColor);
    ImGui::Text("%s  |  %s  |  %s  |  %d",
                g.id.c_str(),
                g.organization.value_or("IMSS").c_str(),
                sp.c_str(),
                g.year.value_or(0));
    ImGui::PopStyleColor();
    if (fontSmall) ImGui::PopFont();

    // ---- Titulo grande ----
    if (fontLarge) ImGui::PushFont(fontLarge);
    ImGui::PushStyleColor(ImGuiCol_Text, TextTitle);
    ImGui::PushTextWrapPos(0);
    ImGui::Text("%s", g.title.c_str());
    ImGui::PopTextWrapPos();
    ImGui::PopStyleColor();
    if (fontLarge) ImGui::PopFont();

    ImGui::Dummy(ImVec2(0, 20));
    ImGui::Separator();
    ImGui::Dummy(ImVec2(0, 12));

    // ---- Resumen ----
    if (g.summary.has_value() && !g.summary.value().empty()) {
        if (fontMedium) ImGui::PushFont(fontMedium);
        ImGui::PushStyleColor(ImGuiCol_Text, TextTitle);
        ImGui::Text("RESUMEN");
        ImGui::PopStyleColor();
        if (fontMedium) ImGui::PopFont();

        ImGui::Dummy(ImVec2(0, 8));

        if (fontRegular) ImGui::PushFont(fontRegular);
        ImGui::PushStyleColor(ImGuiCol_Text, TextPrimary);
        ImGui::PushTextWrapPos(0);
        ImGui::TextWrapped("%s", g.summary.value().c_str());
        ImGui::PopTextWrapPos();
        ImGui::PopStyleColor();
        if (fontRegular) ImGui::PopFont();

        ImGui::Dummy(ImVec2(0, 20));
    }

    // ---- Recomendaciones clave ----
    if (!g.recommendations.empty()) {
        if (fontMedium) ImGui::PushFont(fontMedium);
        ImGui::PushStyleColor(ImGuiCol_Text, TextTitle);
        ImGui::Text("RECOMENDACIONES CLAVE  (%zu)", g.recommendations.size());
        ImGui::PopStyleColor();
        if (fontMedium) ImGui::PopFont();

        ImGui::Dummy(ImVec2(0, 12));

        for (const auto& rec : g.recommendations) {
            ImDrawList* dl = ImGui::GetWindowDrawList();
            ImVec2 pos = ImGui::GetCursorScreenPos();
            float availW = ImGui::GetContentRegionAvail().x;

            // Badge de nivel (A/B/C)
            ImU32 levelColor = ColorForLevel(rec.level);
            ImVec2 badgeMin = pos;
            ImVec2 badgeMax = ImVec2(pos.x + 40.0f, pos.y + 40.0f);
            dl->AddRectFilled(badgeMin, badgeMax, levelColor, 6.0f);

            ImGui::SetCursorScreenPos(ImVec2(pos.x + 14.0f, pos.y + 10.0f));
            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 255, 255));
            if (fontMedium) ImGui::PushFont(fontMedium);
            ImGui::Text("%s", rec.level.c_str());
            if (fontMedium) ImGui::PopFont();
            ImGui::PopStyleColor();

            // Texto de recomendacion
            ImGui::SetCursorScreenPos(ImVec2(pos.x + 56.0f, pos.y + 4.0f));
            ImGui::PushStyleColor(ImGuiCol_Text, TextPrimary);
            if (fontRegular) ImGui::PushFont(fontRegular);
            ImGui::PushTextWrapPos(pos.x + availW - 8.0f);
            ImGui::TextWrapped("%s", rec.text.c_str());
            ImGui::PopTextWrapPos();
            if (fontRegular) ImGui::PopFont();
            ImGui::PopStyleColor();

            ImGui::Dummy(ImVec2(0, 8));
        }

        ImGui::Dummy(ImVec2(0, 20));
    }

    // ---- Tags ----
    if (!g.tags.empty()) {
        if (fontMedium) ImGui::PushFont(fontMedium);
        ImGui::PushStyleColor(ImGuiCol_Text, TextTitle);
        ImGui::Text("TAGS");
        ImGui::PopStyleColor();
        if (fontMedium) ImGui::PopFont();

        ImGui::Dummy(ImVec2(0, 8));

        for (const auto& tag : g.tags) {
            ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(230, 240, 250, 255));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(210, 230, 250, 255));
            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(50, 100, 150, 255));
            ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 12.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 5));

            std::string tagLabel = "#" + tag;
            ImGui::SmallButton(tagLabel.c_str());

            ImGui::PopStyleVar(2);
            ImGui::PopStyleColor(3);
            ImGui::SameLine();
        }
        ImGui::NewLine();

        ImGui::Dummy(ImVec2(0, 20));
    }

    // ---- URL (footer) ----
    if (g.url.has_value() && !g.url.value().empty()) {
        ImGui::Separator();
        ImGui::Dummy(ImVec2(0, 12));
        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(66, 133, 244, 255));
        if (fontSmall) ImGui::PushFont(fontSmall);
        ImGui::Text("Fuente oficial: %s", g.url.value().c_str());
        if (fontSmall) ImGui::PopFont();
        ImGui::PopStyleColor();
    }
}

} // namespace ENARM::GUI::Views