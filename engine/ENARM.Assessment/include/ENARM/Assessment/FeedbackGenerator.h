#pragma once
#include <enarm/assessment/RubricDefinition.h>
#include <string>

// Forward declaration para evitar include pesado
namespace ENARM::AI { class OllamaClient; }

namespace enarm::assessment {

class FeedbackGenerator {
public:
    // Genera retroalimentacion narrativa usando LLM
    // Retorna string vacio si LLM no disponible
    static std::string generateNarrativeFeedback(
        const PerformanceReport& report,
        ENARM::AI::OllamaClient& client);

    // Fallback sin LLM: genera feedback basado en reglas
    static std::string generateStaticFeedback(
        const PerformanceReport& report);

    // Construye el prompt para el LLM
    static std::string buildFeedbackPrompt(
        const PerformanceReport& report);
};

} // namespace enarm::assessment
