// ═══════════════════════════════════════════════════════════════
//   ENARM.Physiology - Tipos comunes
// ═══════════════════════════════════════════════════════════════
#pragma once

#include <chrono>

namespace ENARM::Physiology {

    // Segundos como double (facil para EDOs)
    using SimTime = double;
    using DeltaT  = double;

    // Parametros antropometricos del paciente para el modelo
    struct PatientPhysiology {
        double weight_kg    {70.0};
        double height_cm    {170.0};
        int    age_years    {40};
        bool   male         {true};

        // Blood volume estimado (Nadler): mL
        [[nodiscard]] double GetBloodVolume_mL() const noexcept {
            const double h_m = height_cm / 100.0;
            if (male) {
                return (0.3669 * h_m * h_m * h_m + 0.03219 * weight_kg + 0.6041) * 1000.0;
            } else {
                return (0.3561 * h_m * h_m * h_m + 0.03308 * weight_kg + 0.1833) * 1000.0;
            }
        }

        // BSA (Mosteller): m2
        [[nodiscard]] double GetBSA_m2() const noexcept {
            return std::sqrt((height_cm * weight_kg) / 3600.0);
        }
    };

    // Estado hemodinamico instantaneo
    struct HemodynamicState {
        double aorticPressure_mmHg     {93.0};
        double ventricularVolume_mL    {130.0};
        double ventricularPressure_mmHg{10.0};
        double aorticFlow_mL_s         {0.0};
        double strokeVolume_mL         {70.0};
        double cardiacOutput_Lmin      {5.25};
        double systolicBP_mmHg         {120.0};
        double diastolicBP_mmHg        {80.0};
        double meanArterialP_mmHg      {93.0};
        double pulsePressure_mmHg      {40.0};
        SimTime timeInCycle_s          {0.0};
    };

    // Estado respiratorio instantaneo
    struct RespiratoryState {
        double alveolarVolume_mL      {2400.0};   // FRC
        double alveolarPressure_cmH2O {0.0};
        double airflow_mL_s           {0.0};
        double tidalVolume_mL         {500.0};
        double minuteVentilation_L    {8.0};

        // Gases
        double PaO2_mmHg              {95.0};
        double PaCO2_mmHg             {40.0};
        double PAO2_mmHg              {104.0};   // Alveolar
        double PACO2_mmHg             {40.0};
        double SpO2_pct               {98.0};
        double SaO2_pct               {98.0};
        double etCO2_mmHg             {35.0};

        SimTime timeInBreath_s        {0.0};
        bool inInspiration            {true};
    };

}