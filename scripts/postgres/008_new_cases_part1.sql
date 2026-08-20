-- ================================================================
-- 008_new_cases_part1.sql
-- Casos 004, 005, 006: Neumonia, Preeclampsia, Apendicitis Pediatrica
-- ================================================================

BEGIN;

-- ================================================================
-- CASO 004: Neumonia adquirida en la comunidad (adulto mayor)
-- ================================================================
INSERT INTO clinical_cases (
    title, slug, chief_complaint, specialty, sub_specialty,
    difficulty, setting, patient_json, history_present_illness,
    symptoms_json, expected_diagnosis_json, expected_studies_json,
    expected_treatments_json, expected_followup, learning_objectives_json,
    pubmed_pmids, guidelines_ids, teaching_points, author_name, is_published
) VALUES (
    'Neumonia adquirida en la comunidad en varon de 74 anos con EPOC',
    'nac-adulto-mayor-epoc-004',
    'Tos con expectoracion amarillenta y dificultad para respirar de 4 dias',
    'Pneumology',
    'Infecciones respiratorias',
    'intermediate',
    'emergency',
    '{"full_name": "Ernesto Vazquez Morales", "age": 74, "sex": "M", "occupation": "Jubilado", "weight_kg": 68, "height_cm": 168, "allergies": ["ninguna conocida"], "medications": ["Salbutamol MDI PRN", "Tiotropio 18mcg/dia"], "medical_history": ["EPOC GOLD B", "Tabaquismo 40 paq/ano suspendido hace 5 anos", "HTA controlada"], "vaccinations": {"influenza": "hace 3 anos", "neumococo": "nunca"}}'::jsonb,
    'Paciente masculino de 74 anos, con antecedente de EPOC, que inicia hace 4 dias con tos productiva con expectoracion mucopurulenta amarillo-verdosa, fiebre no cuantificada, escalofrios y progresiva dificultad respiratoria. Refiere dolor toracico pleuritico derecho. En las ultimas 24 horas ha presentado confusion leve segun familiar. Sin viajes recientes ni contactos con enfermos.',
    '["tos productiva", "expectoracion purulenta", "fiebre", "escalofrios", "disnea progresiva", "dolor pleuritico", "confusion leve"]'::jsonb,
    '["Neumonia adquirida en la comunidad", "CURB-65 elevado", "Exacerbacion infecciosa de EPOC"]'::jsonb,
    '["Radiografia PA de torax", "Biometria hematica", "Quimica sanguinea con urea", "Procalcitonina", "Gasometria arterial", "Hemocultivos x2", "Cultivo de expectoracion", "Antigeno urinario neumococo", "Antigeno urinario Legionella", "PCR para SARS-CoV-2 e influenza"]'::jsonb,
    '["Oxigeno suplementario meta SpO2 88-92%", "Ceftriaxona 1g IV cada 24h", "Azitromicina 500mg IV cada 24h", "Hidratacion IV", "Broncodilatador nebulizado (salbutamol + ipratropio)", "Prednisona 40mg VO por exacerbacion EPOC", "Profilaxis TEV con enoxaparina"]'::jsonb,
    'Hospitalizacion en piso. Reevaluar en 48-72h para desescalar antibiotico segun cultivos. Vacunacion contra neumococo al alta. Espirometria de control en 6 semanas. Cesacion de exposicion a humo.',
    '["Aplicar escala CURB-65 para decision de hospitalizacion", "Reconocer neumonia atipica vs tipica", "Manejo empirico segun IDSA/ATS 2019", "Identificar factores de riesgo para Pseudomonas", "Vacunacion en adulto mayor"]'::jsonb,
    ARRAY['31573350', '31573351']::text[],
    ARRAY['IMSS-718-14']::text[],
    'CURB-65 predice mortalidad a 30 dias: 0-1 ambulatorio, 2 hospital, >=3 UCI. La confusion en adulto mayor con neumonia es marcador de gravedad. Cobertura empirica hospitalaria: beta-lactamico + macrolido cubre S. pneumoniae, H. influenzae, M. pneumoniae, C. pneumoniae, Legionella.',
    'ENARM Content Team',
    TRUE
)
ON CONFLICT (slug) DO NOTHING;


