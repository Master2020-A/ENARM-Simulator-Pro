// ═══════════════════════════════════════════════════════════════
//   ENARM.Physiology - ECGSynthesizer
//   Genera trazos ECG sinteticos (12 derivaciones)
//   Ritmo sinusal, taquicardia, FA, IAM (elevacion ST), bloqueos
// ═══════════════════════════════════════════════════════════════
#pragma once

#include <string>
#include <vector>

namespace ENARM::Physiology {

    // Ritmos cardiacos soportados
    enum class ECGRhythm : uint8_t {
        SinusRhythm,       // Ritmo sinusal normal
        SinusTachycardia,  // Taquicardia sinusal
        AtrialFibrillation,// Fibrilacion auricular
        STEMI_Anterior,    // IAM ST elevado anterior (V1-V4)
        STEMI_Inferior,    // IAM ST elevado inferior (DII, DIII, aVF)
        AVBlock_1st,       // Bloqueo AV 1er grado
        VentricularFibrillation // FV (paro)
    };

    // Muestra de una derivacion
    struct ECGLeadSample {
        double time_s{0.0};
        double voltage_mV{0.0};
    };

    class ECGSynthesizer {
    public:
        ECGSynthesizer();

        // Configurar ritmo
        void SetRhythm(ECGRhythm rhythm);
        [[nodiscard]] ECGRhythm GetRhythm() const noexcept { return m_rhythm; }

        // Generar N segundos de ECG para una derivacion
        // leadName: "I","II","III","aVR","aVL","aVF","V1".."V6"
        [[nodiscard]] std::vector<ECGLeadSample> Generate(
            const std::string& lead, double duration_s, double sampleRate_hz = 250.0) const;

        // Nombre legible del ritmo
        [[nodiscard]] static std::string RhythmName(ECGRhythm r);

    private:
        [[nodiscard]] double Baseline(double t, double hr_bpm, const std::string& lead) const;
        [[nodiscard]] double LeadGain(const std::string& lead) const;
        [[nodiscard]] double STShift_mV(const std::string& lead) const;

        ECGRhythm m_rhythm{ECGRhythm::SinusRhythm};
        double m_hr_bpm{75.0};
        double m_pr_interval_s{0.16};
    };

}