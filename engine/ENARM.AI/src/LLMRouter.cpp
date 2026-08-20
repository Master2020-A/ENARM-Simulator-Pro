// ═══════════════════════════════════════════════════════════════
//   ENARM.AI - LLMRouter Implementation
// ═══════════════════════════════════════════════════════════════
#include "ENARM/AI/LLMRouter.h"

namespace ENARM::AI {

    LLMRouter::LLMRouter(std::shared_ptr<ILLMClient> client, RouterConfig config)
        : m_client(std::move(client)), m_config(std::move(config)) {}

    std::string LLMRouter::GetModelForTask(TaskType task) const {
        auto it = m_config.taskModelMap.find(task);
        if (it != m_config.taskModelMap.end()) return it->second;
        return m_config.fallbackModel;
    }

    void LLMRouter::SetModelForTask(TaskType task, const std::string& model) {
        m_config.taskModelMap[task] = model;
    }

    Common::Result<GenerationResult> LLMRouter::Execute(
        TaskType task,
        const std::vector<Message>& messages,
        const GenerationOptions& opts)
    {
        const std::string model = GetModelForTask(task);
        auto result = m_client->Chat(model, messages, opts);

        // Fallback si falla
        if (result.IsErr() && model != m_config.fallbackModel) {
            auto fbResult = m_client->Chat(m_config.fallbackModel, messages, opts);
            return fbResult;
        }
        return result;
    }

    Common::Result<GenerationResult> LLMRouter::ExecuteStream(
        TaskType task,
        const std::vector<Message>& messages,
        std::function<void(const std::string&)> onChunk,
        const GenerationOptions& opts)
    {
        const std::string model = GetModelForTask(task);
        return m_client->ChatStream(model, messages, onChunk, opts);
    }

    Common::Result<GenerationResult> LLMRouter::ExecuteOnModel(
        const std::string& model,
        const std::vector<Message>& messages,
        const GenerationOptions& opts)
    {
        return m_client->Chat(model, messages, opts);
    }

}