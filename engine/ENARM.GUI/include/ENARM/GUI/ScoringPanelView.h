#pragma once

// Forward declaration - no incluir AppContext completo aqui
struct ImDrawList;

namespace enarm::gui {
// Forward
struct AppContext;

class ScoringPanelView {
public:
    void render(AppContext& ctx);

private:
    void renderScoreCircle(float earned, float total);
    void renderDimensionBar(const char* name,
                            int          earned,
                            int          max,
                            unsigned int color);
    void renderTimeInfo(AppContext& ctx);
};

} // namespace enarm::gui
