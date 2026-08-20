// =====================================================================
//   ENARM.Assessment - RUBRICA APENDICITIS AGUDA PEDIATRICA (case_006)
//   Paciente: Diego Ramirez Perez, 8 anos, dolor migratorio
//   GPC IMSS-031-08
// =====================================================================
#include <enarm/assessment/AssessmentEngine.h>

namespace enarm::assessment {

CaseRubric AssessmentEngine::createApendicitisRubric() {
    CaseRubric r;
    r.caseId         = "case_006_apendicitis";
    r.caseName       = "Apendicitis Aguda Pediatrica";
    r.totalMaxPoints = 185;

    // ---------- Interrogatorio 40 pts ----------
    DimensionRubric iv;
    iv.dimension = CompetencyDimension::ClinicalInterview;
    iv.name      = "Interrogatorio Clinico";
    iv.maxPoints = 40;
    iv.criteria = {
        { "ap_hx_01","Semiologia del dolor (migracion clasica)",
          CompetencyDimension::ClinicalInterview, 15,
          "Documenta migracion periumbilical->FID","Documenta dolor abdominal","Vago","No documenta",
          {"periumbilical","ombligo","fosa iliaca","migracion","cuadrante inferior derecho","fid"}, {}, 2.5f },
        { "ap_hx_02","Anorexia, nausea, vomito",
          CompetencyDimension::ClinicalInterview, 8,
          "Explora los 3","2","1","Ninguno",
          {"anorexia","no quiere comer","hiporexia","nausea","vomito","asco"} },
        { "ap_hx_03","Fiebre y tiempo de evolucion",
          CompetencyDimension::ClinicalInterview, 7,
          "Fiebre+tiempo preciso","Uno","MenciÃ³n","Ninguno",
          {"fiebre","calentura","horas","tiempo","cuando empezo"} },
        { "ap_hx_04","Habito intestinal y sintomas urinarios",
          CompetencyDimension::ClinicalInterview, 5,
          "Ambos","Uno","MenciÃ³n","No pregunta",
          {"evacuaciones","diarrea","estrenimiento","disuria","orinar","urinario"} },
        { "ap_hx_05","Antecedentes y alergias en pediatrico",
          CompetencyDimension::ClinicalInterview, 5,
          "Vacunas+alergias+cirugias","2 de 3","1","Ninguno",
          {"vacunas","alergias","cirugias","antecedentes"} }
    };
    r.dimensions.push_back(iv);

    // ---------- Exploracion 25 pts ----------
    DimensionRubric pe;
    pe.dimension = CompetencyDimension::PhysicalExamination;
    pe.name      = "Exploracion Fisica";
    pe.maxPoints = 25;
    pe.criteria = {
        { "ap_pe_01","Signos apendiculares (McBurney, Blumberg, Rovsing)",
          CompetencyDimension::PhysicalExamination, 12,
          "3+ signos evaluados","2","1","Ninguno",
          {"mcburney","blumberg","rovsing","psoas","obturador","defensa","irritacion peritoneal"}, {}, 2.0f },
        { "ap_pe_02","Signos vitales y estado general",
          CompetencyDimension::PhysicalExamination, 6,
          "SV completos+facies","SV completos","Parcial","Minimo",
          {"presion arterial","frecuencia cardiaca","temperatura","facies","posicion antalgica"} },
        { "ap_pe_03","Auscultacion abdominal",
          CompetencyDimension::PhysicalExamination, 4,
          "Peristalsis evaluada","MenciÃ³n","Vago","No ausculta",
          {"peristalsis","ruidos intestinales","auscultacion abdominal"} },
        { "ap_pe_04","Tacto rectal si indicado",
          CompetencyDimension::PhysicalExamination, 3,
          "Considera o realiza","MenciÃ³n","Vago","No considera",
          {"tacto rectal","fondo de saco","exploracion rectal"} }
    };
    r.dimensions.push_back(pe);

    // ---------- Estudios 30 pts ----------
    DimensionRubric st;
    st.dimension = CompetencyDimension::DiagnosticStudies;
    st.name      = "Estudios Paraclinicos";
    st.maxPoints = 30;
    st.criteria = {
        { "ap_st_01","Biometria hematica (leucocitosis)",
          CompetencyDimension::DiagnosticStudies, 10,
          "BH con formula","BH","MenciÃ³n","No solicita",
          {"biometria hematica","bh","leucocitos","neutrofilos","formula blanca"}, {}, 1.5f },
        { "ap_st_02","Ultrasonido abdominal",
          CompetencyDimension::DiagnosticStudies, 12,
          "USG abdominal como 1a linea","Lo solicita","MenciÃ³n","No solicita",
          {"ultrasonido","usg","ecografia","apendice"}, {}, 2.0f },
        { "ap_st_03","EGO para descartar ITU",
          CompetencyDimension::DiagnosticStudies, 4,
          "EGO","Lo menciona","Vago","No solicita",
          {"examen general de orina","ego","urianalisis"} },
        { "ap_st_04","PCR y quimica sanguinea",
          CompetencyDimension::DiagnosticStudies, 4,
          "PCR+QS+electrolitos","2 de 3","1","Ninguno",
          {"proteina c reactiva","pcr","quimica sanguinea","electrolitos"} }
    };
    r.dimensions.push_back(st);

    // ---------- Diagnostico 30 pts ----------
    DimensionRubric dg;
    dg.dimension = CompetencyDimension::ClinicalDiagnosis;
    dg.name      = "Diagnostico Clinico";
    dg.maxPoints = 30;
    dg.criteria = {
        { "ap_dg_01","Diagnostico de apendicitis aguda",
          CompetencyDimension::ClinicalDiagnosis, 15,
          "Apendicitis aguda no complicada","Apendicitis","Abdomen agudo","Incorrecto",
          {"apendicitis","apendicitis aguda","apendicitis no complicada"}, {}, 3.0f },
        { "ap_dg_02","Score de Alvarado o similar",
          CompetencyDimension::ClinicalDiagnosis, 8,
          "Aplica Alvarado o PAS","Menciona escala","Vago","No aplica",
          {"alvarado","score","pas","escala pediatrica","apendicitis score"} },
        { "ap_dg_03","Diagnosticos diferenciales pediatricos",
          CompetencyDimension::ClinicalDiagnosis, 7,
          "3+ DD pediatricos","2","1","Ninguno",
          {"adenitis mesenterica","gastroenteritis","invaginacion","diverticulo de meckel","itu"} }
    };
    r.dimensions.push_back(dg);

    // ---------- Tratamiento 40 pts ----------
    DimensionRubric tx;
    tx.dimension = CompetencyDimension::TreatmentPlan;
    tx.name      = "Plan Terapeutico";
    tx.maxPoints = 40;
    tx.criteria = {
        { "ap_tx_01","Ayuno + hidratacion IV + acceso venoso",
          CompetencyDimension::TreatmentPlan, 10,
          "Los 3","2","1","Ninguno",
          {"ayuno","nada por via oral","npo","hidratacion","solucion","hartmann","acceso venoso"} },
        { "ap_tx_02","Antibiotico preoperatorio pediatrico",
          CompetencyDimension::TreatmentPlan, 12,
          "Cef+metronidazol con dosis kg","ATB apropiado","ATB sin dosis","Sin ATB",
          {"ceftriaxona","metronidazol","antibiotico","preoperatorio","mg/kg"}, {}, 2.0f },
        { "ap_tx_03","Analgesia adecuada",
          CompetencyDimension::TreatmentPlan, 8,
          "Paracetamol+opioide segun dolor","Analgesia","MenciÃ³n","Ninguna",
          {"paracetamol","analgesia","nalbufina","opioide","mg/kg"} },
        { "ap_tx_04","Interconsulta a cirugia pediatrica",
          CompetencyDimension::TreatmentPlan, 10,
          "Interconsulta urgente+apendicectomia","Interconsulta","MenciÃ³n","No solicita",
          {"cirugia pediatrica","apendicectomia","laparoscopica","interconsulta","cirugia urgente"}, {}, 1.5f }
    };
    r.dimensions.push_back(tx);

    // ---------- Seguimiento 20 pts ----------
    DimensionRubric fu;
    fu.dimension = CompetencyDimension::FollowUpMonitoring;
    fu.name      = "Seguimiento y Monitorizacion";
    fu.maxPoints = 20;
    fu.criteria = {
        { "ap_fu_01","Monitoreo preoperatorio",
          CompetencyDimension::FollowUpMonitoring, 8,
          "SV seriados+dolor+abdomen","2 de 3","1","Ninguno",
          {"signos vitales","dolor","abdomen","monitoreo","evolucion"} },
        { "ap_fu_02","Complicaciones a vigilar",
          CompetencyDimension::FollowUpMonitoring, 7,
          "Perforacion+peritonitis+absceso","2","1","Ninguna",
          {"perforacion","peritonitis","absceso","complicaciones"} },
        { "ap_fu_03","Postoperatorio y educacion familiar",
          CompetencyDimension::FollowUpMonitoring, 5,
          "Postop+educacion+signos alarma","2","1","Ninguno",
          {"postoperatorio","educacion","padres","signos de alarma","alta"} }
    };
    r.dimensions.push_back(fu);

    r.expectedTimeMinutes = {
        {CompetencyDimension::ClinicalInterview,   7.0f},
        {CompetencyDimension::PhysicalExamination, 4.0f},
        {CompetencyDimension::DiagnosticStudies,   3.0f},
        {CompetencyDimension::ClinicalDiagnosis,   3.0f},
        {CompetencyDimension::TreatmentPlan,       4.0f},
        {CompetencyDimension::FollowUpMonitoring,  2.0f}
    };
    return r;
}

} // namespace enarm::assessment