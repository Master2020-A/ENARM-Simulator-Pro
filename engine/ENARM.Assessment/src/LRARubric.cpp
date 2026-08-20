// =====================================================================
//   ENARM.Assessment - RUBRICA LRA POR AINEs (case_009)
//   Paciente: Guillermo Salinas Ochoa, 71 anos, HTA + DM2
//   GPC IMSS-395-10
// =====================================================================
#include <enarm/assessment/AssessmentEngine.h>

namespace enarm::assessment {

CaseRubric AssessmentEngine::createLRARubric() {
    CaseRubric r;
    r.caseId         = "case_009_lra";
    r.caseName       = "Lesion Renal Aguda por AINEs";
    r.totalMaxPoints = 185;

    // ---------- Interrogatorio 40 pts ----------
    DimensionRubric iv;
    iv.dimension = CompetencyDimension::ClinicalInterview;
    iv.name      = "Interrogatorio Clinico";
    iv.maxPoints = 40;
    iv.criteria = {
        { "lra_hx_01","Cuantificacion de diuresis y edema",
          CompetencyDimension::ClinicalInterview, 10,
          "Cuantifica uresis + edema","Menciona ambos","Uno","No cuantifica",
          {"orinar","volumen urinario","oliguria","anuria","edema","hinchazon","piernas","ml"} },
        { "lra_hx_02","Medicamentos actuales (INCLUIR automedicacion)",
          CompetencyDimension::ClinicalInterview, 15,
          "Todos + automedicacion (AINEs)","Prescritos + pregunta OTC","Solo prescritos","No pregunta bien",
          {"medicamentos","enalapril","metformina","ibuprofeno","aines","automedicacion","otros","por su cuenta","sin receta"}, {}, 2.5f },
        { "lra_hx_03","Antecedentes de HAS, DM y cronicas",
          CompetencyDimension::ClinicalInterview, 8,
          "HAS+DM+dislipidemia+tiempo","3 de 4","2","<2",
          {"hipertension","presion","diabetes","azucar","dislipidemia","colesterol"} },
        { "lra_hx_04","Sintomas urinarios y de uremia",
          CompetencyDimension::ClinicalInterview, 4,
          "Explora sintomas uremicos+urinarios","Uno","MenciÃ³n","No explora",
          {"nausea","vomito","fatiga","hematuria","disuria","frecuencia"} },
        { "lra_hx_05","Exposicion a nefrotoxicos (contrastes, ATB)",
          CompetencyDimension::ClinicalInterview, 3,
          "Explora contrastes+aminoglucosidos","Uno","MenciÃ³n","No pregunta",
          {"contraste","tomografia","aminoglucosidos","gentamicina","nefrotoxicos"} }
    };
    r.dimensions.push_back(iv);

    // ---------- Exploracion 25 pts ----------
    DimensionRubric pe;
    pe.dimension = CompetencyDimension::PhysicalExamination;
    pe.name      = "Exploracion Fisica";
    pe.maxPoints = 25;
    pe.criteria = {
        { "lra_pe_01","Estado de volumen (edema, IY, TA)",
          CompetencyDimension::PhysicalExamination, 10,
          "Edema+ingurgitacion+TA","2 de 3","1","Ninguno",
          {"edema","ingurgitacion yugular","iy","presion arterial","volumen","hipervolemia"}, {}, 1.5f },
        { "lra_pe_02","Auscultacion cardiopulmonar (sobrecarga)",
          CompetencyDimension::PhysicalExamination, 8,
          "S3+estertores basales+edema","2 de 3","1","Ninguno",
          {"s3","tercer ruido","estertores","crepitantes","edema agudo pulmonar"} },
        { "lra_pe_03","Signos vitales completos",
          CompetencyDimension::PhysicalExamination, 4,
          "5 SV","4","3","<3",
          {"presion arterial","frecuencia cardiaca","frecuencia respiratoria","temperatura","saturacion"} },
        { "lra_pe_04","Puno percusion y globo vesical",
          CompetencyDimension::PhysicalExamination, 3,
          "Puno percusion+globo vesical","Uno","MenciÃ³n","No evalua",
          {"puno percusion","giordano","globo vesical","obstruccion"} }
    };
    r.dimensions.push_back(pe);

    // ---------- Estudios 30 pts ----------
    DimensionRubric st;
    st.dimension = CompetencyDimension::DiagnosticStudies;
    st.name      = "Estudios Paraclinicos";
    st.maxPoints = 30;
    st.criteria = {
        { "lra_st_01","Creatinina + urea seriadas",
          CompetencyDimension::DiagnosticStudies, 10,
          "Cr+urea+comparacion basal","Cr+urea","Solo Cr","No solicita",
          {"creatinina","urea","bun","funcion renal","basal","previa"}, {}, 1.5f },
        { "lra_st_02","Electrolitos (K+ prioritario)",
          CompetencyDimension::DiagnosticStudies, 8,
          "Electrolitos+ECG por K","Electrolitos+K","MenciÃ³n","No solicita",
          {"electrolitos","potasio","hiperkalemia","sodio","electrocardiograma","ecg"}, {}, 1.5f },
        { "lra_st_03","EGO + sedimento + indices urinarios",
          CompetencyDimension::DiagnosticStudies, 6,
          "EGO+sedimento+FeNa","2 de 3","1","Ninguno",
          {"examen general de orina","ego","sedimento","fena","fraccion excretada","sodio urinario","eosinofiluria"} },
        { "lra_st_04","USG renal + gasometria",
          CompetencyDimension::DiagnosticStudies, 6,
          "USG renal+gaso arterial","Uno","MenciÃ³n","Ninguno",
          {"ultrasonido renal","usg renal","gasometria","acidosis metabolica","hidronefrosis"} }
    };
    r.dimensions.push_back(st);

    // ---------- Diagnostico 30 pts ----------
    DimensionRubric dg;
    dg.dimension = CompetencyDimension::ClinicalDiagnosis;
    dg.name      = "Diagnostico Clinico";
    dg.maxPoints = 30;
    dg.criteria = {
        { "lra_dg_01","Diagnostico de LRA con clasificacion",
          CompetencyDimension::ClinicalDiagnosis, 12,
          "LRA con AKIN/KDIGO estadificada","LRA con estadio","LRA","Incorrecto",
          {"lesion renal aguda","lra","insuficiencia renal aguda","akin","kdigo","estadio","grado"}, {}, 2.0f },
        { "lra_dg_02","Etiologia (mixta prerrenal + intrinseca)",
          CompetencyDimension::ClinicalDiagnosis, 12,
          "Prerrenal + NTA/NIA por AINEs","Una etiologia","Vago","No identifica",
          {"prerrenal","intrinseca","aines","nefritis intersticial","nta","necrosis tubular","nia"}, {}, 2.0f },
        { "lra_dg_03","Diagnosticos diferenciales",
          CompetencyDimension::ClinicalDiagnosis, 6,
          "3+ DD","2","1","Ninguno",
          {"sindrome cardiorenal","glomerulonefritis","obstruccion","rabdomiolisis"} }
    };
    r.dimensions.push_back(dg);

    // ---------- Tratamiento 40 pts ----------
    DimensionRubric tx;
    tx.dimension = CompetencyDimension::TreatmentPlan;
    tx.name      = "Plan Terapeutico";
    tx.maxPoints = 40;
    tx.criteria = {
        { "lra_tx_01","SUSPENDER nefrotoxicos (AINE, IECA, metformina)",
          CompetencyDimension::TreatmentPlan, 15,
          "Suspende AINE+IECA+metformina","2 de 3","1","No suspende",
          {"suspender","ibuprofeno","aine","enalapril","iecas","metformina"}, {}, 2.5f },
        { "lra_tx_02","Manejo hidroelectrolitico + hiperkalemia",
          CompetencyDimension::TreatmentPlan, 10,
          "Balance+K con gluconato/insulina","Uno","MenciÃ³n","No maneja",
          {"balance hidrico","hiperkalemia","gluconato de calcio","insulina","glucosa","resinas","kayexalate"}, {}, 1.5f },
        { "lra_tx_03","Diuretico si sobrecarga hidrica",
          CompetencyDimension::TreatmentPlan, 8,
          "Furosemida con dosis y evaluacion","Furosemida","MenciÃ³n","No indica",
          {"furosemida","diuretico","asa","sobrecarga"} },
        { "lra_tx_04","Ajuste de dosis segun TFG + evitar contrastes",
          CompetencyDimension::TreatmentPlan, 7,
          "Ajusta dosis+evita contrastes","Uno","MenciÃ³n","No considera",
          {"ajuste de dosis","tfg","filtrado glomerular","contraste","nefrotoxicos"} }
    };
    r.dimensions.push_back(tx);

    // ---------- Seguimiento 20 pts ----------
    DimensionRubric fu;
    fu.dimension = CompetencyDimension::FollowUpMonitoring;
    fu.name      = "Seguimiento y Monitorizacion";
    fu.maxPoints = 20;
    fu.criteria = {
        { "lra_fu_01","Balance hidrico + peso diario + Cr seriada",
          CompetencyDimension::FollowUpMonitoring, 8,
          "Balance+peso+Cr diarios","2 de 3","1","Nada",
          {"balance hidrico","peso diario","creatinina seriada","diuresis"} },
        { "lra_fu_02","Criterios de dialisis (AEIOU)",
          CompetencyDimension::FollowUpMonitoring, 7,
          "Menciona AEIOU o criterios","Algunos criterios","Vago","No menciona",
          {"dialisis","hemodialisis","terapia reemplazo renal","trr","acidosis","hiperkalemia refractaria","uremia","sobrecarga"} },
        { "lra_fu_03","Interconsulta nefrologia + educacion",
          CompetencyDimension::FollowUpMonitoring, 5,
          "Nefrologia+educacion nefrotoxicos","Uno","MenciÃ³n","Nada",
          {"nefrologia","interconsulta","educacion","evitar aines","no automedicacion"} }
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