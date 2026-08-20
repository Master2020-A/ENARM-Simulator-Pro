// â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•
//   ENARM.CLI v3 - CLI GENERICO (funciona con cualquier caso)
//   El scoring y estudios se leen del caso clinico
// â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•
#include "ENARM/Common/Version.h"
#include "ENARM/Common/Logger.h"

#include "ENARM/AI/OllamaClient.h"
#include "ENARM/AI/LLMRouter.h"
#include "ENARM/AI/PatientAIController.h"

#include "ENARM/Data/DatabaseManager.h"
#include "ENARM/Data/Repositories/CaseRepository.h"
#include "ENARM/Data/Repositories/SessionRepository.h"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include <unordered_map>

using namespace ENARM;
using json = nlohmann::json;

// â”€â”€â”€ Utilidades â”€â”€â”€
static std::string ToLower(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(),
        [](unsigned char c) { return (char)std::tolower(c); });
    return s;
}

static std::string Trim(std::string s) {
    auto notSpace = [](unsigned char c) { return !std::isspace(c); };
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), notSpace));
    s.erase(std::find_if(s.rbegin(), s.rend(), notSpace).base(), s.end());
    return s;
}

static std::string ExtractAfterFirstSpace(const std::string& s) {
    auto pos = s.find(' ');
    if (pos == std::string::npos) return "";
    return Trim(s.substr(pos + 1));
}

// Detecta si un termino de busqueda esta contenido (por palabras clave) en un texto
static bool MatchesKeyword(const std::string& searchTerm, const std::string& targetName) {
    std::string search = ToLower(searchTerm);
    std::string target = ToLower(targetName);

    // Match directo
    if (target.find(search) != std::string::npos) return true;
    if (search.find(target) != std::string::npos) return true;

    // Match por palabras clave (primera palabra significativa)
    std::istringstream iss(search);
    std::string word;
    while (iss >> word) {
        if (word.length() < 3) continue;   // ignorar palabras muy cortas
        if (target.find(word) != std::string::npos) return true;
    }
    std::istringstream iss2(target);
    while (iss2 >> word) {
        if (word.length() < 3) continue;
        if (search.find(word) != std::string::npos) return true;
    }
    return false;
}

// â”€â”€â”€ Colores ANSI â”€â”€â”€
static const char* CYAN    = "\033[36m";
static const char* YELLOW  = "\033[33m";
static const char* GREEN   = "\033[32m";
static const char* RED     = "\033[31m";
static const char* MAGENTA = "\033[35m";
static const char* GRAY    = "\033[90m";
static const char* BOLD    = "\033[1m";
static const char* RESET   = "\033[0m";

// â”€â”€â”€ Scoring â”€â”€â”€
struct SessionScore {
    int interview{0};      int interviewMax{25};
    int physicalExam{0};   int physicalExamMax{15};
    int studies{0};        int studiesMax{50};
    int diagnosis{0};      int diagnosisMax{20};
    int treatment{0};      int treatmentMax{70};
    int followUp{0};       int followUpMax{5};

    [[nodiscard]] int Total() const {
        return interview + physicalExam + studies + diagnosis + treatment + followUp;
    }
    [[nodiscard]] int TotalMax() const {
        return interviewMax + physicalExamMax + studiesMax + diagnosisMax + treatmentMax + followUpMax;
    }
    [[nodiscard]] double Percentage() const {
        return TotalMax() > 0 ? (100.0 * Total() / TotalMax()) : 0.0;
    }
    [[nodiscard]] std::string Level() const {
        auto p = Percentage();
        if (p >= 90) return "EXCELENTE";
        if (p >= 75) return "BUENO";
        if (p >= 60) return "SUFICIENTE";
        if (p >= 40) return "INSUFICIENTE";
        return "REPROBADO";
    }
};

struct SessionState {
    std::vector<std::string> studiesRequested;
    std::vector<std::string> treatmentsApplied;
    std::vector<std::string> examinations;
    std::string diagnosisRegistered;
    bool diagnosisCorrect{false};
    SessionScore score;
};

// â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•
//  CaseSimulator - motor generico basado en el caso
// â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•
class CaseSimulator {
public:
    explicit CaseSimulator(const Data::ClinicalCaseRecord& c) : m_case(c) {
        BuildStudyResultsCatalog();
        BuildTreatmentAliases();
    }

    // Simular resultado de un estudio (basado en el caso)
    std::string SimulateStudyResult(const std::string& studyName, int& scoreGained) {
        scoreGained = 0;

        // Buscar match en los estudios esperados del caso
        if (!m_case.expectedStudies.is_array()) {
            return "âŒ Este caso no tiene estudios esperados configurados.";
        }

        for (const auto& expected : m_case.expectedStudies) {
            if (!expected.contains("name")) continue;
            std::string expName = expected["name"].get<std::string>();

            if (MatchesKeyword(studyName, expName)) {
                scoreGained = expected.value("score_weight", 3);
                std::string result = LookupStudyResult(expName);
                if (result.empty()) {
                    // Resultado generico si no hay simulacion especifica
                    result = GenerateGenericResult(expName, expected);
                }
                return result;
            }
        }

        // No matcheo con ningun estudio esperado - dar sugerencias
        std::ostringstream oss;
        oss << "âŒ Estudio no reconocido para este caso.\n";
        oss << "   Estudios esperados en este caso:\n";
        int count = 0;
        for (const auto& expected : m_case.expectedStudies) {
            if (!expected.contains("name")) continue;
            std::string name = expected["name"].get<std::string>();
            // Mostrar solo primeros 3 palabras clave
            std::istringstream iss(name);
            std::string abbrev;
            std::string w;
            int i = 0;
            while (iss >> w && i < 3) {
                if (!abbrev.empty()) abbrev += " ";
                abbrev += w;
                ++i;
            }
            oss << "     - " << abbrev << "\n";
            if (++count >= 12) break;
        }
        return oss.str();
    }

