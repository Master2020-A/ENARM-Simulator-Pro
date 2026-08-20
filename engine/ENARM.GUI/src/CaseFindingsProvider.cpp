// =====================================================================
//   ENARM.GUI - CaseFindingsProvider Implementation
//   Findings de 10 casos ENARM (case_001 a case_010)
// =====================================================================
#include "ENARM/GUI/CaseFindingsProvider.h"
#include <algorithm>

namespace ENARM::GUI {

// ================================================================
// AREAS Y ESTUDIOS ESTANDAR (mismos para todos los casos)
// ================================================================
std::vector<std::string> CaseFindingsProvider::GetStandardExamAreas() {
    return {
        "Inspeccion general",
        "Cabeza y cuello",
        "Torax - Auscultacion cardiaca",
        "Torax - Auscultacion pulmonar",
        "Abdomen",
        "Extremidades",
        "Neurologico",
        "Piel y tegumentos"
    };
}

std::vector<std::string> CaseFindingsProvider::GetStandardStudies() {
    return {
        // Cardiologicos
        "ECG de 12 derivaciones",
        "Troponina I/T",
        "CPK-MB",
        "BNP/NT-proBNP",
        // Laboratorios generales
        "Biometria hematica",
        "Quimica sanguinea",
        "Electrolitos sericos",
        "Tiempos de coagulacion",
        "Gasometria arterial",
        "Perfil hepatico",
        "Lactato serico",
        "Procalcitonina",
        // Infeccion
        "Hemocultivos",
        "Urocultivo",
        "Cultivo de expectoracion",
        "Panel viral respiratorio",
        // Imagen
        "Radiografia de torax",
        "Tomografia de craneo",
        "Ultrasonido abdominal",
        "Ultrasonido renal",
        // Otros
        "Examen general de orina",
        "Glucosa capilar",
        "Cetonas urinarias",
        "Proteinuria 24h",
        "Registro cardiotocografico"
    };
}

// ================================================================
// NORMALIZADOR DE CASE_ID
// ================================================================
std::string CaseFindingsProvider::NormalizeCaseId(const std::string& raw) {
    std::string s = raw;
    std::transform(s.begin(), s.end(), s.begin(), ::tolower);

    // ORDEN IMPORTA: casos mas especificos primero
    // (ej. "bronquiolitis" antes de generico "neumonia" para no confundir)

    if (s.find("bronquiolitis") != std::string::npos || s.find("vsr") != std::string::npos) return "bronquiolitis";
    if (s.find("cetoacidosis") != std::string::npos || s.find("cad") != std::string::npos || s.find("diabetica") != std::string::npos) return "cad";
    if (s.find("preeclampsia") != std::string::npos || s.find("eclampsia") != std::string::npos) return "preeclampsia";
    if (s.find("apendicitis") != std::string::npos || s.find("apendice") != std::string::npos || s.find("mcburney") != std::string::npos) return "apendicitis";
    if (s.find("neumonia") != std::string::npos || s.find("nac") != std::string::npos) return "neumonia";
    if (s.find("acv") != std::string::npos || s.find("evc") != std::string::npos || s.find("hemiparesia") != std::string::npos || s.find("isquemico") != std::string::npos) return "acv";
    if (s.find("tce") != std::string::npos || s.find("trauma") != std::string::npos || s.find("craneoencefalico") != std::string::npos || s.find("glasgow 7") != std::string::npos) return "tce";
    if (s.find("sepsis") != std::string::npos || s.find("septico") != std::string::npos || s.find("shock septico") != std::string::npos) return "sepsis";
    if (s.find("lra") != std::string::npos || s.find("lesion renal") != std::string::npos || s.find("aines") != std::string::npos || s.find("renal aguda") != std::string::npos) return "lra";
    if (s.find("iam") != std::string::npos || s.find("infarto") != std::string::npos || s.find("stemi") != std::string::npos || s.find("st ") != std::string::npos) return "iam";

    // Fallback por numero de caso
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

// ================================================================
// EXPLORACION FISICA POR CASO
// ================================================================
std::string CaseFindingsProvider::GetExamFinding(const std::string& caseId,
                                                    const std::string& area) {
    const std::string key = NormalizeCaseId(caseId);

    // ---------- CASE_001: IAM ----------
    if (key == "iam") {
        if (area == "Inspeccion general") return "Masculino 58 anos. Diaforetico, palido, facies de dolor toracico, ansioso.";
        if (area == "Cabeza y cuello") return "Sin ingurgitacion yugular. Carotidas sin soplos. Mucosas humedas.";
        if (area == "Torax - Auscultacion cardiaca") return "Ruidos ritmicos, S1/S2 normales. Sin S3/S4. Sin soplos audibles. FC 95 lpm.";
        if (area == "Torax - Auscultacion pulmonar") return "Campos limpios, MV conservado bilateral. Sin estertores.";
        if (area == "Abdomen") return "Blando, depresible, no doloroso. Peristalsis presente.";
        if (area == "Extremidades") return "Pulsos simetricos, llenado capilar 2s, sin edema.";
        if (area == "Neurologico") return "Glasgow 15/15, orientado, sin deficit focal.";
        if (area == "Piel y tegumentos") return "Palido, diaforesis profusa, sin cianosis.";
    }

    // ---------- CASE_002: SEPSIS ----------
    if (key == "sepsis") {
        if (area == "Inspeccion general") return "Femenino 68 anos. Postrada, aletargada, mal estado general. Ictericia leve.";
        if (area == "Cabeza y cuello") return "Mucosas muy secas, ojos hundidos. Sin ingurgitacion yugular (hipovolemia).";
        if (area == "Torax - Auscultacion cardiaca") return "Taquicardia (FC 128 lpm), ritmo regular, sin soplos. Pulsos filiformes.";
        if (area == "Torax - Auscultacion pulmonar") return "MV disminuido en base derecha. Sin estertores francos.";
        if (area == "Abdomen") return "Doloroso a palpacion en hipogastrio y flanco derecho. Puno percusion +.";
        if (area == "Extremidades") return "Frias, marmoreas, llenado capilar >4s. Sin edema.";
        if (area == "Neurologico") return "Glasgow 13/15 (desorientada en tiempo), letargica, sin focalizacion.";
        if (area == "Piel y tegumentos") return "Palida, con marmoleo en rodillas. Sin petequias.";
    }

    // ---------- CASE_003: ACV ISQUEMICO ----------
    if (key == "acv") {
        if (area == "Inspeccion general") return "Masculino 72 anos. Alerta pero con desviacion de comisura labial derecha evidente.";
        if (area == "Cabeza y cuello") return "Desviacion comisura labial a la derecha. Sin rigidez de nuca.";
        if (area == "Torax - Auscultacion cardiaca") return "Ritmo IRREGULAR (fibrilacion auricular). FC 96 lpm. Sin soplos carotideos.";
        if (area == "Torax - Auscultacion pulmonar") return "Campos limpios, sin estertores.";
        if (area == "Abdomen") return "Blando, no doloroso, sin visceromegalias.";
        if (area == "Extremidades") return "HEMIPARESIA DERECHA 3/5. Reflejos aumentados derecho. Babinski + derecho.";
        if (area == "Neurologico") return "Glasgow 14/15. AFASIA de expresion. Hemiparesia derecha faciobraquiocrural. NIHSS estimado 12.";
        if (area == "Piel y tegumentos") return "Sin lesiones. Coloracion normal.";
    }

    // ---------- CASE_004: NEUMONIA + EPOC ----------
    if (key == "neumonia") {
        if (area == "Inspeccion general") return "Masculino 74 anos. Taquipneico (FR 28), uso de musculos accesorios, cianosis peribucal leve, tos productiva.";
        if (area == "Cabeza y cuello") return "Mucosas secas, cianosis peribucal, ingurgitacion yugular leve. Aleteo nasal presente.";
        if (area == "Torax - Auscultacion cardiaca") return "Taquicardia (108 lpm), ritmo regular, sin soplos. S2 aumentado (probable HTP).";
        if (area == "Torax - Auscultacion pulmonar") return "Sindrome de condensacion en base derecha con ESTERTORES CREPITANTES abundantes. SIBILANCIAS espiratorias difusas bilaterales. MV disminuido base derecha.";
        if (area == "Abdomen") return "Blando, no doloroso. Higado no palpable.";
        if (area == "Extremidades") return "Edema maleolar leve bilateral. Cianosis distal en dedos. Sin acropaquia franca.";
        if (area == "Neurologico") return "Glasgow 15/15, orientado, ligeramente ansioso por disnea.";
        if (area == "Piel y tegumentos") return "Piel caliente, febril (38.6C). Cianosis distal.";
    }

    // ---------- CASE_005: PREECLAMPSIA ----------
    if (key == "preeclampsia") {
        if (area == "Inspeccion general") return "Femenino 28 anos, embarazo 32 SDG. Consciente, ansiosa, edema facial evidente. Refiere cefalea y vision borrosa.";
        if (area == "Cabeza y cuello") return "EDEMA PALPEBRAL bilateral marcado. Fondo de ojo con edema de papila incipiente. Mucosas humedas.";
        if (area == "Torax - Auscultacion cardiaca") return "Taquicardia (96 lpm), ritmo regular, sin soplos.";
        if (area == "Torax - Auscultacion pulmonar") return "Estertores crepitantes basales bilaterales (edema pulmonar incipiente). Resto sin alteraciones.";
        if (area == "Abdomen") return "Utero gravido 32 SDG, DOLOR EN EPIGASTRIO TIPO BARRA. FCF 148 lpm. Sin actividad uterina. Cervix cerrado.";
        if (area == "Extremidades") return "EDEMA +++ hasta rodillas y manos con fovea. HIPERREFLEXIA PATELAR +++/++++. Sin clonus.";
        if (area == "Neurologico") return "Alerta, orientada. HIPERREFLEXIA GENERALIZADA. Sin clonus. Sin focalizacion.";
        if (area == "Piel y tegumentos") return "Edema facial, palpebral y de manos evidente. Piel caliente.";
    }


    // ---------- CASE_006: APENDICITIS PEDIATRICA ----------
    if (key == "apendicitis") {
        if (area == "Inspeccion general") return "Escolar 8 anos. Quejumbroso, POSICION ANTALGICA (decubito con flexion de cadera derecha). Facies de dolor.";
        if (area == "Cabeza y cuello") return "Mucosas orales semi-secas. Sin adenopatias cervicales.";
        if (area == "Torax - Auscultacion cardiaca") return "Taquicardia (118 lpm), ritmo regular, sin soplos.";
        if (area == "Torax - Auscultacion pulmonar") return "Campos pulmonares bien ventilados, sin estertores.";
        if (area == "Abdomen") return "DOLOR EN FOSA ILIACA DERECHA. Signo de McBURNEY POSITIVO. BLUMBERG POSITIVO. ROVSING POSITIVO. Defensa muscular localizada. Peristalsis disminuida.";
        if (area == "Extremidades") return "Sin alteraciones. Pulsos normales.";
        if (area == "Neurologico") return "Alerta, orientado. Sin focalizacion.";
        if (area == "Piel y tegumentos") return "Febril (38.2C). Sin lesiones ni rash.";
    }

    // ---------- CASE_007: TCE SEVERO ----------
    if (key == "tce") {
        if (area == "Inspeccion general") return "Masculino 32 anos INCONSCIENTE. Herida contusa frontal derecha 8cm sangrante. HEMATOMA PERIORBITARIO BILATERAL (ojos de mapache). Otorragia derecha.";
        if (area == "Cabeza y cuello") return "SIGNO DE BATTLE POSITIVO derecho. Salida de LCR por oido derecho. Collarin cervical colocado. PUPILAS: derecha 5mm ARREACTIVA, izquierda 3mm hiporreactiva.";
        if (area == "Torax - Auscultacion cardiaca") return "BRADICARDIA (58 lpm), ritmo regular. HTA (178/98). TRIADA DE CUSHING PRESENTE.";
        if (area == "Torax - Auscultacion pulmonar") return "Patron respiratorio IRREGULAR (Cheyne-Stokes). Estertores por broncoaspiracion probable.";
        if (area == "Abdomen") return "Blando, no distendido, sin datos de irritacion peritoneal. FAST pendiente.";
        if (area == "Extremidades") return "POSTURA DE DECORTICACION derecha, DECEREBRACION izquierda. Reflejos osteotendinosos aumentados. BABINSKI BILATERAL.";
        if (area == "Neurologico") return "GLASGOW 7 (O2 V2 M3). Pupilas anisocoricas. Postura patologica bilateral. Sin respuesta verbal comprensible.";
        if (area == "Piel y tegumentos") return "Herida sangrante frontal. Hematomas periorbitarios bilaterales. Sin cianosis.";
    }

    // ---------- CASE_008: CAD PEDIATRICA ----------
    if (key == "cad") {
        if (area == "Inspeccion general") return "Adolescente 15 anos, femenina. Somnolienta pero despertable. SIGNOS DE DESHIDRATACION SEVERA. ALIENTO CETONICO (afrutado) evidente.";
        if (area == "Cabeza y cuello") return "MUCOSAS MUY SECAS, ojos hundidos, TURGENCIA CUTANEA DISMINUIDA. Aliento afrutado.";
        if (area == "Torax - Auscultacion cardiaca") return "Taquicardia (128 lpm), ritmo regular. Pulsos perifericos DEBILES. Llenado capilar 4s.";
        if (area == "Torax - Auscultacion pulmonar") return "RESPIRACION DE KUSSMAUL (FR 32, rapida y profunda). Campos limpios sin estertores.";
        if (area == "Abdomen") return "Blando pero DOLOROSO DIFUSAMENTE (pseudoabdomen agudo por acidosis). Sin datos de irritacion peritoneal. Peristalsis disminuida.";
        if (area == "Extremidades") return "Frias, mal perfundidas. Sin edema. Sin lesiones.";
        if (area == "Neurologico") return "SOMNOLIENTA, orientada parcialmente. Glasgow 13. Sin focalizacion. Reflejos disminuidos.";
        if (area == "Piel y tegumentos") return "Piel seca, palida. Sin cianosis. Perdida de peso evidente.";
    }

    // ---------- CASE_009: LRA POR AINEs ----------
    if (key == "lra") {
        if (area == "Inspeccion general") return "Masculino 71 anos. Consciente, orientado. FACIES EDEMATIZADA. Palidez de tegumentos.";
        if (area == "Cabeza y cuello") return "Mucosas humedas. INGURGITACION YUGULAR ++/++++. Edema facial leve.";
        if (area == "Torax - Auscultacion cardiaca") return "Ritmo regular, S3 AUDIBLE (galope), sin soplos. TA 158/92 (HTA no controlada).";
        if (area == "Torax - Auscultacion pulmonar") return "ESTERTORES CREPITANTES BIBASALES (sobrecarga hidrica). MV conservado en apices.";
        if (area == "Abdomen") return "Blando, depresible, no doloroso. Sin visceromegalias. PUNO PERCUSION LUMBAR NEGATIVA bilateral.";
        if (area == "Extremidades") return "EDEMA +++ HASTA RODILLAS con fovea, simetrico. Sin datos de trombosis venosa.";
        if (area == "Neurologico") return "Alerta, orientado en 3 esferas. Sin focalizacion. Sin flapping.";
        if (area == "Piel y tegumentos") return "Palidez generalizada. Edema en zonas declives. Sin lesiones.";
    }

    // ---------- CASE_010: BRONQUIOLITIS ----------
    if (key == "bronquiolitis") {
        if (area == "Inspeccion general") return "Lactante 6 meses. Irritable, con DIFICULTAD RESPIRATORIA EVIDENTE. Palidez leve. Aleteo nasal marcado.";
        if (area == "Cabeza y cuello") return "Fontanela anterior normotensa. ALETEO NASAL PRESENTE. Rinorrea hialina abundante.";
        if (area == "Torax - Auscultacion cardiaca") return "Taquicardia (158 lpm), sin soplos. Pulsos simetricos. Llenado capilar 3s.";
        if (area == "Torax - Auscultacion pulmonar") return "TIRAJE INTERCOSTAL Y SUBCOSTAL MARCADO. Retraccion supraesternal. SIBILANCIAS ESPIRATORIAS DIFUSAS bilaterales. ESTERTORES CREPITANTES en bases. Espiracion prolongada.";
        if (area == "Abdomen") return "Blando, no distendido, sin visceromegalias. Peristalsis presente.";
        if (area == "Extremidades") return "Bien perfundidas, sin edema. Pulsos simetricos.";
        if (area == "Neurologico") return "Reactivo pero IRRITABLE. Hipotonia leve por fatiga respiratoria. Llanto debil.";
        if (area == "Piel y tegumentos") return "Palidez leve, sin cianosis franca. Piel caliente (37.4C).";
    }

    // Fallback si no matchea ningun caso
    return "Exploracion sin hallazgos relevantes para el caso actual.";
}

// GetStudyResult() implementado en CaseFindingsProvider_Studies.cpp

} // namespace ENARM::GUI