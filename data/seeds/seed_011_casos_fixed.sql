-- ================================================================
--  seed_011_casos_004_010_fixed.sql
--  Versión corregida - usa "specialty" en lugar de "specialty_id"
--  Adaptado al schema real de clinical_cases
-- ================================================================

-- Primero ver el schema exacto
\echo '=== Schema de clinical_cases ==='
\d clinical_cases

\echo ''
\echo '=== Casos existentes ==='
SELECT id, title, specialty, difficulty_level FROM clinical_cases ORDER BY id;

\echo ''
\echo '=== Iniciando inserción de casos 004-010 ==='

BEGIN;

-- ── Diagnóstico previo: ver columnas disponibles ─────────────
DO $$
DECLARE
    col_list TEXT;
BEGIN
    SELECT STRING_AGG(column_name || ' (' || data_type || ')', ', ' ORDER BY ordinal_position)
    INTO col_list
    FROM information_schema.columns
    WHERE table_name = 'clinical_cases';
    
    RAISE NOTICE 'Columnas de clinical_cases: %', col_list;
END $$;

-- ── INTENTO 1: Inserción con specialty como texto ─────────────
-- Basado en la sugerencia del error: la columna se llama "specialty"

INSERT INTO clinical_cases (
    title,
    specialty,
    difficulty_level,
    chief_complaint,
    patient_demographics,
    vitals_json,
    history_of_present_illness,
    physical_exam_json,
    key_labs_json,
    key_imaging_json,
    correct_diagnosis,
    differential_diagnosis_json,
    treatment_plan_json,
    scoring_rubric_json,
    learning_objectives_json,
    tags_json,
    is_active
) VALUES (
    'Angina Inestable en Paciente con Factores de Riesgo Cardiovascular',
    'Cardiología',
    2,
    'Dolor en el pecho que me da en reposo desde ayer',
    '{"age":62,"sex":"M","weight_kg":88,"height_cm":172}'::jsonb,
    '{"systolic_bp":158,"diastolic_bp":96,"heart_rate":94,"respiratory_rate":18,"temperature_c":36.8,"spo2_percent":97}'::jsonb,
    'Masculino 62 años, dolor retroesternal opresivo en reposo 18h, irradiado a brazo izquierdo. Episodio previo hace 3 semanas con esfuerzo. HAS, DM2, tabaquismo, dislipidemia.',
    '{"cardiovascular":"Ruidos rítmicos sin soplos","respiratory":"Limpios","abdomen":"Normal","neurological":"Sin focalización"}'::jsonb,
    '{"troponina_I":"0.04 ng/mL (limite)","CPK_MB":"18 U/L","LDL":"162 mg/dL","glucosa":"148 mg/dL"}'::jsonb,
    '["ECG: Depresión ST 1mm V4-V6 dinámica durante dolor. Sin elevación. Sin ondas Q.","RX tórax: Normal"]'::jsonb,
    'Síndrome coronario agudo sin elevación del ST - Angina inestable',
    '["IAMCEST","IAMSEST","Disección aórtica","Espasmo coronario"]'::jsonb,
    '["Monitorización continua","Aspirina 300mg carga + 100mg/día","Clopidogrel 300mg carga","Enoxaparina 1mg/kg c/12h","Metoprolol 25-50mg VO","NTG SL PRN","Atorvastatina 80mg/noche","Score TIMI/GRACE","Coronariografía 24-72h"]'::jsonb,
    '{"anamnesis":0.20,"physical_exam":0.15,"diagnosis":0.35,"treatment":0.30}'::jsonb,
    '["Distinguir angina inestable de IAMSEST/IAMCEST","Cambios dinámicos ST","Doble antiagregación","Score TIMI/GRACE"]'::jsonb,
    '["SCA","SICA","angina_inestable","cardiologia","ECG"]'::jsonb,
    true
);

