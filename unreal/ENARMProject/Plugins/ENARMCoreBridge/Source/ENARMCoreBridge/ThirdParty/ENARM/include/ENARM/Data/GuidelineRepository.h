// =====================================================================
//   ENARM.Data - GuidelineRepository (v2 - Sprint 7A.5)
//   Con campos ricos: specialty, key_recommendations, tags
// =====================================================================
#pragma once

#include "ENARM/Common/Result.h"
#include "ENARM/Data/DatabaseManager.h"

#include <memory>
#include <string>
#include <vector>
#include <optional>
#include <nlohmann/json.hpp>

namespace ENARM::Data {

    struct GuidelineRecommendation {
        std::string level;   // "A", "B", "C"
        std::string text;
    };

    struct Guideline {
        std::string id;             // "IMSS-234-09"
        std::string title;
        std::optional<std::string> organization;
        std::optional<int> year;
        std::optional<std::string> url;
        std::optional<std::string> summary;
        std::optional<std::string> specialty;

        // Sprint 7A.5: campos nuevos
        std::vector<GuidelineRecommendation> recommendations;
        std::vector<std::string> tags;
    };

    class GuidelineRepository {
    public:
        explicit GuidelineRepository(std::shared_ptr<DatabaseManager> db) : m_db(db) {}

        Common::Result<void> Upsert(const Guideline& g);
        std::optional<Guideline> FindById(const std::string& id);
        std::vector<Guideline> FindAll();
        std::vector<Guideline> FindByOrganization(const std::string& org);
        std::vector<Guideline> FindBySpecialty(const std::string& specialty);
        std::vector<Guideline> Search(const std::string& query);  // busca en titulo/tags
        std::vector<std::string> GetAllSpecialties();  // Para filtro dropdown
        [[nodiscard]] size_t Count();

    
    // -----------------------------------------------------------
    //  Sprint 7B Epica 2: vinculacion explicita caso <-> GPC
    // -----------------------------------------------------------
    struct LinkedGuidelineRow {
        Guideline    guideline;
        int          relevance = 1;   // 1=PRIMARIA 2=SECUNDARIA 3=TERCIARIA
        std::string  rationale;
    };

    // Devuelve GPCs vinculadas explicitamente al caso.
    // Si la tabla case_guidelines no tiene filas para ese caso,
    // devuelve vector vacio (el llamador debe hacer fallback).
    std::vector<LinkedGuidelineRow>
        FindLinkedGuidelines(const std::string& case_id);

    private:
        std::shared_ptr<DatabaseManager> m_db;
    };

}