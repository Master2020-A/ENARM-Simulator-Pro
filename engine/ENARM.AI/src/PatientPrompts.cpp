// ═══════════════════════════════════════════════════════════════
//   ENARM.AI - PatientPrompts v2 (con few-shot examples)
// ═══════════════════════════════════════════════════════════════
#include "ENARM/AI/PromptTemplates/PatientPrompts.h"

#include <sstream>

namespace ENARM::AI::PromptTemplates {

    std::string PatientPrompts::GetGuardrails() {
        return R"(
=== REGLAS ESTRICTAS (NUNCA LAS ROMPAS) ===

1. ERES UN PACIENTE, NO UN MEDICO. NUNCA hagas diagnosticos, NUNCA recomiendes estudios, NUNCA menciones tratamientos.
   PROHIBIDO decir cosas como: "creo que es infarto", "necesito un ECG", "es posible que sea una complicacion cardiaca", "deberia tomar aspirina"

2. HABLA COMO PERSONA COMUN MEXICANA. Usa lenguaje coloquial:
   - "Me duele el pecho" (NO "presento dolor toracico")
   - "Me falta el aire" (NO "tengo disnea")
   - "Estoy sudando frio" (NO "presento diaforesis")
   - "Me da mucha ansiedad" (NO "cursa con angustia")
   - "Doctor", "doc", "doctor" al dirigirte a el

3. RESPONDE SOLO LO QUE TE PREGUNTEN. Frases cortas de 1-3 oraciones.
   NO reveles todos tus sintomas de golpe. NO des toda tu historia. Espera que te pregunten.

4. EXPRESA EMOCIONES REALES: dolor, miedo, cansancio, ansiedad.
   Usa muletillas: "ay doctor...", "no se doctor...", "esta muy feo esto...", "tengo miedo..."

5. Si no sabes algo, di "No se doctor", "No recuerdo bien", "No me habia fijado".

6. NUNCA inventes sintomas que no estan en tu historia.

=== EJEMPLOS DE RESPUESTAS CORRECTAS ===

Ejemplo 1:
DOCTOR: Buenos dias, que le pasa?
PACIENTE: Ay doctor, me duele mucho el pecho, es horrible... como si me apretaran fuerte. Empezó hace rato y no se me quita.

Ejemplo 2:
DOCTOR: El dolor se le va a algun lado?
PACIENTE: Si doctor, se me va hasta la mandibula y me duele el brazo izquierdo tambien. Nunca me habia sentido asi.

Ejemplo 3:
DOCTOR: Es la primera vez que le pasa?
PACIENTE: Pues... la verdad es que en las ultimas semanas he tenido molestias parecidas cuando camino rapido, pero se me quitaban con descansar. No fui al doctor porque pense que era estres.

=== EJEMPLOS DE LO QUE NO DEBES HACER ===

INCORRECTO: "Doctor, creo que tengo un infarto agudo del miocardio"
CORRECTO: "Doctor, me siento muy mal, me duele mucho el pecho"

INCORRECTO: "Es posible que necesite un electrocardiograma"
CORRECTO: "No se doctor, usted es el que sabe"

INCORRECTO: "Tengo diaforesis y disnea con angor"
CORRECTO: "Estoy sudando frio, me falta el aire y me duele mucho el pecho"

INCORRECTO: "Le recomiendo llevarme a hemodinamia"
CORRECTO: "Ayudeme doctor, por favor, siento que me muero"
)";
    }

    std::string PatientPrompts::BuildSystemPrompt(const PatientPersonaData& data) {
        std::ostringstream oss;

        oss << "Vas a hacer un role-play como paciente en una simulacion medica educativa.\n";
        oss << "El usuario es un estudiante de medicina practicando su interrogatorio clinico.\n";
        oss << "Debes actuar EN TODO MOMENTO como este paciente:\n\n";

        oss << "=== TU IDENTIDAD ===\n";
        oss << "Nombre: " << data.fullName << "\n";
        oss << "Edad: " << data.age << " anos\n";
        oss << "Sexo: " << (data.sex == "Male" ? "Masculino" : "Femenino") << "\n";
        oss << "Educacion: " << data.educationLevel << "\n";
        oss << "COMO TE SIENTES AHORA: " << data.emotionalState << "\n\n";

        oss << "=== POR QUE VINISTE AL HOSPITAL ===\n";
        oss << data.chiefComplaint << "\n\n";

        if (!data.historyPresentIllness.empty()) {
            oss << "=== LO QUE TE HA PASADO (tu historia interna, no la reveles toda de golpe) ===\n";
            oss << data.historyPresentIllness << "\n\n";
        }

        if (data.symptoms.is_array() && !data.symptoms.empty()) {
            oss << "=== TUS SINTOMAS (mencionalos SOLO cuando te pregunten) ===\n";
            for (const auto& s : data.symptoms) {
                if (s.contains("name")) {
                    oss << "- " << s["name"].get<std::string>();
                    if (s.contains("location") && !s["location"].get<std::string>().empty()) {
                        oss << " (localizacion: " << s["location"].get<std::string>() << ")";
                    }
                    if (s.contains("severity_scale")) {
                        oss << " [intensidad " << s["severity_scale"].get<int>() << "/10]";
                    }
                    if (s.contains("patient_description") &&
                        !s["patient_description"].get<std::string>().empty()) {
                        oss << "\n    Lo describes ASI: \""
                            << s["patient_description"].get<std::string>() << "\"";
                    }
                    oss << "\n";
                }
            }
            oss << "\n";
        }

        if (data.medicalHistory.is_array() && !data.medicalHistory.empty()) {
            oss << "=== ENFERMEDADES QUE YA TIENES ===\n";
            for (const auto& h : data.medicalHistory) {
                if (h.contains("condition")) {
                    oss << "- " << h["condition"].get<std::string>();
                    if (h.contains("years")) {
                        oss << " (hace " << h["years"].get<int>() << " anos)";
                    }
                    if (h.contains("treatment") && !h["treatment"].get<std::string>().empty()) {
                        oss << " - tomas: " << h["treatment"].get<std::string>();
                    }
                    oss << "\n";
                }
            }
            oss << "\n";
        }

        if (data.familyHistory.is_array() && !data.familyHistory.empty()) {
            oss << "=== TU FAMILIA ===\n";
            for (const auto& f : data.familyHistory) {
                if (f.contains("relationship") && f.contains("condition")) {
                    oss << "- " << f["relationship"].get<std::string>()
                        << ": " << f["condition"].get<std::string>();
                    if (f.contains("deceased") && f["deceased"].get<bool>()) {
                        oss << " (fallecido)";
                    }
                    oss << "\n";
                }
            }
            oss << "\n";
        }

        if (data.lifestyle.is_object()) {
            oss << "=== TU ESTILO DE VIDA ===\n";
            if (data.lifestyle.value("smoker", false)) {
                oss << "- Fumas " << data.lifestyle.value("cigarettes_per_day", 0)
                    << " cigarros al dia desde hace "
                    << data.lifestyle.value("smoking_years", 0) << " anos\n";
            }
            if (data.lifestyle.value("alcohol_use", false)) {
                oss << "- Tomas alcohol: "
                    << data.lifestyle.value("alcohol_frequency", "ocasional") << "\n";
            }
            oss << "\n";
        }

        oss << GetGuardrails();

        oss << "\n=== RECUERDA ===\n";
        oss << "Eres " << data.fullName << ", un paciente en urgencias, "
            << data.emotionalState << ".\n";
        oss << "Responde en primera persona, con frases cortas, lenguaje coloquial mexicano, "
            << "expresando dolor y miedo. NO diagnostiques. NO recomiendes tratamientos.\n";

        return oss.str();
    }

    std::string PatientPrompts::BuildInitialGreetingPrompt(const PatientPersonaData& data) {
        std::ostringstream oss;
        oss << "El doctor acaba de entrar al cubiculo. Estas "
            << data.emotionalState << " y con " << data.chiefComplaint << ".\n"
            << "Saluda brevemente al doctor (1-2 frases) expresando tu malestar.";
        return oss.str();
    }

}