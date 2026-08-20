-- ═══════════════════════════════════════════════════════════════
--   ENARM Database - pgvector para busqueda semantica (003)
-- ═══════════════════════════════════════════════════════════════

DO $$
BEGIN
    IF EXISTS (SELECT 1 FROM pg_extension WHERE extname = 'vector') THEN
        RAISE NOTICE 'pgvector disponible - creando columnas de embeddings';

        BEGIN
            ALTER TABLE clinical_cases ADD COLUMN IF NOT EXISTS embedding vector(768);
        EXCEPTION WHEN OTHERS THEN
            RAISE NOTICE 'Error agregando embedding a clinical_cases: %', SQLERRM;
        END;

        BEGIN
            ALTER TABLE mcq_bank ADD COLUMN IF NOT EXISTS embedding vector(768);
        EXCEPTION WHEN OTHERS THEN
            RAISE NOTICE 'Error agregando embedding a mcq_bank: %', SQLERRM;
        END;

        BEGIN
            ALTER TABLE pubmed_references ADD COLUMN IF NOT EXISTS embedding vector(768);
        EXCEPTION WHEN OTHERS THEN
            RAISE NOTICE 'Error agregando embedding a pubmed: %', SQLERRM;
        END;

        BEGIN
            CREATE INDEX IF NOT EXISTS idx_cases_embedding
                ON clinical_cases USING ivfflat (embedding vector_cosine_ops)
                WITH (lists = 100);
        EXCEPTION WHEN OTHERS THEN NULL; END;

        BEGIN
            CREATE INDEX IF NOT EXISTS idx_mcq_embedding
                ON mcq_bank USING ivfflat (embedding vector_cosine_ops)
                WITH (lists = 100);
        EXCEPTION WHEN OTHERS THEN NULL; END;

        BEGIN
            CREATE INDEX IF NOT EXISTS idx_pubmed_embedding
                ON pubmed_references USING ivfflat (embedding vector_cosine_ops)
                WITH (lists = 100);
        EXCEPTION WHEN OTHERS THEN NULL; END;

        RAISE NOTICE 'Embeddings configurados';
    ELSE
        RAISE NOTICE 'pgvector NO instalado - busqueda semantica desactivada';
    END IF;
END
$$;