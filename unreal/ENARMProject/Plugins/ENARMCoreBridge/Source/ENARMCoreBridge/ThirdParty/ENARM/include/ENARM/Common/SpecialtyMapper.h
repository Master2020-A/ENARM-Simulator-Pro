// =====================================================================
//   ENARM.Common - SpecialtyMapper (Sprint 7B)
//   Mapea specialty de casos (enum ingles) a specialty de GPCs (esp)
// =====================================================================
#pragma once

#include <string>
#include <unordered_map>
#include <vector>

namespace ENARM::Common {

class SpecialtyMapper {
public:
    // Convierte specialty de caso (Ej: "Cardiology") -> GPC (Ej: "Cardiologia")
    static std::string CaseToGuideline(const std::string& caseSpecialty) {
        static const std::unordered_map<std::string, std::string> map = {
            {"Cardiology",              "Cardiologia"},
            {"Pneumology",              "Neumologia"},
            {"Nephrology",              "Nefrologia"},
            {"Neurology",               "Neurologia"},
            {"Infectology",             "Infectologia"},
            {"EmergencyMedicine",       "Urgencias"},
            {"GynecologyObstetrics",    "Gineco-Obstetricia"},
            {"PediatricSurgery",        "Cirugia Pediatrica"},
            {"PediatricEndocrinology",  "Endocrinologia Pediatrica"},
            {"Pediatrics",              "Pediatria"},
            {"Endocrinology",           "Endocrinologia"},
            {"GeneralSurgery",          "Cirugia General"},
            {"InternalMedicine",        "Medicina Interna"},
            {"Psychiatry",              "Psiquiatria"},
            {"Dermatology",             "Dermatologia"},
            {"Ophthalmology",           "Oftalmologia"},
            {"Otorhinolaryngology",     "Otorrinolaringologia"},
            {"Urology",                 "Urologia"},
            {"Orthopedics",             "Ortopedia"},
            {"Oncology",                "Oncologia"},
            {"Hematology",              "Hematologia"},
            {"Rheumatology",            "Reumatologia"},
            {"Gastroenterology",        "Gastroenterologia"},
            {"Traumatology",            "Traumatologia"}
        };
        auto it = map.find(caseSpecialty);
        return (it != map.end()) ? it->second : caseSpecialty;
    }

    // Inverso: GPC -> caso (por si hace falta)
    static std::string GuidelineToCase(const std::string& gpcSpecialty) {
        static const std::unordered_map<std::string, std::string> map = {
            {"Cardiologia",               "Cardiology"},
            {"Neumologia",                "Pneumology"},
            {"Nefrologia",                "Nephrology"},
            {"Neurologia",                "Neurology"},
            {"Infectologia",              "Infectology"},
            {"Urgencias",                 "EmergencyMedicine"},
            {"Gineco-Obstetricia",        "GynecologyObstetrics"},
            {"Cirugia Pediatrica",        "PediatricSurgery"},
            {"Endocrinologia Pediatrica", "PediatricEndocrinology"},
            {"Pediatria",                 "Pediatrics"},
            {"Endocrinologia",            "Endocrinology"},
            {"Cirugia General",           "GeneralSurgery"}
        };
        auto it = map.find(gpcSpecialty);
        return (it != map.end()) ? it->second : gpcSpecialty;
    }

    // Nombre bonito en espanol para UI (a partir de enum ingles)
    static std::string DisplayName(const std::string& caseSpecialty) {
        return CaseToGuideline(caseSpecialty);
    }
};

} // namespace ENARM::Common