// =====================================================================
//   ENARM.Assessment - RUBRICA TCE SEVERO (case_007)
//   Paciente: Carlos Mendez Aguilar, 32 anos, politraumatizado
//   GPC IMSS-604-13
// =====================================================================
#include <enarm/assessment/AssessmentEngine.h>

namespace enarm::assessment {

CaseRubric AssessmentEngine::createTCERubric() {
    CaseRubric r;
    r.caseId         = "case_007_tce";
    r.caseName       = "TCE Severo Politraumatizado";
    r.totalMaxPoints = 185;

    // ---------- Interrogatorio 40 pts ----------
    DimensionRubric iv;
    iv.dimension = CompetencyDimension::ClinicalInterview;
    iv.name      = "Interrogatorio Clinico";
    iv.maxPoints = 40;
    iv.criteria = {
        { "tce_hx_01","Mecanismo de lesion (energia cinetica)",
          CompetencyDimension::ClinicalInterview, 12,
          "Detalla mecanismo, velocidad, cinturon","Mecanismo basico","Vago","No pregunta",
          {"accidente","colision","velocidad","cinturon de seguridad","mecanismo","impacto","cinetica"}, {}, 1.5f },
        { "tce_hx_02","Perdida de conciencia y amnesia",
          CompetencyDimension::ClinicalInterview, 10,
          "Duracion+tipo+lucidez","Duracion","MenciÃ³n","No pregunta",
          {"perdida de conciencia","inconsciente","amnesia","lucidez","desde el evento"}, {}, 1.5f },
        { "tce_hx_03","Convulsiones postraumaticas",
          CompetencyDimension::ClinicalInterview, 6,
          "Explora convulsiones + tipo","MenciÃ³n","Vago","No pregunta",
          {"convulsion","crisis","movimientos anormales","tonico-clonico"} },
        { "tce_hx_04","AMPLE (alergias, medicamentos, patologias)",
          CompetencyDimension::ClinicalInterview, 7,
          "AMPLE completo por familia","Parcial","MenciÃ³n","No aplica",
          {"alergias","medicamentos","antecedentes","ultima ingesta","ample"} },
        { "tce_hx_05","Consumo de alcohol/drogas",
          CompetencyDimension::ClinicalInterview, 5,
          "Explora ambos","Uno","MenciÃ³n","No pregunta",
          {"alcohol","drogas","toxicos","intoxicacion"} }
    };
    r.dimensions.push_back(iv);

    // ---------- Exploracion 25 pts ----------
    DimensionRubric pe;
    pe.dimension = CompetencyDimension::PhysicalExamination;
    pe.name      = "Exploracion Fisica";
    pe.maxPoints = 25;
    pe.criteria = {
        { "tce_pe_01","Escala de Glasgow completa",
          CompetencyDimension::PhysicalExamination, 10,
          "GCS O+V+M completo","GCS numero","MenciÃ³n","No calcula",
          {"glasgow","gcs","apertura ocular","respuesta verbal","respuesta motora"}, {}, 2.0f },
        { "tce_pe_02","Pupilas y focalizacion neurologica",
          CompetencyDimension::PhysicalExamination, 8,
          "Pupilas+focalizacion+postura","2 de 3","1","Ninguno",
          {"pupilas","anisocoria","reactivas","focalizacion","decorticacion","decerebracion"}, {}, 1.5f },
        { "tce_pe_03","Signos de fractura de base de craneo",
          CompetencyDimension::PhysicalExamination, 5,
          "Battle+ojos mapache+LCR oido/nariz","2 de 3","1","No busca",
          {"battle","ojos de mapache","otorragia","otorraquia","rinorraquia","lcr","hematoma periorbitario"} },
        { "tce_pe_04","Estabilizacion cervical y ABCDE",
          CompetencyDimension::PhysicalExamination, 2,
          "Collarin+ABCDE","MenciÃ³n","Vago","No aplica",
          {"collarin","cervical","abcde","estabilizacion"} }
    };
    r.dimensions.push_back(pe);

    // ---------- Estudios 30 pts ----------
    DimensionRubric st;
    st.dimension = CompetencyDimension::DiagnosticStudies;
    st.name      = "Estudios Paraclinicos";
    st.maxPoints = 30;
    st.criteria = {
        { "tce_st_01","TAC de craneo URGENTE sin contraste",
          CompetencyDimension::DiagnosticStudies, 15,
          "TAC craneo inmediato","La solicita","Tarde","No solicita",
          {"tomografia","tac","ct","craneo"}, {}, 2.5f },
        { "tce_st_02","Rx/TAC cervical",
          CompetencyDimension::DiagnosticStudies, 6,
          "Cervical completo","Cervical","MenciÃ³n","No solicita",
          {"columna cervical","cervical","transoral","c1-c7"} },
        { "tce_st_03","FAST y estudios de politrauma",
          CompetencyDimension::DiagnosticStudies, 5,
          "FAST+TAC torax/abdomen","Uno","MenciÃ³n","Ninguno",
          {"fast","abdomen","torax","politraumatizado","ecografia"} },
        { "tce_st_04","Labs completos + toxicologia",
          CompetencyDimension::DiagnosticStudies, 4,
          "BH+QS+coag+gaso+toxico","Mayoria","Algunos","Pocos",
          {"biometria","quimica","coagulacion","gasometria","toxicologia","alcohol","grupo sanguineo"} }
    };
    r.dimensions.push_back(st);

    // ---------- Diagnostico 30 pts ----------
    DimensionRubric dg;
    dg.dimension = CompetencyDimension::ClinicalDiagnosis;
    dg.name      = "Diagnostico Clinico";
    dg.maxPoints = 30;
    dg.criteria = {
        { "tce_dg_01","Clasificacion severidad TCE (Glasgow)",
          CompetencyDimension::ClinicalDiagnosis, 12,
          "TCE severo GCS<9 identificado","TCE","Trauma","Incorrecto",
          {"tce","trauma craneoencefalico","severo","gcs menor","glasgow bajo"}, {}, 2.0f },
        { "tce_dg_02","Lesion intracraneal sospechada",
          CompetencyDimension::ClinicalDiagnosis, 10,
          "Hematoma epidural/subdural/contusion","Hemorragia","Lesion","Nada",
          {"hematoma epidural","hematoma subdural","contusion cerebral","hemorragia intracraneal"} },
        { "tce_dg_03","Triada de Cushing y HTIC",
          CompetencyDimension::ClinicalDiagnosis, 8,
          "Reconoce triada + HTIC","Uno","MenciÃ³n","Nada",
          {"cushing","hipertension endocraneana","htic","hipertension intracraneal","bradicardia"} }
    };
    r.dimensions.push_back(dg);

    // ---------- Tratamiento 40 pts ----------
    DimensionRubric tx;
    tx.dimension = CompetencyDimension::TreatmentPlan;
    tx.name      = "Plan Terapeutico";
    tx.maxPoints = 40;
    tx.criteria = {
        { "tx_tce_01","Via aerea definitiva (IOT con GCS<8)",
          CompetencyDimension::TreatmentPlan, 15,
          "IOT secuencia rapida","IOT","MenciÃ³n","No indica",
          {"intubacion","iot","via aerea definitiva","secuencia rapida"}, {}, 2.5f },
        { "tx_tce_02","Osmoterapia (manitol o SSH)",
          CompetencyDimension::TreatmentPlan, 10,
          "Manitol o SSH con dosis","MenciÃ³n dosis","Vago","No indica",
          {"manitol","solucion salina hipertonica","ssh","osmoterapia"}, {}, 1.5f },
        { "tx_tce_03","Anticonvulsivante profilactico",
          CompetencyDimension::TreatmentPlan, 5,
          "Levetiracetam o fenitoina","MenciÃ³n","Vago","No indica",
          {"levetiracetam","fenitoina","anticonvulsivante","profilaxis"} },
        { "tx_tce_04","Neurocirugia + medidas anti-HTIC",
          CompetencyDimension::TreatmentPlan, 10,
          "Neurocx+cabecera 30+normocap+PAM","3 de 4","2","Menos",
          {"neurocirugia","cabecera","30 grados","normocapnia","pam","presion arterial media","craniectomia"} }
    };
    r.dimensions.push_back(tx);

    // ---------- Seguimiento 20 pts ----------
    DimensionRubric fu;
    fu.dimension = CompetencyDimension::FollowUpMonitoring;
    fu.name      = "Seguimiento y Monitorizacion";
    fu.maxPoints = 20;
    fu.criteria = {
        { "tce_fu_01","Monitorizacion neurologica seriada",
          CompetencyDimension::FollowUpMonitoring, 8,
          "GCS seriado+pupilas+focalizacion","2 de 3","1","Ninguno",
          {"glasgow seriado","pupilas","focalizacion","deterioro neurologico"} },
        { "tce_fu_02","Monitor PIC / PAM / PPC",
          CompetencyDimension::FollowUpMonitoring, 7,
          "PIC+PAM+PPC con metas","2 de 3","1","Ninguno",
          {"pic","presion intracraneal","ppc","presion perfusion cerebral","invasiva"} },
        { "tce_fu_03","Ingreso a UCI + reevaluacion imagen",
          CompetencyDimension::FollowUpMonitoring, 5,
          "UCI+TAC control","Uno","MenciÃ³n","Nada",
          {"uci","cuidados intensivos","tac control","reevaluacion"} }
    };
    r.dimensions.push_back(fu);

    r.expectedTimeMinutes = {
        {CompetencyDimension::ClinicalInterview,   5.0f},   // paciente inconsciente, familia da HC
        {CompetencyDimension::PhysicalExamination, 5.0f},
        {CompetencyDimension::DiagnosticStudies,   3.0f},
        {CompetencyDimension::ClinicalDiagnosis,   3.0f},
        {CompetencyDimension::TreatmentPlan,       5.0f},
        {CompetencyDimension::FollowUpMonitoring,  3.0f}
    };
    return r;
}

} // namespace enarm::assessment