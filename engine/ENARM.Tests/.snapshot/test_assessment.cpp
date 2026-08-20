// test_assessment.cpp - 8 tests AssessmentEngine
#include <catch2/catch_all.hpp>
#include "ENARM/Assessment/AssessmentEngine.h"

using namespace enarm::assessment;

TEST_CASE("AssessmentEngine - Constructor default no lanza", "[assessment]") {
    REQUIRE_NOTHROW(AssessmentEngine{});
}

TEST_CASE("AssessmentEngine - createIAMRubric retorna rubrica valida", "[assessment]") {
    auto rubric = AssessmentEngine::createIAMRubric();
    REQUIRE_FALSE(rubric.caseId.empty());
}

TEST_CASE("AssessmentEngine - Carga rubrica IAM sin crash", "[assessment]") {
    AssessmentEngine engine;
    REQUIRE_NOTHROW(engine.loadRubric(AssessmentEngine::createIAMRubric()));
}

TEST_CASE("AssessmentEngine - Carga rubrica Sepsis sin crash", "[assessment]") {
    AssessmentEngine engine;
    REQUIRE_NOTHROW(engine.loadRubric(AssessmentEngine::createSepsisRubric()));
}

TEST_CASE("AssessmentEngine - Score inicial en rango 0-100", "[assessment]") {
    AssessmentEngine engine;
    engine.loadRubric(AssessmentEngine::createIAMRubric());
    float score = engine.getCurrentScorePercent();
    REQUIRE(score >= 0.0f);
    REQUIRE(score <= 100.0f);
}

TEST_CASE("AssessmentEngine - 3 rubricas originales disponibles", "[assessment]") {
    REQUIRE_NOTHROW(AssessmentEngine::createIAMRubric());
    REQUIRE_NOTHROW(AssessmentEngine::createSepsisRubric());
    REQUIRE_NOTHROW(AssessmentEngine::createACVRubric());
}

TEST_CASE("AssessmentEngine - Reset no lanza", "[assessment]") {
    AssessmentEngine engine;
    engine.loadRubric(AssessmentEngine::createIAMRubric());
    REQUIRE_NOTHROW(engine.reset());
}

TEST_CASE("AssessmentEngine - onDiagnosisSubmitted registra sin crash", "[assessment]") {
    AssessmentEngine engine;
    engine.loadRubric(AssessmentEngine::createIAMRubric());
    REQUIRE_NOTHROW(engine.onDiagnosisSubmitted(
        "IAM CEST", "ECG con supradesnivel ST", 120.0f));
}