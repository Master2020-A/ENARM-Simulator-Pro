-- ═══════════════════════════════════════════════════════════════
--   ENARM Database - Indices avanzados (002)
-- ═══════════════════════════════════════════════════════════════

CREATE INDEX IF NOT EXISTS idx_cases_patient_gin
    ON clinical_cases USING GIN (patient_json);

CREATE INDEX IF NOT EXISTS idx_cases_symptoms_gin
    ON clinical_cases USING GIN (symptoms_json);

CREATE INDEX IF NOT EXISTS idx_cases_diagnosis_gin
    ON clinical_cases USING GIN (expected_diagnosis_json);

CREATE INDEX IF NOT EXISTS idx_sessions_scores_gin
    ON study_sessions USING GIN (scores_json);

CREATE INDEX IF NOT EXISTS idx_events_details_gin
    ON session_events USING GIN (details_json);

CREATE INDEX IF NOT EXISTS idx_mcq_options_gin
    ON mcq_bank USING GIN (options_json);

CREATE INDEX IF NOT EXISTS idx_profiles_weak_gin
    ON student_profiles USING GIN (weak_areas_json);

CREATE INDEX IF NOT EXISTS idx_cases_title_trgm
    ON clinical_cases USING GIN (title gin_trgm_ops);

CREATE INDEX IF NOT EXISTS idx_mcq_vignette_trgm
    ON mcq_bank USING GIN (vignette gin_trgm_ops);