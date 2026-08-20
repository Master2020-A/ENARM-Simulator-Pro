// ═══════════════════════════════════════════════════════════════
//   ENARM.GUI - Theme Implementation (Hero Style)
// ═══════════════════════════════════════════════════════════════
#include "ENARM/GUI/Theme.h"
#include <imgui_internal.h>

namespace ENARM::GUI::Theme {

    void ApplyHeroTheme() {
        ImGuiStyle& style = ImGui::GetStyle();
        ImVec4* colors = style.Colors;
        using namespace Colors;

        // ─── Backgrounds ───
        colors[ImGuiCol_WindowBg]           = BgApp;
        colors[ImGuiCol_ChildBg]            = BgPanel;
        colors[ImGuiCol_PopupBg]            = BgPanel;
        colors[ImGuiCol_MenuBarBg]          = BgPanel;
        colors[ImGuiCol_TitleBg]            = BgPanel;
        colors[ImGuiCol_TitleBgActive]      = BgSubtle;
        colors[ImGuiCol_TitleBgCollapsed]   = BgPanel;

        // ─── Frames ───
        colors[ImGuiCol_FrameBg]            = BgSubtle;
        colors[ImGuiCol_FrameBgHovered]     = BgHover;
        colors[ImGuiCol_FrameBgActive]      = BgHover;

        // ─── Text ───
        colors[ImGuiCol_Text]               = TextPrimary;
        colors[ImGuiCol_TextDisabled]       = TextDim;

        // ─── Borders ───
        colors[ImGuiCol_Border]             = Border;
        colors[ImGuiCol_BorderShadow]       = ImVec4(0, 0, 0, 0);
        colors[ImGuiCol_Separator]          = Border;
        colors[ImGuiCol_SeparatorHovered]   = Accent;
        colors[ImGuiCol_SeparatorActive]    = AccentHover;

        // ─── Buttons ───
        colors[ImGuiCol_Button]             = Accent;
        colors[ImGuiCol_ButtonHovered]      = AccentHover;
        colors[ImGuiCol_ButtonActive]       = AccentDark;

        // ─── Headers ───
        colors[ImGuiCol_Header]             = BgSubtle;
        colors[ImGuiCol_HeaderHovered]      = BgHover;
        colors[ImGuiCol_HeaderActive]       = Accent;

        // ─── Tabs ───
        colors[ImGuiCol_Tab]                = BgSubtle;
        colors[ImGuiCol_TabHovered]         = Accent;

        // ─── Scrollbar ───
        colors[ImGuiCol_ScrollbarBg]        = BgApp;
        colors[ImGuiCol_ScrollbarGrab]      = ImVec4(0.75f, 0.80f, 0.85f, 1.0f);
        colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.65f, 0.72f, 0.80f, 1.0f);
        colors[ImGuiCol_ScrollbarGrabActive]  = Accent;

        // ─── Checkbox/Radio ───
        colors[ImGuiCol_CheckMark]          = Accent;
        colors[ImGuiCol_SliderGrab]         = Accent;
        colors[ImGuiCol_SliderGrabActive]   = AccentHover;

#ifdef ImGuiCol_DockingPreview
        colors[ImGuiCol_DockingPreview]     = ImVec4(Accent.x, Accent.y, Accent.z, 0.35f);
#endif
#ifdef ImGuiCol_DockingEmptyBg
        colors[ImGuiCol_DockingEmptyBg]     = BgApp;
#endif

        colors[ImGuiCol_PlotLines]          = Accent;
        colors[ImGuiCol_PlotLinesHovered]   = AccentHover;
        colors[ImGuiCol_PlotHistogram]      = Accent;
        colors[ImGuiCol_PlotHistogramHovered] = AccentHover;

        // ─── Estilo geometrico moderno ───
        style.WindowRounding    = 12.0f;
        style.ChildRounding     = 12.0f;
        style.FrameRounding     = 8.0f;
        style.PopupRounding     = 10.0f;
        style.ScrollbarRounding = 8.0f;
        style.GrabRounding      = 8.0f;
        style.TabRounding       = 8.0f;

        style.WindowPadding     = ImVec2(20, 20);
        style.FramePadding      = ImVec2(14, 10);
        style.ItemSpacing       = ImVec2(12, 12);
        style.ItemInnerSpacing  = ImVec2(8, 6);
        style.IndentSpacing     = 22;
        style.ScrollbarSize     = 14;
        style.GrabMinSize       = 12;

