-- ═══════════════════════════════════════════════════════════════
--   ENARM Database - Schema Initial (001) - v2 PostgreSQL 18
--   Usa gen_random_uuid() nativo (no requiere uuid-ossp)
-- ═══════════════════════════════════════════════════════════════

-- Asegurar extensiones core
CREATE EXTENSION IF NOT EXISTS pgcrypto;
CREATE EXTENSION IF NOT EXISTS pg_trgm;

-- ─── USUARIOS ───
CREATE TABLE IF NOT EXISTS users (
    id UUID PRIMARY KEY DEFAULT gen_random_uuid(),
    email VARCHAR(255) UNIQUE NOT NULL,
    password_hash VARCHAR(255) NOT NULL,
    full_name VARCHAR(255) NOT NULL,
    role VARCHAR(50) DEFAULT 'student',
    email_verified BOOLEAN DEFAULT FALSE,
    active BOOLEAN DEFAULT TRUE,
    created_at TIMESTAMPTZ DEFAULT NOW(),
    updated_at TIMESTAMPTZ DEFAULT NOW(),
    last_login TIMESTAMPTZ
);
CREATE INDEX IF NOT EXISTS idx_users_email ON users(email);
CREATE INDEX IF NOT EXISTS idx_users_role ON users(role);

-- ─── ESTUDIANTES ───
CREATE TABLE IF NOT EXISTS students (
    id UUID PRIMARY KEY DEFAULT gen_random_uuid(),
    user_id UUID NOT NULL REFERENCES users(id) ON DELETE CASCADE,
    student_code VARCHAR(50) UNIQUE,
    university VARCHAR(255),
    graduation_year INT,
    current_level VARCHAR(50),
    target_specialty VARCHAR(100),
    enarm_attempt_number INT DEFAULT 1,
    enarm_target_score INT,
    created_at TIMESTAMPTZ DEFAULT NOW()
);
CREATE INDEX IF NOT EXISTS idx_students_user_id ON students(user_id);

-- ─── PERFIL DEL ESTUDIANTE ───
CREATE TABLE IF NOT EXISTS student_profiles (
    id UUID PRIMARY KEY DEFAULT gen_random_uuid(),
    student_id UUID NOT NULL REFERENCES students(id) ON DELETE CASCADE,
    preferred_difficulty VARCHAR(50) DEFAULT 'intermediate',
    daily_study_goal_minutes INT DEFAULT 60,
    voice_enabled BOOLEAN DEFAULT FALSE,
    ui_language VARCHAR(10) DEFAULT 'es-MX',
    total_cases_completed INT DEFAULT 0,
    total_mcq_answered INT DEFAULT 0,
    total_study_minutes INT DEFAULT 0,
    average_case_score NUMERIC(5,2) DEFAULT 0.0,
    average_mcq_score NUMERIC(5,2) DEFAULT 0.0,
    weak_areas_json JSONB DEFAULT '{}',
    strong_areas_json JSONB DEFAULT '{}',
    predicted_enarm_score INT,
    prediction_confidence NUMERIC(3,2),
    last_prediction_at TIMESTAMPTZ,
    updated_at TIMESTAMPTZ DEFAULT NOW(),
    UNIQUE(student_id)
);
CREATE INDEX IF NOT EXISTS idx_student_profiles_student ON student_profiles(student_id);

