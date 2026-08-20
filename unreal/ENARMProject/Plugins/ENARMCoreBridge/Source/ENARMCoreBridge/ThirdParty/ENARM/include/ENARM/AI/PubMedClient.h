// ═══════════════════════════════════════════════════════════════
//   ENARM.AI - PubMedClient (Sprint 17)
//   Integracion con PubMed E-utilities (esearch/esummary/efetch)
//   Referencias bibliograficas por caso clinico
// ═══════════════════════════════════════════════════════════════
#pragma once

#include "ENARM/Common/Result.h"

#include <string>
#include <vector>

namespace ENARM::AI {

    // Referencia PubMed completa
    struct PubMedArticle {
        std::string pmid;
        std::string title;
        std::vector<std::string> authors;
        std::string journal;
        std::string date;
        std::string abstract;
        std::string doi;
        std::string url;
    };

    // Configuracion del cliente
    struct PubMedConfig {
        std::string baseUrl{"https://eutils.ncbi.nlm.nih.gov/entrez/eutils"};
        std::string apiKey;                    // Opcional: NCBI API key (10 req/s)
        std::string tool{"ENARM-Simulator-Pro"};
        int         timeoutSeconds{30};
    };

    class PubMedClient {
    public:
        explicit PubMedClient(PubMedConfig config = {});

        // Buscar PMIDs por query (esearch)
        Common::Result<std::vector<std::string>> Search(
            const std::string& query, int maxResults = 10);

        // Obtener resumenes de articulos (esummary)
        Common::Result<std::vector<PubMedArticle>> FetchSummaries(
            const std::vector<std::string>& pmids);

        // Buscar y obtener articulos de una sola vez
        Common::Result<std::vector<PubMedArticle>> SearchArticles(
            const std::string& query, int maxResults = 10);

        // Construye URL amigable del articulo
        [[nodiscard]] static std::string ArticleUrl(const std::string& pmid);

        [[nodiscard]] bool IsAvailable() const noexcept { return true; }

    private:
        [[nodiscard]] std::string BuildUrl(const std::string& endpoint,
                                           const std::string& params) const;

        PubMedConfig m_config;
    };

}