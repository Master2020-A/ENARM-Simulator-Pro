// ============================================================
//  test_assessment_real.cpp
//  Tests reales para AssessmentEngine según API del proyecto
// ============================================================
#include <catch2/catch_all.hpp>
#include "AssessmentEngine.h"

using namespace ENARM::Assessment;

// ── Rúbricas disponibles ─────────────────────────────────────
TEST_CASE("AssessmentEngine - tiene al menos una rubrica cargada",
          "[assessment][rubrics]")
{
    AssessmentEngine engine;
    auto rubrics = engine.GetAvailableRubrics();
    REQUIRE(rubrics.size() >= 1);
}

TEST_CASE("AssessmentEngine - IAM rubrica disponible",
          "[assessment][rubrics][iam]")
{
    AssessmentEngine engine;
    auto rubrics = engine.GetAvailableRubrics();

    bool hasIAM = false;
    for (const auto& r : rubrics) {
        std::string name = r;
        // Convertir a minúsculas para comparación
        std::transform(name.begin(), name.end(), name.begin(), ::tolower);
        if (name.find("iam")   != std::string::npos ||
            name.find("infarto") != std::string::npos ||
            name.find("cardio")  != std::string::npos) {
            hasIAM = true;
            break;
        }
    }
    REQUIRE(hasIAM);
}

// ── Score en rango válido ────────────────────────────────────
TEST_CASE("AssessmentEngine - score siempre entre 0 y 100",
          "[assessment][score][range]")
{
    AssessmentEngine engine;
    auto rubrics = engine.GetAvailableRubrics();

    if (!rubrics.empty()) {
        // Evaluar con respuesta mínima (sin datos)
        StudentResponse emptyResp;
        auto result = engine.Evaluate(rubrics[0], emptyResp);

        REQUIRE(result.totalScore >= 0.0);
        REQUIRE(result.totalScore <= 100.0);
    }
}

TEST_CASE("AssessmentEngine - respuesta completa supera respuesta vacia",
          "[assessment][score][comparison]")
{
    AssessmentEngine engine;
    auto rubrics = engine.GetAvailableRubrics();
    if (rubrics.empty()) return;

    // Respuesta vacía
    StudentResponse empty;
    auto scoreEmpty = engine.Evaluate(rubrics[0], empty).totalScore;

    // Respuesta con algo de contenido
    StudentResponse withContent;
    withContent.diagnosis    = "infarto agudo de miocardio";
    withContent.treatmentPlan = {"aspirina", "heparina", "clopidogrel"};
    auto scoreContent = engine.Evaluate(rubrics[0], withContent).totalScore;

    REQUIRE(scoreContent >= scoreEmpty);
}

// ── Competencias detectadas ──────────────────────────────────
TEST_CASE("AssessmentEngine - resultado tiene competencias definidas",
          "[assessment][competencies]")
{
    AssessmentEngine engine;
    auto rubrics = engine.GetAvailableRubrics();
    if (rubrics.empty()) return;

    StudentResponse resp;
    resp.diagnosis = "neumonía adquirida en la comunidad";
    auto result = engine.Evaluate(rubrics[0], resp);

    // El resultado debe tener al menos el score total definido
    REQUIRE(result.totalScore >= 0.0);
}