-- ================================================================
-- CASO 005: Preeclampsia severa en embarazo de 32 SDG
-- ================================================================
INSERT INTO clinical_cases (
    title, slug, chief_complaint, specialty, sub_specialty,
    difficulty, setting, patient_json, history_present_illness,
    symptoms_json, expected_diagnosis_json, expected_studies_json,
    expected_treatments_json, expected_followup, learning_objectives_json,
    pubmed_pmids, guidelines_ids, teaching_points, author_name, is_published
) VALUES (
    'Preeclampsia severa en primigesta de 28 anos con 32 SDG',
    'preeclampsia-severa-32sdg-005',
    'Cefalea intensa, vision borrosa y edema facial de 24 horas',
    'GynecologyObstetrics',
    'Medicina materno-fetal',
    'advanced',
    'emergency',
    '{"full_name": "Andrea Martinez Ruiz", "age": 28, "sex": "F", "occupation": "Contadora", "weight_kg": 82, "height_cm": 162, "gestational_age_weeks": 32, "gravida": 1, "para": 0, "allergies": ["ninguna"], "medications": ["Acido folico 5mg/dia", "Sulfato ferroso"], "medical_history": ["Sin antecedentes cronicos", "Primigesta"], "prenatal_control": "Regular en primer nivel, ultimo control hace 3 semanas con PA 130/85"}'::jsonb,
    'Primigesta de 28 anos con embarazo de 32 SDG por FUM confiable, quien acude por cefalea holocraneana intensa (8/10) de 24 horas de evolucion, no cede con paracetamol. Asocia vision borrosa con fotopsias, epigastralgia en barra hace 6 horas, y edema facial y de manos progresivo. Refiere movimientos fetales presentes pero disminuidos.',
    '["cefalea intensa persistente", "fotopsias", "vision borrosa", "epigastralgia en barra", "edema facial y manos", "disminucion movimientos fetales"]'::jsonb,
    '["Preeclampsia con datos de severidad", "Sindrome HELLP a descartar", "Riesgo de eclampsia"]'::jsonb,
    '["Presion arterial seriada cada 15 min", "Proteinuria en tira reactiva y 24h", "Biometria hematica completa", "Frotis de sangre periferica", "Pruebas de funcion hepatica (AST/ALT/DHL)", "Creatinina y acido urico", "Tiempos de coagulacion + fibrinogeno", "Ultrasonido obstetrico con Doppler", "Registro cardiotocografico"]'::jsonb,
    '["Sulfato de magnesio: dosis carga 4-6g IV en 20 min, mantenimiento 1-2g/hora", "Antihipertensivo: hidralazina 5mg IV cada 20 min o labetalol 20mg IV", "Meta PA sistolica <160 y diastolica <110", "Maduracion pulmonar fetal: betametasona 12mg IM cada 24h por 2 dosis", "Interrupcion del embarazo segun edad gestacional y estado materno-fetal", "Via de nacimiento segun condiciones obstetricas"]'::jsonb,
    'Interrupcion del embarazo indicada por preeclampsia severa >34 SDG o si hay datos de deterioro materno-fetal antes. Vigilancia postparto por 48-72h por riesgo de eclampsia. Control puerperal a las 6 semanas.',
    '["Diferenciar preeclampsia leve vs severa", "Prevencion de eclampsia con sulfato de magnesio", "Manejo hipertensivo agudo en embarazo (medicamentos seguros)", "Indicaciones de interrupcion del embarazo", "Reconocer sindrome HELLP"]'::jsonb,
    ARRAY['31229148', '32294486']::text[],
    ARRAY['IMSS-706-14']::text[],
    'Criterios de severidad en preeclampsia: PA >=160/110 en 2 ocasiones separadas 4h, plaquetas <100k, transaminasas x2 LSN, creatinina >1.1, edema pulmonar, sintomas neurologicos o epigastralgia persistente. El sulfato de magnesio es el anticonvulsivante de eleccion. Antidoto de toxicidad: gluconato de calcio.',
    'ENARM Content Team',
    TRUE
)
ON CONFLICT (slug) DO NOTHING;


