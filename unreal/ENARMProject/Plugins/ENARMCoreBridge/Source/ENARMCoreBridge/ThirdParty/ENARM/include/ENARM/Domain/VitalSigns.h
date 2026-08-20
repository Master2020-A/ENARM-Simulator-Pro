// ═══════════════════════════════════════════════════════════════
//   ENARM.Domain - Signos vitales
// ═══════════════════════════════════════════════════════════════
#pragma once

#include <chrono>
#include <optional>
#include <string>

namespace ENARM::Domain {

    enum class ClinicalStatus {
        Normal,
        Warning,
        Critical,
        Emergency
    };

    struct VitalSigns {
        std::chrono::system_clock::time_point timestamp{
            std::chrono::system_clock::now()
        };

        // Hemodinamicos
        double systolicBP_mmHg      {120.0};
        double diastolicBP_mmHg     {80.0};
        double meanArterialP_mmHg   {93.0};
        double heartRate_bpm        {75.0};
        double centralVenousP_mmHg  {8.0};
        double cardiacOutput_Lmin   {5.0};
        double systemicResistance   {1100.0};

        // Respiratorios
        double respiratoryRate_rpm  {16.0};
        double spO2_pct             {98.0};
        double etCO2_mmHg           {35.0};
        double tidalVolume_mL       {500.0};
        double minuteVentilation_L  {8.0};
        double peakInspiratoryP     {20.0};

        // Metabolicos
        double temperature_C        {36.8};
        double glucose_mgdL         {90.0};
        double lactate_mmolL        {1.0};
        double pH                   {7.40};
        double pCO2_mmHg            {40.0};
        double pO2_mmHg             {95.0};
        double HCO3_mEqL            {24.0};
        double baseExcess           {0.0};

        // Neurologicos
        int glasgowComaScale        {15};
        double pupilLeftMm          {3.0};
        double pupilRightMm         {3.0};
        bool pupilLeftReactive      {true};
        bool pupilRightReactive     {true};

        std::optional<double> urineOutputML_hr;
        int painScale_0to10         {0};

        [[nodiscard]] ClinicalStatus GetOverallStatus() const noexcept;
        [[nodiscard]] std::string ToJSON() const;
        [[nodiscard]] static VitalSigns FromJSON(const std::string& json);
        [[nodiscard]] std::string ToDisplayString() const;
    };

    struct VitalRanges {
        static constexpr double kSystolicBPMin  = 90.0;
        static constexpr double kSystolicBPMax  = 140.0;
        static constexpr double kDiastolicBPMin = 60.0;
        static constexpr double kDiastolicBPMax = 90.0;
        static constexpr double kHeartRateMin   = 60.0;
        static constexpr double kHeartRateMax   = 100.0;
        static constexpr double kRespRateMin    = 12.0;
        static constexpr double kRespRateMax    = 20.0;
        static constexpr double kSpO2Min        = 95.0;
        static constexpr double kTempMinC       = 36.0;
        static constexpr double kTempMaxC       = 37.5;
        static constexpr double kGlucoseMin     = 70.0;
        static constexpr double kGlucoseMax     = 110.0;
        static constexpr double kpHMin          = 7.35;
        static constexpr double kpHMax          = 7.45;
    };

}