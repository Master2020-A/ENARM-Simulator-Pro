// ═══════════════════════════════════════════════════════════════
//   ENARM.AI - RAGService (Sprint 7B Epica 4)
//   Busqueda semantica de GPCs y casos via pgvector + embeddings
// ═══════════════════════════════════════════════════════════════
#pragma once

#include "ENARM/AI/LLMRouter.h"
#include "ENARM/Data/DatabaseManager.h"

#include <memory>
#include <string>
#include <vector>

namespace ENARM::AI {

    // Resultado de busqueda semantica
    struct SemanticHit {
        std::string id;
        std::string title;
        std::string specialty;
        double similarity{0.0};
        std::string snippet;
    };

    class RAGService {
    public:
        RAGService(std::shared_ptr<LLMRouter> router,
                   std::shared_ptr<Data::DatabaseManager> db);

        // Genera embedding de un texto (via Ollama nomic-embed-text)
        [[nodiscard]] std::vector<float> Embed(const std::string& text) const;

        // Busca GPCs semanticamente similares
        [[nodiscard]] std::vector<SemanticHit> SearchGuidelines(
            const std::string& query, int limit = 3) const;

        // Busca casos clinicos semanticamente similares
        [[nodiscard]] std::vector<SemanticHit> SearchCases(
            const std::string& query, int limit = 5) const;

        // Indexa todas las GPCs que no tienen embedding
        void IndexGuidelines();

        // Indexa todos los casos que no tienen embedding
        void IndexCases();

        [[nodiscard]] bool IsAvailable() const noexcept { return m_router != nullptr && m_db != nullptr; }

    private:
        [[nodiscard]] static std::string VectorToPg(const std::vector<float>& v);
        [[nodiscard]] static std::vector<float> EmbeddingOf(const std::string& text);

        std::shared_ptr<LLMRouter> m_router;
        std::shared_ptr<Data::DatabaseManager> m_db;
    };

}