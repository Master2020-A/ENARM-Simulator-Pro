// ═══════════════════════════════════════════════════════════════
//   ENARM.AI - OpenAIClient (fallback cloud)
//   Misma interfaz ILLMClient, usa OpenAI-compatible API
// ═══════════════════════════════════════════════════════════════
#pragma once

#include "ENARM/AI/ILLMClient.h"

#include <string>

namespace ENARM::AI {

    struct OpenAIConfig {
        std::string apiKey;                     // REQUERIDO (env OPENAI_API_KEY)
        std::string endpoint{"https://api.openai.com/v1"};
        std::string defaultModel{"gpt-4o-mini"};
        int         defaultTimeoutSeconds{120};
        bool        verifySSL{true};
    };

    class OpenAIClient : public ILLMClient {
    public:
        explicit OpenAIClient(OpenAIConfig config = {});

        // ─── ILLMClient interface ───
        Common::Result<GenerationResult> Generate(
            const std::string& model,
            const std::string& prompt,
            const GenerationOptions& opts = {}) override;

        Common::Result<GenerationResult> Chat(
            const std::string& model,
            const std::vector<Message>& messages,
            const GenerationOptions& opts = {}) override;

        Common::Result<GenerationResult> ChatStream(
            const std::string& model,
            const std::vector<Message>& messages,
            std::function<void(const std::string& chunk)> onChunk,
            const GenerationOptions& opts = {}) override;

        [[nodiscard]] std::string GetProvider() const override { return "OpenAI"; }
        [[nodiscard]] std::vector<std::string> ListModels() override;
        [[nodiscard]] bool IsAvailable() override;

    private:
        std::string BuildAuthHeader() const;

        OpenAIConfig m_config;
    };

}