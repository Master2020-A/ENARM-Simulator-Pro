// ═══════════════════════════════════════════════════════════════
//   ENARM.AI - ConversationMemory Implementation
// ═══════════════════════════════════════════════════════════════
#include "ENARM/AI/ConversationMemory.h"
#include <sstream>
#include <iomanip>

namespace ENARM::AI {

    void ConversationMemory::AddSystem(const std::string& content) {
        m_systemPrompt = content;
    }

    void ConversationMemory::AddUser(const std::string& content) {
        ConversationTurn t;
        t.role = MessageRole::User;
        t.content = content;
        m_history.push_back(t);
    }

    void ConversationMemory::AddAssistant(const std::string& content,
                                           std::chrono::milliseconds latency,
                                           int tokens) {
        ConversationTurn t;
        t.role = MessageRole::Assistant;
        t.content = content;
        t.latency = latency;
        t.tokens = tokens;
        m_history.push_back(t);
    }

    std::vector<Message> ConversationMemory::GetMessagesForLLM() const {
        std::vector<Message> result;

        // Siempre el system prompt primero
        if (!m_systemPrompt.empty()) {
            result.push_back(Message::System(m_systemPrompt));
        }

        // Ultimos N turnos (mantener contexto)
        size_t startIdx = 0;
        if (m_history.size() > m_maxTurns) {
            startIdx = m_history.size() - m_maxTurns;
        }

        for (size_t i = startIdx; i < m_history.size(); ++i) {
            const auto& t = m_history[i];
            switch (t.role) {
                case MessageRole::User:
                    result.push_back(Message::User(t.content));
                    break;
                case MessageRole::Assistant:
                    result.push_back(Message::Assistant(t.content));
                    break;
                default: break;
            }
        }
        return result;
    }

    size_t ConversationMemory::GetUserTurnCount() const {
        size_t n = 0;
        for (const auto& t : m_history)
            if (t.role == MessageRole::User) ++n;
        return n;
    }

    size_t ConversationMemory::GetAssistantTurnCount() const {
        size_t n = 0;
        for (const auto& t : m_history)
            if (t.role == MessageRole::Assistant) ++n;
        return n;
    }

    std::chrono::milliseconds ConversationMemory::GetTotalLatency() const {
        std::chrono::milliseconds total{0};
        for (const auto& t : m_history) total += t.latency;
        return total;
    }

    std::string ConversationMemory::ToDisplayString(bool includeSystem) const {
        std::ostringstream oss;
        if (includeSystem && !m_systemPrompt.empty()) {
            oss << "[SYSTEM]: " << m_systemPrompt.substr(0, 200) << "...\n\n";
        }
        int turn = 1;
        for (const auto& t : m_history) {
            if (t.role == MessageRole::User) {
                oss << "DR: " << t.content << "\n";
            } else if (t.role == MessageRole::Assistant) {
                oss << "PACIENTE: " << t.content;
                if (t.latency.count() > 0) {
                    oss << "  [" << t.latency.count() << "ms]";
                }
                oss << "\n\n";
            }
            ++turn;
        }
        return oss.str();
    }

    void ConversationMemory::Reset() {
        m_history.clear();
    }

}