// ═══════════════════════════════════════════════════════════════
//   ENARM.AI - PatientAIController
// ═══════════════════════════════════════════════════════════════
#pragma once

#include "ENARM/AI/ConversationMemory.h"
#include "ENARM/AI/LLMRouter.h"
#include "ENARM/AI/PromptTemplates/PatientPrompts.h"
#include "ENARM/Common/Result.h"

#include <memory>
#include <string>
#include <functional>
#include <vector>

namespace ENARM::AI {

    // Intencion de la pregunta del estudiante
    enum class QuestionIntent : uint8_t {
        Greeting,           // Saludo / presentacion
        OpenEnded,          // Pregunta abierta ("Que le pasa?", "Cuenteme")
        ChiefComplaint,     // Motivo de consulta especifico
        SymptomDetail,      // Detalle de un sintoma (localizacion, intensidad)
        OnsetTimeline,      // Inicio, duracion, evolucion
        PastMedicalHistory, // Antecedentes medicos
        Medications,        // Medicamentos actuales
        Allergies,          // Alergias
        FamilyHistory,      // Antecedentes familiares
        Lifestyle,          // Tabaco, alcohol, drogas
        Emotional,          // Estado emocional / psicologico
        Social              // Trabajo, vivienda, contexto social
    };

    class PatientAIController {
    public:
        PatientAIController(std::shared_ptr<LLMRouter> router,
                            PromptTemplates::PatientPersonaData persona);

        // Preguntar al paciente y recibir respuesta
        Common::Result<std::string> Ask(const std::string& doctorQuestion);

        // Version con streaming (ves las palabras aparecer)
        Common::Result<std::string> AskStream(
            const std::string& doctorQuestion,
            std::function<void(const std::string& chunk)> onChunk);

        // Estado del paciente
        void UpdateEmotionalState(const std::string& newState);
        void UpdatePainLevel(int painScale);

        // Revelacion progresiva: control de cuanto revela el paciente
        void SetDisclosureLevel(int level);   // 1=minimo, 5=maximo
        [[nodiscard]] int GetDisclosureLevel() const noexcept { return m_disclosureLevel; }

        // Intento de la ultima pregunta (para scoring del estudiante)
        [[nodiscard]] QuestionIntent GetLastIntent() const noexcept { return m_lastIntent; }
        [[nodiscard]] int GetQuestionCount() const noexcept { return m_questionCount; }

        // Consultas
        [[nodiscard]] const ConversationMemory& GetMemory() const noexcept { return m_memory; }
        [[nodiscard]] ConversationMemory& GetMemory() noexcept { return m_memory; }
        [[nodiscard]] const PromptTemplates::PatientPersonaData& GetPersona() const noexcept {
            return m_persona;
        }

        void Reset();

    private:
        void RebuildSystemPrompt();
        [[nodiscard]] QuestionIntent ClassifyIntent(const std::string& question) const;
        [[nodiscard]] std::string IntentDirective(QuestionIntent intent) const;
        [[nodiscard]] bool IsProbing(const std::string& question) const;

        std::shared_ptr<LLMRouter> m_router;
        PromptTemplates::PatientPersonaData m_persona;
        ConversationMemory m_memory;
        int m_disclosureLevel{2};
        QuestionIntent m_lastIntent{QuestionIntent::Greeting};
        int m_questionCount{0};
        bool m_chiefComplaintAsked{false};
        bool m_onsetAsked{false};
        bool m_historyAsked{false};
    };

}