// ═══════════════════════════════════════════════════════════════
//   ENARM.Learning - WeaknessDetector + AdaptiveCaseSelector impl
// ═══════════════════════════════════════════════════════════════
#include "ENARM/Learning/WeaknessDetector.h"

#include <algorithm>

namespace ENARM::Learning {

    WeaknessDetector::WeaknessDetector() = default;

    void WeaknessDetector::RecordAttempt(const std::string& specialty,
                                         const std::string& competency,
                                         bool isCorrect) {
        // Registrar por especialidad
        auto& s = m_perf[specialty];
        s.competency = specialty;
        ++s.attempts;
        if (isCorrect) ++s.correct;
        s.score = s.Percent();

        // Registrar por competencia generica (si es distinta)
        if (competency != specialty) {
            auto& c = m_perf[competency];
            c.competency = competency;
            ++c.attempts;
            if (isCorrect) ++c.correct;
            c.score = c.Percent();
        }
    }

    void WeaknessDetector::UpdatePerformance(const std::string& competency,
                                             double percent) {
        auto& c = m_perf[competency];
        c.competency = competency;
        c.score = percent;
    }

    std::vector<CompetencyPerformance> WeaknessDetector::WeakAreas() const {
        std::vector<CompetencyPerformance> out;
        for (const auto& [k, v] : m_perf) {
            if (v.Percent() < 60.0 && v.attempts > 0) {
                out.push_back(v);
            }
        }
        std::sort(out.begin(), out.end(),
            [](const CompetencyPerformance& a, const CompetencyPerformance& b) {
                return a.Percent() < b.Percent();
            });
        return out;
    }

    double WeaknessDetector::WeaknessScore(const std::string& specialty) const {
        auto it = m_perf.find(specialty);
        if (it == m_perf.end() || it->second.attempts == 0) return 0.5;  // neutral
        double pct = it->second.Percent();
        return std::clamp((60.0 - pct) / 60.0, 0.0, 1.0);
    }

    void WeaknessDetector::Reset() { m_perf.clear(); }

    AdaptiveCaseSelector::AdaptiveCaseSelector() = default;

    CaseCandidate AdaptiveCaseSelector::SelectNext(
        const std::vector<CaseCandidate>& casePool,
        const WeaknessDetector& detector) const
    {
        if (casePool.empty()) return {};

        // Calcular combined score: debilidad + repaso pendiente
        std::vector<CaseCandidate> scored = casePool;
        for (auto& c : scored) {
            c.weaknessScore = detector.WeaknessScore(c.specialty);
            c.combinedScore = c.weaknessScore * 0.7 + c.dueScore * 0.3;
        }

        auto best = std::max_element(scored.begin(), scored.end(),
            [](const CaseCandidate& a, const CaseCandidate& b) {
                return a.combinedScore < b.combinedScore;
            });
        return *best;
    }

    std::vector<std::string> AdaptiveCaseSelector::DailyRecommendations(
        const WeaknessDetector& detector) const
    {
        std::vector<std::string> recs;
        auto weak = detector.WeakAreas();
        for (const auto& w : weak) {
            recs.push_back("Tu debilidad es " + w.competency +
                           " (" + std::to_string(static_cast<int>(w.Percent())) +
                           "%). Practica 3 casos de esta area.");
        }
        if (recs.empty()) {
            recs.push_back("Excelente desempeno. Sigue practicando para mantener el nivel.");
        }
        return recs;
    }

}