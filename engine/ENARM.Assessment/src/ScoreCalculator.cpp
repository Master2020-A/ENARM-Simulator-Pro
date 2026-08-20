#include <enarm/assessment/ScoreCalculator.h>
#include <enarm/assessment/CompetencyEvaluator.h>
#include <numeric>

namespace enarm::assessment {

void ScoreCalculator::calculate(PerformanceReport& report) {
    report.totalPointsEarned = 0;
    report.totalMaxPoints    = 0;

    for (const auto& ds : report.dimensionScores) {
        report.totalPointsEarned += ds.pointsEarned;
        report.totalMaxPoints    += ds.maxPoints;
    }

    report.globalPercentage = (report.totalMaxPoints > 0)
        ? (static_cast<float>(report.totalPointsEarned) /
           static_cast<float>(report.totalMaxPoints)) * 100.0f
        : 0.0f;

    report.globalPerformanceLevel = CompetencyEvaluator::getPerformanceLevel(
        report.globalPercentage);
}

void ScoreCalculator::analyzePerformance(PerformanceReport& report) {
    report.strengths.clear();
    report.weaknesses.clear();

    for (const auto& ds : report.dimensionScores) {
        std::string name = dimensionToString(ds.dimension);
        if (ds.percentage >= 80.0f) {
            report.strengths.push_back(
                name + " (" + std::to_string(static_cast<int>(ds.percentage)) + "%)");
        } else if (ds.percentage < 60.0f) {
            report.weaknesses.push_back(
                name + ": " + ds.performanceLevel +
                " (" + std::to_string(static_cast<int>(ds.percentage)) + "%)");
        }
    }
}

void ScoreCalculator::generateRecommendations(PerformanceReport& report) {
    report.recommendations.clear();

    for (const auto& ds : report.dimensionScores) {
        if (ds.percentage >= 70.0f) continue;

        switch (ds.dimension) {
            case CompetencyDimension::ClinicalInterview:
                report.recommendations.push_back(
                    "Practicar interrogatorio con metodo OPQRST para dolor y "
                    "SAMPLE para antecedentes. Referencia: GPC IMSS - Historia Clinica.");
                break;
            case CompetencyDimension::PhysicalExamination:
                report.recommendations.push_back(
                    "Repasar exploracion cardiovascular sistematica: "
                    "signos vitales completos, auscultacion de 4 focos, "
                    "pulsos perifericos. Ref: Bates Propedeutica.");
                break;
            case CompetencyDimension::DiagnosticStudies:
                report.recommendations.push_back(
                    "Estudiar algoritmos diagnosticos por patologia. "
                    "Para IAM: ECG + troponinas en < 10 min. "
                    "Para sepsis: hemocultivos + lactato antes de antibioticos.");
                break;
            case CompetencyDimension::ClinicalDiagnosis:
                report.recommendations.push_back(
                    "Mejorar razonamiento clinico con criterios diagnosticos formales. "
                    "Revisar criterios de Framingham, CURB-65, NIHSS segun patologia.");
                break;
            case CompetencyDimension::TreatmentPlan:
                report.recommendations.push_back(
                    "Estudiar protocolos de tratamiento de las GPCs IMSS. "
                    "Memorizar dosis de medicamentos criticos y tiempos clave "
                    "(puerta-balon < 90 min, antibiotico en sepsis < 1 hora).");
                break;
            case CompetencyDimension::FollowUpMonitoring:
                report.recommendations.push_back(
                    "Incluir plan de monitoreo post-tratamiento: parametros a vigilar, "
                    "complicaciones esperadas, criterios de alarma y referencia a especialidad.");
                break;
        }
    }

    // Recomendacion global si score bajo
    if (report.globalPercentage < 60.0f) {
        report.recommendations.push_back(
            "Score global < 60%: se recomienda revisar los casos basicos del simulador "
            "antes de pasar a casos complejos. Considera usar el Banco MCQ para "
            "reforzar conceptos teoricos.");
    }
}

void ScoreCalculator::evaluateTime(PerformanceReport& report,
                                    const CaseRubric&   rubric)
{
    float totalExpected = 0.0f;
    for (const auto& [dim, mins] : rubric.expectedTimeMinutes)
        totalExpected += mins;

    report.timeEfficient = (report.totalTimeMinutes <= totalExpected * 1.5f);
}

} // namespace enarm::assessment
