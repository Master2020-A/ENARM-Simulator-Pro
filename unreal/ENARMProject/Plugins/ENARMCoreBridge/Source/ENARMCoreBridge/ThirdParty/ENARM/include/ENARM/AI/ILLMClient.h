// ═══════════════════════════════════════════════════════════════
//   ENARM.AI - ILLMClient interface
// ═══════════════════════════════════════════════════════════════
#pragma once

#include "ENARM/AI/LLMTypes.h"
#include "ENARM/Common/Result.h"

#include <string>
#include <vector>
#include <memory>
#include <functional>

namespace ENARM::AI {

    // Interfaz abstracta para clientes LLM
    // Implementaciones: OllamaClient, OpenAIClient, etc.
    class ILLMClient {
    public:
        virtual ~ILLMClient() = default;

        // Generacion simple: 1 prompt -> 1 respuesta
        virtual Common::Result<GenerationResult> Generate(
            const std::string& model,
            const std::string& prompt,
            const GenerationOptions& opts = {}
        ) = 0;

        // Chat: conversacion multi-turn con roles
        virtual Common::Result<GenerationResult> Chat(
            const std::string& model,
            const std::vector<Message>& messages,
            const GenerationOptions& opts = {}
        ) = 0;

        // Streaming: recibe tokens conforme se generan (callback)
        // Retorna resultado final agregado
        virtual Common::Result<GenerationResult> ChatStream(
            const std::string& model,
            const std::vector<Message>& messages,
            std::function<void(const std::string& chunk)> onChunk,
            const GenerationOptions& opts = {}
        ) = 0;

        // Info del cliente
        [[nodiscard]] virtual std::string GetProvider() const = 0;
        [[nodiscard]] virtual std::vector<std::string> ListModels() = 0;
        [[nodiscard]] virtual bool IsAvailable() = 0;
    };

}