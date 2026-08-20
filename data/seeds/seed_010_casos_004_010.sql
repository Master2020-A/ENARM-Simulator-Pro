-- ================================================================
--  seed_010_casos_004_010.sql
--  Inserta casos clínicos 004-010 en la base de datos ENARM
--  Ejecutar: psql -U postgres -d enarm_db -f seed_010_casos_004_010.sql
-- ================================================================

BEGIN;

-- ── Caso 004: Angina Inestable ───────────────────────────────────
INSERT INTO clinical_cases (
    title, specialty_id, difficulty_level, chief_complaint,
    patient_demographics, vitals_json, history_of_present_illness,
    physical_exam_json, key_labs_json, key_imaging_json,
    correct_diagnosis, differential_diagnosis_json,
    treatment_plan_json, scoring_rubric_json,
    learning_objectives_json, tags_json, is_active
) VALUES (
    'Angina Inestable en Paciente con Factores de Riesgo Cardiovascular',
    (SELECT id FROM medical_specialties WHERE name ILIKE '%Cardiolog%' LIMIT 1),
    2,
    'Dolor en el pecho que me da en reposo desde ayer',
    '{"age":62,"sex":"M","weight_kg":88,"height_cm":172,"occupation":"Chofer de camión jubilado"}'::jsonb,
    '{"systolic_bp":158,"diastolic_bp":96,"heart_rate":94,"respiratory_rate":18,"temperature_c":36.8,"spo2_percent":97}'::jsonb,
    'Masculino de 62 años con dolor retroesternal opresivo 7/10 irradiado a brazo izquierdo, episódico en reposo, 18 horas de evolución. Episodio previo hace 3 semanas con esfuerzo.',
    '{"cardiovascular":"Ruidos rítmicos, sin soplos. Sin IY.","respiratory":"Limpios","abdomen":"Normal","neurological":"Sin focalización"}'::jsonb,
    '{"troponina_I":"0.04 ng/mL","CPK_MB":"18 U/L","LDL":"162 mg/dL","glucosa":"148 mg/dL"}'::jsonb,
    '["ECG: Depresión ST 1mm V4-V6 durante dolor, que se normaliza. Sin elevación. Sin ondas Q.","RX tórax: Normal, sin cardiomegalia"]'::jsonb,
    'Síndrome coronario agudo sin elevación del ST - Angina inestable',
    '["IAMCEST","IAMSEST","Disección aórtica","Espasmo coronario","ERGE"]'::jsonb,
    '["Monitorización continua","Aspirina 300 mg carga + 100 mg/día","Clopidogrel 300 mg carga","Enoxaparina 1 mg/kg c/12h","Metoprolol 25-50 mg VO","NTG SL PRN","Atorvastatina 80 mg/noche","Score TIMI/GRACE","Coronariografía en 24-72h"]'::jsonb,
    '{"anamnesis":0.20,"physical_exam":0.15,"diagnosis":0.35,"treatment":0.30}'::jsonb,
    '["Distinguir angina inestable de IAMSEST e IAMCEST","Interpretar cambios dinámicos de ST","Aplicar doble antiagregación","Calcular score TIMI/GRACE"]'::jsonb,
    '["SCA","SICA","angina_inestable","cardiologia","ECG","antiagregacion"]'::jsonb,
    true
);

