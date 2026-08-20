// test_pharmacology.cpp - 8 tests DrugDatabase (Singleton)
#include <catch2/catch_all.hpp>
#include "ENARM/Pharmacology/DrugDatabase.h"

using namespace ENARM::Pharmacology;

TEST_CASE("DrugDatabase - Instance singleton accesible", "[pharmacology]") {
    REQUIRE_NOTHROW(DrugDatabase::Instance());
}

TEST_CASE("DrugDatabase - Singleton retorna misma instancia", "[pharmacology]") {
    auto& db1 = DrugDatabase::Instance();
    auto& db2 = DrugDatabase::Instance();
    REQUIRE(&db1 == &db2);
}

TEST_CASE("DrugDatabase - Count mayor que cero", "[pharmacology]") {
    auto& db = DrugDatabase::Instance();
    REQUIRE(db.Count() > 0);
}

TEST_CASE("DrugDatabase - Al menos 5 drogas registradas", "[pharmacology]") {
    auto& db = DrugDatabase::Instance();
    REQUIRE(db.Count() >= 5);
}

TEST_CASE("DrugDatabase - ListAllNames sin vacios", "[pharmacology]") {
    auto& db = DrugDatabase::Instance();
    auto names = db.ListAllNames();
    REQUIRE(names.size() > 0);
    for (const auto& n : names) {
        REQUIRE_FALSE(n.empty());
    }
}

TEST_CASE("DrugDatabase - FindByName inexistente retorna nullopt", "[pharmacology]") {
    auto& db = DrugDatabase::Instance();
    auto result = db.FindByName("DrogaFictaXYZ_9999");
    REQUIRE_FALSE(result.has_value());
}

TEST_CASE("DrugDatabase - FindByName primera droga funciona", "[pharmacology]") {
    auto& db = DrugDatabase::Instance();
    auto names = db.ListAllNames();
    REQUIRE(names.size() > 0);
    auto result = db.FindByName(names[0]);
    REQUIRE(result.has_value());
}

TEST_CASE("DrugDatabase - FindByCategory no lanza", "[pharmacology]") {
    auto& db = DrugDatabase::Instance();
    REQUIRE_NOTHROW(db.FindByCategory(DrugCategory::Vasopressor));
    REQUIRE_NOTHROW(db.FindByCategory(DrugCategory::Sedative));
    REQUIRE_NOTHROW(db.FindByCategory(DrugCategory::Analgesic));
}