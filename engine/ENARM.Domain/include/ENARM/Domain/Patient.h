// ═══════════════════════════════════════════════════════════════
//   ENARM.Domain - Patient
// ═══════════════════════════════════════════════════════════════
#pragma once

#include "ENARM/Common/UUID.h"
#include "ENARM/Domain/Sex.h"
#include "ENARM/Domain/VitalSigns.h"

#include <string>
#include <vector>
#include <optional>
#include <chrono>

namespace ENARM::Domain {

    struct MedicalHistory {
        std::string condition;
        std::chrono::year_month_day diagnosisDate;
        std::string treatment;
        bool controlled{true};
        std::string notes;
    };

    struct Allergy {
        std::string substance;
        std::string reaction;
        std::string severity;
    };

    struct CurrentMedication {
        std::string name;
        std::string dose;
        std::string frequency;
        std::string indication;
        std::chrono::year_month_day startDate;
    };

    struct FamilyHistory {
        std::string relationship;
        std::string condition;
        std::optional<int> ageAtDiagnosis;
        bool deceased{false};
    };

    struct ObGynHistory {
        std::optional<int> menarcheAge;
        std::optional<int> gravidity;
        std::optional<int> parity;
        std::optional<int> abortions;
        std::optional<int> cesareans;
        std::optional<std::chrono::year_month_day> lastMenstrualPeriod;
        bool pregnant{false};
        std::optional<int> gestationalWeeks;
    };

    struct Lifestyle {
        bool smoker{false};
        int cigarettesPerDay{0};
        int smokingYears{0};

        bool alcoholUse{false};
        std::string alcoholFrequency;

        bool drugUse{false};
        std::vector<std::string> drugs;

        std::string exerciseFrequency;
        std::string diet;
        std::string occupation;
    };

    struct Patient {
        Common::UUID id{Common::UUID::Generate()};
        std::string fullName;
        int age{0};
        Sex sex{Sex::Unknown};

        double weight_kg{70.0};
        double height_cm{170.0};

        VitalSigns currentVitals;

        std::vector<MedicalHistory> medicalHistory;
        std::vector<Allergy> allergies;
        std::vector<CurrentMedication> currentMedications;
        std::vector<FamilyHistory> familyHistory;
        std::optional<ObGynHistory> obGynHistory;
        Lifestyle lifestyle;

        std::string birthPlace;
        std::string residence;
        std::string educationLevel;
        std::string maritalStatus;

        [[nodiscard]] double GetBMI() const noexcept {
            if (height_cm <= 0) return 0.0;
            const double h_m = height_cm / 100.0;
            return weight_kg / (h_m * h_m);
        }

        [[nodiscard]] std::string GetBMICategory() const;
        [[nodiscard]] std::string ToDisplayString() const;
    };

}