// ═══════════════════════════════════════════════════════════════
//   ENARM.Learning - SpacedRepetition (FSRS)
//   Programa repaso optimo segun el desempeno del estudiante
// ═══════════════════════════════════════════════════════════════
#pragma once

#include <chrono>
#include <string>

namespace ENARM::Learning {

    // Estado de repaso de un item (caso o pregunta)
    struct ReviewItem {
        std::string id;
        std::string specialty;
        double stability_days{0.0};    // Estabilidad actual
        int    repetitions{0};          // Repeticiones exitosas
        double ease{2.5};               // Facilidad (2.0-3.5)
        double lastIntervalDays{0.0};   // Intervalo anterior
        int    lapses{0};               // Fallos
        std::chrono::system_clock::time_point dueAt;
        bool   hasReviewed{false};
    };

    // Resultado de una revision
    struct ReviewOutcome {
        int quality{0};  // 0=olvido total, 3=dificil, 4=correcto, 5=facil
    };

    class SpacedRepetition {
    public:
        SpacedRepetition();

        // Crear item nuevo (primera vez)
        static ReviewItem NewItem(const std::string& id,
                                  const std::string& specialty = "");

        // Actualizar despues de una revision (algoritmo FSRS simplificado)
        void Review(ReviewItem& item, int quality);

        // Items que deben repasarse hoy
        [[nodiscard]] bool IsDue(const ReviewItem& item,
                                 std::chrono::system_clock::time_point now = {}) const;

        // Intervalo sugerido en dias
        [[nodiscard]] double NextIntervalDays(const ReviewItem& item) const;
    };

}