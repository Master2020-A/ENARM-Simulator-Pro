// ═══════════════════════════════════════════════════════════════
//   ENARM.Simulation - Bridge farmaco -> fisiologia
// ═══════════════════════════════════════════════════════════════
#pragma once

#include "ENARM/Physiology/HemodynamicModel.h"
#include "ENARM/Physiology/RespiratoryModel.h"
#include "ENARM/Pharmacology/DrugAdministration.h"

namespace ENARM::Simulation {

    // Aplica los efectos combinados de todos los farmacos activos
    // sobre los modelos fisiologicos
    class PhysiologyPharmacologyBridge {
    public:
        // Actualiza los modelos aplicando los efectos combinados
        // Se debe llamar cada tick despues de actualizar farmacos
        static void ApplyPharmacologicalEffects(
            Physiology::HemodynamicModel& heart,
            Physiology::RespiratoryModel& lungs,
            const Pharmacology::DrugAdministration& drugs);

    private:
        // Convierte efecto (0-1) a factor multiplicativo fisiologico
        static double EffectToFactor(double effect, double maxFactor);
    };

}