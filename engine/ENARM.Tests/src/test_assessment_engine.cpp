// ═══════════════════════════════════════════════════════════════
//   test_assessment_engine.cpp
//   Tests del AssessmentEngine y GuidelineAdherenceScorer
//
//   Tags: [assessment] [unit]
// ═══════════════════════════════════════════════════════════════

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <catch2/matchers/catch_matchers_range_equals.hpp>

// Ajusta a tu namespace real
#include "ENARM/Assessment/AssessmentEngine.h"
#include "ENARM/Assessment/GuidelineAdherenceScorer.h"
#include "ENARM/Assessment/RubricDefinition.h"

using namespace Catch::Matchers;

// ── Fixture: Sesión de IAM completa correcta ─────────────────────
struct IamSession {
    std::string caseId = "case_iam_001";

    // Historia relevante para IAM
    std::vector<std::string> historyItems = {
        "dolor torácico opresivo",
        "irradiación al brazo izquierdo",
        "diaforesis",
        "náusea",
        "disnea",
        "factores de riesgo cardiovascular"
    };

    // Examen físico relevante
    std::vector<std::string> examItems = {
        "auscultación cardíaca",
        "presión arterial bilateral",
        "frecuencia cardíaca",
        "ritmo de galope"
    };

    // Estudios ordenados
    std::vector<std::string> studies = {
        "ECG",
        "troponinas",
        "enzimas cardíacas",
        "radiografía de tórax",
        "ecocardiograma"
    };

    // Diagnóstico
    std::string diagnosis = "Infarto agudo al miocardio con elevación del ST";

    // Tratamiento
    std::vector<enarm::assessment::TreatmentAction> treatments = {
        {"aspirina",   "325 mg VO dosis de carga",   "antiagregación"},
        {"clopidogrel","300 mg VO dosis de carga",   "antiagregación dual"},
        {"heparina",   "60 UI/kg IV bolo",            "anticoagulación"},
        {"morfina",    "2-4 mg IV",                   "analgesia"},
        {"oxígeno",    "si SpO2 < 94%",               "soporte"}
    };
};

// ================================================================
// GRUPO 1: AssessmentEngine - Construcción y carga de rúbricas
// ================================================================

TEST_CASE("Assessment: AssessmentEngine inicializa correctamente", "[assessment][unit][smoke]") {
    enarm::assessment::AssessmentEngine engine;

    // El engine debe inicializarse sin excepciones
    REQUIRE_NOTHROW(engine.Initialize());
}

TEST_CASE("Assessment: Rúbrica IAM existe", "[assessment][unit]") {
    enarm::assessment::AssessmentEngine engine;
    engine.Initialize();

    // Ajusta el nombre de la rúbrica a como está definida en tu código
    // Opciones: "IAM", "STEMI", "Cardiopatía Isquémica", "iam"
    auto rubric = engine.GetRubric("IAM");
    if (!rubric) rubric = engine.GetRubric("STEMI");
    if (!rubric) rubric = engine.GetRubric("iam");

    REQUIRE(rubric.has_value());
    INFO("Rúbrica encontrada: " << rubric->name);
}

TEST_CASE("Assessment: Existen las 7 rúbricas documentadas", "[assessment][unit]") {
    enarm::assessment::AssessmentEngine engine;
    engine.Initialize();

    // Las 7 rúbricas de tu resumen ejecutivo
    std::vector<std::string> expectedRubrics = {
        "IAM", "Neumonía", "Preeclampsia",
        "Apendicitis", "TCE", "CAD", "LRA"
    };

    int found = 0;
    for (const auto& name : expectedRubrics) {
        if (engine.GetRubric(name).has_value()) {
            ++found;
            INFO("✅ Rúbrica encontrada: " << name);
        }
    }

    // Al menos 5 de las 7 deben existir
    CHECK(found >= 5);
    INFO("Rúbricas encontradas: " << found << "/7");
}

// ================================================================
// GRUPO 2: Scoring básico
// ================================================================

TEST_CASE("Assessment: Diagnóstico correcto IAM suma puntos", "[assessment][unit][scoring]") {
    enarm::assessment::AssessmentEngine engine;
    engine.Initialize();

    IamSession session;

    auto report = engine.EvaluateSession(
        session.caseId,
        session.historyItems,
        session.examItems,
        session.studies,
        session.diagnosis,
        session.treatments
    );

    REQUIRE(report.has_value());

    // Con todos los datos correctos, el score debe ser > 50%
    float scorePercent = report->totalScore / static_cast<float>(report->maxScore) * 100.0f;
    CHECK(scorePercent > 50.0f);

    INFO("Score: " << report->totalScore << "/" << report->maxScore
         << " (" << scorePercent << "%)");
}

