// ═══════════════════════════════════════════════════════════════
//   ENARM.AI - Memoria conversacional
// ═══════════════════════════════════════════════════════════════
#pragma once

#include "ENARM/AI/LLMTypes.h"

#include <vector>
#include <string>
#include <chrono>

namespace ENARM::AI {

    struct ConversationTurn {
        MessageRole role;
        std::string content;
        std::chrono::system_clock::time_point timestamp{
            std::chrono::system_clock::now()
        };
        std::chrono::milliseconds latency{0};
        int tokens{0};
    };

    class ConversationMemory {
    public:
        explicit ConversationMemory(size_t maxTurns = 20)
            : m_maxTurns(maxTurns) {}

        // Agregar mensaje
        void AddSystem(const std::string& content);
        void AddUser(const std::string& content);
        void AddAssistant(const std::string& content,
                          std::chrono::milliseconds latency = std::chrono::milliseconds(0),
                          int tokens = 0);

        // Obtener mensajes para pasar al LLM (respeta el limite)
        [[nodiscard]] std::vector<Message> GetMessagesForLLM() const;

        // Historial completo
        [[nodiscard]] const std::vector<ConversationTurn>& GetHistory() const noexcept {
            return m_history;
        }

        // Stats
        [[nodiscard]] size_t GetUserTurnCount() const;
        [[nodiscard]] size_t GetAssistantTurnCount() const;
        [[nodiscard]] std::chrono::milliseconds GetTotalLatency() const;

        // Formato para mostrar
        [[nodiscard]] std::string ToDisplayString(bool includeSystem = false) const;

        // Limpiar (mantiene solo el system prompt)
        void Reset();

    private:
        size_t m_maxTurns;
        std::vector<ConversationTurn> m_history;
        std::string m_systemPrompt;
    };

}