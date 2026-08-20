// =====================================================================
//   ENARM.Assessment - RUBRICA CETOACIDOSIS DIABETICA PEDIATRICA (case_008)
//   Paciente: Sofia Torres Delgado, 15 anos, debut DM1
//   GPC IMSS-302-10
// =====================================================================
#include <enarm/assessment/AssessmentEngine.h>

namespace enarm::assessment {

CaseRubric AssessmentEngine::createCADRubric() {
    CaseRubric r;
    r.caseId         = "case_008_cad";
    r.caseName       = "Cetoacidosis Diabetica - Debut DM1";
    r.totalMaxPoints = 185;

    // ---------- Interrogatorio 40 pts ----------
    DimensionRubric iv;
    iv.dimension = CompetencyDimension::ClinicalInterview;
    iv.name      = "Interrogatorio Clinico";
    iv.maxPoints = 40;
    iv.criteria = {
        { "cad_hx_01","Triada clasica polis (poliuria, polidipsia, polifagia)",
          CompetencyDimension::ClinicalInterview, 12,
          "Explora las 4Ps (con perdida peso)","3 Ps","2 Ps","<2",
          {"poliuria","orinar mucho","polidipsia","sed","polifagia","hambre","perdida de peso"}, {}, 1.5f },
        { "cad_hx_02","Sintomas de acidosis (respiracion, dolor abd)",
          CompetencyDimension::ClinicalInterview, 8,
          "Kussmaul+dolor abd+nausea","2 de 3","1","Ninguno",
          {"respiracion rapida","kussmaul","dolor abdominal","nausea","vomito","aliento afrutado"} },
        { "cad_hx_03","Estado de conciencia y evolucion",
          CompetencyDimension::ClinicalInterview, 8,
          "Nivel conciencia+tiempo evolucion","Uno","MenciÃ³n","No pregunta",
          {"somnolencia","confusion","tiempo","evolucion","semanas"} },
        { "cad_hx_04","Antecedentes familiares DM",
          CompetencyDimension::ClinicalInterview, 7,
          "Explora DM1 y DM2 familiar","MenciÃ³n","Vago","No pregunta",
          {"diabetes familiar","dm1","dm2","abuela","tia","hermanos"} },
        { "cad_hx_05","Factores precipitantes (infeccion, estres)",
          CompetencyDimension::ClinicalInterview, 5,
          "Explora infeccion+estres","Uno","MenciÃ³n","No pregunta",
          {"infeccion","fiebre","estres","tos","disuria","dolor"} }
    };
    r.dimensions.push_back(iv);

    // ---------- Exploracion 25 pts ----------
    DimensionRubric pe;
    pe.dimension = CompetencyDimension::PhysicalExamination;
    pe.name      = "Exploracion Fisica";
    pe.maxPoints = 25;
    pe.criteria = {
        { "cad_pe_01","Grado de deshidratacion",
          CompetencyDimension::PhysicalExamination, 10,
          "Cuantifica %deshidratacion","Menciona deshidratacion","Vago","No evalua",
          {"deshidratacion","mucosas secas","ojos hundidos","turgencia","llenado capilar"}, {}, 1.5f },
        { "cad_pe_02","Respiracion de Kussmaul y aliento cetonico",
          CompetencyDimension::PhysicalExamination, 6,
          "Kussmaul+aliento afrutado","Uno","MenciÃ³n","No busca",
          {"kussmaul","respiracion rapida","profunda","aliento cetonico","afrutado"} },
        { "cad_pe_03","Estado mental (Glasgow pediatrico)",
          CompetencyDimension::PhysicalExamination, 5,
          "Glasgow pediatrico","Nivel conciencia","MenciÃ³n","No evalua",
          {"glasgow","estado mental","somnolencia","orientada","alerta"} },
        { "cad_pe_04","Signos vitales completos",
          CompetencyDimension::PhysicalExamination, 4,
          "5 SV con perfusion","4","3","<3",
          {"presion arterial","frecuencia cardiaca","frecuencia respiratoria","temperatura","perfusion"} }
    };
    r.dimensions.push_back(pe);

    // ---------- Estudios 30 pts ----------
    DimensionRubric st;
    st.dimension = CompetencyDimension::DiagnosticStudies;
    st.name      = "Estudios Paraclinicos";
    st.maxPoints = 30;
    st.criteria = {
        { "cad_st_01","Glucosa capilar y central",
          CompetencyDimension::DiagnosticStudies, 8,
          "Glucosa capilar inmediata + central","Solo una","MenciÃ³n","No solicita",
          {"glucosa","glucemia","dextrostix","glucosa capilar"}, {}, 1.5f },
        { "cad_st_02","Gasometria + brecha anionica",
          CompetencyDimension::DiagnosticStudies, 10,
          "Gaso venosa + brecha anionica","Gasometria","MenciÃ³n","No solicita",
          {"gasometria","gases","ph","bicarbonato","brecha anionica","anion gap"}, {}, 1.5f },
        { "cad_st_03","Electrolitos con K+ enfatizado",
          CompetencyDimension::DiagnosticStudies, 6,
          "Electrolitos completos con K","Electrolitos","MenciÃ³n","No solicita",
          {"electrolitos","potasio","sodio","cloro","fosforo","calcio"} },
        { "cad_st_04","Cetonas + HbA1c + anticuerpos DM1",
          CompetencyDimension::DiagnosticStudies, 6,
          "Cetonas+HbA1c+anti-GAD","2 de 3","1","Ninguno",
          {"cetonas","beta-hidroxibutirato","hba1c","hemoglobina glicosilada","anti-gad","anti-insulina"} }
    };
    r.dimensions.push_back(st);

    // ---------- Diagnostico 30 pts ----------
    DimensionRubric dg;
    dg.dimension = CompetencyDimension::ClinicalDiagnosis;
    dg.name      = "Diagnostico Clinico";
    dg.maxPoints = 30;
    dg.criteria = {
        { "cad_dg_01","Diagnostico de CAD (triada bioquimica)",
          CompetencyDimension::ClinicalDiagnosis, 15,
          "CAD con hipergluc+acidosis+cetosis","CAD","Descompensacion","Incorrecto",
          {"cetoacidosis","cad","descompensacion diabetica"}, {}, 3.0f },
        { "cad_dg_02","Clasificacion severidad + debut DM1",
          CompetencyDimension::ClinicalDiagnosis, 8,
          "Severidad + debut DM1","Severidad","MenciÃ³n","Nada",
          {"severa","moderada","debut","dm1","tipo 1","diagnostico nuevo"} },
        { "cad_dg_03","Diagnosticos diferenciales",
          CompetencyDimension::ClinicalDiagnosis, 7,
          "3+ DD","2","1","Ninguno",
          {"cetoacidosis alcoholica","acidosis lactica","salicilatos","abdomen agudo"} }
    };
    r.dimensions.push_back(dg);

    // ---------- Tratamiento 40 pts ----------
    DimensionRubric tx;
    tx.dimension = CompetencyDimension::TreatmentPlan;
    tx.name      = "Plan Terapeutico";
    tx.maxPoints = 40;
    tx.criteria = {
        { "cad_tx_01","Hidratacion en fases (fase 1: 10-20 ml/kg SF)",
          CompetencyDimension::TreatmentPlan, 12,
          "Fase 1 SF 10-20ml/kg en 1h","Hidratacion","Vago","No indica",
          {"solucion fisiologica","sf","0.9","10 ml/kg","20 ml/kg","hidratacion","primera hora"}, {}, 2.0f },
        { "cad_tx_02","Insulina en infusion (NO bolo en pediatria)",
          CompetencyDimension::TreatmentPlan, 12,
          "Infusion 0.05-0.1 UI/kg/h SIN bolo","Insulina infusion","Insulina con bolo","No indica",
          {"insulina","infusion","0.05","0.1","ui/kg/hora","sin bolo"}, {}, 2.5f },
        { "cad_tx_03","Reposicion de potasio",
          CompetencyDimension::TreatmentPlan, 8,
          "KCl 20-40 mEq/L cuando K<5.5","Potasio","MenciÃ³n","No indica",
          {"potasio","kcl","cloruro de potasio","reposicion","mEq"} },
        { "cad_tx_04","Prevencion de edema cerebral (descenso gradual)",
          CompetencyDimension::TreatmentPlan, 8,
          "Descenso gradual + dextrosa cuando gluc<250","Descenso gradual","MenciÃ³n","No previene",
          {"edema cerebral","descenso gradual","dextrosa","gradual","osmolaridad"}, {}, 1.5f }
    };
    r.dimensions.push_back(tx);

    // ---------- Seguimiento 20 pts ----------
    DimensionRubric fu;
    fu.dimension = CompetencyDimension::FollowUpMonitoring;
    fu.name      = "Seguimiento y Monitorizacion";
    fu.maxPoints = 20;
    fu.criteria = {
        { "cad_fu_01","Monitoreo bioquimico horario",
          CompetencyDimension::FollowUpMonitoring, 8,
          "Glucosa 1h + K/gaso 2-4h","Glucosa horaria","MenciÃ³n","No monitoriza",
          {"glucosa horaria","electrolitos","gasometria","monitoreo","cada hora","seriado"} },
        { "cad_fu_02","Vigilancia neurologica (edema cerebral)",
          CompetencyDimension::FollowUpMonitoring, 7,
          "Glasgow+cefalea+bradicardia seriados","2 de 3","1","No vigila",
          {"glasgow","cefalea","bradicardia","edema cerebral","neurologico"} },
        { "cad_fu_03","Transicion a insulina SC + educacion",
          CompetencyDimension::FollowUpMonitoring, 5,
          "Transicion SC + educacion diabetologica","Uno","MenciÃ³n","Nada",
          {"insulina subcutanea","sc","educacion diabetologica","conteo carbohidratos","padres"} }
    };
    r.dimensions.push_back(fu);

    r.expectedTimeMinutes = {
        {CompetencyDimension::ClinicalInterview,   6.0f},
        {CompetencyDimension::PhysicalExamination, 3.0f},
        {CompetencyDimension::DiagnosticStudies,   3.0f},
        {CompetencyDimension::ClinicalDiagnosis,   3.0f},
        {CompetencyDimension::TreatmentPlan,       6.0f},
        {CompetencyDimension::FollowUpMonitoring,  3.0f}
    };
    return r;
}

} // namespace enarm::assessment