    // Simular examen fisico
    std::string SimulateExamination(const std::string& region, int& scoreGained) {
        scoreGained = 3;
        std::string r = ToLower(region);

        // Auscultacion cardiaca
        if (r.find("cardio") != std::string::npos || r.find("corazon") != std::string::npos ||
            r.find("ausculta") != std::string::npos) {
            return GetCardioExam();
        }
        if (r.find("respira") != std::string::npos || r.find("pulmon") != std::string::npos ||
            r.find("torax") != std::string::npos) {
            return GetRespiratoryExam();
        }
        if (r.find("abdomen") != std::string::npos) {
            return GetAbdomenExam();
        }
        if (r.find("neuro") != std::string::npos) {
            return GetNeuroExam();
        }
        if (r.find("general") != std::string::npos || r.find("aspecto") != std::string::npos) {
            return GetGeneralExam();
        }
        if (r.find("extrem") != std::string::npos || r.find("pierna") != std::string::npos) {
            return GetExtremitiesExam();
        }
        if (r.find("cuello") != std::string::npos || r.find("yugular") != std::string::npos) {
            return GetNeckExam();
        }
        if (r.find("renal") != std::string::npos || r.find("lumbar") != std::string::npos ||
            r.find("giordano") != std::string::npos) {
            return GetRenalExam();
        }

        scoreGained = 0;
        return "âŒ Region no reconocida. Ejemplos:\n"
               "   general, cardio, respira, abdomen, neuro, extrem, cuello, renal";
    }

    // Puntuar tratamiento (busca match en tratamientos esperados)
    int ScoreForTreatment(const std::string& tx) {
        if (!m_case.expectedTreatments.is_array()) return 0;

        int totalScore = 0;
        std::string txLower = ToLower(tx);

        for (const auto& expected : m_case.expectedTreatments) {
            if (!expected.contains("intervention")) continue;
            std::string expInterv = expected["intervention"].get<std::string>();

            if (MatchesKeyword(tx, expInterv)) {
                int weight = expected.value("score_weight", 3);
                totalScore += weight;
            }
        }
        return totalScore;
    }

    // Verificar diagnostico (busca palabras clave con sinonimos medicos)
    bool CheckDiagnosis(const std::string& dx) {
        if (!m_case.expectedDiagnosis.is_object()) return false;
        if (!m_case.expectedDiagnosis.contains("primary")) return false;

        std::string expected = m_case.expectedDiagnosis["primary"].get<std::string>();
        std::string dxLower = ToLower(dx);
        std::string expLower = ToLower(expected);

        // Expandir sinonimos medicos comunes (bidireccional)
        auto expandSynonyms = [](std::string s) -> std::string {
            // IAM = infarto agudo miocardio
            // STEMI = elevacion st
            // ACV = evento vascular cerebral / stroke
            // NEUMONIA = pna
            if (s.find("iam") != std::string::npos) s += " infarto miocardio";
            if (s.find("mi ") != std::string::npos || s.find(" mi") != std::string::npos) s += " infarto miocardio";
            if (s.find("stemi") != std::string::npos) s += " elevacion st infarto";
            if (s.find("nstemi") != std::string::npos) s += " sin elevacion st infarto";
            if (s.find("infarto") != std::string::npos) s += " iam";
            if (s.find("acv") != std::string::npos) s += " stroke evento vascular cerebral isquemico";
            if (s.find("stroke") != std::string::npos) s += " acv evento vascular cerebral";
            if (s.find("evento vascular") != std::string::npos) s += " acv stroke";
            if (s.find("pielonefritis") != std::string::npos) s += " itu infeccion urinaria";
            if (s.find("sepsis") != std::string::npos) s += " septico choque";
            if (s.find("septico") != std::string::npos) s += " sepsis";
            if (s.find("neumonia") != std::string::npos) s += " pna";
            return s;
        };
        dxLower  = expandSynonyms(dxLower);
        expLower = expandSynonyms(expLower);

        // Set de palabras funcionales a ignorar (mas completo)
        auto isStopWord = [](const std::string& w) {
            static const std::vector<std::string> stops = {
                "de", "la", "el", "en", "con", "por", "para", "y", "a",
                "los", "las", "del", "al", "un", "una", "sobre", "que",
                "se", "su", "sus", "es", "sin", "muy", "mas", "menos",
                "aguda", "agudo", "cronico", "cronica", "severo", "severa",
                "moderado", "moderada", "leve"
            };
            for (const auto& s : stops) if (w == s) return true;
            return w.length() < 3;
        };

        // Extraer palabras clave del esperado
        std::vector<std::string> expectedKeywords;
        std::istringstream iss(expLower);
        std::string word;
        while (iss >> word) {
            // Limpiar puntuacion
            std::string clean;
            for (char c : word) {
                if (std::isalnum((unsigned char)c) || c == '-') clean += c;
            }
            if (!clean.empty() && !isStopWord(clean)) {
                expectedKeywords.push_back(clean);
            }
        }

        if (expectedKeywords.empty()) return false;

        // Contar matches
        int matches = 0;
        for (const auto& kw : expectedKeywords) {
            if (dxLower.find(kw) != std::string::npos) ++matches;
        }

        // Umbral: 25% de palabras clave (mas permisivo)
        double ratio = (double)matches / expectedKeywords.size();
        return ratio >= 0.25;
    }

