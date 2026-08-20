// =====================================================================
//   ENARM.Assessment - RUBRICA BRONQUIOLITIS AGUDA (case_010)
//   Paciente: Emilio Ruiz Cortes, 6 meses (mama Maria responde)
//   GPC IMSS-032-08
// =====================================================================
#include <enarm/assessment/AssessmentEngine.h>

namespace enarm::assessment {

CaseRubric AssessmentEngine::createBronquiolitisRubric() {
    CaseRubric r;
    r.caseId         = "case_010_bronquiolitis";
    r.caseName       = "Bronquiolitis Aguda por VSR";
    r.totalMaxPoints = 185;

    // ---------- Interrogatorio 40 pts (a la MADRE) ----------
    DimensionRubric iv;
    iv.dimension = CompetencyDimension::ClinicalInterview;
    iv.name      = "Interrogatorio Clinico";
    iv.maxPoints = 40;
    iv.criteria = {
        { "br_hx_01","Sintomas respiratorios progresivos",
          CompetencyDimension::ClinicalInterview, 12,
          "Tos+dificultad+ruidos+tiempo","3 de 4","2","<2",
          {"tos","dificultad respiratoria","respirar rapido","sibilancias","ruiditos","rinorrea","mocos"}, {}, 1.5f },
        { "br_hx_02","Alimentacion y estado general",
          CompetencyDimension::ClinicalInterview, 10,
          "Rechazo alimento cuantificado+irritabilidad","Uno","MenciÃ³n","No pregunta",
          {"alimento","rechazo","no come","irritable","llanto","cansado"}, {}, 1.5f },
        { "br_hx_03","Contactos con enfermos (guarderia, hermanos)",
          CompetencyDimension::ClinicalInterview, 6,
          "Explora contactos+guarderia","Uno","MenciÃ³n","No pregunta",
          {"contactos","enfermos","hermano","guarderia","catarro","gripa"} },
        { "br_hx_04","Antecedentes perinatales y vacunas",
          CompetencyDimension::ClinicalInterview, 7,
          "Perinatal+vacunas+lactancia","2 de 3","1","Ninguno",
          {"embarazo","parto","termino","vacunas","lactancia","materna"} },
        { "br_hx_05","Sintomas de gravedad (apnea, cianosis)",
          CompetencyDimension::ClinicalInterview, 5,
          "Explora apnea+cianosis+letargia","2 de 3","1","No explora",
          {"apnea","dejo de respirar","cianosis","morado","letargia","no responde"} }
    };
    r.dimensions.push_back(iv);

    // ---------- Exploracion 25 pts ----------
    DimensionRubric pe;
    pe.dimension = CompetencyDimension::PhysicalExamination;
    pe.name      = "Exploracion Fisica";
    pe.maxPoints = 25;
    pe.criteria = {
        { "br_pe_01","Trabajo respiratorio (tiraje, aleteo, quejido)",
          CompetencyDimension::PhysicalExamination, 10,
          "Los 3 signos evaluados","2","1","Ninguno",
          {"tiraje","retracciones","aleteo nasal","quejido","trabajo respiratorio","intercostal","subcostal"}, {}, 1.5f },
        { "br_pe_02","Auscultacion pulmonar completa",
          CompetencyDimension::PhysicalExamination, 8,
          "Sibilancias+estertores+espiracion prolongada","2 de 3","1","No ausculta bien",
          {"sibilancias","estertores","crepitantes","espiracion prolongada","murmullo vesicular"}, {}, 1.5f },
        { "br_pe_03","Signos vitales + SpO2",
          CompetencyDimension::PhysicalExamination, 5,
          "5 SV con SpO2","4","3","<3",
          {"frecuencia cardiaca","frecuencia respiratoria","saturacion","spo2","temperatura","llenado capilar"} },
        { "br_pe_04","Estado neurologico e hidratacion",
          CompetencyDimension::PhysicalExamination, 2,
          "Alerta+fontanela+mucosas","2 de 3","1","Ninguno",
          {"fontanela","mucosas","hidratacion","alerta","irritable","hipotonia"} }
    };
    r.dimensions.push_back(pe);

    // ---------- Estudios 30 pts ----------
    DimensionRubric st;
    st.dimension = CompetencyDimension::DiagnosticStudies;
    st.name      = "Estudios Paraclinicos";
    st.maxPoints = 30;
    st.criteria = {
        { "br_st_01","Oximetria continua (NO estudios rutinarios)",
          CompetencyDimension::DiagnosticStudies, 10,
          "Oximetria continua + evita rutinarios","Oximetria","MenciÃ³n","No solicita",
          {"oximetria","saturacion continua","monitorizacion respiratoria"}, {}, 1.5f },
        { "br_st_02","Panel viral (VSR) si hospitaliza",
          CompetencyDimension::DiagnosticStudies, 8,
          "Panel viral con VSR+influenza","Panel viral","MenciÃ³n","No solicita",
          {"panel viral","vsr","virus sincitial","influenza","hisopado nasofaringeo","metapneumovirus"} },
        { "br_st_03","Rx torax solo si complicacion (evitar rutina)",
          CompetencyDimension::DiagnosticStudies, 6,
          "Reconoce Rx NO rutinaria","La solicita","La solicita innecesariamente","No considera",
          {"radiografia","rx","torax","no rutinaria","complicacion","atelectasia","condensacion"} },
        { "br_st_04","Gasometria si deterioro",
          CompetencyDimension::DiagnosticStudies, 6,
          "Gasometria si empeora","Gasometria","MenciÃ³n","No considera",
          {"gasometria","capilar","gases","empeoramiento","deterioro"} }
    };
    r.dimensions.push_back(st);

    // ---------- Diagnostico 30 pts ----------
    DimensionRubric dg;
    dg.dimension = CompetencyDimension::ClinicalDiagnosis;
    dg.name      = "Diagnostico Clinico";
    dg.maxPoints = 30;
    dg.criteria = {
        { "br_dg_01","Diagnostico clinico de bronquiolitis",
          CompetencyDimension::ClinicalDiagnosis, 15,
          "Bronquiolitis aguda con VSR probable","Bronquiolitis","IVRS baja","Incorrecto",
          {"bronquiolitis","bronquiolitis aguda","vsr","virus sincitial"}, {}, 3.0f },
        { "br_dg_02","Clasificacion severidad (Wood-Downes/Tal)",
          CompetencyDimension::ClinicalDiagnosis, 8,
          "Aplica Wood-Downes o Tal","Menciona escala","Vago","No aplica",
          {"wood-downes","wood downes","tal","escala","severidad","moderada","severa"} },
        { "br_dg_03","Diagnosticos diferenciales pediatricos",
          CompetencyDimension::ClinicalDiagnosis, 7,
          "3+ DD","2","1","Ninguno",
          {"neumonia","asma","cuerpo extrano","cardiopatia congenita","insuficiencia cardiaca"} }
    };
    r.dimensions.push_back(dg);

    // ---------- Tratamiento 40 pts (basado en soporte, EVITAR farmacos innecesarios) ----------
    DimensionRubric tx;
    tx.dimension = CompetencyDimension::TreatmentPlan;
    tx.name      = "Plan Terapeutico";
    tx.maxPoints = 40;
    tx.criteria = {
        { "br_tx_01","Oxigeno para SpO2 >92% (soporte principal)",
          CompetencyDimension::TreatmentPlan, 15,
          "O2 con meta SpO2 clara","O2","MenciÃ³n","No indica",
          {"oxigeno","puntas nasales","spo2","saturacion","92","suplementario"}, {}, 2.5f },
        { "br_tx_02","EVITAR broncodilatadores/corticoides/ATB de rutina",
          CompetencyDimension::TreatmentPlan, 15,
          "Reconoce que NO usar 3","2","1","Los prescribe",
          {"no broncodilatadores","no corticoides","no antibioticos","evitar","no de rutina","sin evidencia","salbutamol no"}, {}, 2.5f },
        { "br_tx_03","Aspiracion secreciones + posicion",
          CompetencyDimension::TreatmentPlan, 5,
          "Aspiracion+posicion+solucion salina","2 de 3","1","Nada",
          {"aspiracion","secreciones","solucion salina","posicion","semi-fowler","elevacion"} },
        { "br_tx_04","Hidratacion y alimentacion fraccionada",
          CompetencyDimension::TreatmentPlan, 5,
          "Hidratacion IV + fraccionar VO","Uno","MenciÃ³n","No indica",
          {"hidratacion","liquidos","fraccionar","alimentacion","via oral","iv"} }
    };
    r.dimensions.push_back(tx);

    // ---------- Seguimiento 20 pts ----------
    DimensionRubric fu;
    fu.dimension = CompetencyDimension::FollowUpMonitoring;
    fu.name      = "Seguimiento y Monitorizacion";
    fu.maxPoints = 20;
    fu.criteria = {
        { "br_fu_01","Criterios de hospitalizacion",
          CompetencyDimension::FollowUpMonitoring, 8,
          "Hospitaliza por SpO2<92 + edad <6m + trabajo respiratorio","2 de 3","1","No hospitaliza",
          {"hospitalizacion","internamiento","criterios","edad","saturacion menor","dificultad severa"}, {}, 1.5f },
        { "br_fu_02","Signos de alarma para UCI (apnea, agotamiento)",
          CompetencyDimension::FollowUpMonitoring, 7,
          "Apnea+agotamiento+hipoxemia refractaria","2 de 3","1","No menciona",
          {"apnea","agotamiento","fatiga respiratoria","uci","cuidados intensivos","hipoxemia refractaria"} },
        { "br_fu_03","Educacion a padres + signos alarma domicilio",
          CompetencyDimension::FollowUpMonitoring, 5,
          "Signos alarma+cuando regresar+lactancia","2 de 3","1","Nada",
          {"padres","educacion","signos de alarma","regresar","lactancia","aislamiento respiratorio"} }
    };
    r.dimensions.push_back(fu);

    r.expectedTimeMinutes = {
        {CompetencyDimension::ClinicalInterview,   6.0f},
        {CompetencyDimension::PhysicalExamination, 4.0f},
        {CompetencyDimension::DiagnosticStudies,   3.0f},
        {CompetencyDimension::ClinicalDiagnosis,   3.0f},
        {CompetencyDimension::TreatmentPlan,       4.0f},
        {CompetencyDimension::FollowUpMonitoring,  2.0f}
    };
    return r;
}

} // namespace enarm::assessment