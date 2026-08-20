// ═══════════════════════════════════════════════════════════════
//   ENARM.AI - OpenAIClient Implementation
//   Fallback cloud cuando Ollama no esta disponible
// ═══════════════════════════════════════════════════════════════
#include "ENARM/AI/OpenAIClient.h"
#include "ENARM/AI/LLMTypes.h"

#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

#include <cstdlib>
#include <sstream>

using json = nlohmann::json;

namespace ENARM::AI {

    namespace {
        std::string RoleToStringOpenAI(MessageRole role) {
            switch (role) {
                case MessageRole::System:    return "system";
                case MessageRole::User:      return "user";
                case MessageRole::Assistant: return "assistant";
            }
            return "user";
        }

        json BuildOptionsJSON(const GenerationOptions& o) {
            json opts;
            opts["temperature"] = o.temperature;
            opts["top_p"]       = o.topP;
            opts["max_tokens"]  = o.maxTokens;
            if (o.seed >= 0) opts["seed"] = o.seed;
            return opts;
        }
    }

    OpenAIClient::OpenAIClient(OpenAIConfig config)
        : m_config(std::move(config))
    {
        if (m_config.apiKey.empty()) {
            if (const char* envKey = std::getenv("OPENAI_API_KEY")) {
                m_config.apiKey = envKey;
            }
        }
    }

    std::string OpenAIClient::BuildAuthHeader() const {
        return "Bearer " + m_config.apiKey;
    }

    bool OpenAIClient::IsAvailable() {
        if (m_config.apiKey.empty()) return false;
        try {
            auto r = cpr::Get(
                cpr::Url{m_config.endpoint + "/models"},
                cpr::Header{{"Authorization", BuildAuthHeader()}},
                cpr::Timeout{5000});
            return r.status_code == 200;
        } catch (...) {
            return false;
        }
    }

    std::vector<std::string> OpenAIClient::ListModels() {
        std::vector<std::string> models;
        if (m_config.apiKey.empty()) return models;
        try {
            auto r = cpr::Get(
                cpr::Url{m_config.endpoint + "/models"},
                cpr::Header{{"Authorization", BuildAuthHeader()}},
                cpr::Timeout{10000});
            if (r.status_code != 200) return models;
            auto j = json::parse(r.text);
            if (j.contains("data") && j["data"].is_array()) {
                for (const auto& m : j["data"]) {
                    if (m.contains("id")) {
                        models.push_back(m["id"].get<std::string>());
                    }
                }
            }
        } catch (...) {}
        return models;
    }

    Common::Result<GenerationResult> OpenAIClient::Generate(
        const std::string& model, const std::string& prompt,
        const GenerationOptions& opts)
    {
        std::vector<Message> msgs;
        msgs.push_back(Message::User(prompt));
        return Chat(model, msgs, opts);
    }

    Common::Result<GenerationResult> OpenAIClient::Chat(
        const std::string& model,
        const std::vector<Message>& messages,
        const GenerationOptions& opts)
    {
        if (m_config.apiKey.empty()) {
            return Common::Err<GenerationResult>(401,
                "OpenAI API key no configurada (OPENAI_API_KEY)", "openai");
        }

        auto start = std::chrono::high_resolution_clock::now();

        json body;
        body["model"] = model.empty() ? m_config.defaultModel : model;
        body["messages"] = json::array();
        for (const auto& m : messages) {
            body["messages"].push_back({
                {"role", RoleToStringOpenAI(m.role)},
                {"content", m.content}
            });
        }
        body["options"] = BuildOptionsJSON(opts);
        if (opts.timeoutSeconds > 0) {
            body["timeout"] = opts.timeoutSeconds;
        }

        try {
            auto r = cpr::Post(
                cpr::Url{m_config.endpoint + "/chat/completions"},
                cpr::Body{body.dump()},
                cpr::Header{{"Content-Type", "application/json"},
                            {"Authorization", BuildAuthHeader()}},
                cpr::Timeout{opts.timeoutSeconds * 1000}
            );

            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

            if (r.status_code != 200) {
                return Common::Err<GenerationResult>(
                    r.status_code,
                    "OpenAI HTTP " + std::to_string(r.status_code) + ": " + r.text,
                    "openai");
            }

            auto j = json::parse(r.text);

            GenerationResult result;
            if (j.contains("choices") && !j["choices"].empty() &&
                j["choices"][0].contains("message")) {
                result.content = j["choices"][0]["message"]["content"].get<std::string>();
            }
            if (j.contains("usage")) {
                result.promptTokens     = j["usage"].value("prompt_tokens", 0);
                result.completionTokens = j["usage"].value("completion_tokens", 0);
                result.totalTokens      = result.promptTokens + result.completionTokens;
            }
            result.model    = body["model"].get<std::string>();
            result.duration = duration;
            result.success  = true;
            if (duration.count() > 0 && result.completionTokens > 0) {
                result.tokensPerSecond = (double)result.completionTokens
                                         / (duration.count() / 1000.0);
            }
            return Common::Result<GenerationResult>::Ok(std::move(result));
        } catch (const std::exception& e) {
            return Common::Err<GenerationResult>(500,
                std::string("OpenAI error: ") + e.what(), "openai");
        }
    }

    Common::Result<GenerationResult> OpenAIClient::ChatStream(
        const std::string& model,
        const std::vector<Message>& messages,
        std::function<void(const std::string&)> onChunk,
        const GenerationOptions& opts)
    {
        if (m_config.apiKey.empty()) {
            return Common::Err<GenerationResult>(401,
                "OpenAI API key no configurada (OPENAI_API_KEY)", "openai");
        }

        auto start = std::chrono::high_resolution_clock::now();

        json body;
        body["model"] = model.empty() ? m_config.defaultModel : model;
        body["messages"] = json::array();
        for (const auto& m : messages) {
            body["messages"].push_back({
                {"role", RoleToStringOpenAI(m.role)},
                {"content", m.content}
            });
        }
        body["stream"] = true;

        std::string accumulated;

        try {
            auto r = cpr::Post(
                cpr::Url{m_config.endpoint + "/chat/completions"},
                cpr::Body{body.dump()},
                cpr::Header{{"Content-Type", "application/json"},
                             {"Authorization", BuildAuthHeader()}},
                cpr::Timeout{opts.timeoutSeconds * 1000}
            );

            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

            if (r.status_code != 200) {
                return Common::Err<GenerationResult>(r.status_code,
                    "OpenAI stream HTTP " + std::to_string(r.status_code), "openai");
            }

            // Parsear SSE: "data: {...}\n\n"
            std::istringstream iss(r.text);
            std::string line;
            while (std::getline(iss, line)) {
                if (line.rfind("data: ", 0) != 0) continue;
                std::string payload = line.substr(6);
                if (payload == "[DONE]") break;
                try {
                    auto j = json::parse(payload);
                    if (j.contains("choices") && !j["choices"].empty() &&
                        j["choices"][0].contains("delta") &&
                        j["choices"][0]["delta"].contains("content")) {
                        std::string chunk =
                            j["choices"][0]["delta"]["content"].get<std::string>();
                        accumulated += chunk;
                        if (onChunk) onChunk(chunk);
                    }
                } catch (...) {
                    // Linea malformada: ignorar
                }
            }

            GenerationResult result;
            result.content          = accumulated;
            result.model            = body["model"].get<std::string>();
            result.duration         = duration;
            result.success          = true;
            return Common::Result<GenerationResult>::Ok(std::move(result));
        } catch (const std::exception& e) {
            return Common::Err<GenerationResult>(500,
                std::string("OpenAI stream error: ") + e.what(), "openai");
        }
    }

}