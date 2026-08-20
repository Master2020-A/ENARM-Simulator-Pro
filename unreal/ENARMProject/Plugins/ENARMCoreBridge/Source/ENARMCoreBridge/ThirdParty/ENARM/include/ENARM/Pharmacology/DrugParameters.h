// ═══════════════════════════════════════════════════════════════
//   ENARM.Pharmacology - Parametros PK/PD por farmaco
// ═══════════════════════════════════════════════════════════════
#pragma once

#include "ENARM/Pharmacology/DrugTypes.h"
#include <string>
#include <vector>

namespace ENARM::Pharmacology {

    // Efecto de un farmaco sobre un receptor
    struct ReceptorEffect {
        Receptor receptor;
        double EC50_mcgmL;       // Concentracion para 50% efecto maximo
        double Hill_coefficient; // Cooperatividad (1-3 tipico)
        double Emax_potency;     // Potencia relativa (0-1, 1=agonismo pleno)
        bool isAgonist{true};    // true=agonista, false=antagonista
    };

    // Rango de dosis
    struct DoseRange {
        // IV push (bolo)
        double push_min_mgkg{0.0};
        double push_max_mgkg{0.0};

        // Infusion continua
        double infusion_min_mcg_kg_min{0.0};
        double infusion_max_mcg_kg_min{0.0};

        // Oral (adultos)
        double po_min_mg{0.0};
        double po_max_mg{0.0};

        // Notas de dosificacion
        std::string dosing_notes;
    };

    // ═══════════════════════════════════════════════════════════════
    //  Parametros completos de un farmaco
    // ═══════════════════════════════════════════════════════════════
    struct DrugParameters {
        // Identificacion
        std::string name;              // "Norepinefrina"
        std::string genericName;       // "Norepinephrine"
        std::string tradeName;         // "Levophed"
        std::string atcCode;           // "C01CA03"
        DrugCategory category{DrugCategory::Other};

        // ─── Farmacocinetica (2 compartimentos) ───
        double Vc_L_kg{0.5};          // Volumen central L/kg
        double Vp_L_kg{1.0};          // Volumen periferico L/kg
        double Cl_mL_kg_min{5.0};     // Clearance mL/kg/min
        double halfLife_min{60.0};    // T1/2 eliminacion (min)
        double bioavailability{1.0};  // F (fraccion absorbida)
        double proteinBinding{0.5};   // Fraccion unida proteinas

        ClearanceOrgan clearance{ClearanceOrgan::Hepatic};

        // ─── Cinetica intercompartimental ───
        double k10_per_min{0.02};     // Eliminacion
        double k12_per_min{0.05};     // Central -> periferico
        double k21_per_min{0.03};     // Periferico -> central

        // ─── Onset/duracion ───
        double onset_seconds{60.0};   // Tiempo hasta primer efecto
        double peak_min{5.0};         // Tiempo hasta pico
        double duration_min{30.0};    // Duracion efecto

        // ─── Farmacodinamica ───
        std::vector<ReceptorEffect> effects;

        // ─── Dosis ───
        DoseRange doses;
        std::vector<Route> availableRoutes;

        // ─── Contraindicaciones y advertencias ───
        std::vector<std::string> contraindications;
        std::vector<std::string> sideEffects;
        std::vector<std::string> warnings;
        PregnancyCategory pregnancy{PregnancyCategory::Unknown};

        // ─── Presentacion ───
        std::string concentration;    // "4 mg/4 mL amp"
        std::string dilution;         // "4 mg en 250 mL DW5%"
    };

}