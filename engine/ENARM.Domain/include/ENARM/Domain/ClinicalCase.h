// ═══════════════════════════════════════════════════════════════
//   ENARM.Domain - Caso clinico
// ═══════════════════════════════════════════════════════════════
#pragma once

#include "ENARM/Common/UUID.h"
#include "ENARM/Domain/Patient.h"
#include "ENARM/Domain/Symptom.h"
#include "ENARM/Domain/MedicalSpecialty.h"

#include <string>
#include <vector>
#include <chrono>

namespace ENARM::Domain {

    enum class CaseDifficulty {
        Basic,
        Intermediate,
        Advanced,
        Expert
    };

    enum class ClinicalSetting {
        OutpatientConsult,
        Emergency,
        InpatientWard,
        IntensiveCareUnit,
        OperatingRoom,
        NeonatalICU,
        DeliveryRoom,
        PrimaryCare,
        HomeVisit
    };

    struct ExpectedDiagnosis {
        std::string primary;
        std::vector<std::string> differentials;
        std::string icd10Code;
    };

    struct ExpectedStudy {
        std::string name;
        std::string category;
        bool required{true};
        int scoreWeight{5};
        std::string justification;
    };

    struct ExpectedTreatment {
        std::string intervention;
        std::string category;
        bool required{true};
        int scoreWeight{10};
        std::string contraindications;
    };

    struct ClinicalCase {
        Common::UUID id{Common::UUID::Generate()};
        std::string title;
        std::string chiefComplaint;

        MedicalSpecialty specialty{MedicalSpecialty::Unknown};
        std::vector<MedicalSpecialty> subSpecialties;
        CaseDifficulty difficulty{CaseDifficulty::Intermediate};
        ClinicalSetting setting{ClinicalSetting::Emergency};

        Patient patient;
        std::vector<Symptom> presentingSymptoms;
        std::string historyOfPresentIllness;

        ExpectedDiagnosis expectedDiagnosis;
        std::vector<ExpectedStudy> expectedStudies;
        std::vector<ExpectedTreatment> expectedTreatments;
        std::string expectedFollowUp;

        std::vector<std::string> learningObjectives;
        std::vector<std::string> pubmedReferences;
        std::vector<std::string> clinicalGuidelines;
        std::string teachingPoints;

        std::string authorName;
        std::string reviewedBy;
        std::chrono::year_month_day createdAt;
        std::chrono::year_month_day lastReviewed;
        int version{1};
        std::vector<std::string> tags;

        [[nodiscard]] std::string ToJSON() const;
        [[nodiscard]] static ClinicalCase FromJSON(const std::string& json);
    };

}