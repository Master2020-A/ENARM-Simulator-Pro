#pragma once
#include <enarm/assessment/RubricDefinition.h>
#include <vector>

namespace enarm::assessment {

// Calcula score global y genera fortalezas/debilidades
class ScoreCalculator {
public:
    // Consolida vector de DimensionScore en totales globales
    static void calculate(PerformanceReport& report);

    // Detecta fortalezas (>= 80 %) y debilidades (< 60 %)
    static void analyzePerformance(PerformanceReport& report);

    // Genera recomendaciones segun debilidades
    static void generateRecommendations(PerformanceReport& report);

    // Decide si el tiempo fue eficiente (< 1.5x tiempo esperado)
    static void evaluateTime(PerformanceReport& report,
                             const CaseRubric&   rubric);
};

} // namespace enarm::assessment