INSERT INTO clinical_cases (
    title, specialty, difficulty_level, chief_complaint,
    patient_demographics, vitals_json, history_of_present_illness,
    physical_exam_json, key_labs_json, key_imaging_json,
    correct_diagnosis, differential_diagnosis_json,
    treatment_plan_json, scoring_rubric_json,
    learning_objectives_json, tags_json, is_active
) VALUES (
    'Neumonía Adquirida en la Comunidad Grave - Criterios de UCI',
    'Neumología',
    3,
    'Fiebre alta y me cuesta respirar desde hace 4 días',
    '{"age":71,"sex":"F","weight_kg":62,"height_cm":158}'::jsonb,
    '{"systolic_bp":102,"diastolic_bp":64,"heart_rate":118,"respiratory_rate":28,"temperature_c":39.4,"spo2_percent":88}'::jsonb,
    'Femenino 71 años EPOC GOLD II. 4 días fiebre 40°C, tos con expectoración herrumbrosa, disnea de reposo, confusión. Antígeno neumococo +.',
    '{"general":"Dificultad respiratoria, cianosis, desorientada","respiratory":"Matidez base derecha, crepitantes, broncofonía","cardiovascular":"Taquicardia, llenado capilar 3s"}'::jsonb,
    '{"leucocitos":"22400 bandas 12%","PCR":"284 mg/L","procalcitonina":"4.8 ng/mL","PaO2":"52mmHg","pH":"7.46","lactato":"2.8","antigeno_neumococo":"Positivo"}'::jsonb,
    '["RX tórax: Consolidación lobar LID 2/3 campo pulmonar derecho. Hiperinsuflación bilateral."]'::jsonb,
    'NAC grave por S. pneumoniae - CURB-65: 4 (criterios UCI)',
    '["Agudización EPOC con sobreinfección","NAC Legionella","TEP con infarto"]'::jsonb,
    '["UCI CURB-65 ≥3","O2 Venturi 40% meta SpO2 88-92%","Ceftriaxona 2g IV c/24h + Azitromicina 500mg IV c/24h","Fluidos si hipotensión","Norepinefrina si MAP<65","Broncodilatadores c/4-6h","Profilaxis TVP"]'::jsonb,
    '{"anamnesis":0.20,"physical_exam":0.15,"diagnosis":0.35,"treatment":0.30}'::jsonb,
    '["Calcular CURB-65","Gasometría en EPOC+NAC","Antibiótico empírico NAC grave","O2 en EPOC"]'::jsonb,
    '["NAC","neumonia","CURB65","UCI","EPOC","antibiotico"]'::jsonb,
    true
);

INSERT INTO clinical_cases (
    title, specialty, difficulty_level, chief_complaint,
    patient_demographics, vitals_json, history_of_present_illness,
    physical_exam_json, key_labs_json, key_imaging_json,
    correct_diagnosis, differential_diagnosis_json,
    treatment_plan_json, scoring_rubric_json,
    learning_objectives_json, tags_json, is_active
) VALUES (
    'Pancreatitis Aguda Severa por Litiasis Biliar',
    'Gastroenterología',
    3,
    'Dolor de panza que no se me quita y vomité muchas veces',
    '{"age":45,"sex":"F","weight_kg":82,"height_cm":160}'::jsonb,
    '{"systolic_bp":96,"diastolic_bp":60,"heart_rate":124,"respiratory_rate":22,"temperature_c":38.6,"spo2_percent":94}'::jsonb,
    'Femenino 45 años colelitiasis. 24h dolor epigástrico irradiado en cinturón 9/10, vómito incoercible, distensión, fiebre. Grey-Turner incipiente. Posible perforación.',
    '{"general":"Posición fetal, diaforética, ictérica","abdomen":"Distendido, defensa, Grey-Turner, Murphy+","cardiovascular":"Taquicardia, hipotensión"}'::jsonb,
    '{"lipasa":"1840 U/L","amilasa":"920 U/L","leucocitos":"18600","calcio":"7.8 mg/dL","bilirrubina":"3.2 mg/dL","lactato":"3.2","CTSI":"7/10"}'::jsonb,
    '["USG: Colelitiasis, colédoco 10mm, líquido peripancreático","TAC Balthazar D-E: Necrosis >30%, colección 2cm","RX: Derrame pleural izquierdo"]'::jsonb,
    'Pancreatitis aguda severa por litiasis biliar (Ranson ≥3, CTSI 7)',
    '["Úlcera péptica perforada","Colangitis aguda","Isquemia mesentérica","CAD"]'::jsonb,
    '["UCI","NPO absoluto","Ringer Lactato 250-500 mL/h","Morfina 0.05 mg/kg IV","Ondansetrón IV","CPRE si coledocolitiasis","NO antibiótico profiláctico","Nutrición enteral 24-48h","Colecistectomía diferida 6-8 sem"]'::jsonb,
    '{"anamnesis":0.20,"physical_exam":0.15,"diagnosis":0.35,"treatment":0.30}'::jsonb,
    '["Diagnóstico con lipasa+imagen","Ranson/BISAP/CTSI","Ringer vs SF","Cuándo antibiótico","CPRE en coledocolitiasis"]'::jsonb,
    '["pancreatitis","litiasis_biliar","gastroenterologia","Ranson","CPRE"]'::jsonb,
    true
);

