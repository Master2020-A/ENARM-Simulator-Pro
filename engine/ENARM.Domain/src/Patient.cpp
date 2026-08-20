// ═══════════════════════════════════════════════════════════════
//   ENARM.Domain - Patient Implementation
// ═══════════════════════════════════════════════════════════════
#include "ENARM/Domain/Patient.h"

#include <sstream>
#include <iomanip>

namespace ENARM::Domain {

    std::string Patient::GetBMICategory() const {
        const double bmi = GetBMI();
        if (bmi < 18.5) return "Bajo peso";
        if (bmi < 25.0) return "Normal";
        if (bmi < 30.0) return "Sobrepeso";
        if (bmi < 35.0) return "Obesidad I";
        if (bmi < 40.0) return "Obesidad II";
        return "Obesidad III (morbida)";
    }

    std::string Patient::ToDisplayString() const {
        std::ostringstream oss;
        oss << "===================================================\n";
        oss << " PACIENTE: " << fullName << "\n";
        oss << " ID: " << id.ToString() << "\n";
        oss << "---------------------------------------------------\n";
        oss << " Edad: " << age << " anos | ";

        switch (sex) {
            case Sex::Male:     oss << "Sexo: Masculino"; break;
            case Sex::Female:   oss << "Sexo: Femenino"; break;
            case Sex::Intersex: oss << "Sexo: Intersex"; break;
            default:            oss << "Sexo: No especificado"; break;
        }
        oss << "\n";

        oss << std::fixed << std::setprecision(1);
        oss << " Peso: " << weight_kg << " kg | ";
        oss << "Talla: " << height_cm << " cm | ";
        oss << "IMC: " << GetBMI() << " (" << GetBMICategory() << ")\n";

        oss << "---------------------------------------------------\n";
        oss << " SIGNOS VITALES:\n " << currentVitals.ToDisplayString() << "\n";

        if (!allergies.empty()) {
            oss << "---------------------------------------------------\n";
            oss << " ALERGIAS: ";
            for (size_t i = 0; i < allergies.size(); ++i) {
                oss << allergies[i].substance;
                if (i + 1 < allergies.size()) oss << ", ";
            }
            oss << "\n";
        } else {
            oss << " ALERGIAS: Ninguna conocida\n";
        }

        if (!medicalHistory.empty()) {
            oss << "---------------------------------------------------\n";
            oss << " ANTECEDENTES:\n";
            for (const auto& h : medicalHistory) {
                oss << "   - " << h.condition;
                if (!h.treatment.empty()) oss << " -> " << h.treatment;
                oss << "\n";
            }
        }
        oss << "===================================================\n";
        return oss.str();
    }

}