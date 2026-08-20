#include <enarm/assessment/FeedbackGenerator.h>
#include <ENARM/AI/OllamaClient.h>
#include <ENARM/AI/LLMTypes.h>
#include <sstream>
#include <iomanip>

namespace enarm::assessment {

// ----------------------------------------------------------------
// buildFeedbackPrompt
// ----------------------------------------------------------------
std::string FeedbackGenerator::buildFeedbackPrompt(
    const PerformanceReport& report)
{
    std::ostringstream p;
    p << "Eres un profesor de medicina experto en preparacion ENARM Mexico. "
      << "Analiza el siguiente desempeno de un estudiante en un caso clinico "
      << "y proporciona retroalimentacion formativa en espanol mexicano. "
      << "Se conciso, especifico y motivador.\n\n";

    p << "## Caso clinico: " << report.caseId << "\n";
    p << "## Score global: " << report.totalPointsEarned
      << "/" << report.totalMaxPoints
      << " (" << std::fixed << std::setprecision(1)
      << report.globalPercentage << "%) — "
      << report.globalPerformanceLevel << "\n\n";

    p << "## Resultados por competencia:\n";
    for (const auto& ds : report.dimensionScores) {
        p << "- " << dimensionToString(ds.dimension)
          << ": " << ds.pointsEarned << "/" << ds.maxPoints
          << " (" << static_cast<int>(ds.percentage) << "%) - "
          << ds.performanceLevel << "\n";

        // Elementos faltantes mas importantes
        int missCount = 0;
        for (const auto& cs : ds.criterionScores) {
            for (const auto& missed : cs.elementsMissed) {
                if (missCount++ < 3)
                    p << "  * Faltó preguntar/solicitar: " << missed << "\n";
            }
        }
    }

    if (!report.strengths.empty()) {
        p << "\n## Fortalezas identificadas:\n";
        for (const auto& s : report.strengths) p << "- " << s << "\n";
    }

    if (!report.weaknesses.empty()) {
        p << "\n## Areas de mejora:\n";
        for (const auto& w : report.weaknesses) p << "- " << w << "\n";
    }

    p << "\n## Tu tarea:\n"
      << "1. Felicita brevemente las fortalezas (1-2 oraciones)\n"
      << "2. Explica de forma clara por que son importantes los elementos que falto cubrir\n"
      << "3. Da 2-3 consejos practicos y especificos para mejorar\n"
      << "4. Termina con una frase motivadora relacionada con el ENARM\n"
      << "5. Maximo 300 palabras en total\n"
      << "6. NO uses markdown, usa texto plano\n";

    return p.str();
}

// ----------------------------------------------------------------
// generateNarrativeFeedback (con LLM)
// ----------------------------------------------------------------
std::string FeedbackGenerator::generateNarrativeFeedback(
    const PerformanceReport& report,
    ENARM::AI::OllamaClient& client)
{
    std::string prompt = buildFeedbackPrompt(report);

    // Intentar con modelo de evaluacion
    auto result = client.Generate("qwen2.5:7b", prompt);
    if (result.IsOk() && !result.Value().content.empty()) {
        return result.Value().content;
    }

    // Fallback estatico
    return generateStaticFeedback(report);
}

// ----------------------------------------------------------------
// generateStaticFeedback (sin LLM, basado en reglas)
// ----------------------------------------------------------------
std::string FeedbackGenerator::generateStaticFeedback(
    const PerformanceReport& report)
{
    std::ostringstream fb;

    // Apertura segun score
    if (report.globalPercentage >= 80.0f) {
        fb << "Excelente desempeno en este caso clinico. "
           << "Demostraste un razonamiento clinico solido y sistematico.\n\n";
    } else if (report.globalPercentage >= 60.0f) {
        fb << "Buen esfuerzo. Tienes una base clinica adecuada "
           << "pero hay areas especificas que fortalecer.\n\n";
    } else {
        fb << "Este caso representa una oportunidad de aprendizaje importante. "
           << "Con estudio dirigido puedes mejorar significativamente.\n\n";
    }

    // Fortalezas
    if (!report.strengths.empty()) {
        fb << "FORTALEZAS:\n";
        for (const auto& s : report.strengths)
            fb << "  + " << s << "\n";
        fb << "\n";
    }

    // Areas de mejora con explicacion
    if (!report.weaknesses.empty()) {
        fb << "AREAS A FORTALECER:\n";
        for (const auto& w : report.weaknesses)
            fb << "  - " << w << "\n";
        fb << "\n";
    }

    // Recomendaciones
    if (!report.recommendations.empty()) {
        fb << "RECOMENDACIONES:\n";
        for (size_t i = 0; i < report.recommendations.size(); ++i)
            fb << "  " << (i + 1) << ". " << report.recommendations[i] << "\n";
        fb << "\n";
    }

    // Cierre motivacional
    fb << "Recuerda: el ENARM se prepara con practica deliberada y retroalimentacion. "
       << "Cada caso que practicas te acerca a tu residencia. ¡Sigue adelante!";

    return fb.str();
}

} // namespace enarm::assessment
