// ═══════════════════════════════════════════════════════════════
//   ENARM.Data - MCQEngine (Sprint 8: Modo Practica)
//   Sesion de preguntas estilo ENARM con retroalimentacion
// ═══════════════════════════════════════════════════════════════
#pragma once

#include "ENARM/Data/Repositories/MCQRepository.h"

#include <memory>
#include <string>
#include <vector>

namespace ENARM::Data {

    // Resultado de una pregunta respondida
    struct MCQAnswerResult {
        const MCQuestion* question{nullptr};
        char selected{'?'};
        bool correct{false};
        std::string feedback;       // Explicacion mostrada al estudiante
        double timeSeconds{0.0};
        int sessionIndex{0};
        int sessionTotal{0};
    };

    // Sesion de practica completa
    struct MCQSession {
        std::string mode;              // "practice" | "specialty" | "weak"
        std::string specialty;
        std::string difficulty;
        std::vector<MCQuestion> questions;
        size_t current{0};
        int answered{0};
        int correct{0};
        double totalTimeSeconds{0.0};
        std::vector<MCQAnswerResult> history;

        [[nodiscard]] bool IsFinished() const { return current >= questions.size(); }
        [[nodiscard]] double Percent() const {
            return answered > 0 ? (100.0 * correct / answered) : 0.0;
        }
    };

    class MCQEngine {
    public:
        explicit MCQEngine(std::shared_ptr<DatabaseManager> db);

        // Iniciar sesion de practica libre (especialidad + dificultad opcional)
        MCQSession StartPractice(const std::string& specialty = "",
                                 const std::string& difficulty = "",
                                 int limit = 20);

        // Modo repaso: preguntas falladas por el estudiante
        MCQSession StartWeakAreaSession(const std::string& studentId, int limit = 30);

        // Responder la pregunta actual; registra en BD si hay studentId
        MCQAnswerResult Answer(MCQSession& session,
                               char selectedAnswer,
                               double timeSeconds,
                               const std::string& studentId = "",
                               const std::string& sessionId = "");

        // Avanzar a la siguiente pregunta
        void Next(MCQSession& session);

        // Feedback por opcion (por que cada opcion es correcta/incorrecta)
        static std::string FeedbackForOption(const MCQuestion& q, char option);

    private:
        std::shared_ptr<MCQRepository> m_repo;
    };

}