    // Sugerencias basadas en el caso
    void PrintCaseInfo() {
        std::cout << YELLOW << "\n[INFORMACION DEL CASO ACTUAL]\n" << RESET;

        if (m_case.expectedStudies.is_array()) {
            std::cout << GRAY << "  " << m_case.expectedStudies.size()
                      << " estudios esperados disponibles\n" << RESET;
        }
        if (m_case.expectedTreatments.is_array()) {
            std::cout << GRAY << "  " << m_case.expectedTreatments.size()
                      << " intervenciones esperadas\n" << RESET;
        }
        if (m_case.expectedDiagnosis.is_object() &&
            m_case.expectedDiagnosis.contains("primary")) {
            std::cout << GRAY << "  Setting: " << m_case.setting
                      << " | Especialidad: " << m_case.specialty << "\n" << RESET;
        }
        std::cout << "\n";
    }

private:
    const Data::ClinicalCaseRecord& m_case;
    std::unordered_map<std::string, std::string> m_studyResultsCatalog;
    std::unordered_map<std::string, std::vector<std::string>> m_treatmentAliases;

    // Construir catalogo de resultados especificos para estudios reconocidos
    void BuildStudyResultsCatalog() {
        // â”€â”€â”€ Cardiovascular â”€â”€â”€
        m_studyResultsCatalog["ecg"] =
            "ðŸ“Š ECG 12 DERIVACIONES:\n"
            "   Ritmo: Sinusal a 108 lpm\n"
            "   Elevacion ST 3mm en V1-V6 (cara anterior extensa)\n"
            "   Ondas Q incipientes en V1-V3\n"
            "   Imagen especular en DII, DIII, aVF\n"
            "   -> INTERPRETACION: STEMI ANTERIOR EXTENSO";

        m_studyResultsCatalog["troponina"] =
            "ðŸ“Š TROPONINA I ULTRASENSIBLE:\n"
            "   Resultado: 12.5 ng/mL (VN <0.04)\n"
            "   -> MUY ELEVADA - Necrosis miocardica confirmada";

        // â”€â”€â”€ Radiologia â”€â”€â”€
        m_studyResultsCatalog["rx torax"] =
            "ðŸ“Š RX TORAX PORTATIL:\n"
            "   Silueta cardiaca normal\n"
            "   Datos de congestion pulmonar leve bilateral\n"
            "   Sin neumotorax, sin derrame pleural";

        m_studyResultsCatalog["ecocardiograma"] =
            "ðŸ“Š ECOCARDIOGRAMA TRANSTORACICO:\n"
            "   FEVI: 35% (disminuida)\n"
            "   Hipoquinesia severa de pared anterior y septum\n"
            "   Sin derrame pericardico";

        // â”€â”€â”€ Laboratorios generales â”€â”€â”€
        m_studyResultsCatalog["biometria"] =
            "ðŸ“Š BIOMETRIA HEMATICA:\n"
            "   Hb: 12.8 g/dL | Hto: 39% | Leucos: 18,500 (neutrofilia 85%)\n"
            "   Plaquetas: 165,000 (limitrofe bajo)\n"
            "   -> Leucocitosis con desviacion izquierda";

        m_studyResultsCatalog["quimica"] =
            "ðŸ“Š QUIMICA SANGUINEA:\n"
            "   Glucosa: 342 mg/dL | Urea: 89 | Creatinina: 2.4 (elevada)\n"
            "   Na: 132 | K: 5.1 | Cl: 98\n"
            "   -> Lesion renal aguda + hiperglucemia severa";

        m_studyResultsCatalog["gasometria"] =
            "ðŸ“Š GASOMETRIA ARTERIAL:\n"
            "   pH: 7.28 | PaCO2: 30 | PaO2: 62 | HCO3: 14 | EB: -12\n"
            "   Lactato: 4.5 mmol/L (ELEVADO)\n"
            "   -> Acidosis metabolica con anion gap elevado, hipoxemia";

        m_studyResultsCatalog["hba1c"] =
            "ðŸ“Š HbA1c: 9.4% (control muy deficiente)";

        m_studyResultsCatalog["coagulacion"] =
            "ðŸ“Š TIEMPOS DE COAGULACION:\n"
            "   TP: 14.2 seg (INR 1.2) | TTPa: 32 seg\n"
            "   Fibrinogeno: 285 | Dimero D: 2.8 (elevado)\n"
            "   -> Datos limitrofes, vigilar CID";

        // â”€â”€â”€ Especificos sepsis â”€â”€â”€
        m_studyResultsCatalog["lactato"] =
            "ðŸ“Š LACTATO ARTERIAL:\n"
            "   Resultado: 4.5 mmol/L (VN <2)\n"
            "   -> ELEVADO - hipoperfusion tisular / shock";

        m_studyResultsCatalog["hemocultivo"] =
            "ðŸ“Š HEMOCULTIVOS x2 (tomados antes de antibiotico):\n"
            "   Aerobio 1: pendiente 48-72h\n"
            "   Aerobio 2: pendiente 48-72h\n"
            "   Anaerobio: pendiente\n"
            "   -> Gram inicial: bacilos gram negativos (sugerente E. coli)";

        m_studyResultsCatalog["urocultivo"] =
            "ðŸ“Š UROCULTIVO + EGO:\n"
            "   EGO: leucos incontables, nitritos +, bacterias +++\n"
            "        Cristales, cilindros granulosos\n"
            "   Cultivo: pendiente 48h\n"
            "   -> Sugerente de ITU complicada / pielonefritis";

        m_studyResultsCatalog["procalcitonina"] =
            "ðŸ“Š PROCALCITONINA:\n"
            "   Resultado: 8.4 ng/mL (VN <0.5)\n"
            "   -> MUY ELEVADA - fuerte sugerencia de sepsis bacteriana";

        m_studyResultsCatalog["pcr"] =
            "ðŸ“Š PROTEINA C REACTIVA (PCR):\n"
            "   Resultado: 285 mg/L (VN <10)\n"
            "   -> Muy elevada - respuesta inflamatoria intensa";

        m_studyResultsCatalog["usg renal"] =
            "ðŸ“Š USG RENAL URGENTE:\n"
            "   Rinones de tamano conservado, cortica adelgazada\n"
            "   NO hidronefrosis, NO litos obstructivos\n"
            "   Sin coleccciones ni abscesos perirrenales\n"
            "   -> Sin datos de obstruccion. Compatible con pielonefritis aguda";

        m_studyResultsCatalog["cetonas"] =
            "ðŸ“Š CETONAS SERICAS + EGO (cetonuria):\n"
            "   Beta-hidroxibutirato: 3.8 mmol/L (elevado)\n"
            "   Cetonuria: +++\n"
            "   -> CETOACIDOSIS DIABETICA asociada al shock";

        m_studyResultsCatalog["tc"] =
            "ðŸ“Š TC ABDOMEN CONTRASTADA:\n"
            "   Rinones aumentados de volumen, edema perinefritico\n"
            "   Sin abscesos, sin datos de pielonefritis enfisematosa\n"
            "   -> Confirma pielonefritis aguda complicada";

        // â”€â”€â”€ Neurologicos â”€â”€â”€
        m_studyResultsCatalog["tc craneo"] =
            "ðŸ“Š TC CRANEO SIMPLE:\n"
            "   Sin evidencia de hemorragia intraparenquimatosa\n"
            "   Signos precoces isquemia en teritorio ACM izquierdo\n"
            "   Score ASPECTS: 8/10";

        m_studyResultsCatalog["lipidos"] =
            "ðŸ“Š PERFIL LIPIDICO:\n"
            "   Colesterol total: 245 | LDL: 168 | HDL: 32 | TG: 220";
    }

