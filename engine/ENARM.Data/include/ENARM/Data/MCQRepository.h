#pragma once
// ================================================================
//  MCQRepository.h — Schema REAL de mcq_bank
//  Columnas verificadas: vignette, question, options_json,
//  correct_answer (char), explanation_correct, is_active
// ================================================================
#include <string>
#include <vector>
#include <optional>
#include <map>

namespace ENARM::Data {

struct MCQQuestion {
    std::string id;
    std::string specialty;
    std::string subSpecialty;
    std::string difficulty;        // "basic"|"intermediate"|"advanced"
    std::string topic;
    std::string vignette;          // Contexto clínico del paciente
    std::string question;          // Pregunta concreta
    std::map<std::string,std::string> options; // A,B,C,D
    std::string correctAnswer;     // "A"|"B"|"C"|"D"
    std::string explanationCorrect;
    std::string referencesText;
    bool        isActive = true;
};

struct MCQFilter {
    std::string specialty  = "";
    std::string difficulty = "";
    std::string topic      = "";
    int         limit      = 10;
    int         offset     = 0;
};

struct MCQSessionResult {
    int   total     = 0;
    int   correct   = 0;
    int   incorrect = 0;
    float scorePercent = 0.0f;
    std::vector<std::string> specialtiesToReview;
};

class MCQRepository {
public:
    explicit MCQRepository(const std::string& connStr);

    std::vector<MCQQuestion> GetQuestions(const MCQFilter& f) const;
    std::optional<MCQQuestion> GetRandom(const std::string& specialty = "") const;
    std::vector<std::string>   GetSpecialties() const;
    std::map<std::string,int>  GetCountBySpecialty() const;
    int GetTotal() const;

private:
    std::string m_conn;
    static std::map<std::string,std::string> ParseOptions(const std::string& j);
};

} // namespace ENARM::Data
