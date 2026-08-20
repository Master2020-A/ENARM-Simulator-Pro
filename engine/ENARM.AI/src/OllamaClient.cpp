// ═══════════════════════════════════════════════════════════════
//   ENARM.AI - OllamaClient Implementation
// ═══════════════════════════════════════════════════════════════
#include "ENARM/AI/OllamaClient.h"

#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <chrono>
#include <sstream>

using json = nlohmann::json;

namespace ENARM::AI {

    namespace {
        std::string RoleToString(MessageRole r) {
            switch (r) {
                case MessageRole::System:    return "system";
                case MessageRole::User:      return "user";
                case MessageRole::Assistant: return "assistant";
            }
            return "user";
        }

        json BuildOptions(const GenerationOptions& o) {
            json opts;
            opts["temperature"]    = o.temperature;
            opts["top_p"]          = o.topP;
            opts["top_k"]          = o.topK;
            opts["num_predict"]    = o.maxTokens;
            opts["repeat_penalty"] = o.repeatPenalty;
            opts["num_ctx"]        = o.numCtx;
            if (o.seed >= 0) opts["seed"] = o.seed;
            if (!o.stop.empty()) opts["stop"] = o.stop;
            return opts;
        }
    }

    OllamaClient::OllamaClient(OllamaConfig config)
        : m_config(std::move(config)) {}

    bool OllamaClient::IsAvailable() {
        try {
            auto r = cpr::Get(cpr::Url{m_config.endpoint + "/api/tags"},
                              cpr::Timeout{3000});
            return r.status_code == 200;
        } catch (...) {
            return false;
        }
    }

    std::vector<std::string> OllamaClient::ListModels() {
        std::vector<std::string> models;
        try {
            auto r = cpr::Get(cpr::Url{m_config.endpoint + "/api/tags"},
                              cpr::Timeout{5000});
            if (r.status_code != 200) return models;

            auto j = json::parse(r.text);
            if (j.contains("models") && j["models"].is_array()) {
                for (const auto& m : j["models"]) {
                    if (m.contains("name")) {
                        models.push_back(m["name"].get<std::string>());
                    }
                }
            }
        } catch (...) {}
        return models;
    }

    Common::Result<GenerationResult> OllamaClient::Generate(
        const std::string& model, const std::string& prompt,
        const GenerationOptions& opts)
    {
        auto start = std::chrono::high_resolution_clock::now();

        json body;
        body["model"]   = model;
        body["prompt"]  = prompt;
        body["stream"]  = false;
        body["options"] = BuildOptions(opts);

        try {
            auto r = cpr::Post(
                cpr::Url{m_config.endpoint + "/api/generate"},
                cpr::Body{body.dump()},
                cpr::Header{{"Content-Type", "application/json"}},
                cpr::Timeout{opts.timeoutSeconds * 1000}
            );

            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

            if (r.status_code != 200) {
                return Common::Err<GenerationResult>(
                    r.status_code,
                    "Ollama HTTP " + std::to_string(r.status_code) + ": " + r.text,
                    "ollama");
            }

            auto j = json::parse(r.text);

            GenerationResult result;
            result.content          = j.value("response", "");
            result.model            = model;
            result.promptTokens     = j.value("prompt_eval_count", 0);
            result.completionTokens = j.value("eval_count", 0);
            result.totalTokens      = result.promptTokens + result.completionTokens;
            result.duration         = duration;
            result.success          = true;

            if (result.duration.count() > 0 && result.completionTokens > 0) {
                result.tokensPerSecond = (double)result.completionTokens
                                         / (result.duration.count() / 1000.0);
            }

            return Common::Result<GenerationResult>::Ok(std::move(result));

        } catch (const std::exception& e) {
            return Common::Err<GenerationResult>(500,
                std::string("Ollama error: ") + e.what(), "ollama");
        }
    }

