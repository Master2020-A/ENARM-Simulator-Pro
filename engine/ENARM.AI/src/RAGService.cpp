// ═══════════════════════════════════════════════════════════════
//   ENARM.AI - RAGService Implementation
//   Busqueda semantica via Ollama embeddings + pgvector
// ═══════════════════════════════════════════════════════════════
#include "ENARM/AI/RAGService.h"

#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

#include <sstream>

using json = nlohmann::json;

namespace ENARM::AI {

    RAGService::RAGService(std::shared_ptr<LLMRouter> router,
                           std::shared_ptr<Data::DatabaseManager> db)
        : m_router(std::move(router))
        , m_db(std::move(db)) {}

    std::vector<float> RAGService::EmbeddingOf(const std::string& text) {
        // Via Ollama endpoint /api/embeddings (modelo nomic-embed-text)
        std::vector<float> out;
        try {
            json body;
            body["model"] = "nomic-embed-text:latest";
            body["prompt"] = text;

            auto r = cpr::Post(
                cpr::Url{"http://localhost:11434/api/embeddings"},
                cpr::Body{body.dump()},
                cpr::Header{{"Content-Type", "application/json"}},
                cpr::Timeout{30000}
            );
            if (r.status_code != 200) return out;

            auto j = json::parse(r.text);
            if (j.contains("embedding") && j["embedding"].is_array()) {
                for (const auto& v : j["embedding"]) {
                    out.push_back(v.get<float>());
                }
            }
        } catch (...) {}
        return out;
    }

    std::vector<float> RAGService::Embed(const std::string& text) const {
        return EmbeddingOf(text);
    }

    std::string RAGService::VectorToPg(const std::vector<float>& v) {
        if (v.empty()) return "[]";
        std::string out = "[";
        for (size_t i = 0; i < v.size(); ++i) {
            if (i > 0) out += ",";
            out += std::to_string(v[i]);
        }
        out += "]";
        return out;
    }

    std::vector<SemanticHit> RAGService::SearchGuidelines(
        const std::string& query, int limit) const
    {
        std::vector<SemanticHit> hits;
        if (!IsAvailable()) return hits;

        auto emb = EmbeddingOf(query);
        if (emb.empty()) return hits;

        try {
            auto conn = m_db->Acquire();
            pqxx::work tx(conn.Get());
            auto res = tx.exec(
                "SELECT gl.id, gl.title, gl.specialty, "
                "       1 - (ge.embedding <=> $1::vector) AS similarity "
                "FROM guideline_embeddings ge "
                "JOIN clinical_guidelines gl ON gl.id = ge.guideline_id "
                "ORDER BY ge.embedding <=> $1::vector "
                "LIMIT $2",
                pqxx::params{VectorToPg(emb), limit}
            );
            for (const auto& row : res) {
                SemanticHit h;
                h.id         = row["id"].as<std::string>();
                h.title      = row["title"].as<std::string>();
                h.specialty  = row["specialty"].is_null() ? "" : row["specialty"].as<std::string>();
                h.similarity = row["similarity"].as<double>();
                hits.push_back(std::move(h));
            }
        } catch (...) {}
        return hits;
    }

    std::vector<SemanticHit> RAGService::SearchCases(
        const std::string& query, int limit) const
    {
        std::vector<SemanticHit> hits;
        if (!IsAvailable()) return hits;

        auto emb = EmbeddingOf(query);
        if (emb.empty()) return hits;

        try {
            auto conn = m_db->Acquire();
            pqxx::read_transaction tx(conn.Get());
            auto res = tx.exec(
                "SELECT cc.id, cc.title, cc.specialty, "
                "       1 - (ce.embedding <=> $1::vector) AS similarity "
                "FROM case_embeddings ce "
                "JOIN clinical_cases cc ON cc.id = ce.case_id "
                "ORDER BY ce.embedding <=> $1::vector "
                "LIMIT $2",
                pqxx::params{VectorToPg(emb), limit}
            );
            for (const auto& row : res) {
                SemanticHit h;
                h.id         = row["id"].as<std::string>();
                h.title      = row["title"].as<std::string>();
                h.specialty  = row["specialty"].as<std::string>();
                h.similarity = row["similarity"].as<double>();
                hits.push_back(std::move(h));
            }
        } catch (...) {}
        return hits;
    }

    void RAGService::IndexGuidelines() {
        if (!IsAvailable()) return;
        try {
            auto conn = m_db->Acquire();
            pqxx::work tx(conn.Get());

            auto res = tx.exec(
                "SELECT gl.id, gl.title, COALESCE(gl.summary,'') AS summary "
                "FROM clinical_guidelines gl "
                "WHERE NOT EXISTS (SELECT 1 FROM guideline_embeddings ge "
                "                  WHERE ge.guideline_id = gl.id)"
            );
            for (const auto& row : res) {
                std::string text = row["title"].as<std::string>() + " " +
                                   row["summary"].as<std::string>();
                auto emb = EmbeddingOf(text);
                if (emb.empty()) continue;
                tx.exec(
                    "INSERT INTO guideline_embeddings (guideline_id, section, embedding) "
                    "VALUES ($1, 'full', $2::vector)",
                    pqxx::params{row["id"].as<std::string>(), VectorToPg(emb)}
                );
            }
            tx.commit();
        } catch (...) {}
    }

    void RAGService::IndexCases() {
        if (!IsAvailable()) return;
        try {
            auto conn = m_db->Acquire();
            pqxx::work tx(conn.Get());

            auto res = tx.exec(
                "SELECT cc.id, cc.title, cc.chief_complaint "
                "FROM clinical_cases cc "
                "WHERE NOT EXISTS (SELECT 1 FROM case_embeddings ce "
                "                  WHERE ce.case_id = cc.id)"
            );
            for (const auto& row : res) {
                std::string text = row["title"].as<std::string>() + " " +
                                   row["chief_complaint"].as<std::string>();
                auto emb = EmbeddingOf(text);
                if (emb.empty()) continue;
                tx.exec(
                    "INSERT INTO case_embeddings (case_id, content_type, embedding) "
                    "VALUES ($1::uuid, 'full_case', $2::vector)",
                    pqxx::params{row["id"].as<std::string>(), VectorToPg(emb)}
                );
            }
            tx.commit();
        } catch (...) {}
    }

}