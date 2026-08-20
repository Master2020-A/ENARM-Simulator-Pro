// =====================================================================
//   ENARM.Data - GuidelineRepository (v2 - Sprint 7A.5)
// =====================================================================
#include "ENARM/Data/Repositories/GuidelineRepository.h"
#include "ENARM/Data/JsonHelpers.h"

using json = nlohmann::json;

namespace ENARM::Data {

    namespace {
        template <typename Row>
        Guideline RowToGuideline(const Row& row) {
            Guideline g;
            g.id           = row["id"].template as<std::string>();
            g.title        = row["title"].template as<std::string>();
            g.organization = GetOptString(row, "organization");
            g.year         = GetOptInt(row, "year");
            g.url          = GetOptString(row, "url");
            g.summary      = GetOptString(row, "summary");
            g.specialty    = GetOptString(row, "specialty");

            // Parsear key_recommendations JSONB
            auto recs = GetJson(row, "key_recommendations");
            if (recs.is_array()) {
                for (const auto& r : recs) {
                    GuidelineRecommendation rec;
                    rec.level = r.value("level", "");
                    rec.text  = r.value("text", "");
                    g.recommendations.push_back(rec);
                }
            }

            // Parsear tags TEXT[]
            g.tags = GetStringArray(row, "tags");

            return g;
        }
    }

    Common::Result<void> GuidelineRepository::Upsert(const Guideline& g) {
        try {
            auto conn = m_db->Acquire();
            pqxx::work tx(conn.Get());

            json recsJson = json::array();
            for (const auto& r : g.recommendations) {
                recsJson.push_back({{"level", r.level}, {"text", r.text}});
            }

            tx.exec(
                "INSERT INTO clinical_guidelines "
                "(id, title, organization, year, url, summary, specialty, key_recommendations, tags) "
                "VALUES ($1, $2, $3, $4, $5, $6, $7, $8::jsonb, $9::text[]) "
                "ON CONFLICT (id) DO UPDATE SET "
                "  title=EXCLUDED.title, organization=EXCLUDED.organization, "
                "  year=EXCLUDED.year, url=EXCLUDED.url, summary=EXCLUDED.summary, "
                "  specialty=EXCLUDED.specialty, key_recommendations=EXCLUDED.key_recommendations, "
                "  tags=EXCLUDED.tags",
                pqxx::params{
                    g.id, g.title,
                    g.organization.value_or(""),
                    g.year.value_or(0),
                    g.url.value_or(""),
                    g.summary.value_or(""),
                    g.specialty.value_or(""),
                    recsJson.dump(),
                    ToPgTextArray(g.tags)
                });
            tx.commit();
            return Common::Ok();
        } catch (const std::exception& e) {
            return Common::Err<void>(500, e.what(), "guideline_repo");
        }
    }

    std::optional<Guideline> GuidelineRepository::FindById(const std::string& id) {
        try {
            auto conn = m_db->Acquire();
            pqxx::read_transaction tx(conn.Get());
            auto res = tx.exec("SELECT * FROM clinical_guidelines WHERE id=$1",
                                pqxx::params{id});
            if (res.empty()) return std::nullopt;
            return RowToGuideline(res[0]);
        } catch (...) { return std::nullopt; }
    }

    std::vector<Guideline> GuidelineRepository::FindAll() {
        std::vector<Guideline> out;
        try {
            auto conn = m_db->Acquire();
            pqxx::read_transaction tx(conn.Get());
            auto res = tx.exec("SELECT * FROM clinical_guidelines ORDER BY specialty NULLS LAST, id");
            for (const auto& r : res) out.push_back(RowToGuideline(r));
        } catch (...) {}
        return out;
    }

    std::vector<Guideline> GuidelineRepository::FindByOrganization(const std::string& org) {
        std::vector<Guideline> out;
        try {
            auto conn = m_db->Acquire();
            pqxx::read_transaction tx(conn.Get());
            auto res = tx.exec(
                "SELECT * FROM clinical_guidelines WHERE organization=$1 ORDER BY id",
                pqxx::params{org});
            for (const auto& r : res) out.push_back(RowToGuideline(r));
        } catch (...) {}
        return out;
    }

