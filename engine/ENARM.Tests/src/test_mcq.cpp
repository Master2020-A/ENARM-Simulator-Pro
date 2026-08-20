// ============================================================
//  test_mcq_engine.cpp - Tests MCQEngine (contrato sin BD)
// ============================================================
#include <catch2/catch_all.hpp>
#include "ENARM/Data/MCQEngine.h"

using namespace ENARM::Data;

TEST_CASE("MCQEngine - FeedbackForOption sin explicacion",
          "[mcq][feedback]")
{
    MCQuestion q;
    q.id = "q1";
    q.question = "Cual es el tratamiento de IAM?";
    q.options = nlohmann::json::array({ {"A","Aspirina"},{"B","Ibuprofeno"} });
    q.correctAnswer = 'A';
    q.explanationIncorrect = nlohmann::json::object();

    std::string fb = MCQEngine::FeedbackForOption(q, 'B');
    REQUIRE(fb == "Sin detalle adicional para esta opcion.");
}

TEST_CASE("MCQEngine - Feedback con explicacion por opcion",
          "[mcq][feedback]")
{
    MCQuestion q;
    q.id = "q2";
    q.explanationIncorrect = nlohmann::json::object();
    q.explanationIncorrect["B"] = "Ibuprofeno no es antiagregante";

    std::string fb = MCQEngine::FeedbackForOption(q, 'b');
    REQUIRE(fb == "Ibuprofeno no es antiagregante");
}

TEST_CASE("MCQEngine - Feedback case insensitive",
          "[mcq][feedback]")
{
    MCQuestion q;
    q.explanationIncorrect = nlohmann::json::object();
    q.explanationIncorrect["A"] = "AAS correcta";

    REQUIRE(MCQEngine::FeedbackForOption(q, 'a') == "AAS correcta");
    REQUIRE(MCQEngine::FeedbackForOption(q, 'A') == "AAS correcta");
}