    Common::Result<GenerationResult> OllamaClient::Chat(
        const std::string& model,
        const std::vector<Message>& messages,
        const GenerationOptions& opts)
    {
        auto start = std::chrono::high_resolution_clock::now();

        json body;
        body["model"]   = model;
        body["stream"]  = false;
        body["options"] = BuildOptions(opts);

        json msgs = json::array();
        for (const auto& m : messages) {
            msgs.push_back({
                {"role", RoleToString(m.role)},
                {"content", m.content}
            });
        }
        body["messages"] = msgs;

        try {
            auto r = cpr::Post(
                cpr::Url{m_config.endpoint + "/api/chat"},
                cpr::Body{body.dump()},
                cpr::Header{{"Content-Type", "application/json"}},
                cpr::Timeout{opts.timeoutSeconds * 1000}
            );

            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

            if (r.status_code != 200) {
                return Common::Err<GenerationResult>(
                    r.status_code,
                    "Ollama HTTP " + std::to_string(r.status_code) + ": " + r.text,
                    "ollama");
            }

            auto j = json::parse(r.text);

            GenerationResult result;
            if (j.contains("message") && j["message"].contains("content")) {
                result.content = j["message"]["content"].get<std::string>();
            }
            result.model            = model;
            result.promptTokens     = j.value("prompt_eval_count", 0);
            result.completionTokens = j.value("eval_count", 0);
            result.totalTokens      = result.promptTokens + result.completionTokens;
            result.duration         = duration;
            result.success          = true;

            if (result.duration.count() > 0 && result.completionTokens > 0) {
                result.tokensPerSecond = (double)result.completionTokens
                                         / (result.duration.count() / 1000.0);
            }

            return Common::Result<GenerationResult>::Ok(std::move(result));

        } catch (const std::exception& e) {
            return Common::Err<GenerationResult>(500,
                std::string("Ollama error: ") + e.what(), "ollama");
        }
    }

    Common::Result<GenerationResult> OllamaClient::ChatStream(
        const std::string& model,
        const std::vector<Message>& messages,
        std::function<void(const std::string& chunk)> onChunk,
        const GenerationOptions& opts)
    {
        auto start = std::chrono::high_resolution_clock::now();

        json body;
        body["model"]   = model;
        body["stream"]  = true;
        body["options"] = BuildOptions(opts);

        json msgs = json::array();
        for (const auto& m : messages) {
            msgs.push_back({
                {"role", RoleToString(m.role)},
                {"content", m.content}
            });
        }
        body["messages"] = msgs;

        std::string accumulated;
        int promptTokens = 0;
        int completionTokens = 0;

        try {
            // Streaming con callback
            auto r = cpr::Post(
                cpr::Url{m_config.endpoint + "/api/chat"},
                cpr::Body{body.dump()},
                cpr::Header{{"Content-Type", "application/json"}},
                cpr::Timeout{opts.timeoutSeconds * 1000},
                cpr::WriteCallback([&](std::string_view data, intptr_t /*userdata*/) -> bool {
                    // Parsear NDJSON (una linea = un JSON)
                    std::string dataStr(data);
                    std::istringstream iss(dataStr);
                    std::string line;
                    while (std::getline(iss, line)) {
                        if (line.empty()) continue;
                        try {
                            auto j = json::parse(line);
                            if (j.contains("message") && j["message"].contains("content")) {
                                std::string chunk = j["message"]["content"].get<std::string>();
                                if (!chunk.empty()) {
                                    accumulated += chunk;
                                    if (onChunk) onChunk(chunk);
                                }
                            }
                            if (j.value("done", false)) {
                                promptTokens     = j.value("prompt_eval_count", 0);
                                completionTokens = j.value("eval_count", 0);
                            }
                        } catch (...) {
                            // Ignorar lineas mal formadas
                        }
                    }
                    return true;
                })
            );

            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

            if (r.status_code != 200 && r.status_code != 0) {
                return Common::Err<GenerationResult>(r.status_code,
                    "Ollama stream HTTP " + std::to_string(r.status_code), "ollama");
            }

            GenerationResult result;
            result.content          = accumulated;
            result.model            = model;
            result.promptTokens     = promptTokens;
            result.completionTokens = completionTokens;
            result.totalTokens      = promptTokens + completionTokens;
            result.duration         = duration;
            result.success          = true;

            if (duration.count() > 0 && completionTokens > 0) {
                result.tokensPerSecond = (double)completionTokens
                                         / (duration.count() / 1000.0);
            }

            return Common::Result<GenerationResult>::Ok(std::move(result));

        } catch (const std::exception& e) {
            return Common::Err<GenerationResult>(500,
                std::string("Ollama stream error: ") + e.what(), "ollama");
        }
    }

}