    std::vector<Guideline> GuidelineRepository::FindBySpecialty(const std::string& specialty) {
        std::vector<Guideline> out;
        try {
            auto conn = m_db->Acquire();
            pqxx::read_transaction tx(conn.Get());
            auto res = tx.exec(
                "SELECT * FROM clinical_guidelines WHERE specialty=$1 ORDER BY id",
                pqxx::params{specialty});
            for (const auto& r : res) out.push_back(RowToGuideline(r));
        } catch (...) {}
        return out;
    }

    std::vector<Guideline> GuidelineRepository::Search(const std::string& query) {
        std::vector<Guideline> out;
        if (query.empty()) return FindAll();
        try {
            auto conn = m_db->Acquire();
            pqxx::read_transaction tx(conn.Get());
            std::string q = "%" + query + "%";
            auto res = tx.exec(
                "SELECT * FROM clinical_guidelines "
                "WHERE title ILIKE $1 OR id ILIKE $1 OR "
                "      EXISTS(SELECT 1 FROM unnest(tags) t WHERE t ILIKE $1) "
                "ORDER BY specialty NULLS LAST, id",
                pqxx::params{q});
            for (const auto& r : res) out.push_back(RowToGuideline(r));
        } catch (...) {}
        return out;
    }

    std::vector<std::string> GuidelineRepository::GetAllSpecialties() {
        std::vector<std::string> out;
        try {
            auto conn = m_db->Acquire();
            pqxx::read_transaction tx(conn.Get());
            auto res = tx.exec(
                "SELECT DISTINCT specialty FROM clinical_guidelines "
                "WHERE specialty IS NOT NULL AND specialty != '' "
                "ORDER BY specialty");
            for (const auto& r : res) {
                out.push_back(r["specialty"].as<std::string>());
            }
        } catch (...) {}
        return out;
    }

    size_t GuidelineRepository::Count() {
        try {
            auto conn = m_db->Acquire();
            pqxx::read_transaction tx(conn.Get());
            auto row = tx.exec("SELECT COUNT(*) FROM clinical_guidelines").one_row();
            return row[0].as<size_t>();
        } catch (...) { return 0; }
    }

    // -----------------------------------------------------------
    // Sprint 7B Epica 2: FindLinkedGuidelines
    // -----------------------------------------------------------
    // -----------------------------------------------------------------
// USES_ROW_TO_GUIDELINE_V2
// Sprint 7C: usa RowToGuideline() para heredar el parseo completo
// (incluyendo key_recommendations, tags, etc) que ya funciona
// perfectamente en FindBySpecialty().
// -----------------------------------------------------------------
std::vector<GuidelineRepository::LinkedGuidelineRow>
GuidelineRepository::FindLinkedGuidelines(const std::string& case_id)
{
    std::vector<LinkedGuidelineRow> out;
    if (case_id.empty() || !m_db) return out;

    try {
        auto conn = m_db->Acquire();
        pqxx::read_transaction tx(conn.Get());

        // SELECT * para heredar el parseo completo via RowToGuideline
        // + los 2 campos extra de la tabla case_guidelines
        auto res = tx.exec(
            "SELECT gl.*, cg.relevance AS __cg_relevance, cg.rationale AS __cg_rationale "
            "FROM case_guidelines cg "
            "INNER JOIN clinical_guidelines gl ON gl.id = cg.guideline_id "
            "WHERE cg.case_id = $1 "
            "ORDER BY cg.relevance ASC, gl.id ASC",
            pqxx::params{case_id});

        out.reserve(res.size());
        for (const auto& row : res) {
            LinkedGuidelineRow lg;
            // Usar el mismo parseo que FindBySpecialty (incluye key_recommendations!)
            lg.guideline = RowToGuideline(row);
            // Metadatos del vinculo
            lg.relevance = row["__cg_relevance"].as<int>();
            lg.rationale = row["__cg_rationale"].is_null()
                           ? std::string()
                           : std::string(row["__cg_rationale"].c_str());
            out.push_back(std::move(lg));
        }
    }
    catch (const std::exception&) {
        out.clear();
    }
    return out;
}

}