-- ── Caso 005: NAC Grave ──────────────────────────────────────────
INSERT INTO clinical_cases (
    title, specialty_id, difficulty_level, chief_complaint,
    patient_demographics, vitals_json, history_of_present_illness,
    physical_exam_json, key_labs_json, key_imaging_json,
    correct_diagnosis, differential_diagnosis_json,
    treatment_plan_json, scoring_rubric_json,
    learning_objectives_json, tags_json, is_active
) VALUES (
    'Neumonía Adquirida en la Comunidad Grave - Criterios de UCI',
    (SELECT id FROM medical_specialties WHERE name ILIKE '%Neumol%' LIMIT 1),
    3,
    'Fiebre alta y me cuesta respirar desde hace 4 días',
    '{"age":71,"sex":"F","weight_kg":62,"height_cm":158,"occupation":"Pensionada"}'::jsonb,
    '{"systolic_bp":102,"diastolic_bp":64,"heart_rate":118,"respiratory_rate":28,"temperature_c":39.4,"spo2_percent":88}'::jsonb,
    'Femenino 71 años con EPOC, 4 días de fiebre 40°C, tos con expectoración herrumbrosa, disnea de reposo, confusión leve. Antígeno neumococo positivo.',
    '{"general":"Dificultad respiratoria, cianosis peribucal, desorientada","respiratory":"Matidez base derecha, crepitantes, broncofonía","cardiovascular":"Taquicardia, llenado capilar 3s"}'::jsonb,
    '{"leucocitos":"22400 con bandas 12%","PCR":"284 mg/L","procalcitonina":"4.8 ng/mL","PaO2":"52 mmHg","pH":"7.46","lactato":"2.8 mmol/L","antigeno_neumococo":"Positivo"}'::jsonb,
    '["RX tórax: Consolidación lobar LID. Sin derrame pleural significativo."]'::jsonb,
    'Neumonía adquirida en la comunidad grave por Streptococcus pneumoniae - CURB-65: 4',
    '["Agudización EPOC con sobreinfección","NAC por Legionella","Embolia pulmonar con infarto"]'::jsonb,
    '["UCI por CURB-65 ≥3","O2 Venturi 40% meta SpO2 88-92%","Ceftriaxona 2g IV c/24h + Azitromicina 500mg IV c/24h","Líquidos IV si hipotensión","Vasopresores si MAP <65","Broncodilatadores nebulizados","Profilaxis TVP"]'::jsonb,
    '{"anamnesis":0.20,"physical_exam":0.15,"diagnosis":0.35,"treatment":0.30}'::jsonb,
    '["Calcular CURB-65","Interpretar gasometría en EPOC+NAC","Antibiótico empírico en NAC grave","Manejar O2 en EPOC"]'::jsonb,
    '["NAC","neumonia","CURB65","UCI","neumologia","antibiotico","EPOC"]'::jsonb,
    true
);

-- ── Caso 006: Pancreatitis Aguda Severa ─────────────────────────
INSERT INTO clinical_cases (
    title, specialty_id, difficulty_level, chief_complaint,
    patient_demographics, vitals_json, history_of_present_illness,
    physical_exam_json, key_labs_json, key_imaging_json,
    correct_diagnosis, differential_diagnosis_json,
    treatment_plan_json, scoring_rubric_json,
    learning_objectives_json, tags_json, is_active
) VALUES (
    'Pancreatitis Aguda Severa por Litiasis Biliar',
    (SELECT id FROM medical_specialties WHERE name ILIKE '%Gastro%' LIMIT 1),
    3,
    'Dolor de panza que no se me quita y vomité muchas veces',
    '{"age":45,"sex":"F","weight_kg":82,"height_cm":160,"occupation":"Comerciante"}'::jsonb,
    '{"systolic_bp":96,"diastolic_bp":60,"heart_rate":124,"respiratory_rate":22,"temperature_c":38.6,"spo2_percent":94}'::jsonb,
    'Femenino 45 años con colelitiasis, 24h de dolor epigástrico irradiado a dorso 9/10, vómito incoercible, distensión abdominal progresiva, fiebre. Posible perforación por alivio transitorio.',
    '{"general":"Posición fetal, diaforética, ictérica leve","abdomen":"Distendido, defensa, Grey-Turner incipiente, Murphy +","cardiovascular":"Taquicardia, hipotensión, pulsos débiles"}'::jsonb,
    '{"lipasa":"1840 U/L","amilasa":"920 U/L","leucocitos":"18600 con bandas 8%","calcio":"7.8 mg/dL","bilirrubina":"3.2 mg/dL","trigliceridos":"480 mg/dL","CTSI":"7/10"}'::jsonb,
    '["USG: Colelitiasis, colédoco 10mm dilatado, líquido peripancreático","TAC Balthazar D-E: Necrosis >30%, colección peripancreática 2cm","RX tórax: Derrame pleural izquierdo pequeño"]'::jsonb,
    'Pancreatitis aguda severa por litiasis biliar (Ranson ≥3, CTSI 7, BISAP 2)',
    '["Úlcera péptica perforada","Colangitis aguda","Isquemia mesentérica","CAD con dolor abdominal"]'::jsonb,
    '["UCI/cuidados intermedios","NPO absoluto","Ringer Lactato 250-500 mL/h","Analgesia: morfina 0.05 mg/kg IV","Antieméticos","CPRE urgente si coledocolitiasis","NO antibiótico profiláctico","Nutrición enteral en 24-48h","Calcio IV si sintomático","Colecistectomía diferida 6-8 semanas"]'::jsonb,
    '{"anamnesis":0.20,"physical_exam":0.15,"diagnosis":0.35,"treatment":0.30}'::jsonb,
    '["Diagnosticar con lipasa/amilasa + imagen","Aplicar Ranson/BISAP/CTSI","Indicar Ringer Lactato vs SF","Decidir cuándo dar antibiótico","Indicar CPRE en coledocolitiasis"]'::jsonb,
    '["pancreatitis","litiasis_biliar","gastroenterologia","UCI","Ranson","CPRE"]'::jsonb,
    true
);

