// MCQSession.cpp
#include "ENARM/MCQ/MCQSession.h"
#include <algorithm>
#include <random>
#include <sstream>
#include <iomanip>

namespace ENARM::MCQ {

namespace {
    std::string GenerateUUIDLike() {
        static std::random_device rd;
        static std::mt19937_64 gen(rd());
        std::stringstream ss;
        ss << std::hex << std::setw(8) << std::setfill('0') << (gen() & 0xFFFFFFFF)
           << "-" << std::setw(4) << (gen() & 0xFFFF)
           << "-" << std::setw(4) << (gen() & 0xFFFF)
           << "-" << std::setw(4) << (gen() & 0xFFFF)
           << "-" << std::setw(12) << (gen() & 0xFFFFFFFFFFFF);
        return ss.str();
    }
}

MCQSession::MCQSession(MCQMode mode,
                       std::vector<MCQQuestion> questions,
                       std::chrono::minutes timeLimit)
    : m_mode(mode)
    , m_questions(std::move(questions))
    , m_answered(m_questions.size(), false)
    , m_sessionId(GenerateUUIDLike())
    , m_startTime(std::chrono::system_clock::now())
    , m_timeLimit(timeLimit)
{
    m_attempts.reserve(m_questions.size());
}

std::optional<MCQQuestion> MCQSession::GetCurrentQuestion() const {
    if (m_currentIdx >= m_questions.size()) return std::nullopt;
    return m_questions[m_currentIdx];
}

bool MCQSession::SubmitAnswer(char answer) {
    if (m_finished || m_currentIdx >= m_questions.size()) return false;

    const auto& q = m_questions[m_currentIdx];
    char upperAnswer = static_cast<char>(std::toupper(answer));
    bool correct = (upperAnswer == q.correctAnswer);

    auto now = std::chrono::system_clock::now();
    double timeSpent = 0.0;
    if (!m_attempts.empty()) {
        auto lastAttempt = m_attempts.back().timestamp;
        timeSpent = std::chrono::duration<double>(now - lastAttempt).count();
    } else {
        timeSpent = std::chrono::duration<double>(now - m_startTime).count();
    }

    MCQAttempt attempt;
    attempt.questionId    = q.id;
    attempt.studentAnswer = upperAnswer;
    attempt.correct       = correct;
    attempt.timeSeconds   = timeSpent;
    attempt.timestamp     = now;

    // Actualizar si ya existía intento para esta pregunta
    auto it = std::find_if(m_attempts.begin(), m_attempts.end(),
        [&q](const MCQAttempt& a){ return a.questionId == q.id; });

    if (it != m_attempts.end()) {
        *it = attempt;
    } else {
        m_attempts.push_back(attempt);
    }

    m_answered[m_currentIdx] = true;
    return correct;
}

bool MCQSession::NextQuestion() {
    if (m_currentIdx + 1 >= m_questions.size()) return false;
    m_currentIdx++;
    return true;
}

bool MCQSession::PreviousQuestion() {
    if (m_currentIdx == 0) return false;
    m_currentIdx--;
    return true;
}

void MCQSession::SkipCurrent() {
    NextQuestion();
}

bool MCQSession::HasNext() const {
    return m_currentIdx + 1 < m_questions.size();
}

bool MCQSession::HasPrevious() const {
    return m_currentIdx > 0;
}

bool MCQSession::WasAnswered(size_t idx) const {
    if (idx >= m_answered.size()) return false;
    return m_answered[idx];
}

std::chrono::seconds MCQSession::GetElapsed() const {
    auto now = std::chrono::system_clock::now();
    return std::chrono::duration_cast<std::chrono::seconds>(now - m_startTime);
}

std::chrono::seconds MCQSession::GetRemaining() const {
    if (m_timeLimit.count() == 0) return std::chrono::seconds(0);
    auto limit = std::chrono::duration_cast<std::chrono::seconds>(m_timeLimit);
    auto elapsed = GetElapsed();
    if (elapsed >= limit) return std::chrono::seconds(0);
    return limit - elapsed;
}

bool MCQSession::IsTimeExpired() const {
    if (m_timeLimit.count() == 0) return false;
    return GetElapsed() >= std::chrono::duration_cast<std::chrono::seconds>(m_timeLimit);
}

std::optional<std::string> MCQSession::GetFeedbackForCurrent() const {
    if (m_mode != MCQMode::Practice) return std::nullopt;
    if (m_currentIdx >= m_questions.size()) return std::nullopt;

    const auto& q = m_questions[m_currentIdx];
    auto it = std::find_if(m_attempts.begin(), m_attempts.end(),
        [&q](const MCQAttempt& a){ return a.questionId == q.id; });

    if (it == m_attempts.end()) return std::nullopt;

    std::stringstream ss;
    if (it->correct) {
        ss << "CORRECTO. Respuesta: " << q.correctAnswer << "\n\n";
    } else {
        ss << "INCORRECTO. Tu respuesta: " << it->studentAnswer
           << " | Correcta: " << q.correctAnswer << "\n\n";
    }
    ss << "Explicacion: " << q.explanationCorrect;
    return ss.str();
}

std::optional<MCQAttempt> MCQSession::GetAttemptForCurrent() const {
    if (m_currentIdx >= m_questions.size()) return std::nullopt;
    const auto& q = m_questions[m_currentIdx];
    auto it = std::find_if(m_attempts.begin(), m_attempts.end(),
        [&q](const MCQAttempt& a){ return a.questionId == q.id; });
    if (it == m_attempts.end()) return std::nullopt;
    return *it;
}

SessionStats MCQSession::GetLiveStats() const {
    SessionStats stats;
    stats.totalQuestions = static_cast<int>(m_questions.size());
    stats.answered       = static_cast<int>(m_attempts.size());
    for (const auto& a : m_attempts) {
        if (a.correct) stats.correct++;
        else           stats.incorrect++;
    }
    stats.skipped       = stats.totalQuestions - stats.answered;
    stats.scorePercent  = stats.totalQuestions > 0
        ? (100.0 * stats.correct / stats.totalQuestions) : 0.0;
    stats.elapsedTime   = GetElapsed();
    stats.remainingTime = GetRemaining();
    return stats;
}

std::string MCQSession::ComputePerformanceLevel(double pct) const {
    if (pct >= 85.0) return "Excelente";
    if (pct >= 70.0) return "Aprobado";
    if (pct >= 60.0) return "Aprobado marginal";
    if (pct >= 50.0) return "Reprobado";
    return "Reprobado severo";
}

FinalReport MCQSession::Finish() {
    m_finished = true;

    FinalReport report;
    report.overall       = GetLiveStats();
    report.attempts      = m_attempts;
    report.completedAt   = std::chrono::system_clock::now();
    report.performanceLevel = ComputePerformanceLevel(report.overall.scorePercent);

    // Stats por especialidad
    for (size_t i = 0; i < m_questions.size(); ++i) {
        const auto& q = m_questions[i];
        auto& stat = report.bySpecialty[q.specialty];
        stat.totalQuestions++;

        auto it = std::find_if(m_attempts.begin(), m_attempts.end(),
            [&q](const MCQAttempt& a){ return a.questionId == q.id; });

        if (it != m_attempts.end()) {
            stat.answered++;
            if (it->correct) stat.correct++;
            else {
                stat.incorrect++;
                report.incorrectQuestionIds.push_back(q.id);
            }
        } else {
            stat.skipped++;
        }
    }

    for (auto& [spec, stat] : report.bySpecialty) {
        stat.scorePercent = stat.totalQuestions > 0
            ? (100.0 * stat.correct / stat.totalQuestions) : 0.0;
    }

    return report;
}

} // namespace ENARM::MCQ