// ═══════════════════════════════════════════════════════════════
//   ENARM.Data - StudentRepository
// ═══════════════════════════════════════════════════════════════
#pragma once

#include "ENARM/Common/Result.h"
#include "ENARM/Data/DatabaseManager.h"

#include <memory>
#include <string>
#include <optional>

namespace ENARM::Data {

    struct Student {
        std::string id;
        std::string userId;
        std::string studentCode;
        std::string university;
        std::optional<int> graduationYear;
        std::string currentLevel;
        std::string targetSpecialty;
    };

    struct StudentProfile {
        std::string id;
        std::string studentId;
        std::string preferredDifficulty{"intermediate"};
        int dailyStudyGoalMinutes{60};
        int totalCasesCompleted{0};
        int totalMcqAnswered{0};
        double averageCaseScore{0.0};
        double averageMcqScore{0.0};
    };

    class StudentRepository {
    public:
        explicit StudentRepository(std::shared_ptr<DatabaseManager> db) : m_db(db) {}

        Common::Result<Student> Create(const std::string& userId,
                                        const std::string& studentCode,
                                        const std::string& university,
                                        const std::string& targetSpecialty,
                                        const std::string& currentLevel = "aspirante_enarm");

        std::optional<Student> FindByUserId(const std::string& userId);
        std::optional<Student> FindByCode(const std::string& code);

        // Perfil
        Common::Result<StudentProfile> CreateProfile(const std::string& studentId);
        std::optional<StudentProfile> GetProfile(const std::string& studentId);

        [[nodiscard]] size_t Count();

    private:
        std::shared_ptr<DatabaseManager> m_db;
    };

}