// ═══════════════════════════════════════════════════════════════
//   ENARM.Data - MCQRepository (extendido para Simulador)
// ═══════════════════════════════════════════════════════════════
#pragma once

#include "ENARM/Common/Result.h"
#include "ENARM/Data/DatabaseManager.h"
#include "ENARM/Data/JsonHelpers.h"

#include <memory>
#include <string>
#include <vector>
#include <optional>
#include <nlohmann/json.hpp>

namespace ENARM::Data {

    struct MCQuestion {
        std::string id;
        std::string specialty;
        std::optional<std::string> subSpecialty;
        std::string difficulty{"intermediate"};
        std::optional<std::string> topic;
        std::string vignette;
        std::string question;
        nlohmann::json options;
        char correctAnswer{'A'};
        std::optional<std::string> explanationCorrect;
        nlohmann::json explanationIncorrect;
        std::vector<std::string> pubmedPmids;
        std::vector<std::string> guidelinesIds;
        std::optional<std::string> referencesText;
        int timesAttempted{0};
        int timesCorrect{0};
        std::optional<double> difficultyIndex;
        bool isActive{true};
    };

    class MCQRepository {
    public:
        explicit MCQRepository(std::shared_ptr<DatabaseManager> db) : m_db(db) {}

        // --- CRUD basico ---
        Common::Result<std::string> Insert(const MCQuestion& q);
        std::optional<MCQuestion>   FindById(const std::string& id);
        std::vector<MCQuestion>     FindBySpecialty(const std::string& specialty, int limit = 20);
        std::vector<MCQuestion>     FindRandom(int count = 10, const std::string& specialty = "");

        // --- NUEVO: Modo Practica (especialidad + dificultad) ---
        std::vector<MCQuestion>     FindPracticeSet(const std::string& specialty,
                                                    const std::string& difficulty,
                                                    int limit = 20);

        // --- NUEVO: Modo Repaso (preguntas falladas por alumno) ---
        std::vector<MCQuestion>     FindWeakAreaSet(const std::string& studentId,
                                                    int limit = 30);

        // --- Registro de intentos ---
        Common::Result<void> RecordAttempt(const std::string& studentId,
                                           const std::string& mcqId,
                                           const std::string& sessionId,
                                           char selectedAnswer,
                                           bool isCorrect,
                                           double timeSeconds);

        [[nodiscard]] size_t Count();

        template <typename Row>
        static MCQuestion RowToMCQ(const Row& row) {
            MCQuestion q;
            q.id                    = row["id"].template as<std::string>();
            q.specialty             = row["specialty"].template as<std::string>();
            q.subSpecialty          = GetOptString(row, "sub_specialty");
            q.difficulty            = row["difficulty"].template as<std::string>();
            q.topic                 = GetOptString(row, "topic");
            q.vignette              = row["vignette"].template as<std::string>();
            q.question              = row["question"].template as<std::string>();
            q.options               = GetJson(row, "options_json");
            auto ca                 = row["correct_answer"].template as<std::string>();
            q.correctAnswer         = ca.empty() ? 'A' : ca[0];
            q.explanationCorrect    = GetOptString(row, "explanation_correct");
            q.explanationIncorrect  = GetJson(row, "explanation_incorrect_json");
            q.pubmedPmids           = GetStringArray(row, "pubmed_pmids");
            q.guidelinesIds         = GetStringArray(row, "guidelines_ids");
            q.referencesText        = GetOptString(row, "references_text");
            q.timesAttempted        = row["times_attempted"].template as<int>();
            q.timesCorrect          = row["times_correct"].template as<int>();
            if (!row["difficulty_index"].is_null())
                q.difficultyIndex = row["difficulty_index"].template as<double>();
            q.isActive              = row["is_active"].template as<bool>();
            return q;
        }

    private:
        std::shared_ptr<DatabaseManager> m_db;
    };

}
