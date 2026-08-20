// ═══════════════════════════════════════════════════════════════
//   ENARM.Pharmacology - Tipos comunes
// ═══════════════════════════════════════════════════════════════
#pragma once

#include <cstdint>
#include <string>

namespace ENARM::Pharmacology {

    // Ruta de administracion
    enum class Route : uint8_t {
        IV_Push,          // Bolo IV rapido
        IV_Infusion,      // Infusion continua
        IM,               // Intramuscular
        SC,               // Subcutaneo
        PO,               // Via oral
        SL,               // Sublingual
        Inhaled,          // Inhalado
        Topical,          // Topico
        Rectal,
        Intranasal
    };

    // Categoria terapeutica
    enum class DrugCategory : uint8_t {
        Vasopressor,          // NA, Adrenalina
        Inotrope,             // Dobutamina, Milrinona
        Vasodilator,          // Nitroglicerina, Nitroprusiato
        Sedative,             // Propofol, Midazolam
        Analgesic,            // Fentanilo, Morfina
        Anesthetic,           // Etomidato, Tiopental, Ketamina
        NeuromuscularBlocker, // Roc, Succ, Vec
        Antiarrhythmic,       // Amiodarona, Adenosina
        Antihypertensive,     // Metoprolol, Labetalol
        Diuretic,             // Furosemida
        Anticoagulant,        // Heparina, Enoxaparina
        Antibiotic,
        Bronchodilator,
        Corticosteroid,
        InsulinInfusion,
        Electrolyte,          // KCl, CaCl2, MgSO4
        Buffer,               // Bicarbonato
        Other
    };

    // Receptores/dianas farmacologicas
    enum class Receptor : uint8_t {
        Alpha1,       // Vasoconstriccion arterial
        Alpha2,       // Sedacion, hipotension central
        Beta1,        // Cronotropo/inotropo cardiaco
        Beta2,        // Broncodilatacion, vasodilatacion
        Dopamine1,    // Vasodilatacion renal/mesenterica
        Dopamine2,    // Antiemetico/antipsicotico
        GABA_A,       // Sedacion (BZD, propofol)
        MuOpioid,     // Analgesia (fentanilo, morfina)
        NMDA,         // Antagonismo (ketamina)
        Muscarinic,   // Antimuscarinico (atropina)
        Nicotinic_NMJ,// Bloqueo NM
        H1,           // Antihistaminico
        Vasopressin_V1,// Vasoconstriccion
        Adenosine_A1, // Bloqueo AV
        L_Ca_Channel, // Bloqueo canal Ca (verapamil)
        Na_Channel,   // Bloqueo Na (lidocaina)
        K_Channel     // Bloqueo K (amiodarona)
    };

    // Metabolismo/eliminacion
    enum class ClearanceOrgan : uint8_t {
        Hepatic,      // CYP450
        Renal,        // Filtracion/secrecion
        HepaticRenal, // Ambas
        Plasma,       // Esterasas plasmaticas (succ, remifentanilo)
        Hoffman       // Degradacion espontanea (cisatracurio)
    };

    // Categoria seguridad embarazo (FDA)
    enum class PregnancyCategory : uint8_t {
        A, B, C, D, X, Unknown
    };

}