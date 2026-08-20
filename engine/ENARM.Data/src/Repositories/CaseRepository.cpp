// ═══════════════════════════════════════════════════════════════
//   ENARM.Data - CaseRepository Implementation (libpqxx v7.10+)
// ═══════════════════════════════════════════════════════════════
#include "ENARM/Data/Repositories/CaseRepository.h"
#include "ENARM/Data/JsonHelpers.h"

namespace ENARM::Data {

    Common::Result<std::string> CaseRepository::Insert(const ClinicalCaseRecord& c) {
        try {
            auto conn = m_db->Acquire();
            pqxx::work tx(conn.Get());

            auto row = tx.exec(
                "INSERT INTO clinical_cases ("
                "  title, slug, chief_complaint, specialty, sub_specialty,"
                "  difficulty, setting, patient_json, history_present_illness,"
                "  symptoms_json, expected_diagnosis_json, expected_studies_json,"
                "  expected_treatments_json, expected_followup, learning_objectives_json,"
                "  pubmed_pmids, guidelines_ids, teaching_points, author_name, is_published"
                ") VALUES ("
                "  $1, $2, $3, $4, $5,"
                "  $6, $7, $8::jsonb, $9,"
                "  $10::jsonb, $11::jsonb, $12::jsonb,"
                "  $13::jsonb, $14, $15::jsonb,"
                "  $16::text[], $17::text[], $18, $19, $20"
                ") RETURNING id",
                pqxx::params{
                    c.title, c.slug, c.chiefComplaint, c.specialty,
                    c.subSpecialty.value_or(""),
                    c.difficulty, c.setting,
                    c.patient.dump(),
                    c.historyPresentIllness,
                    c.symptoms.dump(),
                    c.expectedDiagnosis.dump(),
                    c.expectedStudies.dump(),
                    c.expectedTreatments.dump(),
                    c.expectedFollowup.value_or(""),
                    c.learningObjectives.dump(),
                    ToPgTextArray(c.pubmedPmids),
                    ToPgTextArray(c.guidelinesIds),
                    c.teachingPoints.value_or(""),
                    c.authorName.value_or(""),
                    c.isPublished
                }
            ).one_row();

            tx.commit();
            return Common::Result<std::string>::Ok(row["id"].as<std::string>());
        } catch (const std::exception& e) {
            return Common::Err<std::string>(500, e.what(), "case_repo");
        }
    }

    Common::Result<void> CaseRepository::Update(const ClinicalCaseRecord& c) {
        try {
            auto conn = m_db->Acquire();
            pqxx::work tx(conn.Get());
            tx.exec(
                "UPDATE clinical_cases SET "
                "  title=$2, chief_complaint=$3, specialty=$4, difficulty=$5, setting=$6, "
                "  patient_json=$7::jsonb, symptoms_json=$8::jsonb "
                "WHERE id=$1::uuid",
                pqxx::params{c.id, c.title, c.chiefComplaint, c.specialty,
                             c.difficulty, c.setting,
                             c.patient.dump(), c.symptoms.dump()}
            );
            tx.commit();
            return Common::Ok();
        } catch (const std::exception& e) {
            return Common::Err<void>(500, e.what(), "case_repo");
        }
    }

    std::optional<ClinicalCaseRecord> CaseRepository::FindById(const std::string& id) {
        try {
            auto conn = m_db->Acquire();
            pqxx::read_transaction tx(conn.Get());
            auto res = tx.exec(
                "SELECT * FROM clinical_cases WHERE id = $1::uuid",
                pqxx::params{id});
            if (res.empty()) return std::nullopt;
            return RowToRecord(res[0]);
        } catch (...) { return std::nullopt; }
    }

    std::optional<ClinicalCaseRecord> CaseRepository::FindBySlug(const std::string& slug) {
        try {
            auto conn = m_db->Acquire();
            pqxx::read_transaction tx(conn.Get());
            auto res = tx.exec(
                "SELECT * FROM clinical_cases WHERE slug = $1",
                pqxx::params{slug});
            if (res.empty()) return std::nullopt;
            return RowToRecord(res[0]);
        } catch (...) { return std::nullopt; }
    }

