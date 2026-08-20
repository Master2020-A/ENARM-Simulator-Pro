// ═══════════════════════════════════════════════════════════════
//   ENARM.Physiology - OxygenationModel Implementation
// ═══════════════════════════════════════════════════════════════
#include "ENARM/Physiology/OxygenationModel.h"
#include "ENARM/Physiology/Constants.h"

#include <cmath>
#include <algorithm>

namespace ENARM::Physiology {

    double OxygenationModel::CalculateP50Corrected(
        double pH, double PaCO2, double tempC, double DPG) noexcept
    {
        // Ajustes Severinghaus para P50
        // log10(P50/26.86) = 0.024*(37-T) + 0.40*(pH-7.4) + 0.06*log10(40/PaCO2) + ...
        const double dT   = 37.0 - tempC;
        const double dpH  = pH - Constants::kNormalpH;
        const double dCO2 = std::log10(40.0 / std::max(PaCO2, 1.0));
        const double dDPG = 0.5 * std::log10(DPG / 5.0);

        const double log_ratio = -0.024 * dT + 0.40 * dpH * -1.0 +
                                  0.06 * dCO2 + dDPG;
        return Constants::kP50_mmHg * std::pow(10.0, log_ratio);
    }

    double OxygenationModel::CalculateSaO2(
        double PaO2, double pH, double PaCO2, double tempC) noexcept
    {
        if (PaO2 <= 0.0) return 0.0;

        // Correccion Bohr: ajustar PaO2 al equivalente estandar
        const double P50 = CalculateP50Corrected(pH, PaCO2, tempC);
        // Regla practica: escalar PaO2 por relacion P50 estandar/P50 corregido
        const double PaO2_std = PaO2 * (Constants::kP50_mmHg / P50);

        // Ecuacion Severinghaus (1979):
        // S = 1 / (23400 / (P^3 + 150*P) + 1)
        const double P = PaO2_std;
        const double P3 = P * P * P;
        const double denom = 23400.0 / (P3 + 150.0 * P) + 1.0;
        const double S = 1.0 / denom;

        return std::clamp(S * 100.0, 0.0, 100.0);
    }

    double OxygenationModel::CalculatePaO2FromSaO2(
        double SaO2, double pH, double PaCO2, double tempC) noexcept
    {
        // Aproximacion iterativa (bisection)
        if (SaO2 <= 0.0) return 0.0;
        if (SaO2 >= 99.9) return 500.0;

        double lo = 1.0, hi = 500.0;
        for (int i = 0; i < 50; ++i) {
            const double mid = 0.5 * (lo + hi);
            const double S = CalculateSaO2(mid, pH, PaCO2, tempC);
            if (S < SaO2) lo = mid;
            else          hi = mid;
        }
        return 0.5 * (lo + hi);
    }

    double OxygenationModel::CalculateCaO2(
        double Hb, double SaO2, double PaO2) noexcept
    {
        // CaO2 = 1.34 * Hb * SaO2/100 + 0.003 * PaO2
        return Constants::kHbO2Capacity_mL_g * Hb * (SaO2 / 100.0)
             + Constants::kSolubilityO2_ml_mmHg_dL * PaO2;
    }

}