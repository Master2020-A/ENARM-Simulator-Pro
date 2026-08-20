#include <enarm/assessment/AssessmentEngine.h>
#include <stdexcept>
#include <ctime>
#include <iomanip>
#include <sstream>

namespace enarm::assessment {

// ================================================================
// Constructor / reset
// ================================================================
AssessmentEngine::AssessmentEngine() { reset(); }

void AssessmentEngine::reset() {
    m_actions.clear();
    m_dimStartTime.clear();
    m_dimEndTime.clear();
    m_lastElapsed  = 0.0f;
    m_rubricLoaded = false;
}

// ================================================================
// Carga de rubrica
// ================================================================
void AssessmentEngine::loadRubric(const CaseRubric& rubric) {
    m_rubric       = rubric;
    m_rubricLoaded = true;
}

void AssessmentEngine::loadRubricForCase(const std::string& caseId) {
    // Sprint 7A original (3 casos)
    if (caseId.find("001") != std::string::npos ||
        caseId.find("iam") != std::string::npos)
        loadRubric(createIAMRubric());
    else if (caseId.find("002") != std::string::npos ||
             caseId.find("sepsis") != std::string::npos)
        loadRubric(createSepsisRubric());
    else if (caseId.find("003") != std::string::npos ||
             caseId.find("acv") != std::string::npos ||
             caseId.find("evc") != std::string::npos)
        loadRubric(createACVRubric());

    // Sprint 7A.2 (7 casos nuevos)
    else if (caseId.find("004") != std::string::npos ||
             caseId.find("neumonia") != std::string::npos)
        loadRubric(createNeumoniaRubric());
    else if (caseId.find("005") != std::string::npos ||
             caseId.find("preeclampsia") != std::string::npos)
        loadRubric(createPreeclampsiaRubric());
    else if (caseId.find("006") != std::string::npos ||
             caseId.find("apendicitis") != std::string::npos)
        loadRubric(createApendicitisRubric());
    else if (caseId.find("007") != std::string::npos ||
             caseId.find("tce") != std::string::npos ||
             caseId.find("trauma") != std::string::npos)
        loadRubric(createTCERubric());
    else if (caseId.find("008") != std::string::npos ||
             caseId.find("cad") != std::string::npos ||
             caseId.find("cetoacidosis") != std::string::npos)
        loadRubric(createCADRubric());
    else if (caseId.find("009") != std::string::npos ||
             caseId.find("lra") != std::string::npos ||
             caseId.find("renal") != std::string::npos)
        loadRubric(createLRARubric());
    else if (caseId.find("010") != std::string::npos ||
             caseId.find("bronquiolitis") != std::string::npos)
        loadRubric(createBronquiolitisRubric());
    else
        loadRubric(createIAMRubric());  // default fallback
}

// ================================================================
// Tracking de acciones
// ================================================================
void AssessmentEngine::recordAction(StudentAction action) {
    m_lastElapsed = action.timestampSeconds;

    // Registrar ventana de tiempo por dimension
    CompetencyDimension dim;
    switch (action.type) {
        case StudentAction::Type::ConversationTurn:   dim = CompetencyDimension::ClinicalInterview;   break;
        case StudentAction::Type::PhysicalExam:       dim = CompetencyDimension::PhysicalExamination; break;
        case StudentAction::Type::StudyOrdered:       dim = CompetencyDimension::DiagnosticStudies;   break;
        case StudentAction::Type::DiagnosisSubmitted: dim = CompetencyDimension::ClinicalDiagnosis;   break;
        case StudentAction::Type::TreatmentOrdered:   dim = CompetencyDimension::TreatmentPlan;       break;
        case StudentAction::Type::FollowUpAction:     dim = CompetencyDimension::FollowUpMonitoring;  break;
        default: dim = CompetencyDimension::ClinicalInterview; break;
    }

    if (m_dimStartTime.find(dim) == m_dimStartTime.end())
        m_dimStartTime[dim] = action.timestampSeconds;
    m_dimEndTime[dim] = action.timestampSeconds;

    m_actions.push_back(std::move(action));
}

float AssessmentEngine::timeForDimension(CompetencyDimension d) const {
    auto itStart = m_dimStartTime.find(d);
    auto itEnd   = m_dimEndTime.find(d);
    if (itStart == m_dimStartTime.end()) return 0.0f;
    return itEnd->second - itStart->second;
}

void AssessmentEngine::onConversationTurn(
    const std::string& studentMessage,
    const std::string& patientResponse,
    float elapsedSeconds)
{
    StudentAction a;
    a.type             = StudentAction::Type::ConversationTurn;
    a.primary          = studentMessage;
    a.secondary        = patientResponse;
    a.timestampSeconds = elapsedSeconds;
    recordAction(std::move(a));
}

void AssessmentEngine::onPhysicalExamRequested(
    const std::string& examName,
    const std::string& findings,
    float elapsedSeconds)
{
    StudentAction a;
    a.type             = StudentAction::Type::PhysicalExam;
    a.primary          = examName;
    a.secondary        = findings;
    a.timestampSeconds = (elapsedSeconds < 0) ? m_lastElapsed : elapsedSeconds;
    recordAction(std::move(a));
}

void AssessmentEngine::onStudyOrdered(
    const std::string& studyName,
    const std::string& results,
    float elapsedSeconds)
{
    StudentAction a;
    a.type             = StudentAction::Type::StudyOrdered;
    a.primary          = studyName;
    a.secondary        = results;
    a.timestampSeconds = (elapsedSeconds < 0) ? m_lastElapsed : elapsedSeconds;
    recordAction(std::move(a));
}

void AssessmentEngine::onDiagnosisSubmitted(
    const std::string& diagnosis,
    const std::string& justification,
    float elapsedSeconds)
{
    StudentAction a;
    a.type             = StudentAction::Type::DiagnosisSubmitted;
    a.primary          = diagnosis;
    a.secondary        = justification;
    a.timestampSeconds = (elapsedSeconds < 0) ? m_lastElapsed : elapsedSeconds;
    recordAction(std::move(a));
}

void AssessmentEngine::onTreatmentOrdered(
    const std::string& treatment,
    const std::string& dosage,
    const std::string& rationale,
    float elapsedSeconds)
{
    StudentAction a;
    a.type             = StudentAction::Type::TreatmentOrdered;
    a.primary          = treatment + " " + dosage;
    a.secondary        = rationale;
    a.timestampSeconds = (elapsedSeconds < 0) ? m_lastElapsed : elapsedSeconds;
    recordAction(std::move(a));
}

void AssessmentEngine::onFollowUpAction(
    const std::string& action,
    const std::string& rationale,
    float elapsedSeconds)
{
    StudentAction a;
    a.type             = StudentAction::Type::FollowUpAction;
    a.primary          = action;
    a.secondary        = rationale;
    a.timestampSeconds = (elapsedSeconds < 0) ? m_lastElapsed : elapsedSeconds;
    recordAction(std::move(a));
}

// ================================================================
// Score en tiempo real
// ================================================================
std::vector<DimensionScore> AssessmentEngine::getCurrentDimensionScores() const {
    std::vector<DimensionScore> scores;
    if (!m_rubricLoaded) return scores;

    for (const auto& dimRubric : m_rubric.dimensions) {
        float timeSecs = timeForDimension(dimRubric.dimension);
        DimensionScore ds = m_evaluator.evaluateDimension(dimRubric, m_actions, timeSecs);

        // Inyectar tiempo esperado
        auto it = m_rubric.expectedTimeMinutes.find(dimRubric.dimension);
        if (it != m_rubric.expectedTimeMinutes.end())
            ds.expectedTimeMinutes = it->second;

        scores.push_back(ds);
    }
    return scores;
}

float AssessmentEngine::getCurrentScorePercent() const {
    auto scores = getCurrentDimensionScores();
    int earned = 0, maxPts = 0;
    for (const auto& ds : scores) { earned += ds.pointsEarned; maxPts += ds.maxPoints; }
    return (maxPts > 0) ? (static_cast<float>(earned) / maxPts) * 100.0f : 0.0f;
}

int AssessmentEngine::getCurrentScorePoints() const {
    auto scores = getCurrentDimensionScores();
    int earned = 0;
    for (const auto& ds : scores) earned += ds.pointsEarned;
    return earned;
}

// ================================================================
// Reporte final
// ================================================================
PerformanceReport AssessmentEngine::generateReport(
    const std::string& sessionId,
    const std::string& studentId,
    float totalTimeSeconds)
{
    PerformanceReport report;
    report.sessionId    = sessionId;
    report.caseId       = m_rubric.caseId;
    report.studentId    = studentId;
    report.totalTimeMinutes = (totalTimeSeconds > 0)
        ? totalTimeSeconds / 60.0f
        : m_lastElapsed / 60.0f;

    report.dimensionScores = getCurrentDimensionScores();

    ScoreCalculator::calculate(report);
    ScoreCalculator::analyzePerformance(report);
    ScoreCalculator::generateRecommendations(report);
    ScoreCalculator::evaluateTime(report, m_rubric);

    // Timestamp
    auto t  = std::time(nullptr);
    auto tm = *std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%dT%H:%M:%S");
    report.completedAt = oss.str();

    return report;
}

// ================================================================
//  RUBRICA IAM CEST
// ================================================================
CaseRubric AssessmentEngine::createIAMRubric() {
    CaseRubric r;
    r.caseId          = "case_001_iam";
    r.caseName        = "IAM con Elevacion del ST";
    r.totalMaxPoints  = 185;

    // ---------- Interrogatorio 40 pts ----------
    DimensionRubric iv;
    iv.dimension  = CompetencyDimension::ClinicalInterview;
    iv.name       = "Interrogatorio Clinico";
    iv.maxPoints  = 40;
    iv.criteria = {
        { "iam_hx_01","Semiologia del dolor (OPQRST)",
          CompetencyDimension::ClinicalInterview, 12,
          "Exploro OPQRST completo","Exploro mayoria de caracteristicas",
          "Exploro algunas caracteristicas","No exploro semiologia del dolor",
          {"inicio del dolor","localizacion","irradiacion",
           "intensidad","caracter opresivo","duracion",
           "factores que lo empeoran","factores que lo alivian"},
          {}, 1.5f },
        { "iam_hx_02","Sintomas asociados",
          CompetencyDimension::ClinicalInterview, 8,
          "Exploro todos los sintomas","Exploro la mayoria",
          "Exploro algunos","No exploro sintomas asociados",
          {"disnea","diaforesis","nausea","vomito","mareo","palpitaciones"} },
        { "iam_hx_03","Factores de riesgo cardiovascular",
          CompetencyDimension::ClinicalInterview, 10,
          "Exploro todos los FRC","Exploro la mayoria",
          "Exploro algunos","No exploro FRC",
          {"hipertension","diabetes","tabaquismo",
           "dislipidemia","antecedentes familiares"} },
        { "iam_hx_04","Medicamentos y alergias",
          CompetencyDimension::ClinicalInterview, 5,
          "Pregunto medicamentos y alergias","Solo uno",
          "MenciÃ³n superficial","No pregunto",
          {"medicamentos","alergias"} },
        { "iam_hx_05","Contexto del inicio",
          CompetencyDimension::ClinicalInterview, 5,
          "Pregunto actividad y hora de inicio","Uno de los dos",
          "MenciÃ³n vaga","No pregunto",
          {"inicio","esfuerzo fisico"} }
    };
    r.dimensions.push_back(iv);

    // ---------- Exploracion 25 pts ----------
    DimensionRubric pe;
    pe.dimension = CompetencyDimension::PhysicalExamination;
    pe.name      = "Exploracion Fisica";
    pe.maxPoints = 25;
    pe.criteria = {
        { "iam_pe_01","Signos vitales completos",
          CompetencyDimension::PhysicalExamination, 8,
          "Solicito los 5 SV","Solicito 4","Solicito 3","Solicito < 3",
          {"presion arterial","frecuencia cardiaca",
           "frecuencia respiratoria","temperatura","saturacion de oxigeno"} },
        { "iam_pe_02","Exploracion cardiovascular",
          CompetencyDimension::PhysicalExamination, 10,
          "Exploro completo","Mayoria","Algunos","Casi nada",
          {"auscultacion cardiaca","ingurgitacion yugular","pulsos perifericos","llenado capilar"} },
        { "iam_pe_03","Exploracion pulmonar",
          CompetencyDimension::PhysicalExamination, 5,
          "Ausculto pulmones","MenciÃ³n","Superficial","No ausculto",
          {"auscultacion pulmonar"} },
        { "iam_pe_04","Estado general",
          CompetencyDimension::PhysicalExamination, 2,
          "Describo estado general","MenciÃ³n parcial","Muy vago","Nada",
          {"aspecto general","diaforesis"} }
    };
    r.dimensions.push_back(pe);

    // ---------- Estudios 30 pts ----------
    DimensionRubric st;
    st.dimension = CompetencyDimension::DiagnosticStudies;
    st.name      = "Estudios Paraclinicos";
    st.maxPoints = 30;
    st.criteria = {
        { "iam_st_01","ECG de 12 derivaciones",
          CompetencyDimension::DiagnosticStudies, 12,
          "Solicita ECG de inmediato","Lo solicita","Tarda en solicitarlo","No lo solicita",
          {"electrocardiograma"}, {}, 2.0f },
        { "iam_st_02","Biomarcadores cardiacos",
          CompetencyDimension::DiagnosticStudies, 8,
          "Solicita troponinas","Las solicita","MenciÃ³n","No solicita",
          {"troponina"} },
        { "iam_st_03","Laboratorios generales",
          CompetencyDimension::DiagnosticStudies, 6,
          "Solicita panel completo","Mayoria","Algunos","Pocos",
          {"biometria hematica","quimica sanguinea","electrolitos"} },
        { "iam_st_04","Radiografia de torax",
          CompetencyDimension::DiagnosticStudies, 4,
          "Solicita Rx","La menciona","Vago","No solicita",
          {"radiografia de torax"} }
    };
    r.dimensions.push_back(st);

    // ---------- Diagnostico 30 pts ----------
    DimensionRubric dg;
    dg.dimension = CompetencyDimension::ClinicalDiagnosis;
    dg.name      = "Diagnostico Clinico";
    dg.maxPoints = 30;
    dg.criteria = {
        { "iam_dg_01","Diagnostico principal correcto",
          CompetencyDimension::ClinicalDiagnosis, 15,
          "Dx correcto con clasificacion","Dx correcto","Dx parcial","Dx incorrecto",
          {"infarto agudo de miocardio"}, {}, 3.0f },
        { "iam_dg_02","Clasificacion/localizacion",
          CompetencyDimension::ClinicalDiagnosis, 8,
          "Clasifica Killip y localiza","Uno de los dos","MenciÃ³n","Nada",
          {"cara anterior","cara inferior","Killip","TIMI"} },
        { "iam_dg_03","Diagnosticos diferenciales",
          CompetencyDimension::ClinicalDiagnosis, 7,
          "3+ DD relevantes","2 DD","1 DD","Sin DD",
          {"diseccion aortica","tromboembolismo pulmonar","pericarditis"} }
    };
    r.dimensions.push_back(dg);

    // ---------- Tratamiento 40 pts ----------
    DimensionRubric tx;
    tx.dimension = CompetencyDimension::TreatmentPlan;
    tx.name      = "Plan Terapeutico";
    tx.maxPoints = 40;
    tx.criteria = {
        { "iam_tx_01","AntitrombÃ³ticos",
          CompetencyDimension::TreatmentPlan, 15,
          "Doble antiagregacion + anticoagulacion","2 de 3","1 de 3","Ninguno",
          {"aspirina","clopidogrel","heparina"}, {}, 2.0f },
        { "iam_tx_02","Reperfusion",
          CompetencyDimension::TreatmentPlan, 12,
          "Indica PCI o fibrinolÃ­tico con tiempo","Indica reperfusion","MenciÃ³n vaga","No indica",
          {"reperfusion"}, {}, 2.5f },
        { "iam_tx_03","Medidas generales",
          CompetencyDimension::TreatmentPlan, 8,
          "Oxigeno+monitor+via IV+reposo","Mayoria","Algunas","Pocas",
          {"oxigeno","monitor","reposo"} },
        { "iam_tx_04","Prevencion secundaria",
          CompetencyDimension::TreatmentPlan, 5,
          "Beta+IECA+estatina","2 de 3","1","Ninguno",
          {"betabloqueador","estatina"} }
    };
    r.dimensions.push_back(tx);

    // ---------- Seguimiento 20 pts ----------
    DimensionRubric fu;
    fu.dimension = CompetencyDimension::FollowUpMonitoring;
    fu.name      = "Seguimiento y Monitorizacion";
    fu.maxPoints = 20;
    fu.criteria = {
        { "iam_fu_01","Monitorizacion hemodinamica",
          CompetencyDimension::FollowUpMonitoring, 8,
          "Monitor+telemetria+SV seriados","2 de 3","1","Ninguno",
          {"monitor cardiaco","telemetria","signos vitales seriados"} },
        { "iam_fu_02","Complicaciones a vigilar",
          CompetencyDimension::FollowUpMonitoring, 7,
          "Menciona 3+ complicaciones","2","1","Ninguna",
          {"arritmias","insuficiencia cardiaca","choque cardiogenico"} },
        { "iam_fu_03","Plan de alta",
          CompetencyDimension::FollowUpMonitoring, 5,
          "Rehabilitacion+FRC+seguimiento","2 de 3","1","Nada",
          {"rehabilitacion cardiaca","seguimiento por cardiologia"} }
    };
    r.dimensions.push_back(fu);

    r.expectedTimeMinutes = {
        {CompetencyDimension::ClinicalInterview,   8.0f},
        {CompetencyDimension::PhysicalExamination, 3.0f},
        {CompetencyDimension::DiagnosticStudies,   4.0f},
        {CompetencyDimension::ClinicalDiagnosis,   3.0f},
        {CompetencyDimension::TreatmentPlan,       5.0f},
        {CompetencyDimension::FollowUpMonitoring,  2.0f}
    };
    return r;
}

// ================================================================
//  RUBRICA SEPSIS / SHOCK SEPTICO
// ================================================================
CaseRubric AssessmentEngine::createSepsisRubric() {
    CaseRubric r;
    r.caseId         = "case_002_sepsis";
    r.caseName       = "Shock Septico";
    r.totalMaxPoints = 185;

    DimensionRubric iv;
    iv.dimension = CompetencyDimension::ClinicalInterview;
    iv.name      = "Interrogatorio Clinico";
    iv.maxPoints = 40;
    iv.criteria = {
        { "sep_hx_01","Sintomas de infeccion",
          CompetencyDimension::ClinicalInterview, 12,
          "Explora todos","Mayoria","Algunos","Ninguno",
          {"fiebre","escalofrios","malestar general","dolor","origen del foco"} },
        { "sep_hx_02","Foco infeccioso",
          CompetencyDimension::ClinicalInterview, 10,
          "Identifica foco","Sospecha","MenciÃ³n","Nada",
          {"tos","disuria","dolor abdominal","herida","respiratorio","urinario"} },
        { "sep_hx_03","Antecedentes relevantes",
          CompetencyDimension::ClinicalInterview, 10,
          "Completo","Mayoria","Algunos","Ninguno",
          {"diabetes","inmunosupresion","hospitalizacion previa","antibioticos previos"} },
        { "sep_hx_04","Medicamentos y alergias",
          CompetencyDimension::ClinicalInterview, 5,
          "Ambos","Solo uno","MenciÃ³n","Nada",
          {"medicamentos","alergias"} },
        { "sep_hx_05","Tiempo de evolucion",
          CompetencyDimension::ClinicalInterview, 3,
          "Preciso","Aproximado","Vago","No pregunta",
          {"inicio","desde cuando","cuanto tiempo"} }
    };
    r.dimensions.push_back(iv);

    DimensionRubric pe;
    pe.dimension = CompetencyDimension::PhysicalExamination;
    pe.name      = "Exploracion Fisica";
    pe.maxPoints = 25;
    pe.criteria = {
        { "sep_pe_01","Signos vitales + temperatura",
          CompetencyDimension::PhysicalExamination, 8,
          "Los 5 SV","4","3","<3",
          {"presion arterial","frecuencia cardiaca","temperatura","frecuencia respiratoria","saturacion de oxigeno"} },
        { "sep_pe_02","Estado mental y perfusion",
          CompetencyDimension::PhysicalExamination, 8,
          "Glasgow+perfusion periferica","Uno","MenciÃ³n","Ninguno",
          {"estado mental","glasgow","llenado capilar","marmoleo"} },
        { "sep_pe_03","Busqueda del foco",
          CompetencyDimension::PhysicalExamination, 7,
          "Explora 3+ areas","2","1","Ninguna",
          {"auscultacion pulmonar","abdomen","piel","accesos vasculares"} },
        { "sep_pe_04","Diuresis",
          CompetencyDimension::PhysicalExamination, 2,
          "Menciona diuresis","MenciÃ³n","Vago","No menciona",
          {"diuresis","orina"} }
    };
    r.dimensions.push_back(pe);

    DimensionRubric st;
    st.dimension = CompetencyDimension::DiagnosticStudies;
    st.name      = "Estudios Paraclinicos";
    st.maxPoints = 30;
    st.criteria = {
        { "sep_st_01","Hemocultivos antes de ATB",
          CompetencyDimension::DiagnosticStudies, 10,
          "Solicita antes ATB","Los solicita","Orden erroneo","No solicita",
          {"hemocultivos"}, {}, 2.0f },
        { "sep_st_02","Lactato serico",
          CompetencyDimension::DiagnosticStudies, 8,
          "Solicita lactato","Lo menciona","Vago","No solicita",
          {"lactato"}, {}, 1.5f },
        { "sep_st_03","Laboratorios de disfuncion organica",
          CompetencyDimension::DiagnosticStudies, 8,
          "Solicita panel completo","Mayoria","Algunos","Pocos",
          {"biometria hematica","quimica sanguinea","gasometria","procalcitonina"} },
        { "sep_st_04","Cultivos del foco sospechado",
          CompetencyDimension::DiagnosticStudies, 4,
          "Cultivos especificos","Los menciona","Vago","No solicita",
          {"urocultivo","cultivo de liquido","lavado broncoalveolar"} }
    };
    r.dimensions.push_back(st);

    DimensionRubric dg;
    dg.dimension = CompetencyDimension::ClinicalDiagnosis;
    dg.name      = "Diagnostico Clinico";
    dg.maxPoints = 30;
    dg.criteria = {
        { "sep_dg_01","Diagnostico de shock septico",
          CompetencyDimension::ClinicalDiagnosis, 15,
          "Dx correcto con criterios Sepsis-3","Dx correcto","Dx parcial","Incorrecto",
          {"shock septico"}, {}, 3.0f },
        { "sep_dg_02","Identificacion del foco",
          CompetencyDimension::ClinicalDiagnosis, 8,
          "Identifica foco y agente probable","Foco solo","Agente solo","Ninguno",
          {"neumonia","urinario","abdominal","bacteriemia"} },
        { "sep_dg_03","Diagnosticos diferenciales",
          CompetencyDimension::ClinicalDiagnosis, 7,
          "3+ DD","2","1","Ninguno",
          {"shock hipovolemico","shock cardiogenico","insuficiencia suprarrenal"} }
    };
    r.dimensions.push_back(dg);

    DimensionRubric tx;
    tx.dimension = CompetencyDimension::TreatmentPlan;
    tx.name      = "Plan Terapeutico";
    tx.maxPoints = 40;
    tx.criteria = {
        { "sep_tx_01","Antibiotico empirico < 1 hora",
          CompetencyDimension::TreatmentPlan, 12,
          "ATB correcto < 1h","ATB correcto","ATB tarde o incorrecto","Sin ATB",
          {"antibiotico"}, {}, 2.0f },
        { "sep_tx_02","Resucitacion con liquidos",
          CompetencyDimension::TreatmentPlan, 10,
          "30ml/kg cristaloides con reevaluacion","Liquidos sin reevaluacion","Liquidos vagos","Sin liquidos",
          {"solucion","cristaloides","liquidos iv","resucitacion"}, {}, 1.5f },
        { "sep_tx_03","Vasopresores si hipotension persiste",
          CompetencyDimension::TreatmentPlan, 10,
          "Norepinefrina con objetivo MAP>65","Vasopresor sin objetivo","Menciona vasopresores","Ninguno",
          {"norepinefrina","vasopresor","presion arterial media"}, {}, 1.5f },
        { "sep_tx_04","Control del foco y soporte",
          CompetencyDimension::TreatmentPlan, 8,
          "Control foco+soporte organico","Uno de dos","MenciÃ³n","Ninguno",
          {"control del foco","oxigeno","monitorizacion"} }
    };
    r.dimensions.push_back(tx);

    DimensionRubric fu;
    fu.dimension = CompetencyDimension::FollowUpMonitoring;
    fu.name      = "Seguimiento y Monitorizacion";
    fu.maxPoints = 20;
    fu.criteria = {
        { "sep_fu_01","Metas de resucitacion",
          CompetencyDimension::FollowUpMonitoring, 8,
          "MAP>65+diuresis>0.5+lactato<2","2 de 3","1","Ninguna",
          {"presion arterial media","diuresis","lactato"} },
        { "sep_fu_02","Monitorizacion de disfuncion organica",
          CompetencyDimension::FollowUpMonitoring, 7,
          "Monitoriza 3+ organos","2","1","Ninguno",
          {"funcion renal","funcion hepatica","coagulacion","respiratorio"} },
        { "sep_fu_03","Reevaluacion y ajuste",
          CompetencyDimension::FollowUpMonitoring, 5,
          "Reevalua y ajusta plan","Reevalua sin ajuste","MenciÃ³n","Nada",
          {"reevaluacion","resultado de cultivos","desescalada"} }
    };
    r.dimensions.push_back(fu);

    r.expectedTimeMinutes = {
        {CompetencyDimension::ClinicalInterview,   8.0f},
        {CompetencyDimension::PhysicalExamination, 3.0f},
        {CompetencyDimension::DiagnosticStudies,   4.0f},
        {CompetencyDimension::ClinicalDiagnosis,   3.0f},
        {CompetencyDimension::TreatmentPlan,       5.0f},
        {CompetencyDimension::FollowUpMonitoring,  2.0f}
    };
    return r;
}

// ================================================================
//  RUBRICA ACV ISQUEMICO
// ================================================================
CaseRubric AssessmentEngine::createACVRubric() {
    CaseRubric r;
    r.caseId         = "case_003_acv";
    r.caseName       = "ACV Isquemico";
    r.totalMaxPoints = 185;

    DimensionRubric iv;
    iv.dimension = CompetencyDimension::ClinicalInterview;
    iv.name      = "Interrogatorio Clinico";
    iv.maxPoints = 40;
    iv.criteria = {
        { "acv_hx_01","Sintomas neurologicos focales",
          CompetencyDimension::ClinicalInterview, 12,
          "Explora todos los deficits focales","Mayoria","Algunos","Ninguno",
          {"debilidad","paralisis","habla","vision","coordinacion","sensibilidad"} },
        { "acv_hx_02","Tiempo de inicio PRECISO",
          CompetencyDimension::ClinicalInterview, 12,
          "Obtiene hora exacta de inicio o ultima vez bien","Hora aproximada","Vago","No pregunta",
          {"inicio","ultima vez bien","cuando empezo","hora exacta"}, {}, 2.0f },
        { "acv_hx_03","Factores de riesgo cerebrovasculares",
          CompetencyDimension::ClinicalInterview, 8,
          "Explora todos los FRC","Mayoria","Algunos","Ninguno",
          {"hipertension","diabetes","fibrilacion auricular","tabaquismo","ACV previo"} },
        { "acv_hx_04","Contraindicaciones para trombolisis",
          CompetencyDimension::ClinicalInterview, 5,
          "Explora todas las CI","Mayoria","Algunas","Ninguna",
          {"cirugia reciente","sangrado reciente","anticoagulantes","glucosa","trauma"} },
        { "acv_hx_05","Medicamentos actuales",
          CompetencyDimension::ClinicalInterview, 3,
          "Pregunta medicamentos","Lo menciona","Vago","No pregunta",
          {"medicamentos","anticoagulantes"} }
    };
    r.dimensions.push_back(iv);

    DimensionRubric pe;
    pe.dimension = CompetencyDimension::PhysicalExamination;
    pe.name      = "Exploracion Fisica";
    pe.maxPoints = 25;
    pe.criteria = {
        { "acv_pe_01","Signos vitales + glucemia",
          CompetencyDimension::PhysicalExamination, 6,
          "SV completos + glucemia","SV sin glucemia","Parcial","Minimo",
          {"presion arterial","frecuencia cardiaca","glucosa","temperatura"} },
        { "acv_pe_02","Exploracion neurologica NIHSS",
          CompetencyDimension::PhysicalExamination, 12,
          "NIHSS completo o equivalente","Exploracion parcial","Minima","Ninguna",
          {"nivel de conciencia","lenguaje","fuerza","cara","vision","sensibilidad"}, {}, 2.0f },
        { "acv_pe_03","Signos meningeos",
          CompetencyDimension::PhysicalExamination, 4,
          "Busca signos meningeos","Los menciona","Vago","No busca",
          {"rigidez de nuca","kernig","brudzinski"} },
        { "acv_pe_04","Exploracion cardiaca",
          CompetencyDimension::PhysicalExamination, 3,
          "Ausculta y busca FA","Solo ausculta","MenciÃ³n","Nada",
          {"auscultacion cardiaca","fibrilacion auricular"} }
    };
    r.dimensions.push_back(pe);

    DimensionRubric st;
    st.dimension = CompetencyDimension::DiagnosticStudies;
    st.name      = "Estudios Paraclinicos";
    st.maxPoints = 30;
    st.criteria = {
        { "acv_st_01","TAC de craneo SIN contraste URGENTE",
          CompetencyDimension::DiagnosticStudies, 12,
          "TAC urgente sin contraste","La solicita","Tarda en solicitarla","No la solicita",
          {"tomografia","tac","ct","tomografia de craneo"}, {}, 2.5f },
        { "acv_st_02","Glucemia capilar urgente",
          CompetencyDimension::DiagnosticStudies, 6,
          "Solicita glucemia inmediata","La solicita","Tarde","No solicita",
          {"glucosa","glucemia","dextrostix"}, {}, 1.5f },
        { "acv_st_03","Laboratorios para trombolisis",
          CompetencyDimension::DiagnosticStudies, 7,
          "Panel completo pre-trombolisis","Mayoria","Algunos","Pocos",
          {"biometria hematica","tiempos de coagulacion","quimica sanguinea"} },
        { "acv_st_04","ECG",
          CompetencyDimension::DiagnosticStudies, 5,
          "Solicita ECG para buscar FA","Lo solicita","MenciÃ³n","No solicita",
          {"electrocardiograma"} }
    };
    r.dimensions.push_back(st);

    DimensionRubric dg;
    dg.dimension = CompetencyDimension::ClinicalDiagnosis;
    dg.name      = "Diagnostico Clinico";
    dg.maxPoints = 30;
    dg.criteria = {
        { "acv_dg_01","Diagnostico de ACV isquemico",
          CompetencyDimension::ClinicalDiagnosis, 15,
          "ACV isquemico con territorio y NIHSS","ACV isquemico","Sindrome neurologico focal","Incorrecto",
          {"acv isquemico"}, {}, 3.0f },
        { "acv_dg_02","Territorio vascular",
          CompetencyDimension::ClinicalDiagnosis, 8,
          "Identifica territorio (ACM/ACA/ACP)","Lo intenta","MenciÃ³n","Nada",
          {"arteria cerebral media","territorio","isquemia"} },
        { "acv_dg_03","Diagnosticos diferenciales",
          CompetencyDimension::ClinicalDiagnosis, 7,
          "3+ DD relevantes","2","1","Ninguno",
          {"hemorragia cerebral","hipoglucemia","encefalitis","tumor"} }
    };
    r.dimensions.push_back(dg);

    DimensionRubric tx;
    tx.dimension = CompetencyDimension::TreatmentPlan;
    tx.name      = "Plan Terapeutico";
    tx.maxPoints = 40;
    tx.criteria = {
        { "acv_tx_01","Trombolisis IV si elegible",
          CompetencyDimension::TreatmentPlan, 15,
          "Alteplase 0.9mg/kg en ventana con protocolo","Indica trombolisis","MenciÃ³n","No indica",
          {"alteplase","trombolisis","trombolitico"}, {}, 2.5f },
        { "acv_tx_02","Manejo PA segun elegibilidad",
          CompetencyDimension::TreatmentPlan, 8,
          "PA < 185/110 pre-trombolisis con agente correcto","Objetivo de PA","MenciÃ³n","Nada",
          {"presion arterial","labetalol","nicardipino"} },
        { "acv_tx_03","Medidas generales neuroproteccion",
          CompetencyDimension::TreatmentPlan, 10,
          "Oxigeno+glucosa+T+cabecera+NPO","Mayoria","Algunas","Pocas",
          {"oxigeno","glucosa","temperatura","cabecera elevada","npo"} },
        { "acv_tx_04","Antitromboticos y prevencion secundaria",
          CompetencyDimension::TreatmentPlan, 7,
          "Antiagregante 24h post-trombolisis o inmediato","Lo indica","MenciÃ³n","Nada",
          {"aspirina","antiagregante","estatina"} }
    };
    r.dimensions.push_back(tx);

    DimensionRubric fu;
    fu.dimension = CompetencyDimension::FollowUpMonitoring;
    fu.name      = "Seguimiento y Monitorizacion";
    fu.maxPoints = 20;
    fu.criteria = {
        { "acv_fu_01","Monitorizacion neurologica post-trombolisis",
          CompetencyDimension::FollowUpMonitoring, 8,
          "NIHSS seriado + signos de sangrado","NIHSS solo","MenciÃ³n","Ninguno",
          {"escala neurologica","NIHSS","deterioro neurologico","sangrado"} },
        { "acv_fu_02","Control de PA y glucemia",
          CompetencyDimension::FollowUpMonitoring, 7,
          "Monitoriza PA+glucemia con metas","PA o glucemia","MenciÃ³n","Ninguno",
          {"presion arterial","glucosa","glicemia"} },
        { "acv_fu_03","Rehabilitacion y prevencion secundaria",
          CompetencyDimension::FollowUpMonitoring, 5,
          "Rehabilitacion+etiologia+FRC","2 de 3","1","Ninguno",
          {"rehabilitacion","holter","ecocardiograma","etiologia"} }
    };
    r.dimensions.push_back(fu);

    r.expectedTimeMinutes = {
        {CompetencyDimension::ClinicalInterview,   8.0f},
        {CompetencyDimension::PhysicalExamination, 3.0f},
        {CompetencyDimension::DiagnosticStudies,   4.0f},
        {CompetencyDimension::ClinicalDiagnosis,   3.0f},
        {CompetencyDimension::TreatmentPlan,       5.0f},
        {CompetencyDimension::FollowUpMonitoring,  2.0f}
    };
    return r;
}

} // namespace enarm::assessment