INSERT INTO clinical_cases (
    title, specialty, difficulty_level, chief_complaint,
    patient_demographics, vitals_json, history_of_present_illness,
    physical_exam_json, key_labs_json, key_imaging_json,
    correct_diagnosis, differential_diagnosis_json,
    treatment_plan_json, scoring_rubric_json,
    learning_objectives_json, tags_json, is_active
) VALUES (
    'Hemorragia Intracerebral Hipertensiva - Ganglio Basal',
    'Neurología',
    3,
    'Mi marido se desplomó y no puede mover el brazo derecho',
    '{"age":67,"sex":"M","weight_kg":79,"height_cm":168}'::jsonb,
    '{"systolic_bp":210,"diastolic_bp":118,"heart_rate":88,"respiratory_rate":20,"temperature_c":37.1,"spo2_percent":95}'::jsonb,
    'Masculino 67 años HTA mal controlada. Cefalea thunderclap súbita + hemiparesia derecha + afasia mixta. GCS 12. 45 minutos de evolución.',
    '{"neurological":"GCS 12, afasia mixta, hemiparesia derecha 1/5 brazo, Babinski+, desviación mirada izquierda","cardiovascular":"PA 210/118"}'::jsonb,
    '{"INR":"1.0","plaquetas":"198000","glucosa":"138 mg/dL","ECG":"Ritmo sinusal"}'::jsonb,
    '["TAC craneal URGENTE: Hematoma hiperdenso putamen izquierdo 3.2x2.8cm 12mL. Edema perilesional. DLM 2mm. Sin herniación."]'::jsonb,
    'Hemorragia intracerebral espontánea hipertensiva en ganglio basal izquierdo',
    '["ACV isquémico (excluido TAC)","Hemorragia subaracnoidea","Tumor con sangrado","MAV rota"]'::jsonb,
    '["Cabecera 30°","NO trombolíticos","Labetalol 20mg IV meta PAS 130-150","Glucemia 140-180","T° <37.5°C","Neurocirugía (12mL conservador)","UCI neurológica","TAC control 24h","Rehabilitación temprana"]'::jsonb,
    '{"anamnesis":0.20,"physical_exam":0.15,"diagnosis":0.35,"treatment":0.30}'::jsonb,
    '["HIC vs ACV isquémico","Cefalea thunderclap","Urgencia hipertensiva en HIC","Cirugía vs conservador"]'::jsonb,
    '["HIC","hemorragia_cerebral","ACV","neurologia","hipertension"]'::jsonb,
    true
);

