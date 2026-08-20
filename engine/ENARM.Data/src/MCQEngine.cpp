#include "ENARM/Data/MCQEngine.h"

#include <algorithm>
#include <cctype>

namespace ENARM::Data {

    MCQEngine::MCQEngine(std::shared_ptr<DatabaseManager> db)
        : m_repo(std::make_unique<MCQRepository>(std::move(db))) {}

    MCQSession MCQEngine::StartPractice(const std::string& specialty,
                                        const std::string& difficulty,
                                        int limit) {
        MCQSession s;
        s.mode = "practice";
        s.specialty = specialty;
        s.difficulty = difficulty;

        if (!specialty.empty() && !difficulty.empty()) {
            s.questions = m_repo->FindPracticeSet(specialty, difficulty, limit);
        } else if (!specialty.empty()) {
            s.questions = m_repo->FindBySpecialty(specialty, limit);
        } else {
            s.questions = m_repo->FindRandom(limit);
        }
        return s;
    }

    MCQSession MCQEngine::StartWeakAreaSession(const std::string& studentId, int limit) {
        MCQSession s;
        s.mode = "weak";
        s.questions = m_repo->FindWeakAreaSet(studentId, limit);
        return s;
    }

    MCQAnswerResult MCQEngine::Answer(MCQSession& session,
                                      char selectedAnswer,
                                      double timeSeconds,
                                      const std::string& studentId,
                                      const std::string& sessionId) {
        MCQAnswerResult r;
        if (session.IsFinished() || session.questions.empty()) return r;

        r.question = &session.questions[session.current];
        r.selected = static_cast<char>(std::toupper(static_cast<unsigned char>(selectedAnswer)));
        r.correct  = (r.selected == r.question->correctAnswer);
        r.timeSeconds = timeSeconds;
        r.sessionIndex = static_cast<int>(session.current);
        r.sessionTotal = static_cast<int>(session.questions.size());

        // Feedback general
        if (r.correct) {
            r.feedback = r.question->explanationCorrect.value_or(
                "Respuesta correcta.");
        } else {
            r.feedback = "Respuesta incorrecta. La correcta es "
                         + std::string(1, r.question->correctAnswer) + "."
                         + (r.question->explanationCorrect.has_value()
                            ? " " + r.question->explanationCorrect.value() : "");
            r.feedback += "\n\n" + FeedbackForOption(*r.question, r.selected);
        }

        session.history.push_back(r);
        ++session.answered;
        if (r.correct) ++session.correct;
        session.totalTimeSeconds += timeSeconds;

        // Registrar intento en BD si tenemos contexto
        if (!studentId.empty()) {
            m_repo->RecordAttempt(studentId, r.question->id, sessionId,
                                  r.selected, r.correct, timeSeconds);
        }
        return r;
    }

    void MCQEngine::Next(MCQSession& session) {
        if (!session.IsFinished()) ++session.current;
    }

    std::string MCQEngine::FeedbackForOption(const MCQuestion& q, char option) {
        std::string key(1, static_cast<char>(std::toupper(static_cast<unsigned char>(option))));
        if (q.explanationIncorrect.is_object() && q.explanationIncorrect.contains(key)) {
            return q.explanationIncorrect[key].get<std::string>();
        }
        return "Sin detalle adicional para esta opcion.";
    }

}