    void BuildTreatmentAliases() {
        m_treatmentAliases["aspirina"]      = {"aas", "asa", "acido acetilsalicilico"};
        m_treatmentAliases["ticagrelor"]    = {"ticagrelor", "brilinta"};
        m_treatmentAliases["clopidogrel"]   = {"clopidogrel", "plavix"};
        m_treatmentAliases["norepinefrina"] = {"noradrenalina", "levophed", "na"};
        m_treatmentAliases["meropenem"]     = {"meronem"};
        m_treatmentAliases["insulina"]      = {"insulin", "insulina iv"};
    }

    std::string LookupStudyResult(const std::string& expName) {
        std::string key = ToLower(expName);
        for (const auto& [term, result] : m_studyResultsCatalog) {
            if (key.find(term) != std::string::npos) {
                return result;
            }
        }
        return "";
    }

    std::string GenerateGenericResult(const std::string& studyName, const json& expected) {
        std::ostringstream oss;
        oss << "ðŸ“Š " << studyName << ":\n";
        oss << "   Resultado disponible (simulado)\n";
        if (expected.contains("expected_finding")) {
            oss << "   Hallazgo esperado: " << expected["expected_finding"].get<std::string>() << "\n";
        }
        if (expected.contains("justification")) {
            oss << "   Justificacion: " << expected["justification"].get<std::string>() << "\n";
        }
        return oss.str();
    }

    // â”€â”€â”€ Examenes fisicos genericos â”€â”€â”€
    std::string GetGeneralExam() {
        // Usar signos vitales del caso para personalizar
        auto& v = m_case.patient["vital_signs_admission"];
        std::ostringstream oss;
        oss << "ðŸ©º ASPECTO GENERAL:\n";
        if (v.value("glasgow", 15) < 15) {
            oss << "   Paciente OBNUBILADO/DESORIENTADO (Glasgow "
                << v.value("glasgow", 15) << ")\n";
        } else {
            oss << "   Consciente, orientado\n";
        }
        if (v.value("pain_scale", 0) >= 7) {
            oss << "   Facies algica intensa\n";
        }
        if (v.value("temperature_c", 37.0) >= 38.5) {
            oss << "   Aspecto febril, diaforesis\n";
        }
        oss << "   Palidez leve, hidratacion mucosa reducida\n";
        return oss.str();
    }

    std::string GetCardioExam() {
        auto& v = m_case.patient["vital_signs_admission"];
        std::ostringstream oss;
        oss << "ðŸ©º AUSCULTACION CARDIACA:\n";
        oss << "   Ruidos ritmicos, FC " << v.value("heart_rate", 0) << " lpm";
        if (v.value("heart_rate", 0) > 100) oss << " (TAQUICARDIA)";
        oss << "\n";
        if (v.contains("killip_class")) {
            std::string killip = v.value("killip_class", "?");
            if (killip == "II" || killip == "III" || killip == "IV") {
                oss << "   Presencia de 3er ruido (S3 GALOPE)\n";
            }
        }
        if (v.value("systolic_bp", 120) < 90) {
            oss << "   Pulsos perifericos FILIFORMES, llenado capilar >3 seg\n";
            oss << "   Extremidades frias, moteado cutaneo (hipoperfusion)\n";
        } else {
            oss << "   Pulsos perifericos presentes y simetricos\n";
        }
        return oss.str();
    }

    std::string GetRespiratoryExam() {
        auto& v = m_case.patient["vital_signs_admission"];
        std::ostringstream oss;
        oss << "ðŸ©º EXAMEN RESPIRATORIO:\n";
        oss << "   FR: " << v.value("resp_rate", 0) << " rpm";
        if (v.value("resp_rate", 0) > 22) oss << " (TAQUIPNEA)";
        oss << "\n";
        if (v.value("spo2", 100) < 92) {
            oss << "   SpO2 " << v.value("spo2", 0) << "% (HIPOXEMIA)\n";
        }
        if (v.contains("killip_class")) {
            std::string killip = v.value("killip_class", "?");
            if (killip == "II") oss << "   Estertores crepitantes basales bilaterales (Killip II)\n";
            else if (killip == "III") oss << "   Edema pulmonar franco (Killip III)\n";
        } else {
            oss << "   Murmullo vesicular disminuido en bases\n";
        }
        oss << "   No sibilancias\n";
        return oss.str();
    }