-- ── Caso 007: Hemorragia Intracerebral ──────────────────────────
INSERT INTO clinical_cases (
    title, specialty_id, difficulty_level, chief_complaint,
    patient_demographics, vitals_json, history_of_present_illness,
    physical_exam_json, key_labs_json, key_imaging_json,
    correct_diagnosis, differential_diagnosis_json,
    treatment_plan_json, scoring_rubric_json,
    learning_objectives_json, tags_json, is_active
) VALUES (
    'Hemorragia Intracerebral Hipertensiva - Ganglio Basal',
    (SELECT id FROM medical_specialties WHERE name ILIKE '%Neurol%' LIMIT 1),
    3,
    'Mi marido se desplomó y no puede mover el brazo derecho',
    '{"age":67,"sex":"M","weight_kg":79,"height_cm":168,"occupation":"Contador jubilado"}'::jsonb,
    '{"systolic_bp":210,"diastolic_bp":118,"heart_rate":88,"respiratory_rate":20,"temperature_c":37.1,"spo2_percent":95}'::jsonb,
    'Masculino 67 años HTA mal controlada. Cefalea thunderclap + caída + hemiparesia derecha + afasia mixta de inicio súbito. GCS 12. 45 minutos de evolución.',
    '{"neurological":"GCS 12, afasia mixta, hemiparesia derecha 1/5 brazo 2/5 pierna, Babinski derecho +, desviación mirada izquierda, hiperreflexia","cardiovascular":"HTA severa 210/118"}'::jsonb,
    '{"INR":"1.0","plaquetas":"198000","glucosa":"138 mg/dL","ECG":"Ritmo sinusal"}'::jsonb,
    '["TAC craneal URGENTE: Hematoma hiperdenso en putamen izquierdo 3.2x2.8cm (~12mL). Edema perilesional. Desviación línea media 2mm. Sin herniación."]'::jsonb,
    'Hemorragia intracerebral espontánea hipertensiva en ganglio basal izquierdo',
    '["ACV isquémico (excluido por hiperdensidad en TAC)","Hemorragia subaracnoidea","Tumor con sangrado intratumoral","MAV rota"]'::jsonb,
    '["Cabecera 30 grados","NO trombolíticos","Labetalol 20mg IV meta PA 130-150 sistólica","Control glucemia 140-180","Temperatura <37.5","Evaluación neuroquirúrgica (12mL conservador)","UCI neurológica","TAC control 24h","Rehabilitación temprana"]'::jsonb,
    '{"anamnesis":0.20,"physical_exam":0.15,"diagnosis":0.35,"treatment":0.30}'::jsonb,
    '["Distinguir HIC de ACV isquémico","Reconocer cefalea thunderclap","Manejar urgencia hipertensiva en HIC","Criterios quirúrgicos vs conservador"]'::jsonb,
    '["HIC","hemorragia_cerebral","ACV","neurologia","hipertension","emergencia"]'::jsonb,
    true
);

