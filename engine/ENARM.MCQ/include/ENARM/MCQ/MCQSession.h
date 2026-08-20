// MCQSession.h - ENARM Simulator Pro
#pragma once
#include "MCQTypes.h"

namespace ENARM::MCQ {

class MCQSession {
public:
    MCQSession(MCQMode mode,
               std::vector<MCQQuestion> questions,
               std::chrono::minutes timeLimit = std::chrono::minutes(0));

    // Flujo
    std::optional<MCQQuestion> GetCurrentQuestion() const;
    bool SubmitAnswer(char answer);
    bool NextQuestion();
    bool PreviousQuestion();
    void SkipCurrent();

    // Estado
    size_t GetCurrentIndex() const { return m_currentIdx; }
    size_t GetTotalQuestions() const { return m_questions.size(); }
    bool HasNext() const;
    bool HasPrevious() const;
    bool IsFinished() const { return m_finished; }
    bool WasAnswered(size_t idx) const;

    // Tiempo
    std::chrono::seconds GetElapsed() const;
    std::chrono::seconds GetRemaining() const;
    bool IsTimeExpired() const;

    // Feedback (solo modo Practice)
    std::optional<std::string> GetFeedbackForCurrent() const;
    std::optional<MCQAttempt> GetAttemptForCurrent() const;

    // Estado
    MCQMode GetMode() const { return m_mode; }
    SessionStats GetLiveStats() const;
    UUID GetSessionId() const { return m_sessionId; }

    // Finalizar
    FinalReport Finish();

private:
    MCQMode m_mode;
    std::vector<MCQQuestion> m_questions;
    std::vector<MCQAttempt> m_attempts;
    std::vector<bool> m_answered;
    UUID m_sessionId;
    size_t m_currentIdx{0};
    std::chrono::system_clock::time_point m_startTime;
    std::chrono::minutes m_timeLimit;
    bool m_finished{false};

    std::string ComputePerformanceLevel(double scorePercent) const;
};

} // namespace ENARM::MCQ