// ═══════════════════════════════════════════════════════════════
//   ENARM.Data - SessionRepository (libpqxx v7.10+)
// ═══════════════════════════════════════════════════════════════
#include "ENARM/Data/Repositories/SessionRepository.h"
#include "ENARM/Data/JsonHelpers.h"

namespace ENARM::Data {

    namespace {
        template <typename Row>
        StudySession RowToSession(const Row& row) {
            StudySession s;
            s.id             = row["id"].template as<std::string>();
            s.studentId      = row["student_id"].template as<std::string>();
            s.mode           = row["mode"].template as<std::string>();
            s.caseId         = GetOptString(row, "case_id");
            s.durationSeconds = GetOptInt(row, "duration_seconds");
            s.completed      = row["completed"].template as<bool>();
            s.scores         = GetJson(row, "scores_json");
            if (!row["total_score"].is_null()) s.totalScore = row["total_score"].template as<double>();
            if (!row["max_possible_score"].is_null()) s.maxPossibleScore = row["max_possible_score"].template as<double>();
            s.aiFeedback     = GetJsonOr(row, "ai_feedback_json", nlohmann::json::object());
            return s;
        }

        template <typename Row>
        SessionEvent RowToEvent(const Row& row) {
            SessionEvent e;
            e.id              = row["id"].template as<std::string>();
            e.sessionId       = row["session_id"].template as<std::string>();
            if (!row["sim_time_seconds"].is_null())
                e.simTimeSeconds = row["sim_time_seconds"].template as<double>();
            e.eventType       = row["event_type"].template as<std::string>();
            e.category        = GetOptString(row, "category");
            e.action          = GetOptString(row, "action");
            e.details         = GetJson(row, "details_json");
            e.severity        = row["severity"].template as<int>();
            return e;
        }
    }

    Common::Result<std::string> SessionRepository::Start(
        const std::string& studentId, const std::string& mode,
        const std::optional<std::string>& caseId)
    {
        try {
            auto conn = m_db->Acquire();
            pqxx::work tx(conn.Get());
            std::string id;
            if (caseId) {
                auto row = tx.exec(
                    "INSERT INTO study_sessions (student_id, mode, case_id) "
                    "VALUES ($1::uuid, $2, $3::uuid) RETURNING id",
                    pqxx::params{studentId, mode, *caseId}
                ).one_row();
                id = row["id"].as<std::string>();
            } else {
                auto row = tx.exec(
                    "INSERT INTO study_sessions (student_id, mode) "
                    "VALUES ($1::uuid, $2) RETURNING id",
                    pqxx::params{studentId, mode}
                ).one_row();
                id = row["id"].as<std::string>();
            }
            tx.commit();
            return Common::Result<std::string>::Ok(id);
        } catch (const std::exception& e) {
            return Common::Err<std::string>(500, e.what(), "session_repo");
        }
    }

    Common::Result<void> SessionRepository::Complete(
        const std::string& sessionId, double totalScore, double maxScore,
        const nlohmann::json& scores, const nlohmann::json& aiFeedback)
    {
        try {
            auto conn = m_db->Acquire();
            pqxx::work tx(conn.Get());
            tx.exec(
                "UPDATE study_sessions SET "
                "  ended_at=NOW(), completed=TRUE, total_score=$2, max_possible_score=$3, "
                "  scores_json=$4::jsonb, ai_feedback_json=$5::jsonb, "
                "  duration_seconds=EXTRACT(EPOCH FROM (NOW() - started_at))::INT "
                "WHERE id=$1::uuid",
                pqxx::params{sessionId, totalScore, maxScore, scores.dump(), aiFeedback.dump()});
            tx.commit();
            return Common::Ok();
        } catch (const std::exception& e) {
            return Common::Err<void>(500, e.what(), "session_repo");
        }
    }

    std::optional<StudySession> SessionRepository::FindById(const std::string& id) {
        try {
            auto conn = m_db->Acquire();
            pqxx::read_transaction tx(conn.Get());
            auto res = tx.exec("SELECT * FROM study_sessions WHERE id=$1::uuid",
                                pqxx::params{id});
            if (res.empty()) return std::nullopt;
            return RowToSession(res[0]);
        } catch (...) { return std::nullopt; }
    }

    std::vector<StudySession> SessionRepository::FindByStudent(const std::string& studentId, int limit) {
        std::vector<StudySession> out;
        try {
            auto conn = m_db->Acquire();
            pqxx::read_transaction tx(conn.Get());
            auto res = tx.exec(
                "SELECT * FROM study_sessions WHERE student_id=$1::uuid "
                "ORDER BY started_at DESC LIMIT $2",
                pqxx::params{studentId, limit});
            for (const auto& r : res) out.push_back(RowToSession(r));
        } catch (...) {}
        return out;
    }

    Common::Result<void> SessionRepository::LogEvent(
        const std::string& sessionId, double simTime,
        const std::string& eventType, const std::string& category,
        const std::string& action, const nlohmann::json& details, int severity)
    {
        try {
            auto conn = m_db->Acquire();
            pqxx::work tx(conn.Get());
            tx.exec(
                "INSERT INTO session_events "
                "(session_id, sim_time_seconds, event_type, category, action, details_json, severity) "
                "VALUES ($1::uuid, $2, $3, $4, $5, $6::jsonb, $7)",
                pqxx::params{sessionId, simTime, eventType, category, action, details.dump(), severity});
            tx.commit();
            return Common::Ok();
        } catch (const std::exception& e) {
            return Common::Err<void>(500, e.what(), "session_repo");
        }
    }

    std::vector<SessionEvent> SessionRepository::GetEvents(const std::string& sessionId) {
        std::vector<SessionEvent> out;
        try {
            auto conn = m_db->Acquire();
            pqxx::read_transaction tx(conn.Get());
            auto res = tx.exec(
                "SELECT * FROM session_events WHERE session_id=$1::uuid ORDER BY event_time",
                pqxx::params{sessionId});
            for (const auto& r : res) out.push_back(RowToEvent(r));
        } catch (...) {}
        return out;
    }

}