-- ============================================================
-- Sprint 6: Tablas de evaluacion por competencias
-- ============================================================

CREATE TABLE IF NOT EXISTS assessment_rubrics (
    id          UUID PRIMARY KEY DEFAULT gen_random_uuid(),
    case_id     UUID REFERENCES clinical_cases(id) ON DELETE CASCADE,
    name        VARCHAR(200) NOT NULL,
    total_max_points INTEGER DEFAULT 185,
    rubric_data JSONB NOT NULL,
    created_at  TIMESTAMPTZ DEFAULT NOW()
);

CREATE TABLE IF NOT EXISTS assessment_results (
    id          UUID PRIMARY KEY DEFAULT gen_random_uuid(),
    session_id  UUID REFERENCES sessions(id) ON DELETE SET NULL,
    student_id  UUID REFERENCES students(id) ON DELETE SET NULL,
    case_id     UUID REFERENCES clinical_cases(id) ON DELETE SET NULL,

    -- Scores por dimension
    interview_score     INTEGER DEFAULT 0,
    interview_max       INTEGER DEFAULT 40,
    physical_exam_score INTEGER DEFAULT 0,
    physical_exam_max   INTEGER DEFAULT 25,
    studies_score       INTEGER DEFAULT 0,
    studies_max         INTEGER DEFAULT 30,
    diagnosis_score     INTEGER DEFAULT 0,
    diagnosis_max       INTEGER DEFAULT 30,
    treatment_score     INTEGER DEFAULT 0,
    treatment_max       INTEGER DEFAULT 40,
    followup_score      INTEGER DEFAULT 0,
    followup_max        INTEGER DEFAULT 20,

    -- Global
    total_score     INTEGER DEFAULT 0,
    total_max       INTEGER DEFAULT 185,
    percentage      DECIMAL(5,2) DEFAULT 0,
    performance_level VARCHAR(20),
    time_efficient  BOOLEAN DEFAULT FALSE,

    -- Tiempo
    total_time_seconds INTEGER,

    -- Detalle completo (JSON de PerformanceReport)
    detailed_report JSONB,
    narrative_feedback TEXT,

    -- Timestamps
    completed_at TIMESTAMPTZ DEFAULT NOW(),
    created_at   TIMESTAMPTZ DEFAULT NOW()
);

CREATE INDEX IF NOT EXISTS idx_ar_student   ON assessment_results(student_id);
CREATE INDEX IF NOT EXISTS idx_ar_case      ON assessment_results(case_id);
CREATE INDEX IF NOT EXISTS idx_ar_completed ON assessment_results(completed_at DESC);

-- Vista de progreso por estudiante
CREATE OR REPLACE VIEW student_progress AS
SELECT
    s.id                    AS student_id,
    s.name                  AS student_name,
    COUNT(ar.id)            AS total_sessions,
    ROUND(AVG(ar.percentage)::numeric, 1)           AS avg_score,
    MAX(ar.percentage)                              AS best_score,
    MIN(ar.percentage)                              AS worst_score,
    ROUND(AVG(ar.interview_score::float     / NULLIF(ar.interview_max,0)     * 100)::numeric, 1) AS avg_interview,
    ROUND(AVG(ar.physical_exam_score::float / NULLIF(ar.physical_exam_max,0) * 100)::numeric, 1) AS avg_physical_exam,
    ROUND(AVG(ar.studies_score::float       / NULLIF(ar.studies_max,0)       * 100)::numeric, 1) AS avg_studies,
    ROUND(AVG(ar.diagnosis_score::float     / NULLIF(ar.diagnosis_max,0)     * 100)::numeric, 1) AS avg_diagnosis,
    ROUND(AVG(ar.treatment_score::float     / NULLIF(ar.treatment_max,0)     * 100)::numeric, 1) AS avg_treatment,
    ROUND(AVG(ar.followup_score::float      / NULLIF(ar.followup_max,0)      * 100)::numeric, 1) AS avg_followup
FROM students s
LEFT JOIN assessment_results ar ON s.id = ar.student_id
GROUP BY s.id, s.name;
