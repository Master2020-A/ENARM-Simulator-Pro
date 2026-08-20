-- =====================================================================
-- 026_case_guidelines.sql
-- Sprint 7B Epica 2: Vinculacion explicita casos <-> GPCs
-- =====================================================================

BEGIN;

-- Tabla de vinculacion many-to-many con nivel de relevancia
CREATE TABLE IF NOT EXISTS case_guidelines (
    case_id      UUID NOT NULL REFERENCES clinical_cases(id) ON DELETE CASCADE,
    guideline_id VARCHAR(50) NOT NULL REFERENCES clinical_guidelines(id) ON DELETE CASCADE,
    relevance    SMALLINT NOT NULL CHECK (relevance BETWEEN 1 AND 3),
    rationale    TEXT,
    created_at   TIMESTAMPTZ DEFAULT NOW(),
    PRIMARY KEY (case_id, guideline_id)
);

COMMENT ON TABLE case_guidelines IS 'Vinculacion explicita entre casos clinicos y GPCs relevantes';
COMMENT ON COLUMN case_guidelines.relevance IS '1=PRIMARIA (diagnostico directo), 2=SECUNDARIA (comorbilidad), 3=TERCIARIA (dx diferencial/contexto)';

CREATE INDEX IF NOT EXISTS idx_case_guidelines_case ON case_guidelines(case_id);
CREATE INDEX IF NOT EXISTS idx_case_guidelines_gpc ON case_guidelines(guideline_id);
CREATE INDEX IF NOT EXISTS idx_case_guidelines_relevance ON case_guidelines(case_id, relevance);

-- ============================================================
-- POBLADO: 10 casos x ~3 GPCs = ~35 vinculaciones
-- ============================================================

-- 1. IAM STEMI (Cardiology)
INSERT INTO case_guidelines(case_id, guideline_id, relevance, rationale) VALUES
    ('e9604696-819b-4803-99da-0589b6fb8910', 'IMSS-234-09', 1, 'GPC especifica de IAM STEMI'),
    ('e9604696-819b-4803-99da-0589b6fb8910', 'IMSS-357-13', 1, 'ICP primaria en STEMI'),
    ('e9604696-819b-4803-99da-0589b6fb8910', 'IMSS-014-08', 2, 'HTA como comorbilidad'),
    ('e9604696-819b-4803-99da-0589b6fb8910', 'IMSS-076-08', 3, 'Crisis hipertensiva - manejo agudo')
ON CONFLICT DO NOTHING;

-- 2. LRA por AINEs (Nephrology)
INSERT INTO case_guidelines(case_id, guideline_id, relevance, rationale) VALUES
    ('a3525988-53c6-451d-a717-2ff23535b47d', 'IMSS-395-10', 1, 'GPC especifica de LRA'),
    ('a3525988-53c6-451d-a717-2ff23535b47d', 'IMSS-020-08', 2, 'Riesgo de ERC en paciente con DM/HTA'),
    ('a3525988-53c6-451d-a717-2ff23535b47d', 'IMSS-335-10', 3, 'ERC establecida - manejo cronico'),
    ('a3525988-53c6-451d-a717-2ff23535b47d', 'IMSS-014-08', 2, 'HTA como comorbilidad'),
    ('a3525988-53c6-451d-a717-2ff23535b47d', 'IMSS-104-08', 2, 'DM2 como comorbilidad')
ON CONFLICT DO NOTHING;

-- 3. Neumonia en EPOC (Pneumology)
INSERT INTO case_guidelines(case_id, guideline_id, relevance, rationale) VALUES
    ('534b8503-023c-49c9-a1b6-39ced7421991', 'IMSS-718-14', 1, 'Neumonia adquirida en la comunidad'),
    ('534b8503-023c-49c9-a1b6-39ced7421991', 'IMSS-037-08', 1, 'EPOC - comorbilidad critica'),
    ('534b8503-023c-49c9-a1b6-39ced7421991', 'IMSS-085-08', 2, 'Riesgo de sepsis en NAC severa')
ON CONFLICT DO NOTHING;

-- 4. Shock septico ITU (Infectology)
INSERT INTO case_guidelines(case_id, guideline_id, relevance, rationale) VALUES
    ('1a2bb679-5a59-442e-919d-8a8ca2d26748', 'IMSS-085-08', 1, 'Sepsis y choque septico'),
    ('1a2bb679-5a59-442e-919d-8a8ca2d26748', 'IMSS-283-13', 1, 'Surviving Sepsis Campaign'),
    ('1a2bb679-5a59-442e-919d-8a8ca2d26748', 'IMSS-078-08', 1, 'ITU como foco infeccioso'),
    ('1a2bb679-5a59-442e-919d-8a8ca2d26748', 'IMSS-104-08', 2, 'DM2 como comorbilidad')