-- ── Caso 008: CAD ────────────────────────────────────────────────
INSERT INTO clinical_cases (
    title, specialty_id, difficulty_level, chief_complaint,
    patient_demographics, vitals_json, history_of_present_illness,
    physical_exam_json, key_labs_json, key_imaging_json,
    correct_diagnosis, differential_diagnosis_json,
    treatment_plan_json, scoring_rubric_json,
    learning_objectives_json, tags_json, is_active
) VALUES (
    'Cetoacidosis Diabética en Debut de DM Tipo 1',
    (SELECT id FROM medical_specialties WHERE name ILIKE '%Endocrin%' LIMIT 1),
    2,
    'Mi hijo lleva 3 días muy mal, no come y respira raro',
    '{"age":19,"sex":"M","weight_kg":65,"height_cm":178,"occupation":"Estudiante"}'::jsonb,
    '{"systolic_bp":104,"diastolic_bp":68,"heart_rate":116,"respiratory_rate":30,"temperature_c":37.2,"spo2_percent":98}'::jsonb,
    'Masculino 19 años sin antecedentes. 3 días de náusea, vómito, dolor abdominal, 3 semanas de poliuria, polidipsia y pérdida de 8 kg. Aliento a acetona. Respiración de Kussmaul.',
    '{"general":"Delgado, deshidratación severa, somnoliento, aliento a acetona","respiratory":"Kussmaul FR 30","neurological":"GCS 13, somnoliento"}'::jsonb,
    '{"glucosa":"518 mg/dL","pH":"7.18","HCO3":"8 mEq/L","cetonas_orina":"4+","beta_hidroxibutirato":"6.2 mmol/L","potasio":"5.8 mEq/L","anion_gap":"26","HbA1c":"13.8%","peptido_C":"0.1 ng/mL","anti_GAD":"Positivos"}'::jsonb,
    '["RX tórax: Normal","USG: Normal"]'::jsonb,
    'Cetoacidosis Diabética severa (pH <7.2, HCO3 <10) como debut de Diabetes Mellitus tipo 1',
    '["Estado hiperosmolar hiperglucémico","Acidosis láctica","Intoxicación salicilatos","Acidosis urémica"]'::jsonb,
    '["SF 0.9% 1L en 1h","NO insulina hasta K+ >3.5","Insulina regular 0.1 U/kg/h (sin bolo)","SF/0.45% 250-500mL/h","Dextrosa 5% cuando glucosa <200","KCl 20-40 mEq/L en sueros","NO bicarbonato (pH 7.18)","Glucemia c/1h, gases c/2-4h","Insulina basal SC 30min antes de suspender infusión"]'::jsonb,
    '{"anamnesis":0.20,"physical_exam":0.15,"diagnosis":0.35,"treatment":0.30}'::jsonb,
    '["Triada CAD: hiperglucemia+cetonemia+acidosis","Calcular AG y Na corregido","Secuencia: fluidos → K+ → insulina","Cuándo NO usar bicarbonato","Distinguir CAD de EHH"]'::jsonb,
    '["CAD","cetoacidosis","diabetes_tipo1","endocrinologia","Kussmaul","anion_gap"]'::jsonb,
    true
);

-- ── Caso 009: Apendicitis Complicada ────────────────────────────
INSERT INTO clinical_cases (
    title, specialty_id, difficulty_level, chief_complaint,
    patient_demographics, vitals_json, history_of_present_illness,
    physical_exam_json, key_labs_json, key_imaging_json,
    correct_diagnosis, differential_diagnosis_json,
    treatment_plan_json, scoring_rubric_json,
    learning_objectives_json, tags_json, is_active
) VALUES (
    'Apendicitis Aguda Complicada - Perforación con Peritonitis Localizada',
    (SELECT id FROM medical_specialties WHERE name ILIKE '%Cirug%' LIMIT 1),
    2,
    'Dolor en el lado derecho del abdomen que empeoró mucho hoy',
    '{"age":28,"sex":"F","weight_kg":58,"height_cm":162,"occupation":"Secretaria","allergies":["penicilina"]}'::jsonb,
    '{"systolic_bp":112,"diastolic_bp":72,"heart_rate":108,"respiratory_rate":20,"temperature_c":38.8,"spo2_percent":98}'::jsonb,
    'Femenino 28 años, 72h de dolor periumbilical que migró a FID, náusea, vómito, fiebre. Alivio transitorio hace 12h (posible perforación). Beta-hCG negativa.',
    '{"abdomen":"Distendido, Blumberg + FID, Rovsing +, Psoas +, Obturador +, defensa muscular","general":"Posición antálgica, pierna derecha flexionada"}'::jsonb,
    '{"leucocitos":"18200 bandas 8%","PCR":"168 mg/L","beta_hCG":"Negativa","urianálisis":"Normal"}'::jsonb,
    '["TAC abdomen: Apéndice 12mm, colección 2cm FID (absceso periapendicular). Sin neumoperitoneo.","USG: Líquido libre periapendicular, no visualiza apéndice"]'::jsonb,
    'Apendicitis aguda complicada - Perforación con peritonitis localizada y absceso periapendicular',
    '["EPI (beta-hCG negativa)","Quiste ovárico complicado","Diverticulitis de Meckel","Ileítis terminal"]'::jsonb,
    '["NPO + hidratación IV","Ketorolaco 30mg IV c/8h","Ciprofloxacino 400mg IV c/12h + Metronidazol 500mg IV c/8h (alergia penicilina)","Apendicectomía laparoscópica urgente + lavado","Cultivo intraoperatorio","Antibiótico 4-7 días postop"]'::jsonb,
    '{"anamnesis":0.20,"physical_exam":0.15,"diagnosis":0.35,"treatment":0.30}'::jsonb,
    '["Signos físicos de apendicitis","Score de Alvarado","Distinguir apendicitis simple de complicada","Antibiótico en alergia a penicilina","Apendicectomía de intervalo"]'::jsonb,
    '["apendicitis","cirugia","perforacion","peritonitis","Alvarado","laparoscopia"]'::jsonb,
    true
);

