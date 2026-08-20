// ═══════════════════════════════════════════════════════════════
//   ENARM.Learning - WeaknessDetector + AdaptiveCaseSelector
//   Detecta patrones de error y sugiere el proximo caso optimo
// ═══════════════════════════════════════════════════════════════
#pragma once

#include <string>
#include <unordered_map>
#include <vector>

namespace ENARM::Learning {

    // Resultado de evaluacion de una competencia
    struct CompetencyPerformance {
        std::string competency;   // e.g. "Cardiologia", "Interrogatorio"
        int attempts{0};
        int correct{0};
        double score{0.0};        // 0-100

        [[nodiscard]] double Percent() const {
            return attempts > 0 ? (100.0 * correct / attempts) : 0.0;
        }
    };

    // Caso candidato para seleccion adaptativa
    struct CaseCandidate {
        std::string caseId;
        std::string specialty;
        std::string difficulty;
        double weaknessScore{0.0};   // Que tanto ataca las debilidades
        double dueScore{0.0};        // Prioridad por repaso (0-1)
        double combinedScore{0.0};
    };

    class WeaknessDetector {
    public:
        WeaknessDetector();

        // Registrar un resultado de pregunta/caso
        void RecordAttempt(const std::string& specialty,
                           const std::string& competency,
                           bool isCorrect);

        // Actualizar puntaje global por competencia
        void UpdatePerformance(const std::string& competency,
                               double percent);

        // Competencias debiles (percent < 60)
        [[nodiscard]] std::vector<CompetencyPerformance> WeakAreas() const;

        // Score de debilidad de una especialidad (0-1, 1=muy debil)
        [[nodiscard]] double WeaknessScore(const std::string& specialty) const;

        [[nodiscard]] const std::unordered_map<std::string, CompetencyPerformance>&
            All() const noexcept { return m_perf; }

        void Reset();

    private:
        std::unordered_map<std::string, CompetencyPerformance> m_perf;
    };

    class AdaptiveCaseSelector {
    public:
        AdaptiveCaseSelector();

        // Selecciona el proximo caso optimo para el estudiante
        // casePool: casos disponibles; weaknesses: debilidades detectadas
        [[nodiscard]] CaseCandidate SelectNext(
            const std::vector<CaseCandidate>& casePool,
            const WeaknessDetector& detector) const;

        // Recomendaciones diarias
        [[nodiscard]] std::vector<std::string> DailyRecommendations(
            const WeaknessDetector& detector) const;
    };

}