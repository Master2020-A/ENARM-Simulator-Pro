// ═══════════════════════════════════════════════════════════════
//   ENARM.Physiology - Curva disociacion Hb (Kelman + Bohr)
// ═══════════════════════════════════════════════════════════════
//
// Basado en:
//   Kelman GR. "Digital computer subroutine for the conversion of
//   oxygen tension into saturation". J Appl Physiol. 1966;21(4):1375-6.
//
//   Severinghaus JW. "Simple, accurate equations for human blood O2
//   dissociation computations". J Appl Physiol. 1979;46(3):599-602.
// ═══════════════════════════════════════════════════════════════
#pragma once

namespace ENARM::Physiology {

    class OxygenationModel {
    public:
        // Convierte PaO2 (mmHg) a SaO2 (%) usando curva Severinghaus
        // Con correccion por pH, PaCO2, temperatura (efecto Bohr)
        [[nodiscard]] static double CalculateSaO2(
            double PaO2_mmHg,
            double pH = 7.40,
            double PaCO2_mmHg = 40.0,
            double tempC = 37.0
        ) noexcept;

        // Inversa: SaO2 -> PaO2
        [[nodiscard]] static double CalculatePaO2FromSaO2(
            double SaO2_pct,
            double pH = 7.40,
            double PaCO2_mmHg = 40.0,
            double tempC = 37.0
        ) noexcept;

        // Contenido arterial O2 (CaO2, mL O2/dL)
        [[nodiscard]] static double CalculateCaO2(
            double Hb_gdL,
            double SaO2_pct,
            double PaO2_mmHg
        ) noexcept;

        // P50 corregido por Bohr (mmHg)
        [[nodiscard]] static double CalculateP50Corrected(
            double pH,
            double PaCO2_mmHg,
            double tempC,
            double DPG_umolL = 5.0
        ) noexcept;
    };

}