    std::string GetAbdomenExam() {
        std::ostringstream oss;
        oss << "ðŸ©º ABDOMEN:\n";
        // Personalizar segun especialidad
        if (m_case.specialty == "Infectology" || m_case.specialty == "Nephrology") {
            oss << "   Blando, doloroso a palpacion profunda en fosas lumbares\n";
            oss << "   PUNO-PERCUSION RENAL POSITIVA BILATERAL (mas derecha)\n";
            oss << "   Signo de Giordano (+)\n";
            oss << "   Peristalsis disminuida\n";
        } else {
            oss << "   Blando, depresible, no doloroso\n";
            oss << "   Sin visceromegalias, peristalsis presente\n";
        }
        return oss.str();
    }

    std::string GetNeuroExam() {
        auto& v = m_case.patient["vital_signs_admission"];
        std::ostringstream oss;
        oss << "ðŸ©º NEUROLOGICO:\n";
        oss << "   Glasgow " << v.value("glasgow", 15);
        int gcs = v.value("glasgow", 15);
        if (gcs == 15) oss << " (normal)";
        else if (gcs >= 13) oss << " (leve alteracion - O3-V4-M6)";
        else if (gcs >= 9) oss << " (moderada alteracion)";
        else oss << " (COMA)";
        oss << "\n";
        oss << "   Pupilas normorreactivas, sin deficit motor focal\n";
        oss << "   Meningeos negativos\n";
        return oss.str();
    }

    std::string GetExtremitiesExam() {
        auto& v = m_case.patient["vital_signs_admission"];
        std::ostringstream oss;
        oss << "ðŸ©º EXTREMIDADES:\n";
        if (v.value("systolic_bp", 120) < 90) {
            oss << "   FRIAS, con MOTEADO CUTANEO (livedo reticularis)\n";
            oss << "   Llenado capilar 4-5 segundos (retraso severo)\n";
            oss << "   Pulsos perifericos filiformes\n";
        } else {
            oss << "   Sin edema, pulsos presentes\n";
            oss << "   Temperatura conservada\n";
        }
        return oss.str();
    }

    std::string GetNeckExam() {
        std::ostringstream oss;
        oss << "ðŸ©º CUELLO:\n";
        oss << "   Sin ingurgitacion yugular a 45 grados\n";
        oss << "   Tiroides no palpable, sin adenopatias\n";
        return oss.str();
    }

    std::string GetRenalExam() {
        std::ostringstream oss;
        oss << "ðŸ©º EXAMEN RENAL / LUMBAR:\n";
        oss << "   Puno-percusion renal POSITIVA BILATERAL\n";
        oss << "   Mas intensa en fosa renal derecha\n";
        oss << "   Signo de Giordano positivo\n";
        oss << "   -> Sugestivo de pielonefritis / patologia renal\n";
        return oss.str();
    }
};

// â”€â”€â”€ Header y ayuda â”€â”€â”€
static void PrintHeader(const Data::ClinicalCaseRecord& c) {
    std::cout << "\n";
    std::cout << CYAN << "===============================================================\n";
    std::cout << "   ENARM Simulator - Modo Interactivo\n";
    std::cout << "===============================================================\n" << RESET;
    std::cout << "   Caso:      " << c.slug << "\n";
    std::cout << "   Titulo:    " << c.title << "\n";
    std::cout << "   Paciente:  " << c.patient["full_name"].get<std::string>()
              << " (" << c.patient["age"].get<int>() << " anos)\n";
    std::cout << "   Setting:   " << c.setting << " | Especialidad: " << c.specialty << "\n";
    std::cout << CYAN << "===============================================================\n" << RESET;

    auto& v = c.patient["vital_signs_admission"];
    std::cout << YELLOW << "[SIGNOS VITALES]\n" << RESET;
    std::cout << "   PA: " << v.value("systolic_bp", 0) << "/" << v.value("diastolic_bp", 0) << " mmHg | ";
    std::cout << "FC: " << v.value("heart_rate", 0) << " | ";
    std::cout << "FR: " << v.value("resp_rate", 0) << " | ";
    std::cout << "SpO2: " << v.value("spo2", 0) << "% | T: "
              << v.value("temperature_c", 0.0) << "C\n";
    std::cout << "   Glucosa: " << v.value("glucose_mgdl", 0) << " | ";
    std::cout << "EVA: " << v.value("pain_scale", 0) << "/10";
    if (v.contains("glasgow")) std::cout << " | Glasgow: " << v.value("glasgow", 15);
    if (v.contains("killip_class")) std::cout << " | Killip: " << v.value("killip_class", "?");
    if (v.contains("qsofa_score")) std::cout << " | qSOFA: " << v.value("qsofa_score", 0) << "/3";
    std::cout << "\n";
    std::cout << CYAN << "===============================================================\n" << RESET;
}

static void PrintHelp() {
    std::cout << YELLOW << "\nCOMANDOS DISPONIBLES:\n" << RESET;
    std::cout << "  " << GREEN << "<pregunta libre>" << RESET << "     - Preguntale al paciente\n";
    std::cout << "  " << GREEN << "/help" << RESET << "                 - Esta ayuda\n";
    std::cout << "  " << GREEN << "/info" << RESET << "                 - Info del caso (estudios/tx disponibles)\n";
    std::cout << "  " << GREEN << "/vitals" << RESET << "               - Ver signos vitales\n";
    std::cout << "  " << GREEN << "/examinar <region>" << RESET << "    - Examen fisico (general/cardio/respira/abdomen/neuro/extrem/cuello/renal)\n";
    std::cout << "  " << GREEN << "/solicitar <estudio>" << RESET << "  - Solicitar estudio\n";
    std::cout << "  " << GREEN << "/diagnostico <dx>" << RESET << "     - Registrar diagnostico\n";
    std::cout << "  " << GREEN << "/tratar <tx>" << RESET << "          - Aplicar tratamiento\n";
    std::cout << "  " << GREEN << "/historial" << RESET << "            - Ver conversacion\n";
    std::cout << "  " << GREEN << "/score" << RESET << "                - Ver puntuacion parcial\n";
    std::cout << "  " << GREEN << "/salir" << RESET << "                - Terminar sesion\n\n";
    std::cout << YELLOW << "TIP: Comandos empiezan con '/'. Todo lo demas es pregunta al paciente.\n\n" << RESET;
}

