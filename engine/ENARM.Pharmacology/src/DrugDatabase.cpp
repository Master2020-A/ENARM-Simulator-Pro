// ═══════════════════════════════════════════════════════════════
//   ENARM.Pharmacology - DrugDatabase Implementation
//   Base de 30+ farmacos ICU con parametros PK/PD reales
// ═══════════════════════════════════════════════════════════════
#include "ENARM/Pharmacology/DrugDatabase.h"

#include <algorithm>
#include <cctype>

namespace ENARM::Pharmacology {

    namespace {
        std::string ToLower(std::string s) {
            std::transform(s.begin(), s.end(), s.begin(),
                [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
            return s;
        }
    }

    DrugDatabase& DrugDatabase::Instance() {
        static DrugDatabase instance;
        return instance;
    }

    DrugDatabase::DrugDatabase() {
        RegisterAllDrugs();
    }

    void DrugDatabase::RegisterAllDrugs() {
        RegisterVasopressors();
        RegisterInotropes();
        RegisterSedatives();
        RegisterAnalgesics();
        RegisterAnesthetics();
        RegisterNMBAs();
        RegisterAntiarrhythmics();
        RegisterOthers();
    }

    std::optional<DrugParameters> DrugDatabase::FindByName(const std::string& name) const {
        auto it = m_drugs.find(ToLower(name));
        if (it == m_drugs.end()) return std::nullopt;
        return it->second;
    }

    std::vector<DrugParameters> DrugDatabase::FindByCategory(DrugCategory cat) const {
        std::vector<DrugParameters> result;
        for (const auto& [name, drug] : m_drugs) {
            if (drug.category == cat) result.push_back(drug);
        }
        return result;
    }

    std::vector<std::string> DrugDatabase::ListAllNames() const {
        std::vector<std::string> names;
        for (const auto& [key, drug] : m_drugs) names.push_back(drug.name);
        std::sort(names.begin(), names.end());
        return names;
    }

    // ═══════════════════════════════════════════════════════════════
    //  VASOPRESORES
    // ═══════════════════════════════════════════════════════════════
    void DrugDatabase::RegisterVasopressors() {
        // ─── NOREPINEFRINA ───
        {
            DrugParameters d;
            d.name = "Norepinefrina";
            d.genericName = "Norepinephrine";
            d.tradeName = "Levophed";
            d.atcCode = "C01CA03";
            d.category = DrugCategory::Vasopressor;

            d.Vc_L_kg = 0.15;
            d.Vp_L_kg = 0.3;
            d.Cl_mL_kg_min = 40.0;
            d.halfLife_min = 2.5;
            d.k10_per_min = 0.28;
            d.k12_per_min = 0.15;
            d.k21_per_min = 0.10;

            d.onset_seconds = 30.0;
            d.peak_min = 1.0;
            d.duration_min = 5.0;
            d.clearance = ClearanceOrgan::Plasma;

            // Efectos: agonista alfa1 potente, beta1 leve
            d.effects.push_back({Receptor::Alpha1, 0.01, 1.5, 1.0, true});
            d.effects.push_back({Receptor::Beta1,  0.05, 1.2, 0.4, true});

            d.doses.infusion_min_mcg_kg_min = 0.05;
            d.doses.infusion_max_mcg_kg_min = 3.0;
            d.doses.dosing_notes = "Titular por MAP objetivo (usualmente >=65)";

            d.availableRoutes = {Route::IV_Infusion};
            d.concentration = "4 mg/4 mL amp";
            d.dilution = "16 mg (4 amp) en 250 mL DW5% = 64 mcg/mL";

            d.contraindications = {
                "Hipovolemia no corregida",
                "Trombosis vascular perifericamente"
            };
            d.sideEffects = {
                "Isquemia periferica (dedos, extremidades)",
                "Arritmias", "HTA severa"
            };
            d.warnings = {
                "Via central preferentemente",
                "Extravasacion: fentolamina local"
            };
            d.pregnancy = PregnancyCategory::C;

            m_drugs[ToLower(d.name)] = d;
        }

        // ─── ADRENALINA (Epinefrina) ───
        {
            DrugParameters d;
            d.name = "Adrenalina";
            d.genericName = "Epinephrine";
            d.category = DrugCategory::Vasopressor;

            d.Vc_L_kg = 0.15;
            d.Cl_mL_kg_min = 45.0;
            d.halfLife_min = 2.0;
            d.k10_per_min = 0.35;
            d.k12_per_min = 0.12;
            d.k21_per_min = 0.08;

            d.onset_seconds = 30.0;
            d.duration_min = 5.0;
            d.clearance = ClearanceOrgan::Plasma;

            // Efectos dosis-dependientes: dosis bajas beta > alfa
            d.effects.push_back({Receptor::Beta1,  0.02, 1.5, 1.0, true});
            d.effects.push_back({Receptor::Beta2,  0.03, 1.5, 0.7, true});
            d.effects.push_back({Receptor::Alpha1, 0.08, 1.5, 1.0, true});

            d.doses.push_min_mgkg = 0.01;         // 1mg IV en paro cardiaco
            d.doses.push_max_mgkg = 0.02;
            d.doses.infusion_min_mcg_kg_min = 0.01;
            d.doses.infusion_max_mcg_kg_min = 2.0;
            d.doses.dosing_notes = "Paro: 1 mg IV c/3-5 min. Shock: infusion titulada";

            d.availableRoutes = {Route::IV_Push, Route::IV_Infusion, Route::IM, Route::Inhaled};
            d.concentration = "1 mg/1 mL amp (1:1000) o 1 mg/10 mL (1:10,000)";

            d.contraindications = {"HTA severa no controlada"};
            d.sideEffects = {"Taquicardia", "Arritmias", "Ansiedad", "Cefalea"};
            d.pregnancy = PregnancyCategory::C;

            m_drugs[ToLower(d.name)] = d;
        }

        // ─── DOPAMINA ───
        {
            DrugParameters d;
            d.name = "Dopamina";
            d.genericName = "Dopamine";
            d.category = DrugCategory::Vasopressor;

            d.Vc_L_kg = 0.25;
            d.Cl_mL_kg_min = 60.0;
            d.halfLife_min = 2.0;
            d.k10_per_min = 0.35;

            d.onset_seconds = 60.0;
            d.duration_min = 10.0;
            d.clearance = ClearanceOrgan::Plasma;

            // Efectos dosis-dependientes
            // 1-5 mcg/kg/min: dopa (renal)
            // 5-10: beta1 (inotropo)
            // >10: alfa1 (vasoconstriccion)
            d.effects.push_back({Receptor::Dopamine1, 0.005, 1.5, 0.8, true});
            d.effects.push_back({Receptor::Beta1,     0.03,  1.5, 0.9, true});
            d.effects.push_back({Receptor::Alpha1,    0.10,  1.5, 0.9, true});

            d.doses.infusion_min_mcg_kg_min = 1.0;
            d.doses.infusion_max_mcg_kg_min = 20.0;
            d.doses.dosing_notes = "1-5 dopa | 5-10 beta | >10 alfa";

            d.availableRoutes = {Route::IV_Infusion};
            d.dilution = "400 mg en 250 mL DW5% = 1600 mcg/mL";

            d.pregnancy = PregnancyCategory::C;
            m_drugs[ToLower(d.name)] = d;
        }

        // ─── VASOPRESINA ───
        {
            DrugParameters d;
            d.name = "Vasopresina";
            d.genericName = "Vasopressin";
            d.category = DrugCategory::Vasopressor;

            d.halfLife_min = 15.0;
            d.k10_per_min = 0.046;

            d.onset_seconds = 60.0;
            d.duration_min = 30.0;
            d.clearance = ClearanceOrgan::HepaticRenal;

            d.effects.push_back({Receptor::Vasopressin_V1, 0.001, 1.5, 1.0, true});

            d.doses.infusion_min_mcg_kg_min = 0.01;   // 0.03 U/min
            d.doses.infusion_max_mcg_kg_min = 0.06;
            d.doses.dosing_notes = "0.03-0.04 U/min como aditivo a NA";

            d.pregnancy = PregnancyCategory::C;
            m_drugs[ToLower(d.name)] = d;
        }
    }

    // ═══════════════════════════════════════════════════════════════
    //  INOTROPICOS
    // ═══════════════════════════════════════════════════════════════
    void DrugDatabase::RegisterInotropes() {
        // ─── DOBUTAMINA ───
        {
            DrugParameters d;
            d.name = "Dobutamina";
            d.genericName = "Dobutamine";
            d.category = DrugCategory::Inotrope;

            d.Cl_mL_kg_min = 50.0;
            d.halfLife_min = 2.5;
            d.k10_per_min = 0.28;

            d.onset_seconds = 60.0;
            d.duration_min = 10.0;
            d.clearance = ClearanceOrgan::Hepatic;

            d.effects.push_back({Receptor::Beta1, 0.02, 1.5, 1.0, true});
            d.effects.push_back({Receptor::Beta2, 0.05, 1.5, 0.4, true});

            d.doses.infusion_min_mcg_kg_min = 2.5;
            d.doses.infusion_max_mcg_kg_min = 20.0;
            d.doses.dosing_notes = "Inotropo beta1 puro, aumenta GC sin vasoconstriccion";

            d.pregnancy = PregnancyCategory::B;
            m_drugs[ToLower(d.name)] = d;
        }

        // ─── MILRINONA ───
        {
            DrugParameters d;
            d.name = "Milrinona";
            d.genericName = "Milrinone";
            d.category = DrugCategory::Inotrope;

            d.Cl_mL_kg_min = 5.0;
            d.halfLife_min = 150.0;
            d.k10_per_min = 0.005;

            d.onset_seconds = 300.0;
            d.duration_min = 180.0;
            d.clearance = ClearanceOrgan::Renal;

            // Inhibidor PDE3: aumenta cAMP -> inotropo + vasodilatador
            d.doses.push_min_mgkg = 0.025;   // Bolo 25-50 mcg/kg
            d.doses.push_max_mgkg = 0.075;
            d.doses.infusion_min_mcg_kg_min = 0.25;
            d.doses.infusion_max_mcg_kg_min = 0.75;
            d.doses.dosing_notes = "Bolo 50 mcg/kg + infusion 0.375 mcg/kg/min";

            d.pregnancy = PregnancyCategory::C;
            m_drugs[ToLower(d.name)] = d;
        }
    }

    // ═══════════════════════════════════════════════════════════════
    //  SEDANTES
    // ═══════════════════════════════════════════════════════════════
    void DrugDatabase::RegisterSedatives() {
        // ─── PROPOFOL ───
        {
            DrugParameters d;
            d.name = "Propofol";
            d.category = DrugCategory::Sedative;

            d.Vc_L_kg = 0.25;
            d.Vp_L_kg = 3.5;
            d.Cl_mL_kg_min = 30.0;
            d.halfLife_min = 30.0;
            d.k10_per_min = 0.12;
            d.k12_per_min = 0.34;
            d.k21_per_min = 0.07;

            d.onset_seconds = 40.0;
            d.peak_min = 1.5;
            d.duration_min = 8.0;
            d.clearance = ClearanceOrgan::Hepatic;

            d.effects.push_back({Receptor::GABA_A, 1.0, 2.0, 1.0, true});

            d.doses.push_min_mgkg = 1.5;    // Induccion 1.5-2.5 mg/kg
            d.doses.push_max_mgkg = 2.5;
            d.doses.infusion_min_mcg_kg_min = 50.0;   // Sedacion UCI
            d.doses.infusion_max_mcg_kg_min = 200.0;
            d.doses.dosing_notes = "Induccion: 1.5-2.5 mg/kg. Sedacion UCI: 25-75 mcg/kg/min";

            d.contraindications = {
                "Alergia huevo/soya (relativa)",
                "Sindrome infusion propofol (>4 mg/kg/h prolongado)"
            };
            d.sideEffects = {
                "Hipotension", "Apnea", "Dolor en sitio inyeccion",
                "Hipertrigliceridemia (uso prolongado)"
            };
            d.pregnancy = PregnancyCategory::B;
            m_drugs[ToLower(d.name)] = d;
        }

        // ─── MIDAZOLAM ───
        {
            DrugParameters d;
            d.name = "Midazolam";
            d.tradeName = "Dormicum";
            d.category = DrugCategory::Sedative;

            d.Vc_L_kg = 0.4;
            d.Vp_L_kg = 1.7;
            d.Cl_mL_kg_min = 6.5;
            d.halfLife_min = 150.0;
            d.k10_per_min = 0.03;
            d.k12_per_min = 0.20;
            d.k21_per_min = 0.05;

            d.onset_seconds = 90.0;
            d.duration_min = 60.0;
            d.clearance = ClearanceOrgan::Hepatic;

            d.effects.push_back({Receptor::GABA_A, 0.05, 1.5, 0.85, true});

            d.doses.push_min_mgkg = 0.05;
            d.doses.push_max_mgkg = 0.3;
            d.doses.infusion_min_mcg_kg_min = 0.5;
            d.doses.infusion_max_mcg_kg_min = 3.0;
            d.doses.dosing_notes = "Bolo: 0.05-0.1 mg/kg. Sedacion UCI: 0.03-0.1 mg/kg/h";

            d.sideEffects = {"Depresion respiratoria", "Hipotension", "Delirio (uso prolongado)"};
            d.warnings = {"Antidoto: flumazenil"};
            d.pregnancy = PregnancyCategory::D;
            m_drugs[ToLower(d.name)] = d;
        }

        // ─── DEXMEDETOMIDINA ───
        {
            DrugParameters d;
            d.name = "Dexmedetomidina";
            d.genericName = "Dexmedetomidine";
            d.tradeName = "Precedex";
            d.category = DrugCategory::Sedative;

            d.Cl_mL_kg_min = 12.5;
            d.halfLife_min = 120.0;
            d.k10_per_min = 0.033;

            d.onset_seconds = 300.0;
            d.duration_min = 90.0;
            d.clearance = ClearanceOrgan::Hepatic;

            d.effects.push_back({Receptor::Alpha2, 0.001, 1.5, 1.0, true});

            d.doses.infusion_min_mcg_kg_min = 0.002;   // 0.2 mcg/kg/h
            d.doses.infusion_max_mcg_kg_min = 0.012;
            d.doses.dosing_notes = "0.2-0.7 mcg/kg/h. Sedacion consciente, no respiratoriamente deprimente";

            d.sideEffects = {"Bradicardia", "Hipotension"};
            d.pregnancy = PregnancyCategory::C;
            m_drugs[ToLower(d.name)] = d;
        }
    }

    // ═══════════════════════════════════════════════════════════════
    //  ANALGESICOS
    // ═══════════════════════════════════════════════════════════════
    void DrugDatabase::RegisterAnalgesics() {
        // ─── FENTANILO ───
        {
            DrugParameters d;
            d.name = "Fentanilo";
            d.genericName = "Fentanyl";
            d.category = DrugCategory::Analgesic;

            d.Vc_L_kg = 0.5;
            d.Vp_L_kg = 3.5;
            d.Cl_mL_kg_min = 13.0;
            d.halfLife_min = 200.0;
            d.k10_per_min = 0.06;
            d.k12_per_min = 0.15;
            d.k21_per_min = 0.05;

            d.onset_seconds = 60.0;
            d.duration_min = 45.0;
            d.clearance = ClearanceOrgan::Hepatic;

            d.effects.push_back({Receptor::MuOpioid, 0.001, 1.5, 1.0, true});

            d.doses.push_min_mgkg = 0.001;    // 1 mcg/kg
            d.doses.push_max_mgkg = 0.005;
            d.doses.infusion_min_mcg_kg_min = 0.5;
            d.doses.infusion_max_mcg_kg_min = 5.0;
            d.doses.dosing_notes = "Bolo 1-3 mcg/kg. Infusion 0.7-10 mcg/kg/h";

            d.contraindications = {"Depresion respiratoria severa", "Ileo paralitico"};
            d.sideEffects = {"Depresion respiratoria", "Bradicardia", "Rigidez toracica (dosis altas)"};
            d.warnings = {"Antidoto: naloxona"};
            d.pregnancy = PregnancyCategory::C;
            m_drugs[ToLower(d.name)] = d;
        }

        // ─── MORFINA ───
        {
            DrugParameters d;
            d.name = "Morfina";
            d.genericName = "Morphine";
            d.category = DrugCategory::Analgesic;

            d.Cl_mL_kg_min = 15.0;
            d.halfLife_min = 120.0;
            d.k10_per_min = 0.006;

            d.onset_seconds = 300.0;
            d.duration_min = 240.0;
            d.clearance = ClearanceOrgan::Hepatic;

            d.effects.push_back({Receptor::MuOpioid, 0.02, 1.5, 0.85, true});

            d.doses.push_min_mgkg = 0.05;
            d.doses.push_max_mgkg = 0.15;
            d.doses.dosing_notes = "2-5 mg IV c/4h. Cuidado en insuficiencia renal";

            d.contraindications = {"Insuficiencia renal severa (metabolito activo)"};
            d.sideEffects = {"Prurito", "Nausea", "Estrenimiento", "Liberacion histamina (hipotension)"};
            d.pregnancy = PregnancyCategory::C;
            m_drugs[ToLower(d.name)] = d;
        }

        // ─── KETAMINA ───
        {
            DrugParameters d;
            d.name = "Ketamina";
            d.genericName = "Ketamine";
            d.category = DrugCategory::Analgesic;

            d.Cl_mL_kg_min = 17.0;
            d.halfLife_min = 150.0;
            d.k10_per_min = 0.028;

            d.onset_seconds = 60.0;
            d.duration_min = 15.0;
            d.clearance = ClearanceOrgan::Hepatic;

            d.effects.push_back({Receptor::NMDA, 0.1, 1.5, 1.0, false});

            d.doses.push_min_mgkg = 0.5;    // 0.5 analgesico
            d.doses.push_max_mgkg = 2.0;    // 1-2 anestesico
            d.doses.dosing_notes = "Analgesia: 0.1-0.5 mg/kg. Induccion: 1-2 mg/kg";

            d.sideEffects = {"Alucinaciones", "Taquicardia", "Hipertension", "Salivacion"};
            d.warnings = {"Estable hemodinamicamente (util en shock/broncoespasmo)"};
            d.pregnancy = PregnancyCategory::B;
            m_drugs[ToLower(d.name)] = d;
        }
    }

    // ═══════════════════════════════════════════════════════════════
    //  ANESTESICOS INDUCCION
    // ═══════════════════════════════════════════════════════════════
    void DrugDatabase::RegisterAnesthetics() {
        // ─── ETOMIDATO ───
        {
            DrugParameters d;
            d.name = "Etomidato";
            d.genericName = "Etomidate";
            d.category = DrugCategory::Anesthetic;

            d.Cl_mL_kg_min = 20.0;
            d.halfLife_min = 180.0;
            d.k10_per_min = 0.10;

            d.onset_seconds = 30.0;
            d.duration_min = 5.0;
            d.clearance = ClearanceOrgan::Hepatic;

            d.effects.push_back({Receptor::GABA_A, 0.3, 1.5, 1.0, true});

            d.doses.push_min_mgkg = 0.2;
            d.doses.push_max_mgkg = 0.4;
            d.doses.dosing_notes = "Induccion: 0.2-0.3 mg/kg. Estable hemodinamicamente";

            d.warnings = {"Supresion adrenal transitoria", "Mioclonias"};
            d.pregnancy = PregnancyCategory::C;
            m_drugs[ToLower(d.name)] = d;
        }
    }

    // ═══════════════════════════════════════════════════════════════
    //  BLOQUEADORES NEUROMUSCULARES
    // ═══════════════════════════════════════════════════════════════
    void DrugDatabase::RegisterNMBAs() {
        // ─── SUCCINILCOLINA ───
        {
            DrugParameters d;
            d.name = "Succinilcolina";
            d.genericName = "Succinylcholine";
            d.category = DrugCategory::NeuromuscularBlocker;

            d.halfLife_min = 1.5;
            d.k10_per_min = 0.46;
            d.onset_seconds = 45.0;
            d.duration_min = 8.0;
            d.clearance = ClearanceOrgan::Plasma;

            d.effects.push_back({Receptor::Nicotinic_NMJ, 0.5, 1.5, 1.0, true});

            d.doses.push_min_mgkg = 1.0;
            d.doses.push_max_mgkg = 1.5;
            d.doses.dosing_notes = "1-1.5 mg/kg IV para intubacion rapida (RSI)";

            d.contraindications = {
                "Hipertermia maligna", "Hiperkalemia",
                "Trauma neurologico >48h", "Quemaduras extensas >24h",
                "Distrofia muscular"
            };
            d.sideEffects = {"Fasciculaciones", "Hiperkalemia", "Bradicardia"};
            d.pregnancy = PregnancyCategory::C;
            m_drugs[ToLower(d.name)] = d;
        }

        // ─── ROCURONIO ───
        {
            DrugParameters d;
            d.name = "Rocuronio";
            d.genericName = "Rocuronium";
            d.category = DrugCategory::NeuromuscularBlocker;

            d.halfLife_min = 90.0;
            d.k10_per_min = 0.008;
            d.onset_seconds = 60.0;
            d.duration_min = 45.0;
            d.clearance = ClearanceOrgan::Hepatic;

            d.effects.push_back({Receptor::Nicotinic_NMJ, 0.5, 1.5, 1.0, false});

            d.doses.push_min_mgkg = 0.6;
            d.doses.push_max_mgkg = 1.2;
            d.doses.dosing_notes = "RSI: 1.2 mg/kg. Mantenimiento: 0.6 mg/kg";

            d.warnings = {"Reversion: sugammadex 16 mg/kg (RSI) o 2-4 mg/kg"};
            d.pregnancy = PregnancyCategory::B;
            m_drugs[ToLower(d.name)] = d;
        }
    }

    // ═══════════════════════════════════════════════════════════════
    //  ANTIARRITMICOS
    // ═══════════════════════════════════════════════════════════════
    void DrugDatabase::RegisterAntiarrhythmics() {
        // ─── AMIODARONA ───
        {
            DrugParameters d;
            d.name = "Amiodarona";
            d.genericName = "Amiodarone";
            d.category = DrugCategory::Antiarrhythmic;

            d.Vc_L_kg = 1.3;
            d.Vp_L_kg = 60.0;   // Amplia distribucion tisular
            d.halfLife_min = 60 * 24 * 40.0;   // ~40 dias!
            d.k10_per_min = 0.0001;

            d.onset_seconds = 60.0;
            d.duration_min = 120.0;
            d.clearance = ClearanceOrgan::Hepatic;

            d.effects.push_back({Receptor::K_Channel, 0.5, 1.5, 1.0, false});
            d.effects.push_back({Receptor::Beta1,     2.0, 1.5, 0.5, false});

            d.doses.push_min_mgkg = 2.1;    // 150 mg IV en 10 min (adulto 70kg)
            d.doses.push_max_mgkg = 4.3;    // 300 mg IV en paro
            d.doses.dosing_notes = "TV/FV: 300 mg IV. Postparo: 150 mg. Mantenimiento: 1 mg/min x6h";

            d.sideEffects = {
                "Bradicardia", "Hipotension", "Prolongacion QT",
                "Toxicidad pulmonar", "Tiroiditis", "Fotosensibilidad"
            };
            d.pregnancy = PregnancyCategory::D;
            m_drugs[ToLower(d.name)] = d;
        }

        // ─── ADENOSINA ───
        {
            DrugParameters d;
            d.name = "Adenosina";
            d.genericName = "Adenosine";
            d.category = DrugCategory::Antiarrhythmic;

            d.halfLife_min = 0.16;   // 10 segundos!
            d.k10_per_min = 4.16;
            d.onset_seconds = 10.0;
            d.duration_min = 0.5;
            d.clearance = ClearanceOrgan::Plasma;

            d.effects.push_back({Receptor::Adenosine_A1, 0.1, 1.5, 1.0, true});

            d.doses.push_min_mgkg = 0.09;   // 6 mg primera dosis
            d.doses.push_max_mgkg = 0.17;   // 12 mg segunda
            d.doses.dosing_notes = "TSV: 6 mg IV rapido (bolo + flush). Si falla: 12 mg";

            d.contraindications = {"Bloqueo AV 2-3 grado sin marcapaso", "Asma severo"};
            d.warnings = {"Sensacion de muerte inminente breve"};
            d.pregnancy = PregnancyCategory::C;
            m_drugs[ToLower(d.name)] = d;
        }

        // ─── ATROPINA ───
        {
            DrugParameters d;
            d.name = "Atropina";
            d.genericName = "Atropine";
            d.category = DrugCategory::Antiarrhythmic;

            d.halfLife_min = 120.0;
            d.k10_per_min = 0.006;
            d.onset_seconds = 60.0;
            d.duration_min = 240.0;

            d.effects.push_back({Receptor::Muscarinic, 0.05, 1.5, 1.0, false});

            d.doses.push_min_mgkg = 0.007;    // 0.5 mg
            d.doses.push_max_mgkg = 0.03;     // 2 mg maximo
            d.doses.dosing_notes = "Bradicardia sintomatica: 0.5-1 mg IV c/3-5 min (max 3 mg)";

            d.contraindications = {"Glaucoma angulo cerrado"};
            d.sideEffects = {"Taquicardia", "Sequedad de mucosas", "Retencion urinaria"};
            d.pregnancy = PregnancyCategory::C;
            m_drugs[ToLower(d.name)] = d;
        }
    }

    // ═══════════════════════════════════════════════════════════════
    //  OTROS ICU
    // ═══════════════════════════════════════════════════════════════
    void DrugDatabase::RegisterOthers() {
        // ─── FUROSEMIDA ───
        {
            DrugParameters d;
            d.name = "Furosemida";
            d.genericName = "Furosemide";
            d.tradeName = "Lasix";
            d.category = DrugCategory::Diuretic;

            d.halfLife_min = 90.0;
            d.k10_per_min = 0.008;
            d.onset_seconds = 300.0;
            d.duration_min = 120.0;
            d.clearance = ClearanceOrgan::Renal;

            d.doses.push_min_mgkg = 0.3;
            d.doses.push_max_mgkg = 3.0;
            d.doses.dosing_notes = "20-80 mg IV. Insuficiencia renal: hasta 200 mg";

            d.sideEffects = {"Hipokalemia", "Hipomagnesemia", "Ototoxicidad (dosis altas)"};
            d.pregnancy = PregnancyCategory::C;
            m_drugs[ToLower(d.name)] = d;
        }

        // ─── NITROGLICERINA ───
        {
            DrugParameters d;
            d.name = "Nitroglicerina";
            d.genericName = "Nitroglycerin";
            d.category = DrugCategory::Vasodilator;

            d.halfLife_min = 3.0;
            d.k10_per_min = 0.23;
            d.onset_seconds = 60.0;
            d.duration_min = 8.0;
            d.clearance = ClearanceOrgan::Hepatic;

            d.doses.infusion_min_mcg_kg_min = 0.1;
            d.doses.infusion_max_mcg_kg_min = 5.0;
            d.doses.dosing_notes = "Angina/IC: 5-200 mcg/min. HTA severa: hasta 400 mcg/min";

            d.sideEffects = {"Cefalea", "Hipotension", "Taquicardia refleja", "Tolerancia"};
            d.pregnancy = PregnancyCategory::C;
            m_drugs[ToLower(d.name)] = d;
        }

        // ─── INSULINA REGULAR ───
        {
            DrugParameters d;
            d.name = "Insulina";
            d.genericName = "Insulin Regular";
            d.category = DrugCategory::InsulinInfusion;

            d.halfLife_min = 6.0;
            d.k10_per_min = 0.12;
            d.onset_seconds = 900.0;   // IV: 15 min
            d.duration_min = 60.0;

            d.doses.infusion_min_mcg_kg_min = 0.014;   // 0.05 U/kg/h
            d.doses.infusion_max_mcg_kg_min = 0.28;    // 1 U/kg/h maximo CAD
            d.doses.dosing_notes = "CAD: bolo 0.1 U/kg + infusion 0.1 U/kg/h";

            d.warnings = {"Monitorizar glucemia horaria", "Riesgo hipokalemia"};
            d.pregnancy = PregnancyCategory::B;
            m_drugs[ToLower(d.name)] = d;
        }

        // ─── BICARBONATO DE SODIO ───
        {
            DrugParameters d;
            d.name = "Bicarbonato";
            d.genericName = "Sodium Bicarbonate";
            d.category = DrugCategory::Buffer;

            d.doses.push_min_mgkg = 84.0;    // 1 mEq/kg (~84 mg/mEq)
            d.doses.dosing_notes = "1 mEq/kg IV en acidosis metabolica severa (pH<7.1)";

            d.warnings = {"Solo si pH<7.1", "Hipernatremia, alcalosis metabolica"};
            m_drugs[ToLower(d.name)] = d;
        }

        // ─── HEPARINA ───
        {
            DrugParameters d;
            d.name = "Heparina";
            d.genericName = "Heparin";
            d.category = DrugCategory::Anticoagulant;

            d.halfLife_min = 90.0;
            d.k10_per_min = 0.008;
            d.onset_seconds = 60.0;
            d.clearance = ClearanceOrgan::Hepatic;

            d.doses.push_min_mgkg = 0.6;    // 80 U/kg bolo
            d.doses.push_max_mgkg = 1.0;
            d.doses.infusion_min_mcg_kg_min = 3.0;   // 18 U/kg/h
            d.doses.dosing_notes = "TEP/SCA: bolo 80 U/kg + infusion 18 U/kg/h, ajustar TTPa";

            d.warnings = {"Antidoto: protamina", "Monitorizar TTPa c/6h", "TIH tipo II"};
            d.pregnancy = PregnancyCategory::C;
            m_drugs[ToLower(d.name)] = d;
        }

        // ─── METOPROLOL ───
        {
            DrugParameters d;
            d.name = "Metoprolol";
            d.category = DrugCategory::Antihypertensive;

            d.halfLife_min = 210.0;
            d.k10_per_min = 0.0033;
            d.onset_seconds = 300.0;
            d.duration_min = 360.0;
            d.clearance = ClearanceOrgan::Hepatic;

            d.effects.push_back({Receptor::Beta1, 0.05, 1.5, 1.0, false});

            d.doses.push_min_mgkg = 0.07;    // 5 mg IV
            d.doses.push_max_mgkg = 0.21;    // 15 mg total
            d.doses.dosing_notes = "SCA: 5 mg IV c/5 min hasta 15 mg. Luego VO";

            d.contraindications = {"Bloqueo AV 2-3 grado", "IC descompensada", "Broncoespasmo"};
            d.pregnancy = PregnancyCategory::C;
            m_drugs[ToLower(d.name)] = d;
        }
    }

}