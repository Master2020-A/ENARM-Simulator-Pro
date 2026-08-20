-- =============================================================
--  028_pubmed_cache.sql - Cache de referencias PubMed
--  Sprint 17: Referencias bibliograficas por caso
-- =============================================================

-- Cache de articulos PubMed (evita re-consultar E-utilities)
CREATE TABLE IF NOT EXISTS pubmed_cache (
    pmid        TEXT PRIMARY KEY,
    title       TEXT NOT NULL DEFAULT '',
    authors     TEXT[] NOT NULL DEFAULT '{}',
    journal     TEXT NOT NULL DEFAULT '',
    pub_date    TEXT NOT NULL DEFAULT '',
    doi         TEXT NOT NULL DEFAULT '',
    fetched_at  TIMESTAMPTZ NOT NULL DEFAULT NOW()
);

-- Referencias de casos a articulos PubMed
CREATE TABLE IF NOT EXISTS case_references (
    id          BIGSERIAL PRIMARY KEY,
    case_id     UUID NOT NULL REFERENCES clinical_cases(id) ON DELETE CASCADE,
    pmid        TEXT NOT NULL,
    relevance   TEXT NOT NULL DEFAULT 'general',  -- primary | differential | treatment | guideline
    UNIQUE (case_id, pmid)
);
CREATE INDEX IF NOT EXISTS idx_case_refs_case ON case_references(case_id);
CREATE INDEX IF NOT EXISTS idx_case_refs_pmid ON case_references(pmid);
