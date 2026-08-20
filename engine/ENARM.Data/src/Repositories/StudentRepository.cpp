// ═══════════════════════════════════════════════════════════════
//   ENARM.Data - StudentRepository (libpqxx v7.10+)
// ═══════════════════════════════════════════════════════════════
#include "ENARM/Data/Repositories/StudentRepository.h"
#include "ENARM/Data/JsonHelpers.h"

namespace ENARM::Data {

    namespace {
        template <typename Row>
        Student RowToStudent(const Row& row) {
            Student s;
            s.id              = row["id"].template as<std::string>();
            s.userId          = row["user_id"].template as<std::string>();
            s.studentCode     = row["student_code"].is_null() ? "" : row["student_code"].template as<std::string>();
            s.university      = row["university"].is_null() ? "" : row["university"].template as<std::string>();
            s.graduationYear  = GetOptInt(row, "graduation_year");
            s.currentLevel    = row["current_level"].is_null() ? "" : row["current_level"].template as<std::string>();
            s.targetSpecialty = row["target_specialty"].is_null() ? "" : row["target_specialty"].template as<std::string>();
            return s;
        }

        template <typename Row>
        StudentProfile RowToProfile(const Row& row) {
            StudentProfile p;
            p.id                     = row["id"].template as<std::string>();
            p.studentId              = row["student_id"].template as<std::string>();
            p.preferredDifficulty    = row["preferred_difficulty"].template as<std::string>();
            p.dailyStudyGoalMinutes  = row["daily_study_goal_minutes"].template as<int>();
            p.totalCasesCompleted    = row["total_cases_completed"].template as<int>();
            p.totalMcqAnswered       = row["total_mcq_answered"].template as<int>();
            p.averageCaseScore       = row["average_case_score"].template as<double>();
            p.averageMcqScore        = row["average_mcq_score"].template as<double>();
            return p;
        }
    }

    Common::Result<Student> StudentRepository::Create(
        const std::string& userId, const std::string& studentCode,
        const std::string& university, const std::string& targetSpecialty,
        const std::string& currentLevel)
    {
        try {
            auto conn = m_db->Acquire();
            pqxx::work tx(conn.Get());
            auto row = tx.exec(
                "INSERT INTO students (user_id, student_code, university, current_level, target_specialty) "
                "VALUES ($1::uuid, $2, $3, $4, $5) "
                "RETURNING id, user_id, student_code, university, graduation_year, current_level, target_specialty",
                pqxx::params{userId, studentCode, university, currentLevel, targetSpecialty}
            ).one_row();
            tx.commit();
            return Common::Result<Student>::Ok(RowToStudent(row));
        } catch (const std::exception& e) {
            return Common::Err<Student>(500, e.what(), "student_repo");
        }
    }

    std::optional<Student> StudentRepository::FindByUserId(const std::string& userId) {
        try {
            auto conn = m_db->Acquire();
            pqxx::read_transaction tx(conn.Get());
            auto res = tx.exec(
                "SELECT id, user_id, student_code, university, graduation_year, current_level, target_specialty "
                "FROM students WHERE user_id = $1::uuid",
                pqxx::params{userId});
            if (res.empty()) return std::nullopt;
            return RowToStudent(res[0]);
        } catch (...) { return std::nullopt; }
    }

    std::optional<Student> StudentRepository::FindByCode(const std::string& code) {
        try {
            auto conn = m_db->Acquire();
            pqxx::read_transaction tx(conn.Get());
            auto res = tx.exec(
                "SELECT id, user_id, student_code, university, graduation_year, current_level, target_specialty "
                "FROM students WHERE student_code = $1",
                pqxx::params{code});
            if (res.empty()) return std::nullopt;
            return RowToStudent(res[0]);
        } catch (...) { return std::nullopt; }
    }

    Common::Result<StudentProfile> StudentRepository::CreateProfile(const std::string& studentId) {
        try {
            auto conn = m_db->Acquire();
            pqxx::work tx(conn.Get());
            auto row = tx.exec(
                "INSERT INTO student_profiles (student_id) VALUES ($1::uuid) "
                "RETURNING id, student_id, preferred_difficulty, daily_study_goal_minutes, "
                "total_cases_completed, total_mcq_answered, average_case_score, average_mcq_score",
                pqxx::params{studentId}
            ).one_row();
            tx.commit();
            return Common::Result<StudentProfile>::Ok(RowToProfile(row));
        } catch (const std::exception& e) {
            return Common::Err<StudentProfile>(500, e.what(), "profile_repo");
        }
    }

    std::optional<StudentProfile> StudentRepository::GetProfile(const std::string& studentId) {
        try {
            auto conn = m_db->Acquire();
            pqxx::read_transaction tx(conn.Get());
            auto res = tx.exec(
                "SELECT id, student_id, preferred_difficulty, daily_study_goal_minutes, "
                "total_cases_completed, total_mcq_answered, average_case_score, average_mcq_score "
                "FROM student_profiles WHERE student_id = $1::uuid",
                pqxx::params{studentId});
            if (res.empty()) return std::nullopt;
            return RowToProfile(res[0]);
        } catch (...) { return std::nullopt; }
    }

    size_t StudentRepository::Count() {
        try {
            auto conn = m_db->Acquire();
            pqxx::read_transaction tx(conn.Get());
            auto row = tx.exec("SELECT COUNT(*) FROM students").one_row();
            return row[0].as<size_t>();
        } catch (...) { return 0; }
    }

}