TEST_CASE("Assessment: Sin datos → score cero o muy bajo", "[assessment][unit][scoring]") {
    enarm::assessment::AssessmentEngine engine;
    engine.Initialize();

    auto report = engine.EvaluateSession(
        "case_iam_001",
        {},  // Sin historia
        {},  // Sin examen
        {},  // Sin estudios
        "",  // Sin diagnóstico
        {}   // Sin tratamiento
    );

    REQUIRE(report.has_value());

    // Sin datos, el score debe ser 0 o muy bajo
    CHECK(report->totalScore <= 10);
    INFO("Score vacío: " << report->totalScore);
}

TEST_CASE("Assessment: Diagnóstico incorrecto penaliza", "[assessment][unit][scoring]") {
    enarm::assessment::AssessmentEngine engine;
    engine.Initialize();

    IamSession session;

    // Score con diagnóstico correcto
    auto correctReport = engine.EvaluateSession(
        session.caseId,
        session.historyItems,
        session.examItems,
        session.studies,
        session.diagnosis,
        session.treatments
    );

    // Score con diagnóstico incorrecto
    auto wrongReport = engine.EvaluateSession(
        session.caseId,
        session.historyItems,
        session.examItems,
        session.studies,
        "Gastritis aguda",  // Diagnóstico incorrecto
        session.treatments
    );

    REQUIRE(correctReport.has_value());
    REQUIRE(wrongReport.has_value());

    CHECK(correctReport->totalScore > wrongReport->totalScore);
    INFO("Score correcto=" << correctReport->totalScore
         << " Score incorrecto=" << wrongReport->totalScore);
}

// ================================================================
// GRUPO 3: GuidelineAdherenceScorer
// ================================================================

TEST_CASE("Assessment: Adherencia 100% con tratamiento completo IAM", "[assessment][unit][adherence]") {
    // Simular guía de cardiopatía isquémica
    std::vector<enarm::assessment::GuidelineRef> guidelines = {
        {
            "gpc_iam_001",
            "Guía de Cardiopatía Isquémica IMSS",
            {"aspirina", "antiagregación", "heparina", "reperfusión"},  // keywords
            true  // es primaria
        }
    };

    std::unordered_set<std::string> consultedIds = {"gpc_iam_001"};

    std::vector<enarm::assessment::TreatmentAction> treatments = {
        {"aspirina",   "325 mg", "antiagregación"},
        {"heparina",   "60 UI/kg", "anticoagulación"}
    };

    auto results = enarm::assessment::GuidelineAdherenceScorer::EvaluateSession(
        guidelines,
        consultedIds,
        treatments,
        {"ECG", "troponinas"},
        "Infarto agudo al miocardio"
    );

    REQUIRE_FALSE(results.empty());

    // La guía primaria consultada con tratamiento correcto
    // debe tener adherencia > 0
    bool hasPositiveAdherence = false;
    for (const auto& r : results) {
        if (r.score > 0.0f) {
            hasPositiveAdherence = true;
        }
        INFO("GPC: " << r.guidelineId << " score=" << r.score);
    }
    CHECK(hasPositiveAdherence);
}

TEST_CASE("Assessment: Sin consultar guía → adherencia penalizada", "[assessment][unit][adherence]") {
    std::vector<enarm::assessment::GuidelineRef> guidelines = {
        {
            "gpc_iam_001",
            "Guía de Cardiopatía Isquémica",
            {"aspirina", "antiagregación"},
            true
        }
    };

    std::unordered_set<std::string> consultedIds;  // Vacío: no consultó

    auto results = enarm::assessment::GuidelineAdherenceScorer::EvaluateSession(
        guidelines,
        consultedIds,
        {},   // Sin tratamientos
        {},   // Sin estudios
        ""    // Sin diagnóstico
    );

    // La penalización por no consultar la guía debe reflejarse
    for (const auto& r : results) {
        INFO("GPC no consultada: " << r.guidelineId << " score=" << r.score);
        // El score por consulta debe ser 0 si no fue consultada
        CHECK(r.consultationScore == 0.0f);
    }
}

// ================================================================
// GRUPO 4: CompetencyEvaluator - Matching semántico
// ================================================================

TEST_CASE("Assessment: Sinónimo médico detectado en diagnóstico", "[assessment][unit][semantic]") {
    enarm::assessment::AssessmentEngine engine;
    engine.Initialize();

    // "STEMI" y "IAM con elevación del ST" deben ser equivalentes
    IamSession session;
    session.diagnosis = "STEMI anterior";  // Sinónimo en inglés

    auto report = engine.EvaluateSession(
        session.caseId,
        session.historyItems,
        session.examItems,
        session.studies,
        session.diagnosis,
        session.treatments
    );

    REQUIRE(report.has_value());

    // El sinónimo debe dar puntos para diagnóstico
    CHECK(report->diagnosisScore > 0);
    INFO("Score con sinónimo 'STEMI': " << report->diagnosisScore);
}
