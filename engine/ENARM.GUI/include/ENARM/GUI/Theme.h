// ═══════════════════════════════════════════════════════════════
//   ENARM.GUI - Theme (Hero Medical Dashboard)
// ═══════════════════════════════════════════════════════════════
#pragma once

#include <cstdint>
#include <imgui.h>

namespace ENARM::GUI::Theme {

    namespace Colors {
        // ─── Backgrounds claros modernos ───
        inline constexpr ImVec4 BgApp        {0.96f, 0.97f, 0.99f, 1.00f};   // gris muy claro
        inline constexpr ImVec4 BgPanel      {1.00f, 1.00f, 1.00f, 1.00f};   // blanco
        inline constexpr ImVec4 BgPanelAlt   {0.98f, 0.98f, 0.99f, 1.00f};
        inline constexpr ImVec4 BgHover      {0.92f, 0.94f, 0.97f, 1.00f};
        inline constexpr ImVec4 BgSubtle     {0.94f, 0.96f, 0.98f, 1.00f};

        // ─── Textos ───
        inline constexpr ImVec4 TextTitle    {0.10f, 0.14f, 0.22f, 1.00f};   // casi negro
        inline constexpr ImVec4 TextPrimary  {0.20f, 0.24f, 0.32f, 1.00f};
        inline constexpr ImVec4 TextSecondary{0.45f, 0.50f, 0.58f, 1.00f};
        inline constexpr ImVec4 TextDim      {0.60f, 0.64f, 0.70f, 1.00f};
        inline constexpr ImVec4 TextOnColor  {1.00f, 1.00f, 1.00f, 1.00f};   // sobre gradientes
        inline constexpr ImVec4 TextOnColorDim{1.00f, 1.00f, 1.00f, 0.80f};

        // ─── Accent (azul medico principal) ───
        inline constexpr ImVec4 Accent       {0.30f, 0.65f, 0.85f, 1.00f};
        inline constexpr ImVec4 AccentHover  {0.35f, 0.70f, 0.90f, 1.00f};
        inline constexpr ImVec4 AccentDark   {0.20f, 0.50f, 0.70f, 1.00f};

        // ─── HERO header (gradiente cyan-azul) ───
        inline constexpr ImVec4 HeroTop      {0.35f, 0.72f, 0.85f, 1.00f};
        inline constexpr ImVec4 HeroBottom   {0.25f, 0.62f, 0.78f, 1.00f};

        // ─── CARDS con gradientes vibrantes (top -> bottom) ───
        // Card azul cyan
        inline constexpr ImVec4 CardBlueTop     {0.30f, 0.72f, 0.92f, 1.00f};
        inline constexpr ImVec4 CardBlueBottom  {0.20f, 0.55f, 0.80f, 1.00f};

        // Card rosa/rojo
        inline constexpr ImVec4 CardPinkTop     {0.95f, 0.30f, 0.55f, 1.00f};
        inline constexpr ImVec4 CardPinkBottom  {0.90f, 0.20f, 0.40f, 1.00f};

        // Card morado
        inline constexpr ImVec4 CardPurpleTop   {0.55f, 0.45f, 0.90f, 1.00f};
        inline constexpr ImVec4 CardPurpleBottom{0.42f, 0.32f, 0.78f, 1.00f};

        // Card verde/teal
        inline constexpr ImVec4 CardGreenTop    {0.20f, 0.75f, 0.60f, 1.00f};
        inline constexpr ImVec4 CardGreenBottom {0.15f, 0.62f, 0.50f, 1.00f};

        // Card naranja/rojo
        inline constexpr ImVec4 CardOrangeTop   {0.98f, 0.55f, 0.30f, 1.00f};
        inline constexpr ImVec4 CardOrangeBottom{0.92f, 0.40f, 0.20f, 1.00f};

        // Card dorado/amarillo
        inline constexpr ImVec4 CardGoldTop     {0.98f, 0.72f, 0.20f, 1.00f};
        inline constexpr ImVec4 CardGoldBottom  {0.92f, 0.58f, 0.10f, 1.00f};

        // ─── Estados clinicos (semaforos) ───
        inline constexpr ImVec4 StatusNormal   {0.20f, 0.75f, 0.45f, 1.00f};
        inline constexpr ImVec4 StatusWarning  {0.95f, 0.72f, 0.20f, 1.00f};
        inline constexpr ImVec4 StatusCritical {0.95f, 0.35f, 0.30f, 1.00f};
        inline constexpr ImVec4 StatusEmergency{1.00f, 0.15f, 0.10f, 1.00f};

        // ─── Badges y detalles ───
        inline constexpr ImVec4 BadgeBg        {1.00f, 1.00f, 1.00f, 0.25f};   // sobre card gradient
        inline constexpr ImVec4 BadgeAlergy    {1.00f, 0.90f, 0.90f, 1.00f};
        inline constexpr ImVec4 BadgeAlergyTxt {0.90f, 0.20f, 0.20f, 1.00f};

        // ─── Borders y shadows ───
        inline constexpr ImVec4 Border         {0.88f, 0.90f, 0.93f, 1.00f};
        inline constexpr ImVec4 Shadow         {0.10f, 0.14f, 0.22f, 0.08f};
        inline constexpr ImVec4 ShadowStrong   {0.10f, 0.14f, 0.22f, 0.15f};
    }

    // Enum de tipos de card gradient
    enum class CardGradient : uint8_t {
        Blue,
        Pink,
        Purple,
        Green,
        Orange,
        Gold
    };

    // Aplicar tema
    void ApplyHeroTheme();

    // Obtener colores de un gradiente
    void GetGradientColors(CardGradient g, ImVec4& top, ImVec4& bottom);

    // Helpers de dibujo
    void DrawShadowedRect(ImDrawList* dl, ImVec2 min, ImVec2 max, float rounding, ImU32 shadowColor);
    void DrawGradientRect(ImDrawList* dl, ImVec2 min, ImVec2 max,
                           ImU32 topColor, ImU32 bottomColor, float rounding);
    void DrawGradientRectV(ImDrawList* dl, ImVec2 min, ImVec2 max, ImU32 topColor, ImU32 bottomColor);

    ImU32 ColorForVitalStatus(int severity);
    const char* IconForVitalStatus(int severity);
}