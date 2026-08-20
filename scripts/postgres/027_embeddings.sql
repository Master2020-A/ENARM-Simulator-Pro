-- =============================================================
--  027_embeddings.sql - pgvector + embeddings de casos y GPCs
--  Sprint 7B Epica 4: busqueda semantica
-- =============================================================

-- Habilitar extension vector (pgvector)
CREATE EXTENSION IF NOT EXISTS vector;

-- Embeddings de casos clinicos
CREATE TABLE IF NOT EXISTS case_embeddings (
    id          BIGSERIAL PRIMARY KEY,
    case_id     UUID NOT NULL REFERENCES clinical_cases(id) ON DELETE CASCADE,
    content_type TEXT NOT NULL DEFAULT 'full_case',  -- full_case | chief_complaint | diagnosis
    embedding   vector(768),
    created_at  TIMESTAMPTZ NOT NULL DEFAULT NOW()
);
CREATE INDEX IF NOT EXISTS idx_case_embeddings_embedding
    ON case_embeddings USING hnsw (embedding vector_cosine_ops);
CREATE INDEX IF NOT EXISTS idx_case_embeddings_case
    ON case_embeddings(case_id);

-- Embeddings de guias clinicas (RAG)
CREATE TABLE IF NOT EXISTS guideline_embeddings (
    id          BIGSERIAL PRIMARY KEY,
    guideline_id TEXT NOT NULL REFERENCES clinical_guidelines(id) ON DELETE CASCADE,
    section     TEXT NOT NULL DEFAULT 'full',  -- full | summary | recommendation
    embedding   vector(1536),
    created_at  TIMESTAMPTZ NOT NULL DEFAULT NOW()
);
CREATE INDEX IF NOT EXISTS idx_guideline_embeddings_hnsw
    ON guideline_embeddings USING hnsw (embedding vector_cosine_ops);
CREATE INDEX IF NOT EXISTS idx_guideline_embeddings_gid
    ON guideline_embeddings(guideline_id);

-- Funcion de busqueda semantica de casos
CREATE OR REPLACE FUNCTION search_similar_cases(
    query_embedding vector(1536),
    k int DEFAULT 5
) RETURNS TABLE (
    case_id UUID,
    title TEXT,
    specialty TEXT,
    similarity REAL
) AS $$
    SELECT ce.case_id,
           cc.title,
           cc.specialty,
           1 - (ce.embedding <=> query_embedding) AS similarity
    FROM case_embeddings ce
    JOIN clinical_cases cc ON cc.id = ce.case_id
    ORDER BY ce.embedding <=> query_embedding
    LIMIT k;
$$ LANGUAGE sql STABLE;

-- Funcion de busqueda semantica de GPCs
CREATE OR REPLACE FUNCTION search_similar_guidelines(
    query_embedding vector(1536),
    p_limit int DEFAULT 3
) RETURNS TABLE (
    guideline_id TEXT,
    title TEXT,
    specialty TEXT,
    similarity REAL
) AS $$
    SELECT ge.guideline_id,
           gl.title,
           gl.specialty,
           1 - (ge.embedding <=> query_embedding) AS similarity
    FROM guideline_embeddings ge
    JOIN clinical_guidelines gl ON gl.id = ge.guideline_id
    ORDER BY ge.embedding <=> query_embedding
    LIMIT p_limit;
$$ LANGUAGE sql STABLE;
