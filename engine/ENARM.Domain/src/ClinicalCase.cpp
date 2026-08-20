// ═══════════════════════════════════════════════════════════════
//   ENARM.Domain - ClinicalCase Implementation
// ═══════════════════════════════════════════════════════════════
#include "ENARM/Domain/ClinicalCase.h"

#include <nlohmann/json.hpp>
#include <magic_enum/magic_enum.hpp>

using json = nlohmann::json;

namespace ENARM::Domain {

    std::string ClinicalCase::ToJSON() const {
        json j;
        j["id"] = id.ToString();
        j["title"] = title;
        j["chief_complaint"] = chiefComplaint;
        j["specialty"] = std::string(magic_enum::enum_name(specialty));
        j["difficulty"] = std::string(magic_enum::enum_name(difficulty));
        j["setting"] = std::string(magic_enum::enum_name(setting));

        j["patient"] = {
            {"name", patient.fullName},
            {"age", patient.age},
            {"sex", std::string(magic_enum::enum_name(patient.sex))},
            {"weight_kg", patient.weight_kg},
            {"height_cm", patient.height_cm}
        };

        j["history_of_present_illness"] = historyOfPresentIllness;

        json symptoms = json::array();
        for (const auto& s : presentingSymptoms) {
            symptoms.push_back({
                {"name", s.name},
                {"location", s.location},
                {"quality", s.quality},
                {"severity", std::string(magic_enum::enum_name(s.severity))},
                {"onset", std::string(magic_enum::enum_name(s.onset))},
                {"patient_description", s.patientDescription}
            });
        }
        j["symptoms"] = symptoms;

        j["expected_diagnosis"] = {
            {"primary", expectedDiagnosis.primary},
            {"differentials", expectedDiagnosis.differentials},
            {"icd10", expectedDiagnosis.icd10Code}
        };

        json studies = json::array();
        for (const auto& s : expectedStudies) {
            studies.push_back({
                {"name", s.name},
                {"category", s.category},
                {"required", s.required},
                {"score_weight", s.scoreWeight},
                {"justification", s.justification}
            });
        }
        j["expected_studies"] = studies;

        json treatments = json::array();
        for (const auto& t : expectedTreatments) {
            treatments.push_back({
                {"intervention", t.intervention},
                {"category", t.category},
                {"required", t.required},
                {"score_weight", t.scoreWeight},
                {"contraindications", t.contraindications}
            });
        }
        j["expected_treatments"] = treatments;

        j["learning_objectives"] = learningObjectives;
        j["pubmed_references"] = pubmedReferences;
        j["clinical_guidelines"] = clinicalGuidelines;
        j["teaching_points"] = teachingPoints;
        j["tags"] = tags;
        j["version"] = version;

        return j.dump(2);
    }

    ClinicalCase ClinicalCase::FromJSON(const std::string& jsonStr) {
        ClinicalCase c;
        auto j = json::parse(jsonStr);
        c.title = j.value("title", "");
        c.chiefComplaint = j.value("chief_complaint", "");
        c.historyOfPresentIllness = j.value("history_of_present_illness", "");
        return c;
    }

}