INSERT INTO clinical_cases (
    title, specialty, difficulty_level, chief_complaint,
    patient_demographics, vitals_json, history_of_present_illness,
    physical_exam_json, key_labs_json, key_imaging_json,
    correct_diagnosis, differential_diagnosis_json,
    treatment_plan_json, scoring_rubric_json,
    learning_objectives_json, tags_json, is_active
) VALUES (
    'Cetoacidosis Diabética en Debut de DM Tipo 1',
    'Endocrinología',
    2,
    'Mi hijo lleva 3 días muy mal, no come y respira raro',
    '{"age":19,"sex":"M","weight_kg":65,"height_cm":178}'::jsonb,
    '{"systolic_bp":104,"diastolic_bp":68,"heart_rate":116,"respiratory_rate":30,"temperature_c":37.2,"spo2_percent":98}'::jsonb,
    'Masculino 19 años sin antecedentes. 3 días náusea, vómito, dolor abdominal. 3 semanas poliuria, polidipsia, baja 8kg. Aliento acetona. Kussmaul.',
    '{"general":"Delgado, deshidratado severo, somnoliento, acetona","respiratory":"Kussmaul FR 30","neurological":"GCS 13"}'::jsonb,
    '{"glucosa":"518 mg/dL","pH":"7.18","HCO3":"8","cetonas":"4+","betaHB":"6.2 mmol/L","K":"5.8 mEq/L","anion_gap":"26","HbA1c":"13.8%","peptido_C":"0.1","anti_GAD":"Positivos"}'::jsonb,
    '["RX tórax: Normal","USG abdominal: Normal"]'::jsonb,
    'Cetoacidosis Diabética severa (pH<7.2, HCO3<10) como debut de DM tipo 1',
    '["Estado hiperosmolar hiperglucémico","Acidosis láctica","Intoxicación salicilatos","Acidosis urémica"]'::jsonb,
    '["SF 0.9% 1L en 1h","NO insulina hasta K+>3.5","Insulina regular 0.1 U/kg/h SIN bolo","SF/0.45% 250-500mL/h","Dextrosa 5% al llegar glucosa <200","KCl 20-40 mEq/L en sueros","NO bicarbonato (pH 7.18)","Glucemia c/1h, gases c/2-4h","Insulina SC basal 30min antes de suspender infusión"]'::jsonb,
    '{"anamnesis":0.20,"physical_exam":0.15,"diagnosis":0.35,"treatment":0.30}'::jsonb,
    '["Triada CAD","AG y Na corregido","Secuencia fluidos-K-insulina","Cuándo NO bicarbonato","CAD vs EHH"]'::jsonb,
    '["CAD","cetoacidosis","diabetes_tipo1","Kussmaul","anion_gap"]'::jsonb,
    true
);

INSERT INTO clinical_cases (
    title, specialty, difficulty_level, chief_complaint,
    patient_demographics, vitals_json, history_of_present_illness,
    physical_exam_json, key_labs_json, key_imaging_json,
    correct_diagnosis, differential_diagnosis_json,
    treatment_plan_json, scoring_rubric_json,
    learning_objectives_json, tags_json, is_active
) VALUES (
    'Apendicitis Aguda Complicada - Perforación con Peritonitis Localizada',
    'Cirugía General',
    2,
    'Dolor en el lado derecho del abdomen que empeoró mucho hoy',
    '{"age":28,"sex":"F","weight_kg":58,"height_cm":162,"allergies":["penicilina"]}'::jsonb,
    '{"systolic_bp":112,"diastolic_bp":72,"heart_rate":108,"respiratory_rate":20,"temperature_c":38.8,"spo2_percent":98}'::jsonb,
    'Femenino 28 años. 72h dolor periumbilical → FID, náusea, vómito, fiebre. Alivio transitorio hace 12h (posible perforación). Beta-hCG negativa.',
    '{"abdomen":"Blumberg+ FID, Rovsing+, Psoas+, Obturador+, defensa muscular","general":"Posición antálgica pierna derecha flexionada"}'::jsonb,
    '{"leucocitos":"18200 bandas 8%","PCR":"168 mg/L","beta_hCG":"Negativa","urianálisis":"Normal"}'::jsonb,
    '["TAC: Apéndice 12mm, colección 2cm FID (absceso). Sin neumoperitoneo.","USG: Líquido libre periapendicular"]'::jsonb,
    'Apendicitis aguda complicada - Perforación con peritonitis localizada y absceso periapendicular',
    '["EPI (hCG negativa)","Quiste ovárico complicado","Diverticulitis de Meckel","Ileítis terminal"]'::jsonb,
    '["NPO + hidratación IV","Ketorolaco 30mg IV c/8h","Ciprofloxacino 400mg IV c/12h + Metronidazol 500mg IV c/8h","Apendicectomía laparoscópica urgente + lavado","Cultivo intraoperatorio","ATB 4-7 días postop"]'::jsonb,
    '{"anamnesis":0.20,"physical_exam":0.15,"diagnosis":0.35,"treatment":0.30}'::jsonb,
    '["Signos físicos apendicitis","Score Alvarado","Simple vs complicada","ATB en alergia penicilina","Intervalo vs inmediata"]'::jsonb,
    '["apendicitis","cirugia","perforacion","peritonitis","Alvarado"]'::jsonb,
    true
);