    std::vector<ClinicalCaseRecord> CaseRepository::FindBySpecialty(
        const std::string& specialty, bool onlyPublished)
    {
        std::vector<ClinicalCaseRecord> out;
        try {
            auto conn = m_db->Acquire();
            pqxx::read_transaction tx(conn.Get());
            std::string sql = "SELECT * FROM clinical_cases WHERE specialty = $1";
            if (onlyPublished) sql += " AND is_published = TRUE";
            sql += " ORDER BY created_at DESC";
            auto res = tx.exec(sql, pqxx::params{specialty});
            for (const auto& row : res) out.push_back(RowToRecord(row));
        } catch (...) {}
        return out;
    }

    std::vector<ClinicalCaseRecord> CaseRepository::FindAll(bool onlyPublished, int limit) {
        std::vector<ClinicalCaseRecord> out;
        try {
            auto conn = m_db->Acquire();
            pqxx::read_transaction tx(conn.Get());
            std::string sql = "SELECT * FROM clinical_cases";
            if (onlyPublished) sql += " WHERE is_published = TRUE";
            sql += " ORDER BY created_at DESC LIMIT $1";
            auto res = tx.exec(sql, pqxx::params{limit});
            for (const auto& row : res) out.push_back(RowToRecord(row));
        } catch (...) {}
        return out;
    }

    size_t CaseRepository::Count() {
        try {
            auto conn = m_db->Acquire();
            pqxx::read_transaction tx(conn.Get());
            auto row = tx.exec("SELECT COUNT(*) FROM clinical_cases").one_row();
            return row[0].as<size_t>();
        } catch (...) { return 0; }
    }

    size_t CaseRepository::CountBySpecialty(const std::string& specialty) {
        try {
            auto conn = m_db->Acquire();
            pqxx::read_transaction tx(conn.Get());
            auto row = tx.exec(
                "SELECT COUNT(*) FROM clinical_cases WHERE specialty = $1",
                pqxx::params{specialty}
            ).one_row();
            return row[0].as<size_t>();
        } catch (...) { return 0; }
    }

    Common::Result<void> CaseRepository::Publish(const std::string& caseId) {
        try {
            auto conn = m_db->Acquire();
            pqxx::work tx(conn.Get());
            tx.exec(
                "UPDATE clinical_cases SET is_published=TRUE, published_at=NOW() "
                "WHERE id=$1::uuid",
                pqxx::params{caseId});
            tx.commit();
            return Common::Ok();
        } catch (const std::exception& e) {
            return Common::Err<void>(500, e.what(), "case_repo");
        }
    }

    Common::Result<void> CaseRepository::Unpublish(const std::string& caseId) {
        try {
            auto conn = m_db->Acquire();
            pqxx::work tx(conn.Get());
            tx.exec(
                "UPDATE clinical_cases SET is_published=FALSE WHERE id=$1::uuid",
                pqxx::params{caseId});
            tx.commit();
            return Common::Ok();
        } catch (const std::exception& e) {
            return Common::Err<void>(500, e.what(), "case_repo");
        }
    }

    Common::Result<void> CaseRepository::AddTag(const std::string& caseId, const std::string& tag) {
        try {
            auto conn = m_db->Acquire();
            pqxx::work tx(conn.Get());
            tx.exec(
                "INSERT INTO case_tags (case_id, tag) VALUES ($1::uuid, $2) "
                "ON CONFLICT DO NOTHING",
                pqxx::params{caseId, tag});
            tx.commit();
            return Common::Ok();
        } catch (const std::exception& e) {
            return Common::Err<void>(500, e.what(), "case_repo");
        }
    }

    std::vector<std::string> CaseRepository::GetTags(const std::string& caseId) {
        std::vector<std::string> tags;
        try {
            auto conn = m_db->Acquire();
            pqxx::read_transaction tx(conn.Get());
            auto res = tx.exec(
                "SELECT tag FROM case_tags WHERE case_id=$1::uuid ORDER BY tag",
                pqxx::params{caseId});
            for (const auto& row : res) tags.push_back(row[0].as<std::string>());
        } catch (...) {}
        return tags;
    }

}