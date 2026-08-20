// =====================================================================
//   ENARM.Assessment - RUBRICA PREECLAMPSIA SEVERA (case_005)
//   Paciente: Andrea Martinez Ruiz, 28 anos, primigesta 32 SDG
//   GPC IMSS-020-08 / IMSS-058-08
// =====================================================================
#include <enarm/assessment/AssessmentEngine.h>

namespace enarm::assessment {

CaseRubric AssessmentEngine::createPreeclampsiaRubric() {
    CaseRubric r;
    r.caseId         = "case_005_preeclampsia";
    r.caseName       = "Preeclampsia Severa 32 SDG";
    r.totalMaxPoints = 185;

    // ---------- Interrogatorio 40 pts ----------
    DimensionRubric iv;
    iv.dimension = CompetencyDimension::ClinicalInterview;
    iv.name      = "Interrogatorio Clinico";
    iv.maxPoints = 40;
    iv.criteria = {
        { "pre_hx_01","Sintomas de vasoespasmo (cefalea, vision)",
          CompetencyDimension::ClinicalInterview, 12,
          "Explora cefalea+vision+fotopsias","Mayoria","Algunos","Ninguno",
          {"cefalea","dolor de cabeza","vision borrosa","fotopsias","lucecitas","escotomas"}, {}, 1.5f },
        { "pre_hx_02","Epigastralgia y sintomas de HELLP",
          CompetencyDimension::ClinicalInterview, 8,
          "Explora epigastralgia+nausea+vomito","Mayoria","MenciÃ³n","No explora",
          {"epigastralgia","dolor en boca del estomago","dolor abdominal alto","nausea","vomito"} },
        { "pre_hx_03","Edad gestacional y movimientos fetales",
          CompetencyDimension::ClinicalInterview, 8,
          "Ambos precisos","Uno","MenciÃ³n","Nada",
          {"semanas de gestacion","edad gestacional","sdg","movimientos fetales","bebe se mueve"} },
        { "pre_hx_04","Control prenatal y antecedentes",
          CompetencyDimension::ClinicalInterview, 7,
          "Control+FUM+antecedentes","2 de 3","1","Ninguno",
          {"control prenatal","fum","fecha ultima menstruacion","primigesta","antecedentes"} },
        { "pre_hx_05","Antecedentes de HAS previa o familiar",
          CompetencyDimension::ClinicalInterview, 5,
          "Explora HAS personal y familiar","Uno","MenciÃ³n","No explora",
          {"hipertension","presion alta","familiares","madre","hermanas"} }
    };
    r.dimensions.push_back(iv);

    // ---------- Exploracion 25 pts ----------
    DimensionRubric pe;
    pe.dimension = CompetencyDimension::PhysicalExamination;
    pe.name      = "Exploracion Fisica";
    pe.maxPoints = 25;
    pe.criteria = {
        { "pre_pe_01","Presion arterial medida correctamente",
          CompetencyDimension::PhysicalExamination, 10,
          "TA en ambos brazos, seriada","TA correcta","TA simple","No mide bien",
          {"presion arterial","ambos brazos","tension arterial","toma seriada"}, {}, 2.0f },
        { "pre_pe_02","Reflejos osteotendinosos y clonus",
          CompetencyDimension::PhysicalExamination, 6,
          "Reflejos + clonus","Solo reflejos","MenciÃ³n","No evalua",
          {"reflejos","hiperreflexia","clonus","rotuliano","patelar"} },
        { "pre_pe_03","Edema y fondo de ojo",
          CompetencyDimension::PhysicalExamination, 5,
          "Edema+fondo de ojo","Uno","MenciÃ³n","Ninguno",
          {"edema","fondo de ojo","papila","hemorragias retinianas","facial","manos"} },
        { "pre_pe_04","Valoracion obstetrica (altura uterina, FCF)",
          CompetencyDimension::PhysicalExamination, 4,
          "Altura+FCF+actividad uterina","2 de 3","1","Ninguno",
          {"altura uterina","fondo uterino","frecuencia cardiaca fetal","fcf","actividad uterina"} }
    };
    r.dimensions.push_back(pe);

    // ---------- Estudios 30 pts ----------
    DimensionRubric st;
    st.dimension = CompetencyDimension::DiagnosticStudies;
    st.name      = "Estudios Paraclinicos";
    st.maxPoints = 30;
    st.criteria = {
        { "pre_st_01","BH con plaquetas + pruebas hepaticas",
          CompetencyDimension::DiagnosticStudies, 10,
          "BH+plaquetas+TGO+TGP+LDH","Mayoria","Algunos","Pocos",
          {"biometria hematica","plaquetas","transaminasas","tgo","tgp","dhl","ldh","pruebas hepaticas"}, {}, 1.5f },
        { "pre_st_02","Proteinuria (24h o indice)",
          CompetencyDimension::DiagnosticStudies, 8,
          "Proteinuria 24h o Prot/Cr","Tira reactiva","MenciÃ³n","No solicita",
          {"proteinuria","proteinas en orina","indice proteina creatinina","orina 24 horas"} },
        { "pre_st_03","Funcion renal y coagulacion",
          CompetencyDimension::DiagnosticStudies, 6,
          "Creat+urea+TP+TPT","Mayoria","Uno","Ninguno",
          {"creatinina","urea","tiempos de coagulacion","tp","tpt"} },
        { "pre_st_04","USG obstetrico + RCTG",
          CompetencyDimension::DiagnosticStudies, 6,
          "USG con Doppler + RCTG","Uno","MenciÃ³n","Ninguno",
          {"ultrasonido","usg","doppler","registro cardiotocografico","rctg","monitoreo fetal"} }
    };
    r.dimensions.push_back(st);

    // ---------- Diagnostico 30 pts ----------
    DimensionRubric dg;
    dg.dimension = CompetencyDimension::ClinicalDiagnosis;
    dg.name      = "Diagnostico Clinico";
    dg.maxPoints = 30;
    dg.criteria = {
        { "pre_dg_01","Diagnostico de preeclampsia severa",
          CompetencyDimension::ClinicalDiagnosis, 15,
          "Preeclampsia con datos severidad","Preeclampsia","HTA gestacional","Incorrecto",
          {"preeclampsia","preeclampsia severa","con datos de severidad"}, {}, 3.0f },
        { "pre_dg_02","Descarta HELLP y eclampsia inminente",
          CompetencyDimension::ClinicalDiagnosis, 8,
          "Evalua HELLP + eclampsia inminente","Uno","MenciÃ³n","Nada",
          {"hellp","hemolisis","eclampsia","convulsion inminente"} },
        { "pre_dg_03","Diagnosticos diferenciales",
          CompetencyDimension::ClinicalDiagnosis, 7,
          "3+ DD","2","1","Ninguno",
          {"higado graso agudo","crisis hipertensiva","migrana","purpura trombotica"} }
    };
    r.dimensions.push_back(dg);

    // ---------- Tratamiento 40 pts ----------
    DimensionRubric tx;
    tx.dimension = CompetencyDimension::TreatmentPlan;
    tx.name      = "Plan Terapeutico";
    tx.maxPoints = 40;
    tx.criteria = {
        { "pre_tx_01","Sulfato de magnesio (impregnacion + mantto)",
          CompetencyDimension::TreatmentPlan, 15,
          "Impregnacion 4-6g + mantto 1-2g/h","Sulfato sin dosis","MenciÃ³n","No indica",
          {"sulfato de magnesio","mgso4","impregnacion","mantenimiento"}, {}, 2.5f },
        { "pre_tx_02","Antihipertensivo (hidralazina o labetalol)",
          CompetencyDimension::TreatmentPlan, 10,
          "Hidralazina/labetalol con meta 140-155","Antihipertensivo","MenciÃ³n","Ninguno",
          {"hidralazina","labetalol","nifedipino","antihipertensivo"}, {}, 1.5f },
        { "pre_tx_03","Maduracion pulmonar fetal",
          CompetencyDimension::TreatmentPlan, 8,
          "Betametasona 12mg IM 2 dosis","Betametasona","MenciÃ³n","No indica",
          {"betametasona","dexametasona","maduracion pulmonar","corticoide fetal"} },
        { "pre_tx_04","Decision de interrupcion del embarazo",
          CompetencyDimension::TreatmentPlan, 7,
          "Interrumpe post-estabilizacion","Menciona interrupcion","Vago","No decide",
          {"interrupcion","cesarea","parto","nacimiento","termino embarazo"} }
    };
    r.dimensions.push_back(tx);

    // ---------- Seguimiento 20 pts ----------
    DimensionRubric fu;
    fu.dimension = CompetencyDimension::FollowUpMonitoring;
    fu.name      = "Seguimiento y Monitorizacion";
    fu.maxPoints = 20;
    fu.criteria = {
        { "pre_fu_01","Monitoreo materno-fetal",
          CompetencyDimension::FollowUpMonitoring, 8,
          "TA+reflejos+diuresis+FCF continuos","3 de 4","2","Menos",
          {"presion arterial","diuresis","sonda foley","frecuencia cardiaca fetal","reflejos"} },
        { "pre_fu_02","Signos de toxicidad por sulfato",
          CompetencyDimension::FollowUpMonitoring, 7,
          "Reflejos+FR+diuresis+gluconato calcio","3 de 4","2","Menos",
          {"reflejos","frecuencia respiratoria","gluconato de calcio","toxicidad","magnesio serico"} },
        { "pre_fu_03","Vigilancia postparto de complicaciones",
          CompetencyDimension::FollowUpMonitoring, 5,
          "Sulfato 24h postparto + vigilancia","Uno","MenciÃ³n","Nada",
          {"postparto","puerperio","24 horas","vigilancia","complicaciones"} }
    };
    r.dimensions.push_back(fu);

    r.expectedTimeMinutes = {
        {CompetencyDimension::ClinicalInterview,   7.0f},
        {CompetencyDimension::PhysicalExamination, 4.0f},
        {CompetencyDimension::DiagnosticStudies,   4.0f},
        {CompetencyDimension::ClinicalDiagnosis,   3.0f},
        {CompetencyDimension::TreatmentPlan,       6.0f},
        {CompetencyDimension::FollowUpMonitoring,  2.0f}
    };
    return r;
}

} // namespace enarm::assessment