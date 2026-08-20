// =====================================================================
//   ENARM.Assessment - RUBRICA NEUMONIA ADQUIRIDA EN COMUNIDAD (case_004)
//   Paciente: Ernesto Vazquez Morales, 74 anos, EPOC
//   GPC IMSS-234-09 / IMSS-037-08
// =====================================================================
#include <enarm/assessment/AssessmentEngine.h>

namespace enarm::assessment {

CaseRubric AssessmentEngine::createNeumoniaRubric() {
    CaseRubric r;
    r.caseId         = "case_004_neumonia";
    r.caseName       = "Neumonia Adquirida en Comunidad + EPOC";
    r.totalMaxPoints = 185;

    // ---------- Interrogatorio 40 pts ----------
    DimensionRubric iv;
    iv.dimension = CompetencyDimension::ClinicalInterview;
    iv.name      = "Interrogatorio Clinico";
    iv.maxPoints = 40;
    iv.criteria = {
        { "neu_hx_01","Semiologia de tos y expectoracion",
          CompetencyDimension::ClinicalInterview, 10,
          "Explora caracter, tiempo, color, cantidad","Mayoria","Algunos","No explora",
          {"tos","expectoracion","color de flema","tiempo de evolucion","amarilla","verde"}, {}, 1.5f },
        { "neu_hx_02","Sintomas respiratorios asociados",
          CompetencyDimension::ClinicalInterview, 8,
          "Explora todos","Mayoria","Algunos","Ninguno",
          {"disnea","dolor toracico","fiebre","escalofrios","dolor pleuritico"} },
        { "neu_hx_03","Antecedente de EPOC y comorbilidades",
          CompetencyDimension::ClinicalInterview, 10,
          "Identifica EPOC y otras","Identifica EPOC","MenciÃ³n","No identifica",
          {"epoc","enfermedad pulmonar","hipertension","diabetes","exacerbaciones previas"}, {}, 1.5f },
        { "neu_hx_04","Tabaquismo cuantificado",
          CompetencyDimension::ClinicalInterview, 7,
          "IPA calculado o cuantificado","Menciona tabaquismo","Vago","No pregunta",
          {"tabaquismo","paquetes","cigarros","fumador","ipa"} },
        { "neu_hx_05","Vacunacion (influenza/neumococo)",
          CompetencyDimension::ClinicalInterview, 5,
          "Pregunta ambas vacunas","Una vacuna","MenciÃ³n","No pregunta",
          {"vacuna","influenza","neumococo","inmunizacion"} }
    };
    r.dimensions.push_back(iv);

    // ---------- Exploracion 25 pts ----------
    DimensionRubric pe;
    pe.dimension = CompetencyDimension::PhysicalExamination;
    pe.name      = "Exploracion Fisica";
    pe.maxPoints = 25;
    pe.criteria = {
        { "neu_pe_01","Signos vitales + SpO2",
          CompetencyDimension::PhysicalExamination, 8,
          "5 SV con SpO2","4","3","<3",
          {"presion arterial","frecuencia cardiaca","temperatura","frecuencia respiratoria","saturacion","spo2"} },
        { "neu_pe_02","Exploracion pulmonar completa",
          CompetencyDimension::PhysicalExamination, 12,
          "Inspec+palpac+percus+ausc","3 de 4","2","1 o menos",
          {"auscultacion","estertores","crepitantes","sibilancias","matidez","percusion","murmullo vesicular"}, {}, 2.0f },
        { "neu_pe_03","Uso de musculos accesorios",
          CompetencyDimension::PhysicalExamination, 3,
          "Evalua trabajo respiratorio","MenciÃ³n","Vago","No evalua",
          {"musculos accesorios","tiraje","aleteo","trabajo respiratorio"} },
        { "neu_pe_04","Estado de conciencia y perfusion",
          CompetencyDimension::PhysicalExamination, 2,
          "Ambos","Uno","MenciÃ³n","Nada",
          {"consciente","orientado","cianosis","llenado capilar"} }
    };
    r.dimensions.push_back(pe);

    // ---------- Estudios 30 pts ----------
    DimensionRubric st;
    st.dimension = CompetencyDimension::DiagnosticStudies;
    st.name      = "Estudios Paraclinicos";
    st.maxPoints = 30;
    st.criteria = {
        { "neu_st_01","Radiografia de torax",
          CompetencyDimension::DiagnosticStudies, 12,
          "Rx PA y lateral","Rx","MenciÃ³n","No solicita",
          {"radiografia","rx","torax","tele de torax"}, {}, 2.0f },
        { "neu_st_02","Gasometria arterial",
          CompetencyDimension::DiagnosticStudies, 6,
          "Gasometria arterial","Gasometria","MenciÃ³n","No solicita",
          {"gasometria","gases arteriales","ph","pao2"} },
        { "neu_st_03","Laboratorios con PCR/procalcitonina",
          CompetencyDimension::DiagnosticStudies, 8,
          "BH+QS+PCR o procalcitonina","BH+QS","Solo uno","Ninguno",
          {"biometria hematica","quimica sanguinea","proteina c reactiva","procalcitonina"} },
        { "neu_st_04","Cultivos y panel viral",
          CompetencyDimension::DiagnosticStudies, 4,
          "Cultivo expec+hemocultivos+panel","2 de 3","1","Ninguno",
          {"cultivo","hemocultivos","expectoracion","panel viral"} }
    };
    r.dimensions.push_back(st);

    // ---------- Diagnostico 30 pts ----------
    DimensionRubric dg;
    dg.dimension = CompetencyDimension::ClinicalDiagnosis;
    dg.name      = "Diagnostico Clinico";
    dg.maxPoints = 30;
    dg.criteria = {
        { "neu_dg_01","Diagnostico de neumonia",
          CompetencyDimension::ClinicalDiagnosis, 15,
          "NAC + exacerbacion EPOC","Solo NAC","Neumonia vaga","Incorrecto",
          {"neumonia","nac","adquirida en la comunidad","exacerbacion"}, {}, 3.0f },
        { "neu_dg_02","Estratificacion CURB-65 o similar",
          CompetencyDimension::ClinicalDiagnosis, 8,
          "Aplica CURB-65 o PSI","Menciona escala","Vago","No estratifica",
          {"curb-65","psi","gravedad","severidad"} },
        { "neu_dg_03","Diagnosticos diferenciales",
          CompetencyDimension::ClinicalDiagnosis, 7,
          "3+ DD","2","1","Ninguno",
          {"tromboembolia","insuficiencia cardiaca","exacerbacion epoc","tuberculosis"} }
    };
    r.dimensions.push_back(dg);

    // ---------- Tratamiento 40 pts ----------
    DimensionRubric tx;
    tx.dimension = CompetencyDimension::TreatmentPlan;
    tx.name      = "Plan Terapeutico";
    tx.maxPoints = 40;
    tx.criteria = {
        { "neu_tx_01","Antibioticoterapia empirica",
          CompetencyDimension::TreatmentPlan, 15,
          "Betalactamico + macrolido","ATB apropiado","ATB inadecuado","Sin ATB",
          {"ceftriaxona","azitromicina","levofloxacino","antibiotico","betalactamico"}, {}, 2.0f },
        { "neu_tx_02","Oxigenoterapia controlada (EPOC)",
          CompetencyDimension::TreatmentPlan, 10,
          "O2 con meta SpO2 88-92%","O2 sin meta","MenciÃ³n","No indica",
          {"oxigeno","spo2","saturacion","puntas nasales","venturi"}, {}, 1.5f },
        { "neu_tx_03","Broncodilatadores y corticoide",
          CompetencyDimension::TreatmentPlan, 8,
          "Broncodil+corticoide sistemico","Uno","MenciÃ³n","Ninguno",
          {"salbutamol","ipratropio","broncodilatador","metilprednisolona","corticoide"} },
        { "neu_tx_04","Hidratacion y medidas generales",
          CompetencyDimension::TreatmentPlan, 7,
          "Hidratacion+antipiretico+profilaxis","Mayoria","Algunas","Pocas",
          {"hidratacion","liquidos","antipiretico","profilaxis"} }
    };
    r.dimensions.push_back(tx);

    // ---------- Seguimiento 20 pts ----------
    DimensionRubric fu;
    fu.dimension = CompetencyDimension::FollowUpMonitoring;
    fu.name      = "Seguimiento y Monitorizacion";
    fu.maxPoints = 20;
    fu.criteria = {
        { "neu_fu_01","Decision de hospitalizacion",
          CompetencyDimension::FollowUpMonitoring, 8,
          "Hospitaliza con criterios claros","Hospitaliza","MenciÃ³n","No decide",
          {"hospitalizacion","internamiento","uci","medicina interna"} },
        { "neu_fu_02","Monitorizacion respiratoria",
          CompetencyDimension::FollowUpMonitoring, 7,
          "SpO2 continua + FR + gasometria","2 de 3","1","Ninguno",
          {"oximetria","saturacion continua","frecuencia respiratoria","gasometria seriada"} },
        { "neu_fu_03","Criterios de mejoria y alta",
          CompetencyDimension::FollowUpMonitoring, 5,
          "Define criterios de alta","Los menciona","Vago","Nada",
          {"criterios de alta","estabilidad","afebril","tolerancia via oral"} }
    };
    r.dimensions.push_back(fu);

    r.expectedTimeMinutes = {
        {CompetencyDimension::ClinicalInterview,   8.0f},
        {CompetencyDimension::PhysicalExamination, 4.0f},
        {CompetencyDimension::DiagnosticStudies,   4.0f},
        {CompetencyDimension::ClinicalDiagnosis,   3.0f},
        {CompetencyDimension::TreatmentPlan,       5.0f},
        {CompetencyDimension::FollowUpMonitoring,  2.0f}
    };
    return r;
}

} // namespace enarm::assessment