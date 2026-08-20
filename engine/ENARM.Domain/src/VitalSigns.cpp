// ═══════════════════════════════════════════════════════════════
//   ENARM.Domain - VitalSigns Implementation (v2 - con qSOFA)
// ═══════════════════════════════════════════════════════════════
#include "ENARM/Domain/VitalSigns.h"

#include <nlohmann/json.hpp>
#include <sstream>
#include <iomanip>

using json = nlohmann::json;

namespace ENARM::Domain {

    ClinicalStatus VitalSigns::GetOverallStatus() const noexcept {
        // Sistema de scoring por severidad clinica
        int emergencyScore = 0;  // Peligro inminente (>= 1 = Emergency)
        int criticalScore = 0;   // Requiere intervencion (>= 2 = Critical)
        int warningScore = 0;    // Anormal (>= 1 = Warning)

        auto scoreParam = [&](bool emergency, bool critical, bool warning) {
            if (emergency)      ++emergencyScore;
            else if (critical)  ++criticalScore;
            else if (warning)   ++warningScore;
        };

        // ─── Hemodinamico ───
        // TA sistolica: <70 emergencia, <80 critico, <90 warning
        scoreParam(systolicBP_mmHg < 70 || systolicBP_mmHg > 220,
                   systolicBP_mmHg < 80 || systolicBP_mmHg > 200,
                   systolicBP_mmHg < VitalRanges::kSystolicBPMin ||
                   systolicBP_mmHg > VitalRanges::kSystolicBPMax);

        // MAP: <60 hipoperfusion tisular
        scoreParam(meanArterialP_mmHg < 55,
                   meanArterialP_mmHg < 65,
                   meanArterialP_mmHg < 70);

        // FC
        scoreParam(heartRate_bpm < 35 || heartRate_bpm > 160,
                   heartRate_bpm < 45 || heartRate_bpm > 130,
                   heartRate_bpm < VitalRanges::kHeartRateMin ||
                   heartRate_bpm > VitalRanges::kHeartRateMax);

        // ─── Respiratorio ───
        // SpO2: <85 emergencia, <90 critico, <95 warning
        scoreParam(spO2_pct < 85,
                   spO2_pct < 90,
                   spO2_pct < VitalRanges::kSpO2Min);

        // FR: qSOFA >= 22
        scoreParam(respiratoryRate_rpm < 6 || respiratoryRate_rpm > 35,
                   respiratoryRate_rpm < 10 || respiratoryRate_rpm >= 22,
                   respiratoryRate_rpm < VitalRanges::kRespRateMin ||
                   respiratoryRate_rpm > VitalRanges::kRespRateMax);

        // ─── Metabolico ───
        scoreParam(temperature_C < 34.0 || temperature_C > 40.5,
                   temperature_C < 35.0 || temperature_C > 39.0,
                   temperature_C < VitalRanges::kTempMinC ||
                   temperature_C > VitalRanges::kTempMaxC);

        // pH
        scoreParam(pH < 7.20 || pH > 7.60,
                   pH < 7.30 || pH > 7.50,
                   pH < VitalRanges::kpHMin || pH > VitalRanges::kpHMax);

        // Lactato: marcador de hipoperfusion tisular
        // >4 = shock, >2 = hiperlactatemia, >1 = alerta
        scoreParam(lactate_mmolL > 4.0,
                   lactate_mmolL > 2.0,
                   lactate_mmolL > 1.5);

        // Glucosa: hipo severa o hiper extrema
        scoreParam(glucose_mgdL < 40 || glucose_mgdL > 600,
                   glucose_mgdL < 60 || glucose_mgdL > 400,
                   glucose_mgdL < VitalRanges::kGlucoseMin ||
                   glucose_mgdL > 180);

        // ─── Neurologico ───
        // GCS: <=8 coma (emergencia), 9-12 critico, 13-14 warning
        scoreParam(glasgowComaScale <= 8,
                   glasgowComaScale <= 12,
                   glasgowComaScale < 15);

        // ─── qSOFA (Sepsis-3): FR>=22 + PAS<=100 + AlteracionMental >=2 puntos ───
        int qsofa = 0;
        if (respiratoryRate_rpm >= 22) ++qsofa;
        if (systolicBP_mmHg <= 100)    ++qsofa;
        if (glasgowComaScale < 15)     ++qsofa;

        if (qsofa >= 2) {
            // qSOFA >= 2: sospecha de sepsis con alto riesgo mortalidad
            ++criticalScore;
        }

        // ─── Decision final ───
        if (emergencyScore >= 1)                    return ClinicalStatus::Emergency;
        if (criticalScore >= 2)                     return ClinicalStatus::Emergency;
        if (criticalScore >= 1)                     return ClinicalStatus::Critical;
        if (warningScore >= 3)                      return ClinicalStatus::Critical;
        if (warningScore >= 1)                      return ClinicalStatus::Warning;
        return ClinicalStatus::Normal;
    }

