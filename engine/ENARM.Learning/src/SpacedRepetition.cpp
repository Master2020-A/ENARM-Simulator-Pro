// ═══════════════════════════════════════════════════════════════
//   ENARM.Learning - SpacedRepetition Implementation (FSRS-like)
// ═══════════════════════════════════════════════════════════════
#include "ENARM/Learning/SpacedRepetition.h"

#include <algorithm>
#include <cmath>

namespace ENARM::Learning {

    SpacedRepetition::SpacedRepetition() = default;

    ReviewItem SpacedRepetition::NewItem(const std::string& id,
                                         const std::string& specialty) {
        ReviewItem item;
        item.id = id;
        item.specialty = specialty;
        item.stability_days = 0.0;
        item.repetitions = 0;
        item.ease = 2.5;
        item.lastIntervalDays = 0.0;
        item.lapses = 0;
        item.hasReviewed = false;
        item.dueAt = std::chrono::system_clock::now();  // due inmediatamente
        return item;
    }

    void SpacedRepetition::Review(ReviewItem& item, int quality) {
        // Calidad 0-5 (0=olvido, 5=perfecto)
        quality = std::clamp(quality, 0, 5);

        if (quality < 3) {
            // Fallo: reiniciar intervalo, aumentar lapses
            ++item.lapses;
            item.repetitions = 0;
            item.ease = std::max(2.0, item.ease - 0.2);
            item.lastIntervalDays = 0.5;  // repasar en 12h
            item.stability_days = 1.0;
        } else {
            // Exito
            ++item.repetitions;
            // Ajustar facilidad (FSRS-ish)
            item.ease = std::clamp(
                item.ease + (0.1 - (5.0 - quality) * (0.08 + (5.0 - quality) * 0.02)),
                1.3, 3.5);

            // Estabilidad: crece con repeticiones exitosas
            if (item.repetitions == 1) {
                item.stability_days = 1.0;
            } else if (item.repetitions == 2) {
                item.stability_days = 6.0;
            } else {
                item.stability_days = item.lastIntervalDays * item.ease;
            }
            item.stability_days = std::clamp(item.stability_days, 1.0, 365.0);

            item.lastIntervalDays = item.stability_days;
            item.hasReviewed = true;
        }

        // Calcular fecha de vencimiento
        const auto now = std::chrono::system_clock::now();
        item.dueAt = now + std::chrono::hours(static_cast<long long>(item.lastIntervalDays * 24.0));
    }

    bool SpacedRepetition::IsDue(const ReviewItem& item,
                                 std::chrono::system_clock::time_point now) const {
        if (now == std::chrono::system_clock::time_point{}) {
            now = std::chrono::system_clock::now();
        }
        return item.dueAt <= now;
    }

    double SpacedRepetition::NextIntervalDays(const ReviewItem& item) const {
        return item.lastIntervalDays > 0.0 ? item.lastIntervalDays : 1.0;
    }

}