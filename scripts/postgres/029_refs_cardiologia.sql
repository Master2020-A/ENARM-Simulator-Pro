-- Referencias PubMed para casos de cardiologia
INSERT INTO case_references (case_id, pmid, relevance)
SELECT id, '38133979', 'treatment'
FROM clinical_cases
WHERE specialty='Cardiologia' AND title ILIKE '%infarto%'
ON CONFLICT (case_id, pmid) DO NOTHING;

INSERT INTO case_references (case_id, pmid, relevance)
SELECT id, '38133979', 'guideline'
FROM clinical_cases
WHERE specialty='Cardiologia'
ON CONFLICT (case_id, pmid) DO NOTHING;

INSERT INTO pubmed_cache (pmid, title, journal, pub_date, doi)
VALUES
('38133979', '2023 ESC Guidelines for the management of acute coronary syndromes', 'European Heart Journal', '2023', '10.1093/eurheartj/ehad191')
ON CONFLICT (pmid) DO NOTHING;
