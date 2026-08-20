// ═══════════════════════════════════════════════════════════════
//   ENARM.Physiology - Constantes calibradas clinicamente
// ═══════════════════════════════════════════════════════════════
#pragma once

namespace ENARM::Physiology::Constants {

    // Tiempo
    inline constexpr double kSimulationHz = 100.0;
    inline constexpr double kDefaultDt    = 0.01;

    // ─── Hemodinamica adulto sano ───
    // Calibrado para: PA 120/80, MAP 93, GC 5.0, con SV=70 y HR=75
    inline constexpr double kNormalMAP_mmHg          = 93.0;
    inline constexpr double kNormalHR_bpm            = 75.0;
    inline constexpr double kNormalStrokeVolume_mL   = 70.0;
    inline constexpr double kNormalCO_Lmin           = 5.25;

    // Windkessel calibrado (Westerhof + ajuste empirico)
    // Q_avg = 5.25 L/min = 87.5 mL/s
    // MAP objetivo = 93 mmHg
    // Rp = MAP/Q = 93 / 87.5 = 1.063 mmHg.s/mL   OK
    // Compliance: rige la relacion PP = SV/C
    //   PP objetivo = 40 mmHg, SV = 70 mL => C = 70/40 = 1.75   OK
    inline constexpr double kArterialCompliance_mL_mmHg     = 1.75;
    inline constexpr double kPeripheralResistance_mmHg_s_mL = 1.063;
    inline constexpr double kAorticCharImpedance_mmHg_s_mL  = 0.033;  // Reducido de 0.05
    inline constexpr double kBloodInertance_mmHg_s2_mL      = 0.0005;

    // Ciclo cardiaco
    inline constexpr double kSystoleFraction  = 0.35;
    inline constexpr double kEjectionFraction = 0.60;

    // ─── Respiratorio ───
    inline constexpr double kNormalRR_rpm         = 16.0;
    inline constexpr double kNormalTV_mL          = 500.0;
    inline constexpr double kNormalMinuteVent_L   = 8.0;
    inline constexpr double kLungCompliance_mL_cmH2O = 100.0;
    inline constexpr double kAirwayResistance_cmH2O_L_s = 2.0;
    inline constexpr double kChestWallCompliance_mL_cmH2O = 200.0;

    inline constexpr double kAtmosphericP_mmHg = 760.0;
    inline constexpr double kWaterVaporP_mmHg  = 47.0;
    inline constexpr double kFiO2_RoomAir      = 0.21;

    // Curva Hb
    inline constexpr double kP50_mmHg          = 26.86;
    inline constexpr double kHillCoefficient   = 2.7;

    // Fisicoquimica
    inline constexpr double kSolubilityO2_ml_mmHg_dL  = 0.003;
    inline constexpr double kSolubilityCO2_ml_mmHg_dL = 0.067;
    inline constexpr double kHbO2Capacity_mL_g        = 1.34;

    // Metabolico
    inline constexpr double kNormalpH          = 7.40;
    inline constexpr double kNormalPaCO2_mmHg  = 40.0;
    inline constexpr double kNormalHCO3_mEqL   = 24.0;
    inline constexpr double kNormalTempC       = 37.0;
    inline constexpr double kNormalDPG_umolL   = 5.0;

}