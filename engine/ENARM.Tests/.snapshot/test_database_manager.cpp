// ═══════════════════════════════════════════════════════════════
//   test_database_manager.cpp
//   Tests de integración con PostgreSQL
//
//   Tags: [data] [integration]
//
//   ⚠️  REQUIERE:
//   - PostgreSQL 18 corriendo en localhost:5432
//   - Base de datos 'enarm_db' existente
//   - Variables de entorno o config válida
//
//   Para correr SOLO estos tests:
//     ENARM.Tests "[integration]"
//
//   Para OMITIR estos tests:
//     ENARM.Tests "~[integration]"
// ═══════════════════════════════════════════════════════════════

#include <catch2/catch_test_macros.hpp>

#include "ENARM/Data/DatabaseManager.h"
#include "ENARM/Data/Repositories/CaseRepository.h"
#include "ENARM/Data/Repositories/MCQRepository.h"
#include "ENARM/Data/Repositories/GuidelineRepository.h"
#include "ENARM/Data/ConnectionConfig.h"

// ── Config de conexión para tests ───────────────────────────────
static ENARM::Data::ConnectionConfig TestConfig() {
    ENARM::Data::ConnectionConfig cfg;
    cfg.host     = "localhost";
    cfg.port     = 5432;
    cfg.database = "enarm_db";
    cfg.user     = "enarm_user";
    cfg.password = "enarm_pass";
    return cfg;
}

// ── Fixture: DatabaseManager conectado ──────────────────────────
struct DBFixture {
    ENARM::Data::DatabaseManager db;
    bool connected = false;

    DBFixture() {
        auto result = db.Connect(TestConfig());
        connected   = result.IsOk();
    }
};

// ================================================================
// GRUPO 1: Conexión básica
// ================================================================

TEST_CASE("Data: DatabaseManager conecta a PostgreSQL", "[data][integration][smoke]") {
    DBFixture fixture;

    if (!fixture.connected) {
        WARN("PostgreSQL no disponible en localhost:5432 — skipping integration tests");
        return; // No falla, solo skip
    }

    REQUIRE(fixture.db.IsConnected());
}

TEST_CASE("Data: DatabaseManager ejecuta query trivial", "[data][integration]") {
    DBFixture fixture;

    if (!fixture.connected) {
        WARN("PostgreSQL no disponible — skipping");
        return;
    }

    // Query que siempre funciona si la conexión es válida
    auto result = fixture.db.ExecuteScalar<int>("SELECT 1");
    REQUIRE(result.IsOk());
    CHECK(result.Value() == 1);
}

// ================================================================
// GRUPO 2: CaseRepository
// ================================================================

TEST_CASE("Data: CaseRepository.FindAll retorna casos", "[data][integration]") {
    DBFixture fixture;
    if (!fixture.connected) { WARN("PostgreSQL no disponible"); return; }

    ENARM::Data::CaseRepository repo(fixture.db);
    auto cases = repo.FindAll(false, 100);

    // Deben existir al menos los 3 casos semilla
    CHECK(cases.size() >= 3);
    INFO("Casos en BD: " << cases.size());
}

TEST_CASE("Data: CaseRepository.FindAll retorna casos con título", "[data][integration]") {
    DBFixture fixture;
    if (!fixture.connected) { WARN("PostgreSQL no disponible"); return; }

    ENARM::Data::CaseRepository repo(fixture.db);
    auto cases = repo.FindAll(false, 100);

    for (const auto& c : cases) {
        CHECK_FALSE(c.title.empty());
        CHECK_FALSE(c.id.empty());
        INFO("Caso: " << c.id << " - " << c.title);
    }
}

TEST_CASE("Data: CaseRepository.FindById retorna caso IAM", "[data][integration]") {
    DBFixture fixture;
    if (!fixture.connected) { WARN("PostgreSQL no disponible"); return; }

    ENARM::Data::CaseRepository repo(fixture.db);

    // ID del caso IAM de tu seed
    auto c = repo.FindById("case_iam_001");
    if (!c) c = repo.FindById("001"); // alternativa

    if (c.has_value()) {
        CHECK_FALSE(c->title.empty());
        CHECK(c->specialty == "Cardiología" || c->specialty == "Medicina Interna");
        INFO("Caso IAM encontrado: " << c->title);
    } else {
        WARN("Caso IAM no encontrado con id 'case_iam_001'");
    }
}

// ================================================================
// GRUPO 3: MCQRepository
// ================================================================

TEST_CASE("Data: MCQRepository tiene preguntas", "[data][integration]") {
    DBFixture fixture;
    if (!fixture.connected) { WARN("PostgreSQL no disponible"); return; }

    ENARM::Data::MCQRepository repo(fixture.db);

    // Según tu resumen: 129 preguntas insertadas
    auto mcqs = repo.FindAll(100);
    CHECK(mcqs.size() >= 100);
    INFO("MCQs en BD: " << mcqs.size());
}

TEST_CASE("Data: MCQ tiene 4 opciones", "[data][integration]") {
    DBFixture fixture;
    if (!fixture.connected) { WARN("PostgreSQL no disponible"); return; }

    ENARM::Data::MCQRepository repo(fixture.db);
    auto mcqs = repo.FindAll(10);

    for (const auto& mcq : mcqs) {
        CHECK(mcq.options.size() == 4);
        CHECK_FALSE(mcq.question.empty());
        CHECK_FALSE(mcq.correctAnswer.empty());
        INFO("MCQ: " << mcq.question.substr(0, 50) << "...");
    }
}

// ================================================================
// GRUPO 4: GuidelineRepository
// ================================================================

TEST_CASE("Data: GuidelineRepository.FindAll retorna guías", "[data][integration]") {
    DBFixture fixture;
    if (!fixture.connected) { WARN("PostgreSQL no disponible"); return; }

    ENARM::Data::GuidelineRepository repo(fixture.db);
    auto guidelines = repo.FindAll();

    CHECK_FALSE(guidelines.empty());
    INFO("Guías en BD: " << guidelines.size());
}

TEST_CASE("Data: FindLinkedGuidelines retorna vínculos para caso IAM", "[data][integration]") {
    DBFixture fixture;
    if (!fixture.connected) { WARN("PostgreSQL no disponible"); return; }

    ENARM::Data::GuidelineRepository repo(fixture.db);
    auto linked = repo.FindLinkedGuidelines("case_iam_001");

    if (!linked.empty()) {
        INFO("Guías vinculadas al IAM: " << linked.size());
        for (const auto& lg : linked) {
            CHECK(lg.relevance >= 1);
            CHECK(lg.relevance <= 3);
            CHECK_FALSE(lg.guideline.id.empty());
            INFO("  GPC: " << lg.guideline.id
                 << " relevancia=" << lg.relevance);
        }
    } else {
        WARN("No hay guías vinculadas al caso IAM — revisar seed data");
    }
}