ON CONFLICT DO NOTHING;

-- 5. ACV isquemico (Neurology)
INSERT INTO case_guidelines(case_id, guideline_id, relevance, rationale) VALUES
    ('4ce8e5cd-ffd0-462f-80c8-8c838d0410e0', 'IMSS-058-08', 1, 'EVC Isquemico Agudo'),
    ('4ce8e5cd-ffd0-462f-80c8-8c838d0410e0', 'IMSS-102-08', 1, 'EVC Isquemico Agudo (version actualizada)'),
    ('4ce8e5cd-ffd0-462f-80c8-8c838d0410e0', 'IMSS-372-10', 3, 'Hemorragia subaracnoidea - dx diferencial')
ON CONFLICT DO NOTHING;

-- 6. CAD debut adolescente (PediatricEndocrinology)
INSERT INTO case_guidelines(case_id, guideline_id, relevance, rationale) VALUES
    ('e76f38c9-9b36-4603-9068-a68a9259fb11', 'IMSS-302-10', 1, 'CAD pediatrica'),
    ('e76f38c9-9b36-4603-9068-a68a9259fb11', 'IMSS-032-08', 2, 'CAD adultos - referencia'),
    ('e76f38c9-9b36-4603-9068-a68a9259fb11', 'IMSS-104-08', 3, 'DM tipo 2 - contexto general')
ON CONFLICT DO NOTHING;

-- 7. Preeclampsia (GynecologyObstetrics)
INSERT INTO case_guidelines(case_id, guideline_id, relevance, rationale) VALUES
    ('6d24f0a1-1055-4dd3-8381-4a422d2b8dd2', 'IMSS-706-14', 1, 'GPC especifica de preeclampsia-eclampsia'),
    ('6d24f0a1-1055-4dd3-8381-4a422d2b8dd2', 'IMSS-014-08', 2, 'Manejo de HTA'),
    ('6d24f0a1-1055-4dd3-8381-4a422d2b8dd2', 'IMSS-076-08', 2, 'Crisis hipertensiva - urgencia')
ON CONFLICT DO NOTHING;

-- 8. Apendicitis pediatrica (PediatricSurgery)
INSERT INTO case_guidelines(case_id, guideline_id, relevance, rationale) VALUES
    ('b2e73a34-2bed-420c-884b-6446a8f3db76', 'IMSS-031-08', 1, 'GPC especifica de apendicitis'),
    ('b2e73a34-2bed-420c-884b-6446a8f3db76', 'IMSS-524-11', 3, 'Invaginacion intestinal - dx diferencial'),
    ('b2e73a34-2bed-420c-884b-6446a8f3db76', 'IMSS-345-10', 2, 'Cirugia pediatrica general')
ON CONFLICT DO NOTHING;

-- 9. TCE severo (EmergencyMedicine)
INSERT INTO case_guidelines(case_id, guideline_id, relevance, rationale) VALUES
    ('6101e253-07b8-47de-920a-1ff832dc5ab8', 'IMSS-604-13', 1, 'GPC especifica de TCE severo'),
    ('6101e253-07b8-47de-920a-1ff832dc5ab8', 'IMSS-244-10', 2, 'Urgencias medicas - manejo integral')
ON CONFLICT DO NOTHING;

-- 10. Bronquiolitis VSR (Pediatrics)
-- Nota: no hay GPC especifica de bronquiolitis en la BD.
-- Vinculamos con contexto pediatrico y diagnosticos diferenciales.
INSERT INTO case_guidelines(case_id, guideline_id, relevance, rationale) VALUES
    ('204a200e-fb24-44ba-affc-27c7b526ea74', 'IMSS-063-08', 2, 'Control del nino sano - contexto pediatrico'),
    ('204a200e-fb24-44ba-affc-27c7b526ea74', 'IMSS-320-10', 3, 'Asma en ninos - dx diferencial'),
    ('204a200e-fb24-44ba-affc-27c7b526ea74', 'IMSS-009-08', 3, 'Asma - manejo general')
ON CONFLICT DO NOTHING;

COMMIT;

-- Verificacion
SELECT 
    cc.title AS caso,
    COUNT(cg.guideline_id) AS gpcs_vinculadas,
    STRING_AGG(cg.guideline_id || ' (r' || cg.relevance || ')', ', ' ORDER BY cg.relevance, cg.guideline_id) AS vinculos
FROM clinical_cases cc
LEFT JOIN case_guidelines cg ON cg.case_id = cc.id
GROUP BY cc.id, cc.title
ORDER BY cc.title;