        style.WindowBorderSize  = 0.0f;
        style.ChildBorderSize   = 0.0f;
        style.PopupBorderSize   = 1.0f;
        style.FrameBorderSize   = 0.0f;
        style.TabBorderSize     = 0.0f;

        style.WindowTitleAlign  = ImVec2(0.0f, 0.5f);

        // Escala general (mas grande y legible)
        style.ScaleAllSizes(1.05f);
    }

    void GetGradientColors(CardGradient g, ImVec4& top, ImVec4& bottom) {
        using namespace Colors;
        switch (g) {
            case CardGradient::Blue:   top = CardBlueTop;   bottom = CardBlueBottom;   break;
            case CardGradient::Pink:   top = CardPinkTop;   bottom = CardPinkBottom;   break;
            case CardGradient::Purple: top = CardPurpleTop; bottom = CardPurpleBottom; break;
            case CardGradient::Green:  top = CardGreenTop;  bottom = CardGreenBottom;  break;
            case CardGradient::Orange: top = CardOrangeTop; bottom = CardOrangeBottom; break;
            case CardGradient::Gold:   top = CardGoldTop;   bottom = CardGoldBottom;   break;
        }
    }

    // Dibujar sombra suave con multiples layers
    void DrawShadowedRect(ImDrawList* dl, ImVec2 min, ImVec2 max, float rounding, ImU32 shadowColor) {
        // Sombra offset hacia abajo con 3 layers de opacidad decreciente
        const int layers = 4;
        for (int i = 0; i < layers; ++i) {
            float offset = (float)(i + 1);
            ImU32 col = shadowColor;
            // Reducir alpha con cada layer
            unsigned char a = (unsigned char)((shadowColor >> 24) & 0xFF);
            a = (unsigned char)(a * (layers - i) / layers);
            col = (col & 0x00FFFFFF) | ((ImU32)a << 24);

            dl->AddRectFilled(
                ImVec2(min.x - offset * 0.5f, min.y + offset),
                ImVec2(max.x + offset * 0.5f, max.y + offset * 2.0f),
                col, rounding + offset
            );
        }
    }

    // Rectangulo con gradiente vertical y bordes redondeados
    void DrawGradientRect(ImDrawList* dl, ImVec2 min, ImVec2 max,
                           ImU32 topColor, ImU32 bottomColor, float rounding) {
        // Dibujar en varios cuadrantes verticales para gradiente
        const int steps = 32;
        float height = max.y - min.y;
        float stepH = height / steps;

        for (int i = 0; i < steps; ++i) {
            float t0 = (float)i / steps;
            float t1 = (float)(i + 1) / steps;

            ImU32 c0 = ImGui::ColorConvertFloat4ToU32(
                ImLerp(ImGui::ColorConvertU32ToFloat4(topColor),
                       ImGui::ColorConvertU32ToFloat4(bottomColor), t0));
            ImU32 c1 = ImGui::ColorConvertFloat4ToU32(
                ImLerp(ImGui::ColorConvertU32ToFloat4(topColor),
                       ImGui::ColorConvertU32ToFloat4(bottomColor), t1));

            ImVec2 sMin(min.x, min.y + stepH * i);
            ImVec2 sMax(max.x, min.y + stepH * (i + 1));

            dl->AddRectFilledMultiColor(sMin, sMax, c0, c0, c1, c1);
        }

        // Overlay para simular esquinas redondeadas (mask blanca en esquinas)
        // Truco: cubrimos con drawlist con esquinas invertidas
    }

    void DrawGradientRectV(ImDrawList* dl, ImVec2 min, ImVec2 max, ImU32 topColor, ImU32 bottomColor) {
        dl->AddRectFilledMultiColor(min, max, topColor, topColor, bottomColor, bottomColor);
    }

    ImU32 ColorForVitalStatus(int severity) {
        using namespace Colors;
        switch (severity) {
            case 0: return ImGui::ColorConvertFloat4ToU32(StatusNormal);
            case 1: return ImGui::ColorConvertFloat4ToU32(StatusWarning);
            case 2: return ImGui::ColorConvertFloat4ToU32(StatusCritical);
            case 3: return ImGui::ColorConvertFloat4ToU32(StatusEmergency);
        }
        return ImGui::ColorConvertFloat4ToU32(StatusNormal);
    }

    const char* IconForVitalStatus(int severity) {
        switch (severity) {
            case 0: return "OK";
            case 1: return "!";
            case 2: return "!!";
            case 3: return "!!!";
        }
        return "?";
    }
}