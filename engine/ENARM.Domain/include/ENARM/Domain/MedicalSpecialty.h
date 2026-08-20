// ═══════════════════════════════════════════════════════════════
//   ENARM.Domain - Especialidades medicas
// ═══════════════════════════════════════════════════════════════
#pragma once

#include <cstdint>

namespace ENARM::Domain {

    enum class MedicalSpecialty : uint8_t {
        InternalMedicine,
        Cardiology,
        Pneumology,
        Gastroenterology,
        Neurology,
        Gynecology,
        Obstetrics,
        Pediatrics,
        Neonatology,
        Emergency,
        GeneralSurgery,
        Traumatology,
        Endocrinology,
        Nephrology,
        Hematology,
        Infectology,
        Psychiatry,
        Dermatology,
        Ophthalmology,
        Otorhinolaryngology,
        Urology,
        Oncology,
        Rheumatology,
        Anesthesiology,
        FamilyMedicine,
        IntensiveCare,
        Radiology,
        Pathology,
        Unknown
    };

}