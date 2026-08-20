// ═══════════════════════════════════════════════════════════════
//   ENARM.Data - CaseRepository
// ═══════════════════════════════════════════════════════════════
#pragma once

#include "ENARM/Common/Result.h"
#include "ENARM/Data/DatabaseManager.h"
#include "ENARM/Data/JsonHelpers.h"

#include <memory>
#include <string>
#include <vector>
#include <optional>
#include <nlohmann/json.hpp>

namespace ENARM::Data {

    struct ClinicalCaseRecord {
        std::string id;
        std::string title;
        std::string slug;
        std::string chiefComplaint;
        std::string specialty;
        std::optional<std::string> subSpecialty;
        std::string difficulty;
        std::string setting;
        nlohmann::json patient;
        std::string historyPresentIllness;
        nlohmann::json symptoms;
        nlohmann::json expectedDiagnosis;
        nlohmann::json expectedStudies;
        nlohmann::json expectedTreatments;
        std::optional<std::string> expectedFollowup;
        nlohmann::json learningObjectives;
        std::vector<std::string> pubmedPmids;
        std::vector<std::string> guidelinesIds;
        std::optional<std::string> teachingPoints;
        std::optional<std::string> authorName;
        int version{1};
        bool isPublished{false};
    };

    class CaseRepository {
    public:
        explicit CaseRepository(std::shared_ptr<DatabaseManager> db) : m_db(db) {}

        Common::Result<std::string> Insert(const ClinicalCaseRecord& c);
        Common::Result<void> Update(const ClinicalCaseRecord& c);

        std::optional<ClinicalCaseRecord> FindById(const std::string& id);
        std::optional<ClinicalCaseRecord> FindBySlug(const std::string& slug);
        std::vector<ClinicalCaseRecord> FindBySpecialty(const std::string& specialty,
                                                          bool onlyPublished = false);
        std::vector<ClinicalCaseRecord> FindAll(bool onlyPublished = false, int limit = 100);

        [[nodiscard]] size_t Count();
        [[nodiscard]] size_t CountBySpecialty(const std::string& specialty);

        Common::Result<void> Publish(const std::string& caseId);
        Common::Result<void> Unpublish(const std::string& caseId);

        Common::Result<void> AddTag(const std::string& caseId, const std::string& tag);
        std::vector<std::string> GetTags(const std::string& caseId);

        // Template para funcionar con row y row_ref
        template <typename Row>
        static ClinicalCaseRecord RowToRecord(const Row& row) {
            ClinicalCaseRecord c;
            c.id                     = row["id"].template as<std::string>();
            c.title                  = row["title"].template as<std::string>();
            c.slug                   = row["slug"].template as<std::string>();
            c.chiefComplaint         = row["chief_complaint"].template as<std::string>();
            c.specialty              = row["specialty"].template as<std::string>();
            c.subSpecialty           = GetOptString(row, "sub_specialty");
            c.difficulty             = row["difficulty"].template as<std::string>();
            c.setting                = row["setting"].template as<std::string>();
            c.patient                = GetJson(row, "patient_json");
            c.historyPresentIllness  = row["history_present_illness"].is_null()
                                        ? "" : row["history_present_illness"].template as<std::string>();
            c.symptoms               = GetJson(row, "symptoms_json");
            c.expectedDiagnosis      = GetJson(row, "expected_diagnosis_json");
            c.expectedStudies        = GetJson(row, "expected_studies_json");
            c.expectedTreatments     = GetJson(row, "expected_treatments_json");
            c.expectedFollowup       = GetOptString(row, "expected_followup");
            c.learningObjectives     = GetJson(row, "learning_objectives_json");
            c.pubmedPmids            = GetStringArray(row, "pubmed_pmids");
            c.guidelinesIds          = GetStringArray(row, "guidelines_ids");
            c.teachingPoints         = GetOptString(row, "teaching_points");
            c.authorName             = GetOptString(row, "author_name");
            c.version                = row["version"].template as<int>();
            c.isPublished            = row["is_published"].template as<bool>();
            return c;
        }

    private:
        std::shared_ptr<DatabaseManager> m_db;
    };

}