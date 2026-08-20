// ═══════════════════════════════════════════════════════════════
//   ENARM.AI - PatientAIController Implementation
//   Con revelacion progresiva y deteccion de intencion
// ═══════════════════════════════════════════════════════════════
#include "ENARM/AI/PatientAIController.h"

#include <algorithm>
#include <cctype>

namespace ENARM::AI {

    namespace {
        std::string ToLower(std::string s) {
            std::transform(s.begin(), s.end(), s.begin(),
                [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
            return s;
        }

        bool ContainsAny(const std::string& haystack,
                         const std::vector<std::string>& needles) {
            const std::string h = ToLower(haystack);
            for (const auto& n : needles) {
                if (h.find(ToLower(n)) != std::string::npos) return true;
            }
            return false;
        }
    }

    PatientAIController::PatientAIController(
        std::shared_ptr<LLMRouter> router,
        PromptTemplates::PatientPersonaData persona)
        : m_router(std::move(router))
        , m_persona(std::move(persona))
    {
        RebuildSystemPrompt();
    }

    void PatientAIController::RebuildSystemPrompt() {
        std::string prompt = PromptTemplates::PatientPrompts::BuildSystemPrompt(m_persona);
        prompt += "\n=== NIVEL DE REVELACION ACTUAL: "
               + std::to_string(m_disclosureLevel) + " de 5 ===\n";
        m_memory.AddSystem(prompt);
    }

    void PatientAIController::SetDisclosureLevel(int level) {
        m_disclosureLevel = std::clamp(level, 1, 5);
        // Reconstruir prompt con nuevo nivel
        RebuildSystemPrompt();
    }

    QuestionIntent PatientAIController::ClassifyIntent(const std::string& question) const {
        if (ContainsAny(question, {"hola", "buenos dias", "buenas tardes", "buenas noches",
                                   "mucho gusto", "como esta", "como se siente"})) {
            return QuestionIntent::Greeting;
        }
        if (ContainsAny(question, {"que le pasa", "que tiene", "que le ocurre", "cuenteme",
                                   "que lo trae", "que la trae", "motivo", "platicame",
                                   "que le molesta", "como empieza"})) {
            return QuestionIntent::OpenEnded;
        }
        if (ContainsAny(question, {"dolor", "pecho", "abdomen", "cabeza", "fiebre",
                                   "tos", "vomito", "nausea", "disnea", "falta de aire",
                                   "diarrea", "orina", "sangrado"})) {
            return QuestionIntent::SymptomDetail;
        }
        if (ContainsAny(question, {"cuando empezo", "cuando comenzo", "desde cuando",
                                   "cuanto tiempo", "duracion", "empezo", "comenzo",
                                   "evolucion", "ha sido continuo", "intermitente"})) {
            return QuestionIntent::OnsetTimeline;
        }
        if (ContainsAny(question, {"antecedentes", "ha tenido", "enfermedades", "padece",
                                   "diabetes", "hipertension", "cardiaco", "opera", "cirugia"})) {
            return QuestionIntent::PastMedicalHistory;
        }
        if (ContainsAny(question, {"medicamentos", "toma algo", "que toma", "tratamiento",
                                   "pastillas", "inyecciones", "dosis"})) {
            return QuestionIntent::Medications;
        }
        if (ContainsAny(question, {"alergia", "alergico"})) {
            return QuestionIntent::Allergies;
        }
        if (ContainsAny(question, {"familia", "padre", "madre", "hermano", "hermana",
                                   "abuelo", "hijo", "hija"})) {
            return QuestionIntent::FamilyHistory;
        }
        if (ContainsAny(question, {"fuma", "tabaco", "alcohol", "toma", "cigarro",
                                   "droga", "marihuana", "cocaina"})) {
            return QuestionIntent::Lifestyle;
        }
        if (ContainsAny(question, {"siente", "emocion", "miedo", "nervioso", "angustia",
                                   "triste", "animo", "deprimido"})) {
            return QuestionIntent::Emotional;
        }
        if (ContainsAny(question, {"trabaja", "vive", "casa", "familia", "pareja",
                                   "hijos", "solventes"})) {
            return QuestionIntent::Social;
        }
        return QuestionIntent::OpenEnded;
    }

    bool PatientAIController::IsProbing(const std::string& question) const {
        // Preguntas dirigidas que merecen respuestas especificas
        return ClassifyIntent(question) != QuestionIntent::OpenEnded &&
               ClassifyIntent(question) != QuestionIntent::Greeting;
    }

    std::string PatientAIController::IntentDirective(QuestionIntent intent) const {
        switch (intent) {
            case QuestionIntent::Greeting:
                return "El doctor te saludo. Responde el saludo de forma breve y "
                       "menciona brevemente tu malestar.";
            case QuestionIntent::OpenEnded:
                return "El doctor te hizo una pregunta abierta. Responde con tu sintoma "
                       "principal, pero NO des detalles completos. Deja que pregunte mas.";
            case QuestionIntent::ChiefComplaint:
            case QuestionIntent::SymptomDetail:
                return "El doctor pregunta especificamente por tu sintoma. Responde con "
                       "detalle moderado: localizacion, intensidad, caracter.";
            case QuestionIntent::OnsetTimeline:
                return "El doctor pregunta por el inicio/duracion. Responde con el tiempo "
                       "exacto de inicio y su evolucion.";
            case QuestionIntent::PastMedicalHistory:
                return "El doctor pregunta por tus antecedentes. Revela las enfermedades "
                       "cronicas que tienes (de tu historia) sin omitirlas.";
            case QuestionIntent::Medications:
                return "El doctor pregunta por tus medicamentos. Lista los que tomas con "
                       "sus dosis si las recuerdas.";
            case QuestionIntent::Allergies:
                return "El doctor pregunta por alergias. Responde claramente si tienes o no.";
            case QuestionIntent::FamilyHistory:
                return "El doctor pregunta por tu familia. Menciona las enfermedades "
                       "relevantes de tus familiares.";
            case QuestionIntent::Lifestyle:
                return "El doctor pregunta por tu estilo de vida. Responde honestamente "
                       "sobre tabaco, alcohol y drogas.";
            case QuestionIntent::Emotional:
                return "El doctor pregunta como te sientes emocionalmente. Explica tus "
                       "emociones y miedos.";
            case QuestionIntent::Social:
                return "El doctor pregunta por tu contexto social. Responde sobre trabajo, "
                       "vivienda y familia.";
        }
        return "";
    }

    Common::Result<std::string> PatientAIController::Ask(const std::string& doctorQuestion) {
        m_memory.AddUser(doctorQuestion);
        ++m_questionCount;

        // Clasificar intencion
        m_lastIntent = ClassifyIntent(doctorQuestion);
        const bool probing = IsProbing(doctorQuestion);
        if (probing && !m_chiefComplaintAsked) m_chiefComplaintAsked = true;

        auto opts = GenerationOptions::Patient();
        opts.timeoutSeconds = 120;
        opts.maxTokens = 250;

        // Prompt con directiva de revelacion
        std::string userMsg = doctorQuestion;
        if (probing) {
            userMsg += "\n[Instruccion paciente: " + IntentDirective(m_lastIntent) + "]";
        }

        auto result = m_router->Execute(TaskType::PatientResponse,
                                          m_memory.GetMessagesForLLM(),
                                          opts);
        if (result.IsErr()) {
            return Common::Err<std::string>(result.Error().code,
                                             result.Error().message,
                                             "patient_ai");
        }

        const auto& r = result.Value();
        m_memory.AddAssistant(r.content, r.duration, r.completionTokens);
        return Common::Result<std::string>::Ok(r.content);
    }

    Common::Result<std::string> PatientAIController::AskStream(
        const std::string& doctorQuestion,
        std::function<void(const std::string&)> onChunk)
    {
        m_memory.AddUser(doctorQuestion);
        ++m_questionCount;

        m_lastIntent = ClassifyIntent(doctorQuestion);

        auto opts = GenerationOptions::Patient();
        opts.timeoutSeconds = 120;
        opts.maxTokens = 250;

        std::string userMsg = doctorQuestion;
        if (IsProbing(doctorQuestion)) {
            userMsg += "\n\n[Instruccion paciente: " + IntentDirective(m_lastIntent) + "]";
        }

        auto result = m_router->ExecuteStream(TaskType::PatientResponse,
                                                m_memory.GetMessagesForLLM(),
                                                onChunk, opts);
        if (result.IsErr()) {
            return Common::Err<std::string>(result.Error().code,
                                             result.Error().message,
                                             "patient_ai");
        }

        const auto& r = result.Value();
        m_memory.AddAssistant(r.content, r.duration, r.completionTokens);
        return Common::Result<std::string>::Ok(r.content);
    }

    void PatientAIController::UpdateEmotionalState(const std::string& newState) {
        m_persona.emotionalState = newState;
        m_memory.Reset();
        RebuildSystemPrompt();
    }

    void PatientAIController::UpdatePainLevel(int /*painScale*/) {
        // TODO Sprint 5: propagar cambio de dolor al system prompt
    }

    void PatientAIController::Reset() {
        m_memory.Reset();
        RebuildSystemPrompt();
        m_disclosureLevel = 2;
        m_lastIntent = QuestionIntent::Greeting;
        m_questionCount = 0;
        m_chiefComplaintAsked = false;
    }

}