-- ── Caso 010: Preeclampsia + HELLP ──────────────────────────────
INSERT INTO clinical_cases (
    title, specialty_id, difficulty_level, chief_complaint,
    patient_demographics, vitals_json, history_of_present_illness,
    physical_exam_json, key_labs_json, key_imaging_json,
    correct_diagnosis, differential_diagnosis_json,
    treatment_plan_json, scoring_rubric_json,
    learning_objectives_json, tags_json, is_active
) VALUES (
    'Preeclampsia con Criterios de Severidad - 34 Semanas',
    (SELECT id FROM medical_specialties WHERE name ILIKE '%Ginecol%' OR name ILIKE '%Obstet%' LIMIT 1),
    3,
    'Dolor de cabeza muy fuerte y veo borroso, estoy embarazada',
    '{"age":32,"sex":"F","weight_kg":78,"height_cm":163,"occupation":"Contadora","gestational_weeks":34.2,"gravida":2,"para":1}'::jsonb,
    '{"systolic_bp":168,"diastolic_bp":112,"heart_rate":96,"respiratory_rate":18,"temperature_c":37.0,"spo2_percent":97}'::jsonb,
    'Femenino 32 años, 34.2 SDG. Cefalea intensa + visión borrosa + fotofobia 6h. Edema facial y manos 3 días. Epigastralgia. PA previa 160/105. Sin convulsiones.',
    '{"general":"Edema facial ++, ansiosa","neurological":"Hiperreflexia rotuliana 3+, edema papila, espasmo arteriolar","abdomen":"Fondo uterino 33cm, FCF 148, epigastralgia","extremidades":"Edema +++, godete"}'::jsonb,
    '{"proteinuria_24h":"4.8 g/24h","plaquetas":"89000","ALT":"186 U/L","AST":"210 U/L","LDH":"820 U/L","esquistocitos":"Presentes","creatinina":"1.2 mg/dL"}'::jsonb,
    '["USG: RCIU (percentil 8), ILA 5.8 (oligohidramnios), Doppler umbilical flujo diastólico ausente","NST: Reactivo sin desaceleraciones"]'::jsonb,
    'Preeclampsia con criterios de severidad + Síndrome HELLP completo',
    '["Hipertensión gestacional","HTA crónica con preeclampsia sobreagregada","PTT","Hígado graso agudo del embarazo"]'::jsonb,
    '["UCI obstétrica","Sulfato de magnesio: 4-6g IV carga en 15-20min, luego 1-2g/h mantenimiento","Labetalol 20mg IV c/10min meta PA 140-150/90-100","Betametasona 12mg IM c/24h x2 (maduración pulmonar)","Cesárea urgente (HELLP + plaquetas <100,000)","Transfusión plaquetas si <50,000","Continuar MgSO4 24-48h postparto","Gluconato calcio disponible como antídoto"]'::jsonb,
    '{"anamnesis":0.20,"physical_exam":0.15,"diagnosis":0.35,"treatment":0.30}'::jsonb,
    '["Criterios de severidad en preeclampsia","Diagnóstico HELLP por laboratorio","Administrar sulfato de magnesio","Antihipertensivos seguros en embarazo","Vía e indicación de interrupción"]'::jsonb,
    '["preeclampsia","HELLP","obstetricia","sulfato_magnesio","eclampsia"]'::jsonb,
    true
);

-- Verificar inserción
SELECT 
    COUNT(*) as total_casos,
    STRING_AGG(title, E'\\n  ') as casos_insertados
FROM clinical_cases
WHERE id >= 4;

COMMIT;

-- Reporte final
SELECT 
    specialty_id,
    COUNT(*) as casos_por_especialidad
FROM clinical_cases
GROUP BY specialty_id
ORDER BY specialty_id;
