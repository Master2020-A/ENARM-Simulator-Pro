// ═══════════════════════════════════════════════════════════════
//   ENARM.Simulation - Clinical Scenarios v2
// ═══════════════════════════════════════════════════════════════
#include "ENARM/Simulation/ClinicalScenarios.h"
#include "ENARM/Simulation/PatientSimulator.h"

namespace ENARM::Simulation {

    std::string ClinicalScenarios::GetName(ScenarioType t) {
        switch (t) {
            case ScenarioType::HealthyBaseline:       return "Adulto sano";
            case ScenarioType::SepticShock:           return "Shock septico";
            case ScenarioType::CardiogenicShock:      return "Shock cardiogenico";
            case ScenarioType::HypovolemicShock:      return "Shock hipovolemico";
            case ScenarioType::AcuteMI_STEMI:         return "IAM STEMI";
            case ScenarioType::AcutePulmonaryEdema:   return "Edema pulmonar agudo";
            case ScenarioType::SevereAsthma:          return "Crisis asmatica";
            case ScenarioType::SevereARDS:            return "SDRA severo";
            case ScenarioType::COPDExacerbation:      return "EPOC reagudizado";
            case ScenarioType::MassivePE:             return "TEP masivo";
            case ScenarioType::DKA:                   return "Cetoacidosis diabetica";
            case ScenarioType::HypoglycemicComa:      return "Coma hipoglucemico";
            case ScenarioType::OpioidOverdose:        return "Sobredosis opioides";
            case ScenarioType::CardiacArrest_VF:      return "Paro cardiaco FV";
            case ScenarioType::SevereTBI:             return "Trauma craneal severo";
        }
        return "Desconocido";
    }

    void ClinicalScenarios::Apply(PatientSimulator& patient, ScenarioType type) {
        auto& heart = patient.GetHeart();
        auto& lungs = patient.GetLungs();

        switch (type) {
            case ScenarioType::HealthyBaseline:
                heart.ResetToNormal();
                lungs.ResetToNormal();
                break;

            case ScenarioType::SepticShock:
                heart.SimulateShock(0.8);
                lungs.SetRespiratoryRate(28);
                break;

            case ScenarioType::CardiogenicShock:
                heart.SetStrokeVolume(35);
                heart.SetHeartRate(110);
                heart.ApplyVasoconstrictionBaseline(1.5);
                break;

            case ScenarioType::HypovolemicShock:
                heart.SetStrokeVolume(40);
                heart.SetHeartRate(130);
                heart.ApplyVasoconstrictionBaseline(1.4);
                break;

            case ScenarioType::AcuteMI_STEMI:
                heart.SetHeartRate(100);
                heart.SetStrokeVolume(55);
                break;

            case ScenarioType::AcutePulmonaryEdema:
                lungs.SimulatePneumonia(0.7);
                lungs.SetRespiratoryRate(32);
                heart.SetHeartRate(115);
                break;

            case ScenarioType::SevereAsthma:
                lungs.SimulateAsthma(0.85);
                break;

            case ScenarioType::SevereARDS:
                lungs.SimulateARDS(0.9);
                break;

            case ScenarioType::COPDExacerbation:
                lungs.SimulateCOPD(0.8);
                lungs.SetFiO2(0.24);
                break;

            case ScenarioType::MassivePE:
                lungs.SetRespiratoryRate(28);
                heart.SetHeartRate(130);
                heart.SimulateShock(0.5);
                break;

            case ScenarioType::DKA:
                lungs.SetRespiratoryRate(28);
                lungs.SetTidalVolume(700);
                heart.SetHeartRate(115);
                break;

            case ScenarioType::HypoglycemicComa:
                heart.SetHeartRate(105);
                break;

            case ScenarioType::OpioidOverdose:
                lungs.SetRespiratoryRate(6);
                lungs.SetTidalVolume(300);
                heart.SetHeartRate(55);
                break;

            case ScenarioType::CardiacArrest_VF:
                heart.SetHeartRate(0);
                heart.SetStrokeVolume(0);
                break;

            case ScenarioType::SevereTBI:
                heart.ApplyVasoconstrictionBaseline(1.6);
                heart.SetHeartRate(50);
                break;
        }
    }

    ScenarioInfo ClinicalScenarios::GetInfo(ScenarioType type) {
        ScenarioInfo info;
        info.type = type;
        info.name = GetName(type);

        switch (type) {
            case ScenarioType::SepticShock:
                info.description = "Sepsis con disfuncion cardiovascular refractaria a fluidos";
                info.expectedPresentation = "Fiebre, alteracion mental, hipotension, taquicardia, oliguria";
                info.keyDiagnostics = {"Lactato", "Hemocultivos", "Procalcitonina", "Foco infeccioso"};
                info.firstLineTreatment = {
                    "Cristaloides 30 mL/kg en 3h",
                    "Antibiotico empirico en primera hora",
                    "Norepinefrina si MAP<65 tras fluidos",
                    "Hidrocortisona si refractario"
                };
                info.difficultyLevel = "Intermedio";
                break;

            case ScenarioType::AcuteMI_STEMI:
                info.description = "Oclusion coronaria completa con elevacion ST";
                info.expectedPresentation = "Dolor toracico opresivo >20min, diaforesis, disnea";
                info.keyDiagnostics = {"ECG 12 derivaciones", "Troponinas", "Rx torax", "Ecocardiograma"};
                info.firstLineTreatment = {
                    "AAS 300 mg VO", "Clopidogrel/Ticagrelor", "Heparina",
                    "ICP primaria <90 min o fibrinolisis <30 min"
                };
                info.difficultyLevel = "Intermedio";
                break;

            case ScenarioType::CardiacArrest_VF:
                info.description = "Fibrilacion ventricular sin pulso";
                info.expectedPresentation = "Perdida de conciencia, ausencia pulso";
                info.firstLineTreatment = {
                    "RCP inmediata 30:2", "Desfibrilacion 200J bifasico",
                    "Adrenalina 1 mg IV c/3-5 min", "Amiodarona 300 mg tras 3er choque"
                };
                info.difficultyLevel = "Avanzado";
                break;

            default:
                info.description = "Escenario clinico";
                info.difficultyLevel = "Variable";
                break;
        }
        return info;
    }

    std::vector<ScenarioInfo> ClinicalScenarios::ListAll() {
        std::vector<ScenarioInfo> result;
        for (uint8_t i = 0; i <= (uint8_t)ScenarioType::SevereTBI; ++i) {
            result.push_back(GetInfo((ScenarioType)i));
        }
        return result;
    }

}