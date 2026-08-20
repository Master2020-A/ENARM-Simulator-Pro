// ═══════════════════════════════════════════════════════════════
//   ENARM.Data - MCQRepository Implementation (libpqxx v7.10+)
// ═══════════════════════════════════════════════════════════════
#include "ENARM/Data/Repositories/MCQRepository.h"

namespace ENARM::Data {

    Common::Result<std::string> MCQRepository::Insert(const MCQuestion& q) {
        try {
            auto conn = m_db->Acquire();
            pqxx::work tx(conn.Get());
            std::string correct(1, q.correctAnswer);

            auto row = tx.exec(
                "INSERT INTO mcq_bank ("
                "  specialty, sub_specialty, difficulty, topic, vignette, question,"
                "  options_json, correct_answer, explanation_correct, explanation_incorrect_json,"
                "  pubmed_pmids, guidelines_ids, references_text"
                ") VALUES ("
                "  $1, $2, $3, $4, $5, $6,"
                "  $7::jsonb, $8, $9, $10::jsonb,"
                "  $11::text[], $12::text[], $13"
                ") RETURNING id",
                pqxx::params{
                    q.specialty, q.subSpecialty.value_or(""), q.difficulty,
                    q.topic.value_or(""), q.vignette, q.question,
                    q.options.dump(), correct, q.explanationCorrect.value_or(""),
                    q.explanationIncorrect.dump(),
                    ToPgTextArray(q.pubmedPmids),
                    ToPgTextArray(q.guidelinesIds),
                    q.referencesText.value_or("")
                }
            ).one_row();
            tx.commit();
            return Common::Result<std::string>::Ok(row["id"].as<std::string>());
        } catch (const std::exception& e) {
            return Common::Err<std::string>(500, e.what(), "mcq_repo");
        }
    }

    std::optional<MCQuestion> MCQRepository::FindById(const std::string& id) {
        try {
            auto conn = m_db->Acquire();
            pqxx::read_transaction tx(conn.Get());
            auto res = tx.exec("SELECT * FROM mcq_bank WHERE id=$1::uuid",
                                pqxx::params{id});
            if (res.empty()) return std::nullopt;
            return RowToMCQ(res[0]);
        } catch (...) { return std::nullopt; }
    }

    std::vector<MCQuestion> MCQRepository::FindBySpecialty(
        const std::string& specialty, int limit)
    {
        std::vector<MCQuestion> out;
        try {
            auto conn = m_db->Acquire();
            pqxx::read_transaction tx(conn.Get());
            auto res = tx.exec(
                "SELECT * FROM mcq_bank WHERE specialty=$1 AND is_active=TRUE "
                "ORDER BY created_at DESC LIMIT $2",
                pqxx::params{specialty, limit});
            for (const auto& r : res) out.push_back(RowToMCQ(r));
        } catch (...) {}
        return out;
    }

    std::vector<MCQuestion> MCQRepository::FindRandom(int count, const std::string& specialty) {
        std::vector<MCQuestion> out;
        try {
            auto conn = m_db->Acquire();
            pqxx::read_transaction tx(conn.Get());
            if (!specialty.empty()) {
                auto res = tx.exec(
                    "SELECT * FROM mcq_bank WHERE is_active=TRUE AND specialty=$1 "
                    "ORDER BY RANDOM() LIMIT $2",
                    pqxx::params{specialty, count});
                for (const auto& r : res) out.push_back(RowToMCQ(r));
            } else {
                auto res = tx.exec(
                    "SELECT * FROM mcq_bank WHERE is_active=TRUE "
                    "ORDER BY RANDOM() LIMIT $1",
                    pqxx::params{count});
                for (const auto& r : res) out.push_back(RowToMCQ(r));
            }
        } catch (...) {}
        return out;
    }

    std::vector<MCQuestion> MCQRepository::FindPracticeSet(
        const std::string& specialty, const std::string& difficulty, int limit)
    {
        std::vector<MCQuestion> out;
        try {
            auto conn = m_db->Acquire();
            pqxx::read_transaction tx(conn.Get());
            auto res = tx.exec(
                "SELECT * FROM mcq_bank WHERE is_active=TRUE "
                "AND specialty=$1 AND difficulty=$2 "
                "ORDER BY RANDOM() LIMIT $3",
                pqxx::params{specialty, difficulty, limit});
            for (const auto& r : res) out.push_back(RowToMCQ(r));
        } catch (...) {}
        return out;
    }

    std::vector<MCQuestion> MCQRepository::FindWeakAreaSet(
        const std::string& studentId, int limit)
    {
        std::vector<MCQuestion> out;
        try {
            auto conn = m_db->Acquire();
            pqxx::read_transaction tx(conn.Get());
            auto res = tx.exec(
                "SELECT m.* FROM mcq_bank m "
                "WHERE m.is_active=TRUE AND m.id IN ("
                "  SELECT a.mcq_id FROM mcq_attempts a "
                "  WHERE a.student_id=$1::uuid AND a.is_correct=FALSE "
                "  GROUP BY a.mcq_id ORDER BY COUNT(*) DESC"
                ") LIMIT $2",
                pqxx::params{studentId, limit});
            for (const auto& r : res) out.push_back(RowToMCQ(r));
        } catch (...) {}
        return out;
    }

    Common::Result<void> MCQRepository::RecordAttempt(
        const std::string& studentId, const std::string& mcqId,
        const std::string& sessionId, char selectedAnswer,
        bool isCorrect, double timeSeconds)
    {
        try {
            auto conn = m_db->Acquire();
            pqxx::work tx(conn.Get());
            std::string sel(1, selectedAnswer);
            tx.exec(
                "INSERT INTO mcq_attempts (student_id, mcq_id, session_id, "
                "selected_answer, is_correct, time_seconds) "
                "VALUES ($1::uuid, $2::uuid, $3::uuid, $4, $5, $6)",
                pqxx::params{studentId, mcqId, sessionId, sel, isCorrect, timeSeconds}
            );
            tx.commit();
            return Common::Ok();
        } catch (const std::exception& e) {
            return Common::Err<void>(500, e.what(), "mcq_repo");
        }
    }

    size_t MCQRepository::Count() {
        try {
            auto conn = m_db->Acquire();
            pqxx::read_transaction tx(conn.Get());
            auto row = tx.exec("SELECT COUNT(*) FROM mcq_bank").one_row();
            return row[0].as<size_t>();
        } catch (...) { return 0; }
    }

}