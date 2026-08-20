// ============================================================
//  test_competency_real.cpp
//  Tests reales para CompetencyEvaluator
// ============================================================
#include <catch2/catch_all.hpp>
#include "CompetencyEvaluator.h"

using namespace ENARM::Assessment;

TEST_CASE("CompetencyEvaluator - instancia se crea correctamente",
          "[competency][init]")
{
    CompetencyEvaluator eval;
    // Sin crash = OK
    REQUIRE(true);
}

TEST_CASE("CompetencyEvaluator - terminos iguales son sinonimos",
          "[competency][synonyms]")
{
    CompetencyEvaluator eval;
    // El mismo término siempre debe ser sinónimo de sí mismo
    REQUIRE(eval.AreSynonyms("IAM", "IAM"));
}

TEST_CASE("CompetencyEvaluator - terminos sin relacion no son sinonimos",
          "[competency][synonyms][negative]")
{
    CompetencyEvaluator eval;
    // Términos completamente distintos
    REQUIRE_FALSE(eval.AreSynonyms("apendice", "miocardio"));
}

TEST_CASE("CompetencyEvaluator - score con lista vacia es 0",
          "[competency][score]")
{
    CompetencyEvaluator eval;
    std::vector<std::string> empty;
    double score = eval.ComputeScore(empty, "IAM");
    REQUIRE(score == Catch::Approx(0.0).epsilon(0.01));
}

TEST_CASE("CompetencyEvaluator - score con termino exacto es mayor que 0",
          "[competency][score][positive]")
{
    CompetencyEvaluator eval;
    std::vector<std::string> terms = {"IAM"};
    double score = eval.ComputeScore(terms, "IAM");
    REQUIRE(score > 0.0);
}
