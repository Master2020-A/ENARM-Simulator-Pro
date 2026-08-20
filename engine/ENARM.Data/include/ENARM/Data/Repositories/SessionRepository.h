// ═══════════════════════════════════════════════════════════════
//   ENARM.Data - SessionRepository
// ═══════════════════════════════════════════════════════════════
#pragma once

#include "ENARM/Common/Result.h"
#include "ENARM/Data/DatabaseManager.h"

#include <memory>
#include <string>
#include <vector>
#include <optional>
#include <nlohmann/json.hpp>

namespace ENARM::Data {

    struct StudySession {
        std::string id;
        std::string studentId;
        std::string mode;             // "case", "mcq_practice", "mcq_exam", "osce"
        std::optional<std::string> caseId;
        std::optional<int> durationSeconds;
        bool completed{false};
        nlohmann::json scores;
        std::optional<double> totalScore;
        std::optional<double> maxPossibleScore;
        nlohmann::json aiFeedback;
    };

    struct SessionEvent {
        std::string id;
        std::string sessionId;
        double simTimeSeconds{0.0};
        std::string eventType;
        std::optional<std::string> category;
        std::optional<std::string> action;
        nlohmann::json details;
        int severity{0};
    };

    class SessionRepository {
    public:
        explicit SessionRepository(std::shared_ptr<DatabaseManager> db) : m_db(db) {}

        Common::Result<std::string> Start(const std::string& studentId,
                                           const std::string& mode,
                                           const std::optional<std::string>& caseId = std::nullopt);

        Common::Result<void> Complete(const std::string& sessionId,
                                       double totalScore,
                                       double maxScore,
                                       const nlohmann::json& scores,
                                       const nlohmann::json& aiFeedback = {});

        std::optional<StudySession> FindById(const std::string& id);
        std::vector<StudySession> FindByStudent(const std::string& studentId,
                                                  int limit = 50);

        // Eventos
        Common::Result<void> LogEvent(const std::string& sessionId,
                                       double simTimeSeconds,
                                       const std::string& eventType,
                                       const std::string& category,
                                       const std::string& action,
                                       const nlohmann::json& details = {},
                                       int severity = 0);

        std::vector<SessionEvent> GetEvents(const std::string& sessionId);

    private:
        std::shared_ptr<DatabaseManager> m_db;
    };

}