INSERT INTO clinical_cases (
    title, specialty, difficulty_level, chief_complaint,
    patient_demographics, vitals_json, history_of_present_illness,
    physical_exam_json, key_labs_json, key_imaging_json,
    correct_diagnosis, differential_diagnosis_json,
    treatment_plan_json, scoring_rubric_json,
    learning_objectives_json, tags_json, is_active
) VALUES (
    'Preeclampsia con Criterios de Severidad - 34 Semanas',
    'Ginecología y Obstetricia',
    3,
    'Dolor de cabeza muy fuerte y veo borroso, estoy embarazada',
    '{"age":32,"sex":"F","weight_kg":78,"height_cm":163,"gestational_weeks":34.2,"gravida":2,"para":1}'::jsonb,
    '{"systolic_bp":168,"diastolic_bp":112,"heart_rate":96,"respiratory_rate":18,"temperature_c":37.0,"spo2_percent":97}'::jsonb,
    'Femenino 32 años 34.2 SDG. Cefalea intensa + visión borrosa + fotofobia 6h. Edema facial y manos 3 días. Epigastralgia. PA previa 160/105. Sin convulsiones.',
    '{"general":"Edema facial++, ansiosa","neurological":"Hiperreflexia 3+, edema papila, espasmo arteriolar","abdomen":"FU 33cm, FCF 148, epigastralgia","extremidades":"Edema +++ godete"}'::jsonb,
    '{"proteinuria_24h":"4.8 g/24h","plaquetas":"89000","ALT":"186 U/L","AST":"210 U/L","LDH":"820 U/L","esquistocitos":"Presentes","creatinina":"1.2 mg/dL"}'::jsonb,
    '["USG: RCIU percentil 8, ILA 5.8 (oligohidramnios), Doppler umbilical flujo diastólico ausente","NST: Reactivo sin desaceleraciones"]'::jsonb,
    'Preeclampsia con criterios de severidad + Síndrome HELLP completo',
    '["Hipertensión gestacional","HTA crónica con preeclampsia sobreagregada","PTT","Hígado graso agudo embarazo"]'::jsonb,
    '["UCI obstétrica","MgSO4: 4-6g IV carga 15-20min + 1-2g/h mantenimiento","Labetalol 20mg IV c/10min meta PA 140-150/90-100","Betametasona 12mg IM c/24h x2","Cesárea urgente (HELLP + plaquetas <100K)","Plaquetas si <50K preqx","MgSO4 24-48h postparto","Gluconato calcio antídoto disponible"]'::jsonb,
    '{"anamnesis":0.20,"physical_exam":0.15,"diagnosis":0.35,"treatment":0.30}'::jsonb,
    '["Criterios severidad preeclampsia","HELLP por laboratorio","Sulfato magnesio dosis/monitorización","Antihipertensivos embarazo","Interrupción preeclampsia severa"]'::jsonb,
    '["preeclampsia","HELLP","obstetricia","sulfato_magnesio","eclampsia"]'::jsonb,
    true
);

-- ── Verificar inserción exitosa ──────────────────────────────
\echo ''
\echo '=== Verificación post-inserción ==='

SELECT 
    id,
    LEFT(title, 50) AS titulo,
    specialty,
    difficulty_level AS dif
FROM clinical_cases 
ORDER BY id;

\echo ''
SELECT 
    specialty,
    COUNT(*) as casos
FROM clinical_cases
GROUP BY specialty
ORDER BY specialty;

\echo ''
SELECT 
    COUNT(*) AS total_casos_en_db
FROM clinical_cases;

COMMIT;