-- ─── CASOS CLINICOS ───
CREATE TABLE IF NOT EXISTS clinical_cases (
    id UUID PRIMARY KEY DEFAULT gen_random_uuid(),
    title VARCHAR(500) NOT NULL,
    slug VARCHAR(255) UNIQUE NOT NULL,
    chief_complaint TEXT NOT NULL,
    specialty VARCHAR(100) NOT NULL,
    sub_specialty VARCHAR(100),
    difficulty VARCHAR(50) NOT NULL,
    setting VARCHAR(50) NOT NULL,
    patient_json JSONB NOT NULL,
    history_present_illness TEXT,
    symptoms_json JSONB DEFAULT '[]',
    expected_diagnosis_json JSONB NOT NULL,
    expected_studies_json JSONB DEFAULT '[]',
    expected_treatments_json JSONB DEFAULT '[]',
    expected_followup TEXT,
    learning_objectives_json JSONB DEFAULT '[]',
    pubmed_pmids TEXT[],
    guidelines_ids TEXT[],
    teaching_points TEXT,
    author_name VARCHAR(255),
    reviewed_by VARCHAR(255),
    version INT DEFAULT 1,
    is_published BOOLEAN DEFAULT FALSE,
    created_at TIMESTAMPTZ DEFAULT NOW(),
    updated_at TIMESTAMPTZ DEFAULT NOW(),
    published_at TIMESTAMPTZ
);
CREATE INDEX IF NOT EXISTS idx_cases_specialty ON clinical_cases(specialty);
CREATE INDEX IF NOT EXISTS idx_cases_difficulty ON clinical_cases(difficulty);
CREATE INDEX IF NOT EXISTS idx_cases_setting ON clinical_cases(setting);
CREATE INDEX IF NOT EXISTS idx_cases_slug ON clinical_cases(slug);
CREATE INDEX IF NOT EXISTS idx_cases_published ON clinical_cases(is_published) WHERE is_published = TRUE;

-- ─── VERSIONES DE CASOS ───
CREATE TABLE IF NOT EXISTS case_versions (
    id UUID PRIMARY KEY DEFAULT gen_random_uuid(),
    case_id UUID NOT NULL REFERENCES clinical_cases(id) ON DELETE CASCADE,
    version_number INT NOT NULL,
    full_snapshot_json JSONB NOT NULL,
    change_summary TEXT,
    changed_by VARCHAR(255),
    changed_at TIMESTAMPTZ DEFAULT NOW(),
    UNIQUE(case_id, version_number)
);
CREATE INDEX IF NOT EXISTS idx_case_versions_case ON case_versions(case_id);

-- ─── TAGS DE CASOS ───
CREATE TABLE IF NOT EXISTS case_tags (
    id SERIAL PRIMARY KEY,
    case_id UUID NOT NULL REFERENCES clinical_cases(id) ON DELETE CASCADE,
    tag VARCHAR(100) NOT NULL,
    UNIQUE(case_id, tag)
);
CREATE INDEX IF NOT EXISTS idx_case_tags_case ON case_tags(case_id);
CREATE INDEX IF NOT EXISTS idx_case_tags_tag ON case_tags(tag);

-- ─── SESIONES DE ESTUDIO ───
CREATE TABLE IF NOT EXISTS study_sessions (
    id UUID PRIMARY KEY DEFAULT gen_random_uuid(),
    student_id UUID NOT NULL REFERENCES students(id) ON DELETE CASCADE,
    mode VARCHAR(50) NOT NULL,
    case_id UUID REFERENCES clinical_cases(id),
    started_at TIMESTAMPTZ DEFAULT NOW(),
    ended_at TIMESTAMPTZ,
    duration_seconds INT,
    completed BOOLEAN DEFAULT FALSE,
    scores_json JSONB DEFAULT '{}',
    total_score NUMERIC(5,2),
    max_possible_score NUMERIC(5,2),
    ai_feedback_json JSONB,
    notes TEXT
);
CREATE INDEX IF NOT EXISTS idx_sessions_student ON study_sessions(student_id);
CREATE INDEX IF NOT EXISTS idx_sessions_case ON study_sessions(case_id);
CREATE INDEX IF NOT EXISTS idx_sessions_started ON study_sessions(started_at DESC);
CREATE INDEX IF NOT EXISTS idx_sessions_completed ON study_sessions(completed);

