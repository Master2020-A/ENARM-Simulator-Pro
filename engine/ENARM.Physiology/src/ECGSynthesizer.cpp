#include "ENARM/Physiology/ECGSynthesizer.h"

#include <algorithm>
#include <cmath>
#include <random>

namespace ENARM::Physiology {

    ECGSynthesizer::ECGSynthesizer() = default;

    void ECGSynthesizer::SetRhythm(ECGRhythm rhythm) {
        m_rhythm = rhythm;
        switch (rhythm) {
            case ECGRhythm::SinusRhythm:        m_hr_bpm = 75.0; break;
            case ECGRhythm::SinusTachycardia:   m_hr_bpm = 120.0; break;
            case ECGRhythm::AtrialFibrillation: m_hr_bpm = 140.0; break;
            case ECGRhythm::STEMI_Anterior:     m_hr_bpm = 95.0; break;
            case ECGRhythm::STEMI_Inferior:     m_hr_bpm = 90.0; break;
            case ECGRhythm::AVBlock_1st:        m_hr_bpm = 60.0; m_pr_interval_s = 0.28; break;
            case ECGRhythm::VentricularFibrillation: m_hr_bpm = 0.0; break;
        }
    }

    std::string ECGSynthesizer::RhythmName(ECGRhythm r) {
        switch (r) {
            case ECGRhythm::SinusRhythm:        return "Ritmo sinusal";
            case ECGRhythm::SinusTachycardia:   return "Taquicardia sinusal";
            case ECGRhythm::AtrialFibrillation: return "Fibrilacion auricular";
            case ECGRhythm::STEMI_Anterior:     return "IAMCEST anterior (V1-V4)";
            case ECGRhythm::STEMI_Inferior:     return "IAMCEST inferior (DII, DIII, aVF)";
            case ECGRhythm::AVBlock_1st:        return "Bloqueo AV 1er grado";
            case ECGRhythm::VentricularFibrillation: return "Fibrilacion ventricular";
        }
        return "Desconocido";
    }

    double ECGSynthesizer::LeadGain(const std::string& lead) const {
        if (lead == "V1" || lead == "V2") return 1.2;
        if (lead == "V3" || lead == "V4") return 1.0;
        if (lead == "V5" || lead == "V6") return 0.8;
        if (lead == "II" || lead == "III") return 1.1;
        return 1.0;
    }

    double ECGSynthesizer::STShift_mV(const std::string& lead) const {
        if (m_rhythm == ECGRhythm::STEMI_Anterior) {
            if (lead == "V1" || lead == "V2" || lead == "V3" || lead == "V4")
                return 0.35;  // elevacion ST en anteriores
            if (lead == "II" || lead == "III" || lead == "aVF")
                return -0.15; // imagen especular
        }
        if (m_rhythm == ECGRhythm::STEMI_Inferior) {
            if (lead == "II" || lead == "III" || lead == "aVF")
                return 0.35;
            if (lead == "V1" || lead == "V2")
                return -0.15;
        }
        return 0.0;
    }

    double ECGSynthesizer::Baseline(double t, double hr_bpm, const std::string& lead) const {
        // FV: ondas irregulares rapidas
        if (m_rhythm == ECGRhythm::VentricularFibrillation) {
            return 0.5 * std::sin(2.0 * 3.14159 * 5.0 * t)
                 + 0.3 * std::sin(2.0 * 3.14159 * 7.5 * t + 1.0)
                 + 0.2 * std::sin(2.0 * 3.14159 * 3.0 * t + 2.0);
        }

        const double rr = 60.0 / std::max(hr_bpm, 1.0);  // segundos entre QRS
        const double tc = std::fmod(t, rr);              // tiempo en el ciclo

        // FA: RR irregular, sin onda P
        if (m_rhythm == ECGRhythm::AtrialFibrillation) {
            static std::mt19937 rng(42);
            std::uniform_real_distribution<double> rrJitter(0.85, 1.15);
            double v = 0.0;
            // Onda P ausente; oscilacion fina de linea base
            v += 0.05 * std::sin(2.0 * 3.14159 * 6.0 * t);
            // QRS
            if (tc < 0.10) {
                double x = tc / 0.10;
                v += 1.0 * std::exp(-((x - 0.5) * (x - 0.5)) / 0.02);
            }
            // T ondulante
            if (tc > 0.25 && tc < 0.45) {
                double x = (tc - 0.25) / 0.20;
                v += 0.25 * std::sin(3.14159 * x);
            }
            return v * 0.9;
        }

        double v = 0.0;

        // Onda P (ausente en FA)
        if (m_rhythm != ECGRhythm::AtrialFibrillation) {
            if (tc < m_pr_interval_s) {
                double x = tc / m_pr_interval_s;
                v += 0.15 * std::exp(-((x - 0.5) * (x - 0.5)) * 30.0);
            }
        }

        // Complejo QRS
        const double qrsStart = m_pr_interval_s + 0.04;
        if (tc > qrsStart && tc < qrsStart + 0.10) {
            double x = (tc - qrsStart) / 0.10;
            v += 1.2 * std::exp(-((x - 0.5) * (x - 0.5)) * 20.0);
            // Onda Q (pequena)
            if (x < 0.2) v -= 0.2;
        }

        // Onda T
        const double tStart = qrsStart + 0.14;
        if (tc > tStart && tc < tStart + 0.20) {
            double x = (tc - tStart) / 0.20;
            v += 0.35 * std::exp(-((x - 0.5) * (x - 0.5)) * 12.0);
        }

        // Elevacion ST (IAM)
        const double stStart = qrsStart + 0.10;
        if (tc > stStart && tc < stStart + 0.12) {
            v += STShift_mV(lead);
        }

        return v;
    }

    std::vector<ECGLeadSample> ECGSynthesizer::Generate(
        const std::string& lead, double duration_s, double sampleRate_hz) const {
        std::vector<ECGLeadSample> out;
        const size_t n = static_cast<size_t>(duration_s * sampleRate_hz);
        out.reserve(n);

        const double gain = LeadGain(lead);
        for (size_t i = 0; i < n; ++i) {
            const double t = static_cast<double>(i) / sampleRate_hz;
            ECGLeadSample s;
            s.time_s = t;
            s.voltage_mV = Baseline(t, m_hr_bpm, lead) * gain;
            out.push_back(s);
        }
        return out;
    }

}