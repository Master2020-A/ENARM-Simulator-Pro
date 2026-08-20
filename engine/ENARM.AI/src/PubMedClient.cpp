// ═══════════════════════════════════════════════════════════════
//   ENARM.AI - PubMedClient Implementation
//   E-utilities: esearch + esummary
// ═══════════════════════════════════════════════════════════════
#include "ENARM/AI/PubMedClient.h"

#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

#include <sstream>

using json = nlohmann::json;

namespace ENARM::AI {

    PubMedClient::PubMedClient(PubMedConfig config)
        : m_config(std::move(config)) {}

    std::string PubMedClient::BuildUrl(const std::string& endpoint,
                                       const std::string& params) const {
        std::string url = m_config.baseUrl + "/" + endpoint + "?";
        url += params;
        if (!m_config.apiKey.empty()) {
            url += "&api_key=" + m_config.apiKey;
        }
        url += "&tool=" + m_config.tool;
        return url;
    }

    Common::Result<std::vector<std::string>> PubMedClient::Search(
        const std::string& query, int maxResults)
    {
        std::vector<std::string> pmids;
        try {
            // esearch devuelve XML con <IdList>
            std::string params = "db=pubmed&retmode=json&retmax=" +
                                 std::to_string(maxResults) + "&term=" + query;
            auto r = cpr::Get(cpr::Url{BuildUrl("esearch.fcgi", params)},
                              cpr::Timeout{m_config.timeoutSeconds * 1000});
            if (r.status_code != 200) {
                return Common::Err<std::vector<std::string>>(
                    r.status_code, "PubMed HTTP " + std::to_string(r.status_code), "pubmed");
            }

            auto j = json::parse(r.text);
            if (j.contains("esearchresult") &&
                j["esearchresult"].contains("idlist") &&
                j["esearchresult"]["idlist"].is_array()) {
                for (const auto& id : j["esearchresult"]["idlist"]) {
                    pmids.push_back(id.get<std::string>());
                }
            }
            return Common::Result<std::vector<std::string>>::Ok(std::move(pmids));
        } catch (const std::exception& e) {
            return Common::Err<std::vector<std::string>>(500,
                std::string("PubMed search error: ") + e.what(), "pubmed");
        }
    }

    Common::Result<std::vector<PubMedArticle>> PubMedClient::FetchSummaries(
        const std::vector<std::string>& pmids)
    {
        std::vector<PubMedArticle> articles;
        if (pmids.empty()) return Common::Ok(articles);

        try {
            // esummary en JSON
            std::string idList;
            for (size_t i = 0; i < pmids.size(); ++i) {
                if (i > 0) idList += ",";
                idList += pmids[i];
            }
            std::string params = "db=pubmed&retmode=json&id=" + idList;
            auto r = cpr::Get(cpr::Url{BuildUrl("esummary.fcgi", params)},
                              cpr::Timeout{m_config.timeoutSeconds * 1000});
            if (r.status_code != 200) {
                return Common::Err<std::vector<PubMedArticle>>(
                    r.status_code, "PubMed HTTP " + std::to_string(r.status_code), "pubmed");
            }

            auto j = json::parse(r.text);
            if (j.contains("result")) {
                for (const auto& pmid : pmids) {
                    if (!j["result"].contains(pmid)) continue;
                    const auto& a = j["result"][pmid];
                    if (!a.contains("uid")) continue;  // uid = pmid

                    PubMedArticle art;
                    art.pmid = a.value("uid", "");
                    art.title = a.value("title", "");
                    if (a.contains("authors") && a["authors"].is_array()) {
                        for (const auto& au : a["authors"]) {
                            if (au.contains("name")) {
                                art.authors.push_back(au["name"].get<std::string>());
                            }
                        }
                    }
                    art.journal = a.value("fulljournalname", a.value("source", ""));
                    art.date = a.value("pubdate", "");
                    art.doi = a.value("elocationid", "");
                    art.url = ArticleUrl(art.pmid);
                    articles.push_back(std::move(art));
                }
            }
            return Common::Result<std::vector<PubMedArticle>>::Ok(std::move(articles));
        } catch (const std::exception& e) {
            return Common::Err<std::vector<PubMedArticle>>(500,
                std::string("PubMed summary error: ") + e.what(), "pubmed");
        }
    }

    Common::Result<std::vector<PubMedArticle>> PubMedClient::SearchArticles(
        const std::string& query, int maxResults)
    {
        auto searchRes = Search(query, maxResults);
        if (searchRes.IsErr()) {
            return Common::Err<std::vector<PubMedArticle>>(
                searchRes.Error().code, searchRes.Error().message, "pubmed");
        }
        return FetchSummaries(searchRes.Value());
    }

    std::string PubMedClient::ArticleUrl(const std::string& pmid) {
        return "https://pubmed.ncbi.nlm.nih.gov/" + pmid + "/";
    }

}