-- ================================================================
-- CASO 006: Apendicitis aguda en nino de 8 anos
-- ================================================================
INSERT INTO clinical_cases (
    title, slug, chief_complaint, specialty, sub_specialty,
    difficulty, setting, patient_json, history_present_illness,
    symptoms_json, expected_diagnosis_json, expected_studies_json,
    expected_treatments_json, expected_followup, learning_objectives_json,
    pubmed_pmids, guidelines_ids, teaching_points, author_name, is_published
) VALUES (
    'Apendicitis aguda en nino de 8 anos con dolor migratorio',
    'apendicitis-aguda-pediatrica-006',
    'Dolor abdominal de 12 horas, primero periumbilical ahora en fosa iliaca derecha',
    'PediatricSurgery',
    'Abdomen agudo pediatrico',
    'intermediate',
    'emergency',
    '{"full_name": "Diego Ramirez Perez", "age": 8, "sex": "M", "weight_kg": 28, "height_cm": 128, "allergies": ["ninguna"], "medications": ["ninguna"], "medical_history": ["Sano", "Vacunas al corriente"], "school_grade": "3ro de primaria"}'::jsonb,
    'Escolar de 8 anos previamente sano, quien inicia hace 12 horas con dolor abdominal difuso periumbilical acompanado de nausea. En las ultimas 4 horas el dolor migro a fosa iliaca derecha, es constante y se intensifica al caminar o brincar. Presenta 2 vomitos posprandiales, hiporexia y febricula (37.9C). Sin evacuaciones diarreicas.',
    '["dolor abdominal migratorio periumbilical a FID", "vomitos", "hiporexia", "febricula", "dolor a la deambulacion", "signo del brinco positivo"]'::jsonb,
    '["Apendicitis aguda no complicada", "Score de Alvarado o PAS elevado", "Descartar perforacion"]'::jsonb,
    '["Biometria hematica completa", "Proteina C reactiva", "Examen general de orina", "Ultrasonido abdominal con enfoque en FID", "Considerar TAC si USG no concluyente", "Grupo sanguineo y pruebas cruzadas"]'::jsonb,
    '["NPO", "Solucion salina 0.9% a requerimientos", "Analgesia: metamizol 20mg/kg IV o paracetamol 15mg/kg", "Antibioticos preoperatorios: ceftriaxona 50mg/kg + metronidazol 15mg/kg IV", "Interconsulta a cirugia pediatrica", "Apendicectomia laparoscopica"]'::jsonb,
    'Postoperatorio inmediato en piso quirurgico. Tolerancia oral progresiva a las 6-12h. Alta a las 24-48h si no hay complicaciones. Retiro de puntos en 7-10 dias. Reintegro a actividad escolar en 1-2 semanas.',
    '["Reconocer patron migratorio del dolor en apendicitis", "Aplicar score de Alvarado o PAS (Pediatric Appendicitis Score)", "Diferenciar apendicitis vs adenitis mesenterica vs GEA", "USG es primera linea en ninos (evitar radiacion)", "Manejo perioperatorio pediatrico"]'::jsonb,
    ARRAY['28460712', '30217666']::text[],
    ARRAY[]::text[],
    'Pediatric Appendicitis Score (PAS): dolor migratorio (1), anorexia (1), nausea/vomito (1), dolor FID (2), rebote al saltar/tos (2), fiebre >38 (1), leucocitosis >10k (1), neutrofilia (1). Total 10. Score 1-3 baja probabilidad, 4-6 observacion, >=7 alta probabilidad. En ninos <5 anos es dificil el diagnostico y hay mayor riesgo de perforacion.',
    'ENARM Content Team',
    TRUE
)
ON CONFLICT (slug) DO NOTHING;

COMMIT;

-- Verificacion
SELECT id, title, specialty, difficulty
FROM clinical_cases
WHERE slug LIKE '%-004%' OR slug LIKE '%-005%' OR slug LIKE '%-006%'
ORDER BY slug;

SELECT COUNT(*) AS total_casos FROM clinical_cases;