// test_competency.cpp - 6 tests CompetencyEvaluator
#include <catch2/catch_all.hpp>
#include "ENARM/Assessment/CompetencyEvaluator.h"

using namespace enarm::assessment;

TEST_CASE("CompetencyEvaluator - Constructor no lanza", "[competency]") {
    REQUIRE_NOTHROW(CompetencyEvaluator{});
}

TEST_CASE("CompetencyEvaluator - Match directo contiene concepto", "[competency]") {
    CompetencyEvaluator ev;
    REQUIRE(ev.containsSemanticMatch(
        "El paciente presenta IAMCEST anterior", "IAMCEST") == true);
}

TEST_CASE("CompetencyEvaluator - Texto no relacionado no matchea", "[competency]") {
    CompetencyEvaluator ev;
    REQUIRE(ev.containsSemanticMatch(
        "diagnostico de apendicitis aguda", "IAMCEST") == false);
}

TEST_CASE("CompetencyEvaluator - Matching insensible a mayusculas", "[competency]") {
    CompetencyEvaluator ev;
    REQUIRE(ev.containsSemanticMatch(
        "el paciente tiene iamcest confirmado", "IAMCEST") == true);
}

TEST_CASE("CompetencyEvaluator - Sinonimos AAS aspirina", "[competency]") {
    CompetencyEvaluator ev;
    std::vector<std::string> synonyms = {"aspirina", "AAS", "acido acetilsalicilico"};
    REQUIRE(ev.containsSemanticMatch(
        "administrar aspirina 300mg VO", "AAS", synonyms) == true);
}

TEST_CASE("CompetencyEvaluator - getPerformanceLevel estatica", "[competency]") {
    std::string lvlExcellent = CompetencyEvaluator::getPerformanceLevel(95.0f);
    std::string lvlPoor      = CompetencyEvaluator::getPerformanceLevel(30.0f);
    REQUIRE_FALSE(lvlExcellent.empty());
    REQUIRE_FALSE(lvlPoor.empty());
}