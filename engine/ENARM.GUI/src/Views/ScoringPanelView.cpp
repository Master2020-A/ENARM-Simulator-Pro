#include "ENARM/GUI/Views/ScoringPanelView.h"
#include "ENARM/GUI/AppContext.h"
#include "ENARM/GUI/Theme.h"

#include <imgui.h>
#include <cmath>
#include <cstdio>

#ifndef IM_PI
#  define IM_PI 3.14159265358979323846f
#endif

using CD = enarm::assessment::CompetencyDimension;

namespace ENARM::GUI {

using namespace Theme::Colors;

// Colores por dimension (mismos gradientes que el dashboard)
static const ImVec4 COL_INTERVIEW  = ImVec4(0.55f, 0.45f, 0.90f, 1);   // Purple
static const ImVec4 COL_PHYSICAL   = ImVec4(0.95f, 0.30f, 0.55f, 1);   // Pink
static const ImVec4 COL_STUDIES    = ImVec4(0.98f, 0.55f, 0.30f, 1);   // Orange
static const ImVec4 COL_DIAGNOSIS  = ImVec4(0.98f, 0.72f, 0.20f, 1);   // Gold
static const ImVec4 COL_TREATMENT  = ImVec4(0.20f, 0.75f, 0.60f, 1);   // Green
static const ImVec4 COL_FOLLOWUP   = ImVec4(0.30f, 0.72f, 0.92f, 1);   // Blue

void ScoringPanelView::render(AppContext& ctx) {
    if (!ctx.assessmentEngine) return;

    // Panel con sombra + fondo blanco
    ImDrawList* dl = ImGui::GetWindowDrawList();
    ImVec2 pos = ImGui::GetCursorScreenPos();
    float w = ImGui::GetContentRegionAvail().x;
    float h = ImGui::GetContentRegionAvail().y - 4;

    Theme::DrawShadowedRect(dl,
        ImVec2(pos.x, pos.y + 2),
        ImVec2(pos.x + w, pos.y + h + 2),
        14.0f,
        ImGui::ColorConvertFloat4ToU32(ShadowStrong));

    ImGui::PushStyleColor(ImGuiCol_ChildBg, BgPanel);
    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 14.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(18, 18));

    if (!ImGui::BeginChild("##scoringpanel", ImVec2(w, h), true,
                            ImGuiWindowFlags_NoScrollbar)) {
        ImGui::EndChild();
        ImGui::PopStyleVar(2);
        ImGui::PopStyleColor();
        return;
    }

    ImGui::PushStyleColor(ImGuiCol_Text, TextTitle);
    ImGui::SetWindowFontScale(1.10f);
    ImGui::Text("Evaluacion en Tiempo Real");
    ImGui::SetWindowFontScale(1.0f);
    ImGui::PopStyleColor();

    ImGui::PushStyleColor(ImGuiCol_Separator, Border);
    ImGui::Separator();
    ImGui::PopStyleColor();
    ImGui::Spacing();

    float pct    = ctx.assessmentEngine->getCurrentScorePercent();
    float earned = static_cast<float>(ctx.assessmentEngine->getCurrentScorePoints());
    float maxPt  = 185.0f;

    renderScoreCircle(earned, maxPt);

    const char* nivel     = "Insuficiente";
    ImVec4      nivelCol  = StatusCritical;
    if      (pct >= 90.0f) { nivel = "Excelente";  nivelCol = StatusNormal; }
    else if (pct >= 70.0f) { nivel = "Bueno";       nivelCol = Accent; }
    else if (pct >= 50.0f) { nivel = "Adecuado";    nivelCol = StatusWarning; }

    float textW = ImGui::CalcTextSize(nivel).x;
    ImGui::SetCursorPosX((w - textW) * 0.5f);
    ImGui::TextColored(nivelCol, "%s", nivel);
    ImGui::Spacing();

    ImGui::PushStyleColor(ImGuiCol_Separator, Border);
    ImGui::Separator();
    ImGui::PopStyleColor();
    ImGui::Spacing();

    struct DimDef { CD dim; const char* name; ImVec4 color; };
    static const DimDef dims[] = {
        { CD::ClinicalInterview,   "Interrogatorio", COL_INTERVIEW },
        { CD::PhysicalExamination, "Exploracion",    COL_PHYSICAL  },
        { CD::DiagnosticStudies,   "Estudios",       COL_STUDIES   },
        { CD::ClinicalDiagnosis,   "Diagnostico",    COL_DIAGNOSIS },
        { CD::TreatmentPlan,       "Tratamiento",    COL_TREATMENT },
        { CD::FollowUpMonitoring,  "Seguimiento",    COL_FOLLOWUP  },
    };

    auto scores = ctx.assessmentEngine->getCurrentDimensionScores();

    for (const auto& dd : dims) {
        int pts = 0, maxPts = 1;
        for (const auto& ds : scores) {
            if (ds.dimension == dd.dim) {
                pts    = ds.pointsEarned;
                maxPts = ds.maxPoints;
                break;
            }
        }
        renderDimensionBar(dd.name, pts, maxPts,
            ImGui::ColorConvertFloat4ToU32(dd.color));
    }

    ImGui::Spacing();
    ImGui::PushStyleColor(ImGuiCol_Separator, Border);
    ImGui::Separator();
    ImGui::PopStyleColor();
    ImGui::Spacing();

    renderTimeInfo(ctx);

    ImGui::Spacing();