static void PrintScore(const SessionScore& s) {
    std::cout << "\n" << CYAN << "===============================================================\n";
    std::cout << "   PUNTUACION PARCIAL\n";
    std::cout << "===============================================================\n" << RESET;

    auto bar = [](int val, int max) {
        int pct = max > 0 ? (12 * val / max) : 0;
        std::string b;
        for (int i = 0; i < 12; ++i) b += (i < pct ? "#" : ".");
        return b;
    };

    std::cout << "   Interrogatorio:   " << std::setw(3) << s.interview << "/" << s.interviewMax
              << "   [" << bar(s.interview, s.interviewMax) << "]\n";
    std::cout << "   Examen fisico:    " << std::setw(3) << s.physicalExam << "/" << s.physicalExamMax
              << "   [" << bar(s.physicalExam, s.physicalExamMax) << "]\n";
    std::cout << "   Estudios:         " << std::setw(3) << s.studies << "/" << s.studiesMax
              << "   [" << bar(s.studies, s.studiesMax) << "]\n";
    std::cout << "   Diagnostico:      " << std::setw(3) << s.diagnosis << "/" << s.diagnosisMax
              << "   [" << bar(s.diagnosis, s.diagnosisMax) << "]\n";
    std::cout << "   Tratamiento:      " << std::setw(3) << s.treatment << "/" << s.treatmentMax
              << "   [" << bar(s.treatment, s.treatmentMax) << "]\n";
    std::cout << "   Seguimiento:      " << std::setw(3) << s.followUp << "/" << s.followUpMax
              << "   [" << bar(s.followUp, s.followUpMax) << "]\n";
    std::cout << CYAN << "   -------------------------------------------------------------\n" << RESET;
    std::cout << "   TOTAL:            " << BOLD << std::setw(3) << s.Total() << "/" << s.TotalMax()
              << "   (" << std::fixed << std::setprecision(0) << s.Percentage() << "%)"
              << "  Nivel: " << s.Level() << RESET << "\n";
    std::cout << CYAN << "===============================================================\n\n" << RESET;
}

