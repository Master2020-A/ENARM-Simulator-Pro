// ═══════════════════════════════════════════════════════════════
//   ENARM.GUI - ECGMonitorView Implementation
// ═══════════════════════════════════════════════════════════════
#include "ENARM/GUI/Views/ECGMonitorView.h"
#include "ENARM/GUI/Theme.h"

#include <algorithm>
#include <vector>

namespace ENARM::GUI::Views {

    // Estado estatico compartido entre frames
    static Physiology::ECGSynthesizer g_synth;
    static Physiology::ECGRhythm g_lastRhythm = Physiology::ECGRhythm::SinusRhythm;

    ECGMonitorView::ECGMonitorView() = default;

    void ECGMonitorView::Render(AppContext& ctx,
                                ImFont* fontHuge, ImFont* fontLarge,
                                ImFont* fontMedium, ImFont* fontRegular,
                                ImFont* fontSmall) {
        static ECGMonitorView s_view;

        // Selector de ritmo
        s_view.RenderRhythmSelector();
        ImGui::Spacing();

        // Header con nombre del ritmo
        if (fontMedium) ImGui::PushFont(fontMedium);
        ImGui::TextColored(ImVec4(0.30f, 0.85f, 0.60f, 1.0f),
            "Ritmo: %s",
            Physiology::ECGSynthesizer::RhythmName(g_synth.GetRhythm()).c_str());
        if (fontMedium) ImGui::PopFont();
        ImGui::Spacing();

        s_view.Render12LeadGrid();

        // Legenda
        if (fontSmall) ImGui::PushFont(fontSmall);
        ImGui::TextColored(ImVec4(0.6f, 0.6f, 0.6f, 1.0f),
            "Papel: 25 mm/s | 10 mm/mV | Elevacion ST visible en derivaciones correspondientes");
        ImGui::PopFont();
        (void)ctx; (void)fontHuge; (void)fontLarge; (void)fontRegular;
    }

    void ECGMonitorView::RenderRhythmSelector() {
        const char* rhythms[] = {
            "Ritmo sinusal", "Taquicardia sinusal", "Fibrilacion auricular",
            "IAMCEST anterior (V1-V4)", "IAMCEST inferior (DII, DIII, aVF)",
            "Bloqueo AV 1er grado", "Fibrilacion ventricular"
        };

        ImGui::Text("Ritmo:");
        ImGui::SameLine();
        int currentIdx = static_cast<int>(g_synth.GetRhythm());
        if (ImGui::Combo("##rhythm", &currentIdx, rhythms, IM_ARRAYSIZE(rhythms))) {
            g_synth.SetRhythm(static_cast<Physiology::ECGRhythm>(currentIdx));
        }
    }

    void ECGMonitorView::Render12LeadGrid() {
        const float plotH = 60.0f;
        const int perRow = 4;

        for (int i = 0; i < 12; ++i) {
            if (i > 0 && (i % perRow) != 0) ImGui::SameLine(0, 8.0f);

            float avail = ImGui::GetContentRegionAvail().x;
            float plotW = (avail - (perRow - 1) * 8.0f) / perRow;

            ImGui::BeginGroup();
            ImGui::TextColored(ImVec4(0.5f, 0.7f, 0.9f, 1.0f), "%s", m_leadNames[i]);
            char plotId[32];
            snprintf(plotId, sizeof(plotId), "##ecg_%d", i);
            if (ImPlot::BeginPlot(plotId, ImVec2(plotW, plotH))) {
                ImPlot::SetupAxes(nullptr, nullptr,
                    ImPlotAxisFlags_NoDecorations | ImPlotAxisFlags_NoTickLabels,
                    ImPlotAxisFlags_NoDecorations | ImPlotAxisFlags_NoTickLabels);
                ImPlot::SetupAxisLimits(ImAxis_X1, 0, 3, ImPlotCond_Always);
                ImPlot::SetupAxisLimits(ImAxis_Y1, -1.5, 1.5, ImPlotCond_Always);

                // Generar ventana de 3 segundos
                const double sr = 250.0;
                auto samples = g_synth.Generate(m_leadNames[i], 3.0, sr);
                std::vector<float> xs, ys;
                xs.reserve(samples.size());
                ys.reserve(samples.size());
                for (const auto& s : samples) {
                    xs.push_back((float)s.time_s);
                    ys.push_back((float)s.voltage_mV);
                }
                ImPlot::PlotLine("##line", xs.data(), ys.data(), (int)xs.size());
                ImPlot::EndPlot();
            }
            ImGui::EndGroup();
        }
    }

} // namespace ENARM::GUI::Views
