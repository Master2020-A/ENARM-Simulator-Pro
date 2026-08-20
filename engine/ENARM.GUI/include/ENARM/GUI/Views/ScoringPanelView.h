#pragma once
namespace ENARM::GUI {
    struct AppContext;

    class ScoringPanelView {
    public:
        void render(AppContext& ctx);
    private:
        void renderScoreCircle(float earned, float total);
        void renderDimensionBar(const char* name, int earned, int max, unsigned int color);
        void renderTimeInfo(AppContext& ctx);
    };
}