// â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•
//  MAIN
// â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•
int main(int argc, char* argv[]) {
#ifdef _WIN32
    system("chcp 65001 > nul");
#endif

    std::cout << "\n" << BOLD << CYAN
              << "==============================================================\n"
              << "   " << Common::kProjectName << " CLI v" << Common::kVersion << "\n"
              << "==============================================================\n" << RESET;

    Common::LoggerConfig logCfg;
    logCfg.enableFile = false;
    logCfg.asyncMode = false;
    Common::Logger::Initialize(logCfg);

    std::string caseSlug = "iam-stemi-anterior-varon-58";
    if (argc > 1) caseSlug = argv[1];

    std::cout << "Iniciando caso: " << caseSlug << "\n\n";

    // â”€â”€â”€ PostgreSQL â”€â”€â”€
    std::cout << "[Conectando a PostgreSQL...]\n";
    Data::ConnectionConfig dbCfg;
    dbCfg.database = "enarm_db";
    dbCfg.user = "postgres";
    dbCfg.password = "manager";

    auto dbRes = Data::DatabaseManager::Create(dbCfg, 2);
    if (dbRes.IsErr()) {
        std::cerr << RED << "ERROR: " << dbRes.Error().ToString() << RESET << "\n";
        return 1;
    }
    auto db = dbRes.Value();
    std::cout << GREEN << "  OK\n" << RESET;

    // â”€â”€â”€ Cargar caso â”€â”€â”€
    Data::CaseRepository caseRepo(db);
    auto caseOpt = caseRepo.FindBySlug(caseSlug);
    if (!caseOpt) {
        std::cerr << RED << "ERROR: caso '" << caseSlug << "' no encontrado\n" << RESET;
        return 1;
    }
    auto& c = *caseOpt;

    // â”€â”€â”€ Ollama â”€â”€â”€
    std::cout << "[Conectando a Ollama...]\n";
    AI::OllamaConfig ollamaCfg;
    ollamaCfg.endpoint = "http://localhost:11434";
    ollamaCfg.defaultTimeoutSeconds = 180;

    auto ollama = std::make_shared<AI::OllamaClient>(ollamaCfg);
    if (!ollama->IsAvailable()) {
        std::cerr << RED << "ERROR: Ollama no disponible\n" << RESET;
        return 1;
    }
    std::cout << GREEN << "  OK\n" << RESET;

    auto router = std::make_shared<AI::LLMRouter>(ollama);
    std::cout << "  Modelo paciente: " << router->GetModelForTask(AI::TaskType::PatientResponse) << "\n";

    // â”€â”€â”€ Persona â”€â”€â”€
    AI::PromptTemplates::PatientPersonaData persona;
    persona.fullName              = c.patient["full_name"].get<std::string>();
    persona.age                   = c.patient["age"].get<int>();
    persona.sex                   = c.patient["sex"].get<std::string>();
    persona.chiefComplaint        = c.chiefComplaint;
    persona.historyPresentIllness = c.historyPresentIllness;
    persona.symptoms              = c.symptoms;
    persona.medicalHistory        = c.patient.value("medical_history", json::array());
    persona.vitalSigns            = c.patient.value("vital_signs_admission", json::object());
    persona.familyHistory         = c.patient.value("family_history", json::array());
    persona.lifestyle             = c.patient.value("lifestyle", json::object());
    persona.educationLevel        = c.patient.value("education", "licenciatura");
    persona.emotionalState        = "muy asustado, con dolor, buscando ayuda urgente";

    AI::PatientAIController patient(router, persona);
    CaseSimulator simulator(c);

    PrintHeader(c);
    PrintHelp();
    simulator.PrintCaseInfo();

    // â”€â”€â”€ LOOP â”€â”€â”€
    SessionState state;
    std::string rawInput;
    bool running = true;
    auto startTime = std::chrono::system_clock::now();

    while (running) {
        std::cout << BOLD << MAGENTA << "Doctor > " << RESET;

        if (!std::getline(std::cin, rawInput)) break;

        std::string input = Trim(rawInput);
        while (!input.empty() && input[0] == '>') input = Trim(input.substr(1));
        if (input.empty()) continue;

        std::string lowerInput = ToLower(input);

        if (input[0] == '/') {
            if (lowerInput == "/salir" || lowerInput == "/exit" || lowerInput == "/quit") {
                running = false;
            }
            else if (lowerInput == "/help" || lowerInput == "/?" || lowerInput == "/ayuda") {
                PrintHelp();
            }
            else if (lowerInput == "/info") {
                simulator.PrintCaseInfo();
            }
            else if (lowerInput == "/score" || lowerInput == "/puntuacion") {
                PrintScore(state.score);
            }
            else if (lowerInput == "/historial") {
                std::cout << "\n" << CYAN << "=== HISTORIAL ===\n" << RESET;
                std::cout << patient.GetMemory().ToDisplayString(false);
            }
            else if (lowerInput == "/vitals" || lowerInput == "/signos") {
                auto& v = c.patient["vital_signs_admission"];
                std::cout << "\n" << YELLOW << "SIGNOS VITALES:\n" << RESET;
                std::cout << "  PA: " << v.value("systolic_bp", 0) << "/" << v.value("diastolic_bp", 0) << " mmHg\n";
                std::cout << "  FC: " << v.value("heart_rate", 0) << " lpm\n";
                std::cout << "  FR: " << v.value("resp_rate", 0) << " rpm\n";
                std::cout << "  SpO2: " << v.value("spo2", 0) << "%\n";
                std::cout << "  T: " << v.value("temperature_c", 0.0) << " C\n";
                std::cout << "  Glucosa: " << v.value("glucose_mgdl", 0) << " mg/dL\n";
                std::cout << "  EVA: " << v.value("pain_scale", 0) << "/10\n";
                if (v.contains("glasgow")) std::cout << "  Glasgow: " << v.value("glasgow", 15) << "\n";
                if (v.contains("killip_class")) std::cout << "  Killip: " << v.value("killip_class", "?") << "\n";
                if (v.contains("qsofa_score")) std::cout << "  qSOFA: " << v.value("qsofa_score", 0) << "/3\n";
                std::cout << "\n";
            }
            else if (lowerInput.rfind("/solicitar", 0) == 0 || lowerInput.rfind("/estudio", 0) == 0) {
                std::string study = ExtractAfterFirstSpace(input);
                if (study.empty()) {
                    std::cout << RED << "Uso: /solicitar <estudio>\n\n" << RESET;
                    continue;
                }
                state.studiesRequested.push_back(study);
                std::cout << GRAY << "[Solicitando " << study << "...]\n" << RESET;
                int pts = 0;
                std::cout << simulator.SimulateStudyResult(study, pts) << "\n";
                if (pts > 0) {
                    state.score.studies += pts;
                    if (state.score.studies > state.score.studiesMax)
                        state.score.studies = state.score.studiesMax;
                    std::cout << GREEN << "   +" << pts << " pts\n" << RESET;
                }
                std::cout << "\n";
            }
            else if (lowerInput.rfind("/examinar", 0) == 0 || lowerInput.rfind("/examen", 0) == 0 ||
                     lowerInput.rfind("/ef", 0) == 0) {
                std::string region = ExtractAfterFirstSpace(input);
                if (region.empty()) {
                    std::cout << RED << "Uso: /examinar <region>\n\n" << RESET;
                    continue;
                }
                state.examinations.push_back(region);
                std::cout << GRAY << "[Examinando " << region << "...]\n" << RESET;
                int pts = 0;
                std::cout << simulator.SimulateExamination(region, pts) << "\n";
                if (pts > 0) {
                    state.score.physicalExam += pts;
                    if (state.score.physicalExam > state.score.physicalExamMax)
                        state.score.physicalExam = state.score.physicalExamMax;
                    std::cout << GREEN << "   +" << pts << " pts\n" << RESET;
                }
                std::cout << "\n";
            }
            else if (lowerInput.rfind("/diagnostico", 0) == 0 || lowerInput.rfind("/dx", 0) == 0) {
                std::string dx = ExtractAfterFirstSpace(input);
                if (dx.empty()) {
                    std::cout << RED << "Uso: /diagnostico <dx>\n\n" << RESET;
                    continue;
                }
                state.diagnosisRegistered = dx;
                state.diagnosisCorrect = simulator.CheckDiagnosis(dx);
                if (state.diagnosisCorrect) {
                    state.score.diagnosis = state.score.diagnosisMax;
                    std::cout << GREEN << "\nâœ… DIAGNOSTICO CORRECTO: " << dx << "\n";
                    std::cout << "   +" << state.score.diagnosisMax << " pts\n\n" << RESET;
                } else {
                    state.score.diagnosis = 5;
                    std::cout << RED << "\nâš ï¸ Diagnostico no coincide con el esperado\n";
                    if (c.expectedDiagnosis.contains("primary")) {
                        std::cout << "   Esperado: "
                                  << c.expectedDiagnosis["primary"].get<std::string>() << "\n";
                    }
                    std::cout << "   Registrado: " << dx << "\n";
                    std::cout << "   +5 pts (parcial)\n\n" << RESET;
                }
            }
            else if (lowerInput.rfind("/tratar", 0) == 0 || lowerInput.rfind("/tx", 0) == 0 ||
                     lowerInput.rfind("/tratamiento", 0) == 0) {
                std::string tx = ExtractAfterFirstSpace(input);
                if (tx.empty()) {
                    std::cout << RED << "Uso: /tratar <tx>\n\n" << RESET;
                    continue;
                }
                state.treatmentsApplied.push_back(tx);
                int pts = simulator.ScoreForTreatment(tx);
                std::cout << GRAY << "[Aplicando: " << tx << "]\n" << RESET;
                if (pts > 0) {
                    std::cout << GREEN << "   +" << pts << " pts\n" << RESET;
                } else {
                    std::cout << YELLOW << "   (0 pts - no coincide con intervenciones esperadas)\n" << RESET;
                }
                state.score.treatment += pts;
                if (state.score.treatment > state.score.treatmentMax)
                    state.score.treatment = state.score.treatmentMax;
                std::cout << "\n";
            }
            else {
                std::cout << RED << "Comando desconocido. Usa /help\n\n" << RESET;
            }
        }
        else {
            std::cout << GRAY << "[qwen2.5:7b pensando...]\n" << RESET;
            auto response = patient.Ask(input);
            if (response.IsOk()) {
                std::cout << BOLD << GREEN << "PACIENTE: " << RESET << response.Value() << "\n";
                if (!patient.GetMemory().GetHistory().empty()) {
                    auto& last = patient.GetMemory().GetHistory().back();
                    std::cout << GRAY << "   [" << last.latency.count() << "ms]\n\n" << RESET;
                }
                if (state.score.interview < state.score.interviewMax) {
                    state.score.interview += 2;
                    if (state.score.interview > state.score.interviewMax)
                        state.score.interview = state.score.interviewMax;
                }
            } else {
                std::cout << RED << "ERROR: " << response.Error().ToString() << "\n\n" << RESET;
            }
        }
    }

    // â”€â”€â”€ RESUMEN Y GUARDADO â”€â”€â”€
    auto endTime = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(endTime - startTime);

    std::cout << "\n" << CYAN << "===============================================================\n";
    std::cout << "   RESUMEN DE SESION\n";
    std::cout << "===============================================================\n" << RESET;
    std::cout << "   Duracion:       " << duration.count() / 60 << " min " << duration.count() % 60 << " seg\n";
    std::cout << "   Preguntas:      " << patient.GetMemory().GetUserTurnCount() << "\n";
    std::cout << "   Ex. fisicos:    " << state.examinations.size() << "\n";
    std::cout << "   Estudios:       " << state.studiesRequested.size() << "\n";
    std::cout << "   Tratamientos:   " << state.treatmentsApplied.size() << "\n";
    if (!state.diagnosisRegistered.empty()) {
        std::cout << "   Diagnostico:    " << state.diagnosisRegistered
                  << (state.diagnosisCorrect ? " âœ…" : " âš ï¸") << "\n";
    }
    PrintScore(state.score);

    std::cout << GRAY << "Guardando sesion en PostgreSQL...\n" << RESET;
    try {
        auto conn = db->Acquire();
        pqxx::read_transaction rtx(conn.Get());
        auto res = rtx.exec("SELECT id FROM students LIMIT 1");
        if (!res.empty()) {
            std::string studentId = res[0]["id"].as<std::string>();
            rtx.commit();

            Data::SessionRepository sesRepo(db);
            auto sesRes = sesRepo.Start(studentId, "case", c.id);
            if (sesRes.IsOk()) {
                std::string sessionId = sesRes.Value();

                double simTime = 0.0;
                for (const auto& study : state.studiesRequested) {
                    simTime += 30.0;
                    auto r = sesRepo.LogEvent(sessionId, simTime, "study", "diagnostic",
                        "Solicito: " + study, {{"study", study}}, 1);
                    (void)r;
                }
                for (const auto& tx : state.treatmentsApplied) {
                    simTime += 60.0;
                    auto r = sesRepo.LogEvent(sessionId, simTime, "treatment", "treatment",
                        "Aplico: " + tx, {{"treatment", tx}}, 2);
                    (void)r;
                }
                if (!state.diagnosisRegistered.empty()) {
                    auto r = sesRepo.LogEvent(sessionId, simTime, "diagnosis", "diagnostic",
                        "Diagnostico: " + state.diagnosisRegistered,
                        {{"diagnosis", state.diagnosisRegistered},
                         {"correct", state.diagnosisCorrect}}, 2);
                    (void)r;
                }

                nlohmann::json scoresJson = {
                    {"interview", state.score.interview},
                    {"physical_exam", state.score.physicalExam},
                    {"studies", state.score.studies},
                    {"diagnosis", state.score.diagnosis},
                    {"treatment", state.score.treatment},
                    {"followup", state.score.followUp}
                };
                nlohmann::json feedback = {
                    {"level", state.score.Level()},
                    {"percentage", state.score.Percentage()},
                    {"case_slug", c.slug}
                };

                auto completeRes = sesRepo.Complete(sessionId,
                    (double)state.score.Total(),
                    (double)state.score.TotalMax(),
                    scoresJson, feedback);
                (void)completeRes;

                std::cout << GREEN << "  Sesion guardada. ID: " << sessionId << "\n" << RESET;
            }
        }
    } catch (const std::exception& e) {
        std::cout << YELLOW << "  Advertencia: " << e.what() << "\n" << RESET;
    }

    std::cout << GREEN << "\nÂ¡Hasta pronto, Dr. Estudiante!\n\n" << RESET;
    Common::Logger::Shutdown();
    return 0;
}