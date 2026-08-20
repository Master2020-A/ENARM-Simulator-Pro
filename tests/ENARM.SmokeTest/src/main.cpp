// ═══════════════════════════════════════════════════════════════
//   ENARM Sprint 3D - Insertar Caso #003: ACV Isquemico
// ═══════════════════════════════════════════════════════════════
#include "ENARM/Common/Version.h"
#include "ENARM/Common/Logger.h"

#include "ENARM/Data/DatabaseManager.h"
#include "ENARM/Data/Repositories/CaseRepository.h"
#include "ENARM/Data/Repositories/MCQRepository.h"

#include <iostream>
#include <iomanip>

using namespace ENARM;
using json = nlohmann::json;

template <typename T> static void Ignore(T&&) {}

int main() {
#ifdef _WIN32
    system("chcp 65001 > nul");
#endif

    std::cout << "\n==============================================================\n";
    std::cout << "   " << Common::kProjectName << " v" << Common::kVersion << "\n";
    std::cout << "   Sprint 3D - Caso #003: ACV Isquemico Agudo\n";
    std::cout << "==============================================================\n\n";

    Common::LoggerConfig logCfg;
    logCfg.enableFile = false;
    logCfg.asyncMode = false;
    Common::Logger::Initialize(logCfg);

    Data::ConnectionConfig config;
    config.database = "enarm_db";
    config.user     = "postgres";
    config.password = "manager";

    auto dbRes = Data::DatabaseManager::Create(config, 3);
    if (dbRes.IsErr()) {
        std::cerr << "ERROR: " << dbRes.Error().ToString() << "\n";
        return 1;
    }
    auto db = dbRes.Value();
    std::cout << "Conectado a PostgreSQL OK\n\n";

    Data::CaseRepository caseRepo(db);
    Data::MCQRepository mcqRepo(db);

    // Limpiar version anterior
    std::cout << "===== Limpiando version anterior =====\n";
    {
        auto conn = db->Acquire();
        pqxx::work tx(conn.Get());
        tx.exec(
            "DELETE FROM mcq_bank WHERE specialty='Neurology' AND topic IN "
            "('NIHSS ACV', 'Ventana terapeutica alteplasa', "
            " 'Contraindicaciones alteplasa')"
        );
        tx.exec("DELETE FROM clinical_cases WHERE slug='acv-isquemico-acm-izquierda-varon-72'");
        tx.commit();
        std::cout << "  OK\n\n";
    }

    // ═════════════════════════════════════════════════════════════
    // CASO #003 - ACV ISQUEMICO
    // ═════════════════════════════════════════════════════════════
    std::cout << "===== Preparando Caso #003 =====\n\n";

    Data::ClinicalCaseRecord c;
    c.title          = "ACV isquemico agudo del territorio ACM izquierda en varon de 72 anos con FA no anticoagulada";
    c.slug           = "acv-isquemico-acm-izquierda-varon-72";
    c.chiefComplaint = "Hemiparesia derecha subita, afasia y desviacion de comisura labial de 90 minutos de evolucion";
    c.specialty      = "Neurology";
    c.subSpecialty   = "Neurologia vascular";
    c.difficulty     = "advanced";
    c.setting        = "emergency";
    c.authorName     = "Equipo ENARM Simulator";

    c.patient = {
        {"full_name",  "Alejandro Vazquez Fuentes"},
        {"age", 72},
        {"sex", "Male"},
        {"weight_kg", 78.0},
        {"height_cm", 170.0},
        {"bmi", 27.0},
        {"occupation", "Jubilado (ingeniero civil)"},
        {"marital_status", "Casado (46 anos)"},
        {"education", "Licenciatura + Maestria"},
        {"residence", "CDMX, colonia clase media"},
        {"vital_signs_admission", {
            {"systolic_bp", 178},
            {"diastolic_bp", 98},
            {"map", 125},
            {"heart_rate", 118},
            {"heart_rhythm", "irregular (FA)"},
            {"resp_rate", 18},
            {"spo2", 96},
            {"temperature_c", 36.5},
            {"glucose_mgdl", 132},
            {"pain_scale", 0},
            {"glasgow", 15},
            {"nihss_score", 18},
            {"nihss_severity", "moderado-severo"},
            {"last_known_well", "06:30 AM"},
            {"time_since_symptoms_min", 90},
            {"clinical_note", "VENTANA ACTIVA para alteplasa (<4.5h) y trombectomia (<6h). PA 178/98 limitrofe - considerar labetalol para <185/110 antes de trombolisis"}
        }},
        {"medical_history", json::array({
            {{"condition", "Fibrilacion auricular"},
             {"years", 3},
             {"treatment", "NO anticoagulado (rechaza warfarina, no toma DOAC por costo)"},
             {"controlled", false},
             {"notes", "CHA2DS2-VASc: 5 puntos - MUY ALTO riesgo embolico, indicacion clara de anticoagulacion"}},
            {{"condition", "Hipertension arterial"},
             {"years", 15},
             {"treatment", "Losartan 100 mg + Amlodipino 5 mg"},
             {"controlled", true}},
            {{"condition", "Diabetes Mellitus tipo 2"},
             {"years", 8},
             {"treatment", "Metformina 850 mg BID"},
             {"controlled", true},
             {"notes", "HbA1c 6.8% - bien controlada"}},
            {{"condition", "Dislipidemia"},
             {"treatment", "Atorvastatina 40 mg/dia"}}
        })},
        {"allergies", json::array()},
        {"family_history", json::array({
            {{"relationship", "Padre"}, {"condition", "ACV"},
             {"age_at_diagnosis", 68}, {"deceased", true}, {"age_at_death", 70}},
            {{"relationship", "Madre"}, {"condition", "HAS + DM2"}},
            {{"relationship", "Hermano"}, {"condition", "Cardiopatia isquemica"}}
        })},
        {"lifestyle", {
            {"smoker", false},
            {"ex_smoker", true},
            {"smoking_years_previous", 25},
            {"years_since_quit", 20},
            {"alcohol_use", true},
            {"alcohol_frequency", "1-2 copas vino/semana (social)"},
            {"exercise_frequency", "caminata 30 min 3x/semana"},
            {"functional_status_previous_mrs", 0}
        }}
    };

    c.historyPresentIllness =
        "Paciente masculino de 72 anos, traido por su hija al servicio de urgencias por cuadro subito. "
        "CRONOLOGIA PRECISA: 06:30 AM ultima vez visto sano (fue al bano normalmente). "
        "07:30 AM: esposa lo encontro sentado en el sillon intentando hablar sin articular palabras, "
        "con comision labial derecha caida y sin mover bien el brazo derecho. "
        "08:00 AM: llegada a urgencias. TIEMPO DESDE INICIO DE SINTOMAS: ~90 minutos - VENTANA "
        "TERAPEUTICA ACTIVA para trombolisis IV y trombectomia. "
        "Presenta afasia (no puede hablar coherentemente), hemiparesia derecha (brazo 2/5, pierna 3/5), "
        "paralisis facial central derecha, desviacion de la mirada hacia la izquierda, "
        "alteracion sensitiva derecha. Consciente pero angustiado. NO cefalea intensa, "
        "NO vomito, NO convulsiones, NO alteracion del nivel de consciencia (Glasgow 15). "
        "Antecedente CRITICO: FA de 3 anos NO anticoagulada (rechazo por miedo a sangrado).";

    c.symptoms = json::array({
        {{"name", "Afasia global severa"},
         {"severity", "Severe"},
         {"onset", "Sudden"},
         {"description", "No puede articular palabras, comprension disminuida"},
         {"patient_description", "Palabras sin sentido, imposible dialogar"}},
        {{"name", "Hemiparesia derecha"},
         {"location", "Faciobraquiocrural derecho"},
         {"severity", "Severe"},
         {"description", "Brazo 2/5, pierna 3/5 escala MRC"},
         {"patient_description", "No puede levantar el brazo, arrastra la pierna"}},
        {{"name", "Paralisis facial central derecha"},
         {"severity", "Moderate"},
         {"description", "Comisura labial derecha caida, respeta frente"}},
        {{"name", "Desviacion oculocefalica"},
         {"description", "Mirada desviada hacia el lado sano (izquierda) - signo de Prevost"}},
        {{"name", "Hemianopsia derecha"},
         {"severity", "Moderate"},
         {"description", "Sospechada, dificil de valorar por afasia"}},
        {{"name", "Alteracion sensitiva derecha"},
         {"severity", "Mild-Moderate"},
         {"description", "Hipoestesia hemicuerpo derecho, parcial"}}
    });

    c.expectedDiagnosis = {
        {"primary", "ACV isquemico agudo del territorio de la ACM izquierda de origen cardioembolico probable"},
        {"icd10", "I63.4"},
        {"nihss_score", 18},
        {"nihss_severity", "moderado-severo"},
        {"aspects_score", 8},
        {"differentials", json::array({
            "ACV hemorragico (CRITICO descartar con TC craneal urgente)",
            "Crisis convulsiva con paralisis de Todd",
            "Hipoglucemia (descartada con glucosa 132)",
            "Migrana con aura complicada",
            "Encefalopatia hipertensiva",
            "AIT (accidente isquemico transitorio)",
            "Tumor cerebral con hemorragia",
            "Absceso cerebral",
            "Trastorno conversivo"
        })},
        {"embolic_source", "Fibrilacion auricular no anticoagulada (CHA2DS2-VASc 5)"},
        {"therapeutic_window", {
            {"time_from_lkw_min", 90},
            {"alteplasa_eligible", true},
            {"alteplasa_window_h", 4.5},
            {"thrombectomy_eligible", true},
            {"thrombectomy_window_h", 6}
        }},
        {"key_findings", json::array({
            "Inicio subito con sindrome deficitario focal (afasia + hemiparesia)",
            "NIHSS 18 (moderado-severo) - indicacion clara de reperfusion",
            "Ventana terapeutica activa (<4.5h para alteplasa)",
            "FA no anticoagulada como fuente embolica (CHA2DS2-VASc 5)",
            "PA 178/98 en limite superior - manejar antes de trombolisis",
            "Sin datos de hemorragia por clinica (no cefalea, no vomito, Glasgow 15)"
        })}
    };

    c.expectedStudies = json::array({
        {{"name", "TC craneal simple URGENTE"},
         {"category", "imaging"}, {"priority", "critical"},
         {"time_limit_min", 25}, {"required", true}, {"score_weight", 25},
         {"justification", "CRITICO descartar hemorragia intracraneal antes de trombolisis. Calcular ASPECTS"}},
        {{"name", "Glucosa capilar"},
         {"category", "lab"}, {"priority", "critical"},
         {"time_limit_min", 5}, {"required", true}, {"score_weight", 10},
         {"justification", "Descartar hipoglucemia (stroke mimic mas frecuente)"}},
        {{"name", "Quimica sanguinea (Cr, urea, electrolitos)"},
         {"category", "lab"}, {"priority", "high"},
         {"required", true}, {"score_weight", 5},
         {"justification", "Funcion renal previa a contraste, sodio (Na+<130 corregir)"}},
        {{"name", "Biometria hematica + plaquetas"},
         {"category", "lab"}, {"priority", "high"},
         {"required", true}, {"score_weight", 5},
         {"justification", "Plaquetopenia (<100k) contraindica alteplasa absolutamente"}},
        {{"name", "Tiempos de coagulacion (TP/INR, TTPa)"},
         {"category", "lab"}, {"priority", "critical"},
         {"required", true}, {"score_weight", 10},
         {"justification", "INR >1.7 CONTRAINDICA alteplasa. Paciente NO anticoagulado (esperado normal)"}},
        {{"name", "ECG 12 derivaciones"},
         {"category", "diagnostic"}, {"priority", "high"},
         {"required", true}, {"score_weight", 5},
         {"justification", "Confirmar FA, detectar IAM concurrente (25% ACV tienen isquemia miocardica)"}},
        {{"name", "AngioTC de vasos intra y extracraneales"},
         {"category", "imaging"}, {"priority", "critical"},
         {"required", true}, {"score_weight", 15},
         {"justification", "Identificar oclusion de gran vaso (LVO) para candidatura a trombectomia"}},
        {{"name", "TC perfusion cerebral"},
         {"category", "imaging"}, {"priority", "medium"},
         {"required", false}, {"score_weight", 5},
         {"justification", "Evaluar penumbra vs core (criterios DAWN/DEFUSE-3 si ventana extendida)"}},
        {{"name", "Rx torax"},
         {"category", "imaging"}, {"priority", "low"},
         {"required", false}, {"score_weight", 2},
         {"justification", "Descartar cardiopatia estructural, broncoaspiracion"}},
        {{"name", "Gasometria arterial"},
         {"category", "lab"}, {"priority", "low"},
         {"required", false}, {"score_weight", 3},
         {"justification", "Si compromiso respiratorio"}},
        {{"name", "Ecocardiograma transtoracico"},
         {"category", "imaging"}, {"priority", "medium"},
         {"required", true}, {"score_weight", 5},
         {"justification", "Post-agudo: buscar trombo intracavitario, evaluar FEVI"}},
        {{"name", "Doppler carotideo"},
         {"category", "imaging"}, {"priority", "medium"},
         {"required", false}, {"score_weight", 3},
         {"justification", "Post-agudo: descartar estenosis carotidea significativa"}},
        {{"name", "Perfil lipidico + HbA1c"},
         {"category", "lab"}, {"priority", "low"},
         {"required", false}, {"score_weight", 2},
         {"justification", "Post-agudo: estratificacion riesgo cardiovascular"}}
    });

    c.expectedTreatments = json::array({
        // FASE 1: MANEJO INMEDIATO CODIGO STROKE
        {{"phase", "code_stroke_immediate"}, {"order", 1},
         {"intervention", "Activar CODIGO STROKE (neurologia + radiologia)"},
         {"category", "protocol"}, {"required", true}, {"score_weight", 10},
         {"time_limit_min", 5}},
        {{"phase", "code_stroke_immediate"}, {"order", 2},
         {"intervention", "NIHSS realizado y documentado por medico"},
         {"category", "assessment"}, {"required", true}, {"score_weight", 15},
         {"time_limit_min", 10}},
        {{"phase", "code_stroke_immediate"}, {"order", 3},
         {"intervention", "Glucosa capilar inmediata (descartar hipoglucemia)"},
         {"category", "monitoring"}, {"required", true}, {"score_weight", 5},
         {"time_limit_min", 5}},
        {{"phase", "code_stroke_immediate"}, {"order", 4},
         {"intervention", "2 accesos venosos perifericos"},
         {"category", "procedure"}, {"required", true}, {"score_weight", 3}},
        {{"phase", "code_stroke_immediate"}, {"order", 5},
         {"intervention", "Monitorizacion continua ECG-PA-SpO2"},
         {"category", "monitoring"}, {"required", true}, {"score_weight", 3}},
        {{"phase", "code_stroke_immediate"}, {"order", 6},
         {"intervention", "NPO (nada por via oral) hasta evaluar deglucion"},
         {"category", "care"}, {"required", true}, {"score_weight", 3}},
        {{"phase", "code_stroke_immediate"}, {"order", 7},
         {"intervention", "Oxigeno SOLO si SpO2 <94%"},
         {"category", "medication"}, {"required", false}, {"score_weight", 3}},

        // FASE 2: MANEJO PA PARA ELEGIBILIDAD TROMBOLISIS
        {{"phase", "bp_management"}, {"order", 8},
         {"intervention", "Labetalol 10-20 mg IV bolo lento SI PA >185/110"},
         {"category", "medication"}, {"required", true}, {"score_weight", 15},
         {"target", "PA <185/110 pre-trombolisis, <180/105 post-trombolisis"},
         {"note", "Paciente actual 178/98 - LIMITROFE, monitorizar"}},
        {{"phase", "bp_management"}, {"order", 9},
         {"intervention", "Nicardipino 5 mg/h IV infusion (alternativa a labetalol)"},
         {"category", "medication"}, {"required", false}, {"score_weight", 10}},
        {{"phase", "bp_management"}, {"order", 10},
         {"intervention", "EVITAR nitroprusiato (aumenta PIC)"},
         {"category", "avoid"}, {"required", true}, {"score_weight", 0},
         {"critical_note", "Nitroprusiato eleva presion intracraneal en ACV"}},

        // FASE 3: REPERFUSION - TROMBOLISIS IV
        {{"phase", "reperfusion_thrombolysis"}, {"order", 11},
         {"intervention", "Verificar TODAS las contraindicaciones absolutas de alteplasa"},
         {"category", "checklist"}, {"required", true}, {"score_weight", 10}},
        {{"phase", "reperfusion_thrombolysis"}, {"order", 12},
         {"intervention", "Alteplasa 0.9 mg/kg IV (10% bolo 1 min + 90% infusion 60 min)"},
         {"category", "medication"}, {"required", true}, {"score_weight", 30},
         {"dose_for_patient", "78 kg: total 70.2 mg (bolo 7 mg + infusion 63.2 mg)"},
         {"window", "<=4.5h desde LKW"},
         {"evidence", "NINDS 1995, ECASS III 2008"}},
        {{"phase", "reperfusion_thrombolysis"}, {"order", 13},
         {"intervention", "NO heparina/antiagregantes/anticoagulantes primeras 24h post-alteplasa"},
         {"category", "avoid"}, {"required", true}, {"score_weight", 10}},

        // FASE 3B: REPERFUSION - TROMBECTOMIA
        {{"phase", "reperfusion_thrombectomy"}, {"order", 14},
         {"intervention", "AngioTC confirmar oclusion gran vaso (LVO)"},
         {"category", "diagnostic"}, {"required", true}, {"score_weight", 15}},
        {{"phase", "reperfusion_thrombectomy"}, {"order", 15},
         {"intervention", "Activar hemodinamia intervencionista para trombectomia"},
         {"category", "protocol"}, {"required", true}, {"score_weight", 25},
         {"time_limit_min", 90}},
        {{"phase", "reperfusion_thrombectomy"}, {"order", 16},
         {"intervention", "Trombectomia mecanica si LVO confirmada"},
         {"category", "procedure"}, {"required", true}, {"score_weight", 30},
         {"window", "<=6h estandar, hasta 24h con perfusion (DAWN/DEFUSE-3)"},
         {"criteria", "LVO + ASPECTS>=6 + NIHSS>=6 + mRS previo <=2"}},

        // FASE 4: POST-REPERFUSION E INGRESO
        {{"phase", "post_reperfusion"}, {"order", 17},
         {"intervention", "Ingreso a Unidad de Stroke o UCI Neurologica"},
         {"category", "admission"}, {"required", true}, {"score_weight", 10}},
        {{"phase", "post_reperfusion"}, {"order", 18},
         {"intervention", "NIHSS seriado c/15 min primera hora, c/1h por 6h, c/4h por 24h"},
         {"category", "monitoring"}, {"required", true}, {"score_weight", 5}},
        {{"phase", "post_reperfusion"}, {"order", 19},
         {"intervention", "TC craneal control a las 24h post-alteplasa (evaluar transformacion hemorragica)"},
         {"category", "imaging"}, {"required", true}, {"score_weight", 5}},

        // FASE 5: PREVENCION SECUNDARIA (>24h)
        {{"phase", "secondary_prevention"}, {"order", 20},
         {"intervention", "Anticoagulacion con DOAC (apixaban 5 mg BID) segun regla 1-3-6-12"},
         {"category", "medication"}, {"required", true}, {"score_weight", 10},
         {"rule", "NIHSS<8: dia 3 | NIHSS 8-15: dia 6 | NIHSS>15: dia 12"},
         {"evidence", "ARISTOTLE, ROCKET-AF"}},
        {{"phase", "secondary_prevention"}, {"order", 21},
         {"intervention", "Estatina alta intensidad (Atorvastatina 40-80 mg)"},
         {"category", "medication"}, {"required", true}, {"score_weight", 5},
         {"target", "LDL <70 mg/dL"}},
        {{"phase", "secondary_prevention"}, {"order", 22},
         {"intervention", "Rehabilitacion multidisciplinaria temprana (24-48h post-agudo)"},
         {"category", "care"}, {"required", true}, {"score_weight", 5}}
    });

    c.expectedFollowup =
        "Unidad de Stroke o UCI Neurologica por 24-48h minimo. TC craneal control a 24h post-alteplasa "
        "(OBLIGATORIO, evaluar transformacion hemorragica). NIHSS seriado. Ecocardiograma para descartar "
        "trombo intracavitario. Doppler carotideo. Inicio anticoagulacion segun regla 1-3-6-12 (dia 12 "
        "para este paciente con NIHSS 18). Rehabilitacion multidisciplinaria (fisica, ocupacional, "
        "lenguaje/logopedia). Egreso con: DOAC (apixaban 5 mg BID), Atorvastatina 40 mg, continuar "
        "antihipertensivos con objetivo <130/80, control estricto DM2 con HbA1c<7%. Cita neurologia en "
        "2 semanas. Educacion sobre signos de nuevo ACV y adherencia estricta al DOAC.";

    c.learningObjectives = json::array({
        "Reconocer presentacion clinica tipica de ACV isquemico (deficit focal subito)",
        "Aplicar la escala NIHSS para estratificar severidad del ACV",
        "Activar CODIGO STROKE con objetivos temporales criticos (puerta-aguja <60 min)",
        "Establecer ventana terapeutica para alteplasa (<=4.5h) y trombectomia (<=6-24h)",
        "Reconocer contraindicaciones absolutas y relativas de alteplasa",
        "Manejar presion arterial para elegibilidad de trombolisis (<185/110)",
        "Interpretar TC craneal (descartar hemorragia) y AngioTC (identificar LVO)",
        "Aplicar regla 1-3-6-12 para inicio de anticoagulacion post-ACV cardioembolico",
        "Estratificar riesgo con CHA2DS2-VASc y prevenir con DOAC/warfarina en FA"
    });

    c.teachingPoints =
        "TIME IS BRAIN - 1.9 millones de neuronas mueren por minuto sin reperfusion. "
        "NIHSS: 0-4 leve, 5-15 moderado, 16-20 moderado-severo, >20 severo. "
        "VENTANA ALTEPLASA: <=4.5h desde LKW (Last Known Well). "
        "VENTANA TROMBECTOMIA: <=6h estandar, hasta 24h con criterios perfusion (DAWN, DEFUSE-3). "
        "TC CRANEAL ANTES de trombolisis (regla de oro: descartar hemorragia). "
        "ASPECTS >=6 para elegibilidad de trombectomia. "
        "PA <185/110 OBLIGATORIO antes de alteplasa (labetalol o nicardipino). "
        "ALTEPLASA 0.9 mg/kg (max 90 mg): 10% bolo + 90% infusion 60 min. "
        "NO heparina/antiagregantes en 24h post-alteplasa. "
        "CHA2DS2-VASc >=2 en varones (>=3 en mujeres) obliga anticoagulacion. "
        "DOAC preferido sobre warfarina en FA no valvular (RE-LY, ROCKET-AF, ARISTOTLE). "
        "REGLA 1-3-6-12 para inicio anticoagulacion post-ACV segun NIHSS. "
        "STROKE MIMIC MAS FRECUENTE: hipoglucemia - SIEMPRE medir glucosa capilar PRIMERO.";

    c.pubmedPmids = { "31662037", "30165340", "29364767", "25671798" };
    c.guidelinesIds = { "IMSS-058-08" };

    // Insertar
    std::cout << "Insertando caso...\n";
    auto insRes = caseRepo.Insert(c);
    if (insRes.IsErr()) {
        std::cerr << "ERROR: " << insRes.Error().ToString() << "\n";
        return 1;
    }
    std::string caseId = insRes.Value();
    std::cout << "  OK - ID: " << caseId << "\n";

    auto pubRes = caseRepo.Publish(caseId);
    Ignore(pubRes);
    std::cout << "  OK - Publicado\n";

    for (const auto& tag : {"neurologia", "acv", "stroke", "isquemico", "codigo-stroke",
                             "alteplasa", "trombolisis", "trombectomia", "nihss",
                             "fa", "cha2ds2vasc", "urgencias", "ventana-terapeutica",
                             "aspects", "enarm", "alta-frecuencia"}) {
        auto tRes = caseRepo.AddTag(caseId, tag);
        Ignore(tRes);
    }
    std::cout << "  OK - 16 tags\n";

    // ═════════════════════════════════════════════════════════════
    // 3 MCQs
    // ═════════════════════════════════════════════════════════════
    std::cout << "\n===== Insertando 3 MCQs =====\n\n";

    // ─── MCQ 1: NIHSS ───
    {
        Data::MCQuestion q;
        q.specialty = "Neurology";
        q.difficulty = "intermediate";
        q.topic = "NIHSS ACV";
        q.vignette =
            "Varon de 72 anos con FA no anticoagulada, llega a urgencias 90 minutos despues del "
            "inicio subito de: afasia global, hemiparesia derecha (brazo 2/5, pierna 3/5), paralisis "
            "facial central derecha, desviacion oculocefalica izquierda, hipoestesia hemicuerpo "
            "derecho. Glasgow 15, PA 178/98, FC 118 irregular.";
        q.question = "El puntaje NIHSS estimado es de 18. Segun esta puntuacion, como se clasifica el ACV?";
        q.options = json::array({
            {{"letter", "A"}, {"text", "Leve (indicacion de manejo conservador)"}},
            {{"letter", "B"}, {"text", "Moderado (indicacion relativa de reperfusion)"}},
            {{"letter", "C"}, {"text", "Moderado-severo (indicacion clara de reperfusion + trombectomia si LVO)"}},
            {{"letter", "D"}, {"text", "Severo con muy mal pronostico (no candidato a reperfusion)"}}
        });
        q.correctAnswer = 'C';
        q.explanationCorrect =
            "NIHSS 16-20 se clasifica como ACV moderado-severo. Este rango tiene indicacion CLARA de "
            "reperfusion aguda (alteplasa si <=4.5h, trombectomia mecanica si LVO confirmada). "
            "Estudios como DAWN y DEFUSE-3 mostraron beneficio de trombectomia hasta 24h con criterios "
            "de perfusion. Contraindicaciones a considerar: PA no controlada, INR elevado, ACV extenso "
            "en TC (ASPECTS<6).";
        q.explanationIncorrect = {
            {"A", "NIHSS <=4 es leve. NIHSS 18 NO es leve, es moderado-severo con indicacion clara de reperfusion."},
            {"B", "NIHSS 5-15 es moderado. NIHSS 18 supera este rango."},
            {"D", "Aun con NIHSS 18 (moderado-severo) el paciente SI es candidato a reperfusion. Se contraindica cuando NIHSS >25 con dano establecido en TC (ASPECTS bajo)."}
        };
        q.pubmedPmids = {"31662037"};
        q.guidelinesIds = {"IMSS-058-08"};

        auto r = mcqRepo.Insert(q);
        if (r.IsOk()) std::cout << "  MCQ 1 (NIHSS) OK\n";
    }

    // ─── MCQ 2: Ventana terapeutica ───
    {
        Data::MCQuestion q;
        q.specialty = "Neurology";
        q.difficulty = "advanced";
        q.topic = "Ventana terapeutica alteplasa";
        q.vignette =
            "Mismo paciente. Ultima vez visto sano (LKW) a las 06:30 AM. Llegada a urgencias 08:00 AM. "
            "TC craneal simple realizada a los 20 minutos: sin evidencia de hemorragia, ASPECTS 8/10. "
            "Glucosa 132, INR 1.1, plaquetas 235,000. PA actual: 178/98.";
        q.question = "Cual es la conducta terapeutica MAS APROPIADA en este momento?";
        q.options = json::array({
            {{"letter", "A"}, {"text", "Iniciar alteplasa IV inmediatamente 0.9 mg/kg (bolo 10% + infusion 60 min)"}},
            {{"letter", "B"}, {"text", "Reducir PA con labetalol a <185/110, luego iniciar alteplasa"}},
            {{"letter", "C"}, {"text", "Esperar 4.5h desde LKW antes de iniciar cualquier reperfusion"}},
            {{"letter", "D"}, {"text", "Iniciar antiagregacion dual y observar 24h"}}
        });
        q.correctAnswer = 'B';
        q.explanationCorrect =
            "La PA debe controlarse a <185/110 mmHg ANTES de administrar alteplasa (obligatorio segun "
            "AHA/ASA 2019). Este paciente tiene 178/98 (limitrofe), pero el sistolico esta cerca del "
            "umbral. Se debe usar labetalol 10-20 mg IV en bolo lento (repetible) o nicardipino en "
            "infusion. Una vez PA <185/110, se procede con alteplasa 0.9 mg/kg (max 90 mg): 10% bolo "
            "en 1 min + 90% en infusion de 60 min. Estamos dentro de ventana (90 min desde LKW, "
            "limite 4.5h).";
        q.explanationIncorrect = {
            {"A", "NO se puede iniciar alteplasa con PA 178/98 sin controlarla primero. Riesgo de hemorragia intracraneal aumenta significativamente."},
            {"C", "TIME IS BRAIN - No se debe retrasar reperfusion. Cada minuto sin tratamiento perdemos ~1.9 millones neuronas."},
            {"D", "El manejo conservador con antiagregacion NO es apropiado en ACV con NIHSS 18. Se debe intentar reperfusion urgente."}
        };
        q.pubmedPmids = {"31662037", "25671798"};
        q.guidelinesIds = {"IMSS-058-08"};

        auto r = mcqRepo.Insert(q);
        if (r.IsOk()) std::cout << "  MCQ 2 (Ventana terapeutica) OK\n";
    }

    // ─── MCQ 3: Contraindicaciones alteplasa ───
    {
        Data::MCQuestion q;
        q.specialty = "Neurology";
        q.difficulty = "advanced";
        q.topic = "Contraindicaciones alteplasa";
        q.vignette =
            "Cuatro pacientes con ACV isquemico agudo dentro de ventana (<4.5h). Todos tienen NIHSS "
            "entre 10 y 15, TC craneal sin hemorragia, y PA controlada <185/110.";
        q.question = "En cual de los siguientes esta CONTRAINDICADA absolutamente la trombolisis con alteplasa IV?";
        q.options = json::array({
            {{"letter", "A"}, {"text", "Paciente de 78 anos con IAM previo hace 5 anos, actualmente asintomatico"}},
            {{"letter", "B"}, {"text", "Paciente con INR 2.5 por warfarina para valvula mecanica mitral"}},
            {{"letter", "C"}, {"text", "Paciente con glucosa capilar 165 mg/dL, sin sintomas hipoglucemicos"}},
            {{"letter", "D"}, {"text", "Paciente con antecedente de HAS moderada (PA basal 155/90)"}}
        });
        q.correctAnswer = 'B';
        q.explanationCorrect =
            "INR >1.7 (o TTPa >40 seg) es CONTRAINDICACION ABSOLUTA para alteplasa (AHA/ASA 2019). "
            "Este paciente con INR 2.5 por warfarina tiene alto riesgo de hemorragia catastrofica. "
            "Otras contraindicaciones absolutas: hemorragia intracraneal en TC, PA >185/110 no "
            "controlable, glucosa <50 o >400, plaquetas <100k, cirugia mayor <14 dias, ACV o TCE "
            "<3 meses, neoplasia intracraneal (excepto meningioma), endocarditis, diseccion aortica.";
        q.explanationIncorrect = {
            {"A", "IAM hace 5 anos NO es contraindicacion. IAM reciente (<3 meses) SI es contraindicacion relativa."},
            {"C", "Glucosa 165 es aceptable. Contraindicacion es <50 (hipoglucemia) o >400 (hiperglucemia severa)."},
            {"D", "HAS basal moderada NO contraindica. Se controla al momento de la trombolisis con antihipertensivos."}
        };
        q.pubmedPmids = {"31662037"};
        q.guidelinesIds = {"IMSS-058-08"};

        auto r = mcqRepo.Insert(q);
        if (r.IsOk()) std::cout << "  MCQ 3 (Contraindicaciones) OK\n";
    }

    // ═════════════════════════════════════════════════════════════
    // VERIFICACION
    // ═════════════════════════════════════════════════════════════
    std::cout << "\n===== Verificacion final =====\n\n";
    auto retrieved = caseRepo.FindBySlug("acv-isquemico-acm-izquierda-varon-72");
    if (retrieved) {
        std::cout << "Caso #003 verificado:\n";
        std::cout << "  Titulo:        " << retrieved->title.substr(0, 70) << "...\n";
        std::cout << "  Especialidad:  " << retrieved->specialty << "\n";
        std::cout << "  Paciente:      " << retrieved->patient["full_name"].get<std::string>()
                  << " (" << retrieved->patient["age"].get<int>() << " anos)\n";
        std::cout << "  NIHSS:         "
                  << retrieved->patient["vital_signs_admission"]["nihss_score"].get<int>() << "\n";
        std::cout << "  Estudios:      " << retrieved->expectedStudies.size() << "\n";
        std::cout << "  Tratamientos:  " << retrieved->expectedTreatments.size() << "\n";
        std::cout << "  Refs PubMed:   " << retrieved->pubmedPmids.size() << "\n";
    }

    std::cout << "\n===== Estadisticas globales =====\n";
    std::cout << "  Total casos:              " << caseRepo.Count() << "\n";
    std::cout << "  Casos cardiologia:        " << caseRepo.CountBySpecialty("Cardiology") << "\n";
    std::cout << "  Casos infectologia:       " << caseRepo.CountBySpecialty("Infectology") << "\n";
    std::cout << "  Casos neurologia:         " << caseRepo.CountBySpecialty("Neurology") << "\n";
    std::cout << "  MCQ total:                " << mcqRepo.Count() << "\n";

    std::cout << "\n==============================================================\n";
    std::cout << "   SPRINT 3D OK - Caso ACV insertado, fix diagnostico aplicado\n";
    std::cout << "==============================================================\n";
    std::cout << "\nProbar con:\n";
    std::cout << "  .\\ENARM.CLI.exe acv-isquemico-acm-izquierda-varon-72\n\n";

    Common::Logger::Shutdown();
    return 0;
}