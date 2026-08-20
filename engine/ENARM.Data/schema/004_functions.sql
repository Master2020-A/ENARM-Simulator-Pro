-- ═══════════════════════════════════════════════════════════════
--   ENARM Database - Funciones y triggers (004)
-- ═══════════════════════════════════════════════════════════════

CREATE OR REPLACE FUNCTION set_updated_at()
RETURNS TRIGGER AS $$
BEGIN
    NEW.updated_at = NOW();
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

DROP TRIGGER IF EXISTS trg_users_updated ON users;
CREATE TRIGGER trg_users_updated
    BEFORE UPDATE ON users
    FOR EACH ROW EXECUTE FUNCTION set_updated_at();

DROP TRIGGER IF EXISTS trg_cases_updated ON clinical_cases;
CREATE TRIGGER trg_cases_updated
    BEFORE UPDATE ON clinical_cases
    FOR EACH ROW EXECUTE FUNCTION set_updated_at();

DROP TRIGGER IF EXISTS trg_profiles_updated ON student_profiles;
CREATE TRIGGER trg_profiles_updated
    BEFORE UPDATE ON student_profiles
    FOR EACH ROW EXECUTE FUNCTION set_updated_at();

DROP TRIGGER IF EXISTS trg_mcq_updated ON mcq_bank;
CREATE TRIGGER trg_mcq_updated
    BEFORE UPDATE ON mcq_bank
    FOR EACH ROW EXECUTE FUNCTION set_updated_at();

-- Actualizar stats MCQ tras cada intento
CREATE OR REPLACE FUNCTION update_mcq_stats()
RETURNS TRIGGER AS $$
BEGIN
    UPDATE mcq_bank
    SET
        times_attempted = times_attempted + 1,
        times_correct = times_correct + CASE WHEN NEW.is_correct THEN 1 ELSE 0 END,
        difficulty_index = CASE
            WHEN times_attempted + 1 > 0 THEN
                (times_correct + CASE WHEN NEW.is_correct THEN 1 ELSE 0 END)::NUMERIC
                / (times_attempted + 1)
            ELSE NULL
        END
    WHERE id = NEW.mcq_id;
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

DROP TRIGGER IF EXISTS trg_mcq_attempt_stats ON mcq_attempts;
CREATE TRIGGER trg_mcq_attempt_stats
    AFTER INSERT ON mcq_attempts
    FOR EACH ROW EXECUTE FUNCTION update_mcq_stats();