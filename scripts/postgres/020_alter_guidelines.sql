-- ================================================================
-- 020_alter_guidelines.sql
-- Sprint 7A.4.1: Enriquecer schema de clinical_guidelines
-- ================================================================

BEGIN;

-- Agregar columnas nuevas si no existen
ALTER TABLE clinical_guidelines
    ADD COLUMN IF NOT EXISTS specialty VARCHAR(100),
    ADD COLUMN IF NOT EXISTS key_recommendations JSONB,
    ADD COLUMN IF NOT EXISTS tags TEXT[];

-- Comentarios de documentacion
COMMENT ON COLUMN clinical_guidelines.specialty IS 'Especialidad medica principal';
COMMENT ON COLUMN clinical_guidelines.key_recommendations IS 'JSON con recomendaciones nivel A/B/C';
COMMENT ON COLUMN clinical_guidelines.tags IS 'Tags para busqueda';

-- Indice para busqueda por especialidad
CREATE INDEX IF NOT EXISTS idx_guidelines_specialty ON clinical_guidelines(specialty);

-- Indice GIN para tags (busqueda rapida en arrays)
CREATE INDEX IF NOT EXISTS idx_guidelines_tags ON clinical_guidelines USING GIN(tags);

COMMIT;

-- Verificacion
\d clinical_guidelines