-- ─── EVENTOS DE SESION ───
CREATE TABLE IF NOT EXISTS session_events (
    id UUID PRIMARY KEY DEFAULT gen_random_uuid(),
    session_id UUID NOT NULL REFERENCES study_sessions(id) ON DELETE CASCADE,
    event_time TIMESTAMPTZ DEFAULT NOW(),
    sim_time_seconds NUMERIC(10,3),
    event_type VARCHAR(50) NOT NULL,
    category VARCHAR(50),
    action TEXT,
    details_json JSONB DEFAULT '{}',
    severity INT DEFAULT 0
);
CREATE INDEX IF NOT EXISTS idx_events_session ON session_events(session_id);
CREATE INDEX IF NOT EXISTS idx_events_time ON session_events(session_id, event_time);
CREATE INDEX IF NOT EXISTS idx_events_type ON session_events(event_type);

-- ─── BANCO MCQ ───
CREATE TABLE IF NOT EXISTS mcq_bank (
    id UUID PRIMARY KEY DEFAULT gen_random_uuid(),
    specialty VARCHAR(100) NOT NULL,
    sub_specialty VARCHAR(100),
    difficulty VARCHAR(50) DEFAULT 'intermediate',
    topic VARCHAR(255),
    vignette TEXT NOT NULL,
    question TEXT NOT NULL,
    options_json JSONB NOT NULL,
    correct_answer CHAR(1) NOT NULL,
    explanation_correct TEXT,
    explanation_incorrect_json JSONB,
    pubmed_pmids TEXT[],
    guidelines_ids TEXT[],
    references_text TEXT,
    times_attempted INT DEFAULT 0,
    times_correct INT DEFAULT 0,
    difficulty_index NUMERIC(3,2),
    discrimination_index NUMERIC(3,2),
    author VARCHAR(255),
    reviewed BOOLEAN DEFAULT FALSE,
    is_active BOOLEAN DEFAULT TRUE,
    year_source INT,
    created_at TIMESTAMPTZ DEFAULT NOW(),
    updated_at TIMESTAMPTZ DEFAULT NOW()
);
CREATE INDEX IF NOT EXISTS idx_mcq_specialty ON mcq_bank(specialty);
CREATE INDEX IF NOT EXISTS idx_mcq_difficulty ON mcq_bank(difficulty);
CREATE INDEX IF NOT EXISTS idx_mcq_active ON mcq_bank(is_active) WHERE is_active = TRUE;

-- ─── INTENTOS MCQ ───
CREATE TABLE IF NOT EXISTS mcq_attempts (
    id UUID PRIMARY KEY DEFAULT gen_random_uuid(),
    session_id UUID REFERENCES study_sessions(id) ON DELETE CASCADE,
    student_id UUID NOT NULL REFERENCES students(id) ON DELETE CASCADE,
    mcq_id UUID NOT NULL REFERENCES mcq_bank(id) ON DELETE CASCADE,
    selected_answer CHAR(1),
    is_correct BOOLEAN,
    time_seconds NUMERIC(6,2),
    attempted_at TIMESTAMPTZ DEFAULT NOW()
);
CREATE INDEX IF NOT EXISTS idx_attempts_student ON mcq_attempts(student_id);
CREATE INDEX IF NOT EXISTS idx_attempts_mcq ON mcq_attempts(mcq_id);
CREATE INDEX IF NOT EXISTS idx_attempts_session ON mcq_attempts(session_id);

-- ─── REFERENCIAS PUBMED ───
CREATE TABLE IF NOT EXISTS pubmed_references (
    pmid VARCHAR(20) PRIMARY KEY,
    title TEXT NOT NULL,
    authors TEXT,
    journal VARCHAR(255),
    year INT,
    doi VARCHAR(100),
    abstract TEXT,
    mesh_terms TEXT[],
    fetched_at TIMESTAMPTZ DEFAULT NOW()
);
CREATE INDEX IF NOT EXISTS idx_pubmed_year ON pubmed_references(year DESC);

-- ─── GUIAS CLINICAS ───
CREATE TABLE IF NOT EXISTS clinical_guidelines (
    id VARCHAR(50) PRIMARY KEY,
    title TEXT NOT NULL,
    organization VARCHAR(100),
    year INT,
    url TEXT,
    summary TEXT,
    updated_at TIMESTAMPTZ DEFAULT NOW()
);