    // Boton Finalizar - rojo pastel
    ImGui::PushStyleColor(ImGuiCol_Button,        StatusCritical);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered,  ImVec4(1.0f, 0.42f, 0.35f, 1));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive,   ImVec4(0.85f, 0.30f, 0.25f, 1));
    ImGui::PushStyleColor(ImGuiCol_Text,          TextOnColor);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 10.0f);
    if (ImGui::Button("Finalizar Sesion", ImVec2(-1.0f, 42.0f)))
        ctx.EndSession();
    ImGui::PopStyleVar();
    ImGui::PopStyleColor(4);

    ImGui::EndChild();
    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor();
}

void ScoringPanelView::renderScoreCircle(float earned, float total) {
    float pct    = (total > 0.0f) ? (earned / total) : 0.0f;
    float panelW = ImGui::GetContentRegionAvail().x;
    float radius = 52.0f;

    ImVec2 cursor = ImGui::GetCursorScreenPos();
    ImVec2 center(cursor.x + panelW * 0.5f, cursor.y + radius + 10.0f);

    ImDrawList* dl = ImGui::GetWindowDrawList();

    // Fondo del circulo
    dl->AddCircle(center, radius, ImGui::ColorConvertFloat4ToU32(Border), 64, 6.0f);

    ImVec4 arcCol4;
    if      (pct >= 0.80f) arcCol4 = StatusNormal;
    else if (pct >= 0.60f) arcCol4 = StatusWarning;
    else                   arcCol4 = StatusCritical;
    ImU32 arcColor = ImGui::ColorConvertFloat4ToU32(arcCol4);

    const float startAngle = -IM_PI * 0.5f;
    const int   totalSegs  = 60;
    const int   fillSegs   = static_cast<int>(pct * totalSegs);

    for (int i = 0; i < fillSegs; ++i) {
        float a1 = startAngle + (static_cast<float>(i)   / totalSegs) * 2.0f * IM_PI;
        float a2 = startAngle + (static_cast<float>(i+1) / totalSegs) * 2.0f * IM_PI;
        dl->AddLine(
            ImVec2(center.x + cosf(a1)*radius, center.y + sinf(a1)*radius),
            ImVec2(center.x + cosf(a2)*radius, center.y + sinf(a2)*radius),
            arcColor, 6.5f);
    }

    // Texto centro
    char pctBuf[16];
    snprintf(pctBuf, sizeof(pctBuf), "%.0f%%", pct * 100.0f);
    ImVec2 pctSz = ImGui::CalcTextSize(pctBuf);
    dl->AddText(ImGui::GetFont(), 22.0f,
        ImVec2(center.x - pctSz.x*0.5f, center.y - 14.0f),
        ImGui::ColorConvertFloat4ToU32(TextTitle), pctBuf);

    char ptsBuf[24];
    snprintf(ptsBuf, sizeof(ptsBuf), "%.0f / %.0f", earned, total);
    ImVec2 ptsSz = ImGui::CalcTextSize(ptsBuf);
    dl->AddText(
        ImVec2(center.x - ptsSz.x*0.5f, center.y + 8.0f),
        ImGui::ColorConvertFloat4ToU32(TextSecondary), ptsBuf);

    ImGui::Dummy(ImVec2(panelW, radius * 2.0f + 22.0f));
}

void ScoringPanelView::renderDimensionBar(const char* name, int earned,
                                           int max, unsigned int color)
{
    float pct    = (max > 0) ? (float)earned / (float)max : 0.0f;
    float panelW = ImGui::GetContentRegionAvail().x;
    float scoreW = 50.0f;
    float barW   = panelW - scoreW - 8.0f;
    float barH   = 8.0f;

    ImVec4 colorF = ImGui::ColorConvertU32ToFloat4(color);
    ImGui::PushStyleColor(ImGuiCol_Text, colorF);
    ImGui::Text("%s", name);
    ImGui::PopStyleColor();

    ImVec2 barPos = ImGui::GetCursorScreenPos();
    ImVec2 barEnd = ImVec2(barPos.x + barW, barPos.y + barH);

    ImDrawList* dl = ImGui::GetWindowDrawList();
    // Fondo barra: gris claro
    dl->AddRectFilled(barPos, barEnd,
                      ImGui::ColorConvertFloat4ToU32(BgSubtle), 4.0f);
    if (pct > 0.0f)
        dl->AddRectFilled(barPos,
                          ImVec2(barPos.x + barW*pct, barPos.y + barH),
                          color, 4.0f);

    char buf[12];
    snprintf(buf, sizeof(buf), "%d/%d", earned, max);
    ImGui::SetCursorScreenPos(ImVec2(barEnd.x + 6.0f, barPos.y - 3.0f));
    ImGui::PushStyleColor(ImGuiCol_Text, TextDim);
    ImGui::Text("%s", buf);
    ImGui::PopStyleColor();

    ImGui::SetCursorScreenPos(ImVec2(barPos.x, barPos.y + barH + 12.0f));
}

void ScoringPanelView::renderTimeInfo(AppContext& ctx) {
    int min = static_cast<int>(ctx.sessionElapsedSeconds) / 60;
    int sec = static_cast<int>(ctx.sessionElapsedSeconds) % 60;

    ImGui::PushStyleColor(ImGuiCol_Text, TextSecondary);
    ImGui::Text("Tiempo: %02d:%02d", min, sec);
    ImGui::PopStyleColor();

    float elapsedMin = ctx.sessionElapsedSeconds / 60.0f;
    ImVec4 col; const char* txt;
    if      (elapsedMin <= 20.0f) { col = StatusNormal;  txt = "Ritmo: Rapido"; }
    else if (elapsedMin <= 25.0f) { col = Accent;        txt = "Ritmo: Adecuado"; }
    else                          { col = StatusWarning; txt = "Ritmo: Lento"; }
    ImGui::TextColored(col, "%s", txt);
}

} // namespace ENARM::GUI
