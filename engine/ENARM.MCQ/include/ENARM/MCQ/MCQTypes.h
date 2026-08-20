// MCQTypes.h - ENARM Simulator Pro
#pragma once
#include <string>
#include <vector>
#include <chrono>
#include <optional>
#include <unordered_map>

namespace ENARM::MCQ {

using UUID = std::string;

enum class MCQMode {
    Practice,
    Simulacro280,
    Review,
    Targeted
};

enum class MCQDifficulty {
    Basic,
    Intermediate,
    Advanced
};

inline std::string DifficultyToString(MCQDifficulty d) {
    switch (d) {
        case MCQDifficulty::Basic:        return "basic";
        case MCQDifficulty::Intermediate: return "intermediate";
        case MCQDifficulty::Advanced:     return "advanced";
    }
    return "intermediate";
}

inline MCQDifficulty DifficultyFromString(const std::string& s) {
    if (s == "basic")    return MCQDifficulty::Basic;
    if (s == "advanced") return MCQDifficulty::Advanced;
    return MCQDifficulty::Intermediate;
}

struct MCQOption {
    char letter{'A'};
    std::string text;
};

struct MCQQuestion {
    UUID id;
    std::string specialty;
    MCQDifficulty difficulty{MCQDifficulty::Intermediate};
    std::string topic;
    std::string vignette;
    std::string question;
    std::vector<MCQOption> options;
    char correctAnswer{'A'};
    std::string explanationCorrect;
    int timesAttempted{0};
    int timesCorrect{0};
    double difficultyIndex{0.5};
    bool isActive{true};
};

struct MCQAttempt {
    UUID questionId;
    char studentAnswer{'?'};
    bool correct{false};
    double timeSeconds{0.0};
    std::chrono::system_clock::time_point timestamp;
};

struct SessionStats {
    int totalQuestions{0};
    int answered{0};
    int correct{0};
    int incorrect{0};
    int skipped{0};
    double scorePercent{0.0};
    std::chrono::seconds elapsedTime{0};
    std::chrono::seconds remainingTime{0};
};

struct FinalReport {
    SessionStats overall;
    std::unordered_map<std::string, SessionStats> bySpecialty;
    std::vector<MCQAttempt> attempts;
    std::vector<UUID> incorrectQuestionIds;
    std::string performanceLevel;
    std::chrono::system_clock::time_point completedAt;
};

} // namespace ENARM::MCQ