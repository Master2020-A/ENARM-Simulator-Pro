// test_mcq.cpp - 12 tests para MCQSession
#include <catch2/catch_all.hpp>
#include "ENARM/MCQ/MCQSession.h"
#include "ENARM/MCQ/MCQTypes.h"

using namespace ENARM::MCQ;

// Helper: crear preguntas de prueba
static std::vector<MCQQuestion> MakeSampleQuestions(size_t count) {
    std::vector<MCQQuestion> qs;
    for (size_t i = 0; i < count; i++) {
        MCQQuestion q;
        q.id = "test-id-" + std::to_string(i);
        q.specialty = (i % 2 == 0) ? "Cardiologia" : "Neurologia";
        q.difficulty = MCQDifficulty::Intermediate;
        q.vignette = "Vignette " + std::to_string(i);
        q.question = "Cual es la conducta?";
        q.options = {
            {'A', "Opcion A"},
            {'B', "Opcion B"},
            {'C', "Opcion C"},
            {'D', "Opcion D"}
        };
        q.correctAnswer = 'B';
        q.explanationCorrect = "B es correcta porque X";
        qs.push_back(q);
    }
    return qs;
}

TEST_CASE("MCQSession - Constructor no lanza", "[mcq]") {
    auto qs = MakeSampleQuestions(5);
    REQUIRE_NOTHROW(MCQSession(MCQMode::Practice, qs));
}

TEST_CASE("MCQSession - Total questions correcto", "[mcq]") {
    auto qs = MakeSampleQuestions(10);
    MCQSession s(MCQMode::Practice, qs);
    REQUIRE(s.GetTotalQuestions() == 10);
    REQUIRE(s.GetCurrentIndex() == 0);
}

TEST_CASE("MCQSession - Current question presente al iniciar", "[mcq]") {
    auto qs = MakeSampleQuestions(3);
    MCQSession s(MCQMode::Practice, qs);
    auto current = s.GetCurrentQuestion();
    REQUIRE(current.has_value());
    REQUIRE(current->id == "test-id-0");
}

TEST_CASE("MCQSession - SubmitAnswer correcto retorna true", "[mcq]") {
    auto qs = MakeSampleQuestions(1);
    MCQSession s(MCQMode::Practice, qs);
    REQUIRE(s.SubmitAnswer('B') == true);
}

TEST_CASE("MCQSession - SubmitAnswer incorrecto retorna false", "[mcq]") {
    auto qs = MakeSampleQuestions(1);
    MCQSession s(MCQMode::Practice, qs);
    REQUIRE(s.SubmitAnswer('A') == false);
}

TEST_CASE("MCQSession - SubmitAnswer insensible a mayusculas", "[mcq]") {
    auto qs = MakeSampleQuestions(1);
    MCQSession s(MCQMode::Practice, qs);
    REQUIRE(s.SubmitAnswer('b') == true);
}

TEST_CASE("MCQSession - NextQuestion avanza indice", "[mcq]") {
    auto qs = MakeSampleQuestions(3);
    MCQSession s(MCQMode::Practice, qs);
    REQUIRE(s.NextQuestion() == true);
    REQUIRE(s.GetCurrentIndex() == 1);
}

TEST_CASE("MCQSession - NextQuestion al final retorna false", "[mcq]") {
    auto qs = MakeSampleQuestions(2);
    MCQSession s(MCQMode::Practice, qs);
    s.NextQuestion();
    REQUIRE(s.NextQuestion() == false);
}

TEST_CASE("MCQSession - PreviousQuestion regresa indice", "[mcq]") {
    auto qs = MakeSampleQuestions(3);
    MCQSession s(MCQMode::Practice, qs);
    s.NextQuestion();
    REQUIRE(s.PreviousQuestion() == true);
    REQUIRE(s.GetCurrentIndex() == 0);
}

TEST_CASE("MCQSession - Feedback solo en modo Practice", "[mcq][practice]") {
    auto qs = MakeSampleQuestions(1);

    MCQSession practice(MCQMode::Practice, qs);
    practice.SubmitAnswer('B');
    REQUIRE(practice.GetFeedbackForCurrent().has_value());

    MCQSession simulacro(MCQMode::Simulacro280, qs);
    simulacro.SubmitAnswer('B');
    REQUIRE_FALSE(simulacro.GetFeedbackForCurrent().has_value());
}

TEST_CASE("MCQSession - Finish calcula score correcto", "[mcq]") {
    auto qs = MakeSampleQuestions(4);
    MCQSession s(MCQMode::Practice, qs);

    s.SubmitAnswer('B'); s.NextQuestion();  // correcta
    s.SubmitAnswer('B'); s.NextQuestion();  // correcta
    s.SubmitAnswer('A'); s.NextQuestion();  // incorrecta
    s.SubmitAnswer('B');                    // correcta

    auto report = s.Finish();
    REQUIRE(report.overall.correct == 3);
    REQUIRE(report.overall.incorrect == 1);
    REQUIRE(report.overall.scorePercent == Catch::Approx(75.0));
    REQUIRE(report.performanceLevel == "Aprobado");
}

TEST_CASE("MCQSession - Stats por especialidad", "[mcq]") {
    auto qs = MakeSampleQuestions(4);   // 2 Cardio + 2 Neuro
    MCQSession s(MCQMode::Practice, qs);

    s.SubmitAnswer('B'); s.NextQuestion();  // Cardio - correcta
    s.SubmitAnswer('A'); s.NextQuestion();  // Neuro - incorrecta
    s.SubmitAnswer('B'); s.NextQuestion();  // Cardio - correcta
    s.SubmitAnswer('B');                    // Neuro - correcta

    auto report = s.Finish();
    REQUIRE(report.bySpecialty["Cardiologia"].correct == 2);
    REQUIRE(report.bySpecialty["Cardiologia"].scorePercent == Catch::Approx(100.0));
    REQUIRE(report.bySpecialty["Neurologia"].correct == 1);
    REQUIRE(report.bySpecialty["Neurologia"].scorePercent == Catch::Approx(50.0));
}