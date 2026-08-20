// =====================================================================
//   ENARM.GUI - CaseFindingsProvider Studies (parte 2/2)
//   Resultados de estudios paraclinicos por caso (10 casos)
// =====================================================================
#include "ENARM/GUI/CaseFindingsProvider.h"
#include <algorithm>

namespace ENARM::GUI {

// Reimplementacion de NormalizeCaseId (mismo codigo que .cpp principal)
static std::string NormId(const std::string& raw) {
    std::string s = raw;
    std::transform(s.begin(), s.end(), s.begin(), ::tolower);

    if (s.find("bronquiolitis") != std::string::npos || s.find("vsr") != std::string::npos) return "bronquiolitis";
    if (s.find("cetoacidosis") != std::string::npos || s.find("cad") != std::string::npos || s.find("diabetica") != std::string::npos) return "cad";
    if (s.find("preeclampsia") != std::string::npos || s.find("eclampsia") != std::string::npos) return "preeclampsia";
    if (s.find("apendicitis") != std::string::npos || s.find("apendice") != std::string::npos || s.find("mcburney") != std::string::npos) return "apendicitis";
    if (s.find("neumonia") != std::string::npos || s.find("nac") != std::string::npos) return "neumonia";
    if (s.find("acv") != std::string::npos || s.find("evc") != std::string::npos || s.find("hemiparesia") != std::string::npos || s.find("isquemico") != std::string::npos) return "acv";
    if (s.find("tce") != std::string::npos || s.find("trauma") != std::string::npos || s.find("craneoencefalico") != std::string::npos) return "tce";
    if (s.find("sepsis") != std::string::npos || s.find("septico") != std::string::npos) return "sepsis";
    if (s.find("lra") != std::string::npos || s.find("lesion renal") != std::string::npos || s.find("aines") != std::string::npos) return "lra";
    if (s.find("iam") != std::string::npos || s.find("infarto") != std::string::npos || s.find("stemi") != std::string::npos) return "iam";

    if (s.find("010") != std::string::npos) return "bronquiolitis";
    if (s.find("009") != std::string::npos) return "lra";
    if (s.find("008") != std::string::npos) return "cad";
    if (s.find("007") != std::string::npos) return "tce";
    if (s.find("006") != std::string::npos) return "apendicitis";
    if (s.find("005") != std::string::npos) return "preeclampsia";
    if (s.find("004") != std::string::npos) return "neumonia";
    if (s.find("003") != std::string::npos) return "acv";
    if (s.find("002") != std::string::npos) return "sepsis";
    if (s.find("001") != std::string::npos) return "iam";

    return "default";
}

std::string CaseFindingsProvider::GetStudyResult(const std::string& caseId,
                                                    const std::string& study) {
    const std::string key = NormId(caseId);

    // ==================== IAM ====================
    if (key == "iam") {
        if (study == "ECG de 12 derivaciones") return "*** CRITICO *** RS 95 lpm. ELEVACION ST 3-4mm en V1-V4. IAMCEST ANTERIOR. Sin bloqueos.";
        if (study == "Troponina I/T") return "*** ELEVADA *** Troponina I 8.4 ng/mL (normal <0.04). IAM AGUDO confirmado.";
        if (study == "CPK-MB") return "CPK 480 U/L | CPK-MB 62 U/L (elevados). Compatible con IAM.";
        if (study == "BNP/NT-proBNP") return "BNP 180 pg/mL (leve elevacion). Disfuncion ventricular inicial.";
        if (study == "Biometria hematica") return "Hb 14.2 | Leu 12,400 (leve leucocitosis por stress) | Plaq 234K.";
        if (study == "Quimica sanguinea") return "Glucosa 118 | Urea 28 | Creatinina 0.9. Normal.";
        if (study == "Radiografia de torax") return "Silueta cardiaca normal. Campos limpios. Sin edema pulmonar.";
    }

    // ==================== SEPSIS ====================
    if (key == "sepsis") {
        if (study == "Biometria hematica") return "*** ANORMAL *** Leu 22,800 con bandemia 15%. Hb 10.8. Plaq 78,000 (trombocitopenia).";
        if (study == "Lactato serico") return "*** CRITICO *** Lactato 4.8 mmol/L (normal <2). HIPOPERFUSION severa.";
        if (study == "Procalcitonina") return "*** ELEVADA *** PCT 12.4 ng/mL. Sepsis bacteriana confirmada.";
        if (study == "Hemocultivos") return "Muestras x2 tomadas antes de ATB. Resultado preliminar 24h: BGN. Cultivo definitivo 48-72h.";
        if (study == "Urocultivo") return "Piuria masiva. Cultivo pendiente 48h. Sospecha E. coli.";
        if (study == "Examen general de orina") return "*** ANORMAL *** Leucocitos incontables, nitritos +, esterasa leucocitaria +++.";
        if (study == "Gasometria arterial") return "*** ACIDOSIS *** pH 7.28 | HCO3 15 | Lactato 4.8. Acidosis metabolica con brecha aumentada.";
        if (study == "Quimica sanguinea") return "Creatinina 2.1 (LRA), urea 68, glucosa 168.";
        if (study == "ECG de 12 derivaciones") return "Taquicardia sinusal 128 lpm. Sin cambios isquemicos.";
    }

    // ==================== ACV ====================
    if (key == "acv") {
        if (study == "Tomografia de craneo") return "*** SIN HEMORRAGIA *** TAC sin contraste: NO hemorragia intracraneal. Signos precoces de isquemia territorio ACM izquierda (borramiento surcos, perdida diferenciacion sustancia).";
        if (study == "Glucosa capilar") return "Glucosa 142 mg/dL. Normal (descarta hipoglucemia como diferencial).";
        if (study == "ECG de 12 derivaciones") return "*** FIBRILACION AURICULAR *** con respuesta ventricular media 96 lpm. Sin isquemia.";
        if (study == "Biometria hematica") return "Hb 13.8 | Leu 8,200 | Plaq 245K. Normal.";
        if (study == "Tiempos de coagulacion") return "TP 12.8s | INR 1.05 | TTPa 30s. Normal (elegible para trombolisis).";
        if (study == "Quimica sanguinea") return "Glucosa 142 | Urea 32 | Creatinina 1.0. Normal.";
    }

    // ==================== NEUMONIA ====================
    if (key == "neumonia") {
        if (study == "Radiografia de torax") return "*** CONSOLIDACION *** Opacidad alveolar en lobulo inferior derecho con broncograma aereo. Compatible con neumonia bacteriana. Sin derrame.";
        if (study == "Biometria hematica") return "*** LEUCOCITOSIS *** Leu 18,400 con neutrofilia 82%. Hb 13.2. Plaq 245K.";
        if (study == "Procalcitonina") return "*** ELEVADA *** PCT 3.2 ng/mL. Sugiere infeccion bacteriana.";
        if (study == "Gasometria arterial") return "*** INSUFICIENCIA RESPIRATORIA *** pH 7.32 | PaO2 56 | PaCO2 52 | SatO2 88%. Hipoxemia con hipercapnia (EPOC descompensado).";
        if (study == "Cultivo de expectoracion") return "Expectoracion purulenta obtenida. Tincion Gram: cocos gram positivos en cadenas. Cultivo 48-72h.";
        if (study == "Hemocultivos") return "Muestras x2 tomadas. Resultado en 48h.";
        if (study == "Panel viral respiratorio") return "Negativo para influenza A/B, VSR, metapneumovirus, adenovirus.";
        if (study == "ECG de 12 derivaciones") return "Taquicardia sinusal 108 lpm. P pulmonar (crecimiento AD). Sin isquemia.";
        if (study == "Quimica sanguinea") return "Glucosa 132 | Urea 42 | Creatinina 1.1. Leve azoemia prerrenal.";
    }

    // ==================== PREECLAMPSIA ====================
    if (key == "preeclampsia") {
        if (study == "Proteinuria 24h") return "*** ANORMAL *** Proteinuria 4.2 g/24h (normal <0.3). Sindrome nefrotico incipiente.";
        if (study == "Biometria hematica") return "*** TROMBOCITOPENIA *** Hb 11.8 | Plaq 89,000 (baja). Sin anemia hemolitica evidente.";
        if (study == "Perfil hepatico") return "*** ELEVADAS *** AST 168 | ALT 142 | DHL 720. Sugiere HELLP incipiente.";
        if (study == "Quimica sanguinea") return "Creatinina 1.3 (elevada para gestante), urea 32, acido urico 7.8 (elevado).";
        if (study == "Tiempos de coagulacion") return "TP 13.5s | TTPa 33s | Fibrinogeno 320. Normal aun.";
        if (study == "Registro cardiotocografico") return "FCF basal 148 lpm, variabilidad conservada, sin desaceleraciones. Bienestar fetal.";
        if (study == "Ultrasonido abdominal") return "Feto unico vivo, PFE 1,850g (percentil 45). Placenta anterior grado II. ILA 12 (normal). Doppler umbilical normal.";
        if (study == "Examen general de orina") return "Proteinas +++ (tira). Sin leucocitos ni nitritos. No infeccion.";
    }

    // ==================== APENDICITIS ====================
    if (key == "apendicitis") {
        if (study == "Biometria hematica") return "*** LEUCOCITOSIS *** Leu 16,200 con neutrofilia 84%. Hb 13.4. Plaq 289K.";
        if (study == "Ultrasonido abdominal") return "*** POSITIVO *** Apendice cecal visualizado en FID, DIAMETRO 9mm (>6mm patologico), NO COMPRESIBLE, con liquido periapendicular. Compatible con APENDICITIS AGUDA.";
        if (study == "Examen general de orina") return "Normal. Sin leucocitos, sin nitritos. Descarta ITU.";
        if (study == "Quimica sanguinea") return "Glucosa 98 | Urea 22 | Creatinina 0.6. Normal para edad.";
        if (study == "Electrolitos sericos") return "Na 138 | K 4.2 | Cl 102. Normal.";
        if (study == "Procalcitonina") return "PCT 0.8 ng/mL (levemente elevada). Compatible con proceso inflamatorio localizado.";
    }

    // ==================== TCE ====================
    if (key == "tce") {
        if (study == "Tomografia de craneo") return "*** CRITICO *** HEMATOMA EPIDURAL frontotemporoparietal derecho de 45cc con efecto de masa. Desviacion linea media 8mm. Compresion ventriculo lateral. FRACTURA LINEAL de hueso temporal derecho. Signos de HTIC.";
        if (study == "Radiografia de torax") return "Sin lesiones toracicas evidentes. Silueta cardiaca normal.";
        if (study == "Biometria hematica") return "Hb 11.2 (leve anemia por sangrado). Leu 15,600. Plaq 234K.";
        if (study == "Tiempos de coagulacion") return "TP 13.2s | INR 1.08 | TTPa 32s. Normal.";
        if (study == "Gasometria arterial") return "pH 7.30 | PaCO2 48 (leve hipercapnia). PaO2 88. Requiere ventilacion.";
        if (study == "Glucosa capilar") return "Glucosa 168 mg/dL (hiperglucemia por stress). Descarta hipoglucemia.";
    }

    // ==================== CAD ====================
    if (key == "cad") {
        if (study == "Glucosa capilar") return "*** CRITICO *** Glucosa 542 mg/dL. Hiperglucemia severa.";
        if (study == "Gasometria arterial") return "*** ACIDOSIS SEVERA *** pH 7.08 | HCO3 6 mEq/L | pCO2 22 | Brecha anionica 28. Acidosis metabolica con brecha aumentada.";
        if (study == "Cetonas urinarias") return "*** POSITIVAS +++ *** Cetonuria masiva. Beta-hidroxibutirato serico 8.2 mmol/L (normal <0.6).";
        if (study == "Electrolitos sericos") return "Na 128 (pseudohiponatremia por hipergluc) | K 5.4 | Cl 98 | Fosforo 2.1 (bajo).";
        if (study == "Examen general de orina") return "Glucosa ++++ | Cetonas +++ | Densidad 1.030 | Sin infeccion.";
        if (study == "Biometria hematica") return "Leu 14,200 (leucocitosis por deshidratacion). Hb 15.8 (hemoconcentracion). Plaq 320K.";
        if (study == "Quimica sanguinea") return "Glucosa 542 | Urea 68 | Creatinina 1.4 (LRA prerrenal) | Osmolaridad 328 mOsm/kg.";
    }

    // ==================== LRA ====================
    if (key == "lra") {
        if (study == "Quimica sanguinea") return "*** LRA *** Creatinina 3.8 mg/dL (basal 1.1) | Urea 128 | Glucosa 168.";
        if (study == "Electrolitos sericos") return "*** HIPERKALEMIA *** Na 132 | K 6.2 mEq/L (alta) | Cl 105 | HCO3 18.";
        if (study == "Gasometria arterial") return "*** ACIDOSIS METABOLICA *** pH 7.28 | HCO3 18 | Brecha anionica 16.";
        if (study == "Examen general de orina") return "Densidad 1.010 (isostenuria). Proteinas +. EOSINOFILURIA presente (sugiere nefritis intersticial por AINEs).";
        if (study == "Ultrasonido renal") return "Rinones tamano normal (11cm). Ecogenicidad conservada. SIN DILATACION pielocalicial. Descarta uropatia obstructiva.";
        if (study == "ECG de 12 derivaciones") return "*** ANORMAL *** Ondas T picudas en precordiales. Compatible con HIPERKALEMIA. Sin bloqueos.";
        if (study == "Biometria hematica") return "Hb 10.2 (anemia leve). Leu 8,400. Plaq 245K.";
    }

    // ==================== BRONQUIOLITIS ====================
    if (key == "bronquiolitis") {
        if (study == "Panel viral respiratorio") return "*** POSITIVO PARA VSR *** Virus sincitial respiratorio detectado por PCR. Negativo para influenza, metapneumovirus, adenovirus.";
        if (study == "Radiografia de torax") return "Hiperinsuflacion pulmonar. Infiltrados peribronquiales bilaterales. ATELECTASIAS subsegmentarias en LSD. Sin consolidacion franca.";
        if (study == "Gasometria arterial") return "pH 7.35 | PaO2 62 (hipoxemia) | PaCO2 45 (leve hipercapnia) | SatO2 89%. Insuficiencia respiratoria tipo I.";
        if (study == "Biometria hematica") return "Leu 12,400 con linfocitosis 68% (viral). Hb 11.8. Plaq 320K.";
        if (study == "Procalcitonina") return "PCT 0.12 ng/mL (normal). Descarta sobreinfeccion bacteriana.";
        if (study == "Electrolitos sericos") return "Na 138 | K 4.4 | Cl 102. Normal para edad.";
    }

    // ==================== FALLBACK GENERICO ====================
    // Si el estudio no aplica al caso, dar respuesta neutra
    return "Resultado de " + study + ": sin hallazgos relevantes para este caso.";
}

} // namespace ENARM::GUI