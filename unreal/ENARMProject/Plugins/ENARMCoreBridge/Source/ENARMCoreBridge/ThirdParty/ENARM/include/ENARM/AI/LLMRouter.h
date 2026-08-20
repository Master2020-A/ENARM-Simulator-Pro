// =====================================================================
//   ENARM.AI - LLMRouter (v3 - qwen2.5:7b para paciente)
// =====================================================================
#pragma once

#include "ENARM/AI/LLMTypes.h"
#include "ENARM/AI/ILLMClient.h"

#include <memory>
#include <string>
#include <unordered_map>

namespace ENARM::AI {

    struct RouterConfig {
        // FIX v3: qwen2.5:7b es mucho mejor que phi3.5 para role-play en espanol
        // phi3.5 alucina y genera meta-texto ("## Instruction", "### Pregunta")
        std::unordered_map<TaskType, std::string> taskModelMap = {
            {TaskType::PatientResponse,      "qwen2.5:7b"},          // actor paciente (rapido + buen espanol)
            {TaskType::PatientResponseFast,  "qwen2.5:7b"},          // mismo (rapido)
            {TaskType::ClinicalReasoning,    "meditron:70b"},        // meditron para razonamiento medico profundo
            {TaskType::DosageCalculation,    "qwen2-math:72b"},      // math para dosis
            {TaskType::CaseGeneration,       "qwen2.5:7b"},          // generacion de casos
            {TaskType::EmbeddingGeneration,  "nomic-embed-text:latest"}
        };

        std::string fallbackModel{"qwen2.5:7b"};  // fallback tambien mejorado
    };

    class LLMRouter {
    public:
        LLMRouter(std::shared_ptr<ILLMClient> client, RouterConfig config = {});

        Common::Result<GenerationResult> Execute(
            TaskType task,
            const std::vector<Message>& messages,
            const GenerationOptions& opts = {});

        Common::Result<GenerationResult> ExecuteStream(
            TaskType task,
            const std::vector<Message>& messages,
            std::function<void(const std::string&)> onChunk,
            const GenerationOptions& opts = {});

        Common::Result<GenerationResult> ExecuteOnModel(
            const std::string& model,
            const std::vector<Message>& messages,
            const GenerationOptions& opts = {});

        [[nodiscard]] std::string GetModelForTask(TaskType task) const;
        void SetModelForTask(TaskType task, const std::string& model);
        [[nodiscard]] std::shared_ptr<ILLMClient> GetClient() const noexcept { return m_client; }

    private:
        std::shared_ptr<ILLMClient> m_client;
        RouterConfig m_config;
    };

}
