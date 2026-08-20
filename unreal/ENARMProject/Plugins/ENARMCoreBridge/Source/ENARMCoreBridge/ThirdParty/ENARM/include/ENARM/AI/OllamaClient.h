// ═══════════════════════════════════════════════════════════════
//   ENARM.AI - OllamaClient
// ═══════════════════════════════════════════════════════════════
#pragma once

#include "ENARM/AI/ILLMClient.h"

#include <string>
#include <memory>

namespace ENARM::AI {

    struct OllamaConfig {
        std::string endpoint{"http://localhost:11434"};
        int         defaultTimeoutSeconds{120};
        std::string defaultModel{"phi3.5:latest"};
        bool        verifySSL{false};
    };

    class OllamaClient : public ILLMClient {
    public:
        explicit OllamaClient(OllamaConfig config = {});

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

        [[nodiscard]] std::string GetProvider() const override { return "Ollama"; }
        [[nodiscard]] std::vector<std::string> ListModels() override;
        [[nodiscard]] bool IsAvailable() override;

        // Ollama-specific
        [[nodiscard]] std::string GetEndpoint() const noexcept { return m_config.endpoint; }

    private:
        OllamaConfig m_config;
    };

}