    std::string VitalSigns::ToJSON() const {
        json j;
        j["timestamp"] = std::chrono::system_clock::to_time_t(timestamp);
        j["hemodynamic"] = {
            {"systolic_bp", systolicBP_mmHg},
            {"diastolic_bp", diastolicBP_mmHg},
            {"map", meanArterialP_mmHg},
            {"heart_rate", heartRate_bpm},
            {"cvp", centralVenousP_mmHg},
            {"cardiac_output", cardiacOutput_Lmin},
            {"svr", systemicResistance}
        };
        j["respiratory"] = {
            {"resp_rate", respiratoryRate_rpm},
            {"spo2", spO2_pct},
            {"etco2", etCO2_mmHg},
            {"tidal_volume", tidalVolume_mL},
            {"minute_ventilation", minuteVentilation_L}
        };
        j["metabolic"] = {
            {"temperature", temperature_C},
            {"glucose", glucose_mgdL},
            {"lactate", lactate_mmolL},
            {"ph", pH},
            {"pco2", pCO2_mmHg},
            {"po2", pO2_mmHg},
            {"hco3", HCO3_mEqL},
            {"base_excess", baseExcess}
        };
        j["neurological"] = {
            {"gcs", glasgowComaScale},
            {"pupil_left_mm", pupilLeftMm},
            {"pupil_right_mm", pupilRightMm},
            {"pupil_left_reactive", pupilLeftReactive},
            {"pupil_right_reactive", pupilRightReactive}
        };
        j["pain_scale"] = painScale_0to10;
        if (urineOutputML_hr) j["urine_output_ml_hr"] = *urineOutputML_hr;
        return j.dump(2);
    }

    VitalSigns VitalSigns::FromJSON(const std::string& jsonStr) {
        VitalSigns v;
        auto j = json::parse(jsonStr);

        if (j.contains("timestamp")) {
            v.timestamp = std::chrono::system_clock::from_time_t(j["timestamp"]);
        }
        if (j.contains("hemodynamic")) {
            auto& h = j["hemodynamic"];
            v.systolicBP_mmHg      = h.value("systolic_bp", 120.0);
            v.diastolicBP_mmHg     = h.value("diastolic_bp", 80.0);
            v.meanArterialP_mmHg   = h.value("map", 93.0);
            v.heartRate_bpm        = h.value("heart_rate", 75.0);
            v.centralVenousP_mmHg  = h.value("cvp", 8.0);
            v.cardiacOutput_Lmin   = h.value("cardiac_output", 5.0);
            v.systemicResistance   = h.value("svr", 1100.0);
        }
        if (j.contains("respiratory")) {
            auto& r = j["respiratory"];
            v.respiratoryRate_rpm  = r.value("resp_rate", 16.0);
            v.spO2_pct             = r.value("spo2", 98.0);
            v.etCO2_mmHg           = r.value("etco2", 35.0);
            v.tidalVolume_mL       = r.value("tidal_volume", 500.0);
            v.minuteVentilation_L  = r.value("minute_ventilation", 8.0);
        }
        if (j.contains("metabolic")) {
            auto& m = j["metabolic"];
            v.temperature_C = m.value("temperature", 36.8);
            v.glucose_mgdL  = m.value("glucose", 90.0);
            v.lactate_mmolL = m.value("lactate", 1.0);
            v.pH            = m.value("ph", 7.40);
            v.pCO2_mmHg     = m.value("pco2", 40.0);
            v.pO2_mmHg      = m.value("po2", 95.0);
            v.HCO3_mEqL     = m.value("hco3", 24.0);
            v.baseExcess    = m.value("base_excess", 0.0);
        }
        if (j.contains("neurological")) {
            auto& n = j["neurological"];
            v.glasgowComaScale     = n.value("gcs", 15);
            v.pupilLeftMm          = n.value("pupil_left_mm", 3.0);
            v.pupilRightMm         = n.value("pupil_right_mm", 3.0);
            v.pupilLeftReactive    = n.value("pupil_left_reactive", true);
            v.pupilRightReactive   = n.value("pupil_right_reactive", true);
        }
        v.painScale_0to10 = j.value("pain_scale", 0);
        if (j.contains("urine_output_ml_hr")) {
            v.urineOutputML_hr = j["urine_output_ml_hr"].get<double>();
        }
        return v;
    }

    std::string VitalSigns::ToDisplayString() const {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(1);
        oss << "PA: " << systolicBP_mmHg << "/" << diastolicBP_mmHg << " mmHg  ";
        oss << "FC: " << heartRate_bpm << " lpm  ";
        oss << "FR: " << respiratoryRate_rpm << " rpm  ";
        oss << "SpO2: " << spO2_pct << "%  ";
        oss << "T: " << temperature_C << "C  ";
        oss << "GCS: " << glasgowComaScale;
        return oss.str();
    }

}