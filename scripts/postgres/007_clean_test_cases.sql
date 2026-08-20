-- ================================================================
-- 007_clean_test_cases.sql (v2)
-- Elimina casos basura + sus sesiones asociadas
-- ================================================================

BEGIN;

-- 1. Ver que casos basura existen ANTES
SELECT '--- ANTES ---' AS info;
SELECT id, title FROM clinical_cases WHERE title LIKE '%Caso de prueba%';

-- 2. Borrar sesiones que referencian casos basura
DELETE FROM study_sessions
WHERE case_id IN (
    SELECT id FROM clinical_cases
    WHERE title LIKE '%Caso de prueba%'
       OR title LIKE '%prueba Sprint%'
);

-- 3. Borrar case_tags asociados
DELETE FROM case_tags
WHERE case_id IN (
    SELECT id FROM clinical_cases
    WHERE title LIKE '%Caso de prueba%'
       OR title LIKE '%prueba Sprint%'
);

-- 4. Borrar case_versions asociadas
DELETE FROM case_versions
WHERE case_id IN (
    SELECT id FROM clinical_cases
    WHERE title LIKE '%Caso de prueba%'
       OR title LIKE '%prueba Sprint%'
);

-- 5. Ahora si borrar los casos basura
DELETE FROM clinical_cases
WHERE title LIKE '%Caso de prueba%'
   OR title LIKE '%prueba Sprint%';

COMMIT;

-- 6. Ver resultado final
SELECT '--- DESPUES ---' AS info;
SELECT id, title, specialty, difficulty
FROM clinical_cases
ORDER BY created_at;

SELECT COUNT(*) AS total_casos_limpios FROM clinical_cases;