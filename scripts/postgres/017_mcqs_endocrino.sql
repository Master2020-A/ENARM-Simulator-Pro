-- ================================================================
-- 017_mcqs_endocrino.sql
-- Sprint 7A.3: 10 MCQs de Endocrinologia estilo ENARM
-- ================================================================

BEGIN;

-- ---------- MCQ 1: CAD manejo ----------
INSERT INTO mcq_bank (specialty, difficulty, topic, vignette, question, options_json, correct_answer, explanation_correct, guidelines_ids, is_active)
VALUES ('Endocrinologia', 'advanced', 'CAD',
'Adolescente 16 anos con debut DM1: poliuria, polidipsia, perdida de peso 6 kg en 3 semanas, respiracion Kussmaul, deshidratacion severa. Glucosa 542, pH 7.08, HCO3 6, cetonas +++, K serico 5.4.',
'Cual es la SECUENCIA correcta de tratamiento?',
'{"A":"Bolo insulina 0.1 UI/kg + potasio + bicarbonato inmediato","B":"1) LIQUIDOS: SF 0.9% 10-20 ml/kg en 1a hora + 2) INSULINA infusion 0.05-0.1 UI/kg/h SIN bolo en pediatricos (1h despues de iniciar liquidos) + 3) POTASIO cuando K menor a 5.5 y hay diuresis + 4) DEXTROSA cuando glucosa menor a 250 + 5) Bicarbonato SOLO si pH menor a 6.9","C":"Solo hidratacion oral","D":"Metformina IV"}'::jsonb,
'B',
'La respuesta correcta es B. CAD pediatrica orden estricto: 1) HIDRATACION primero (SF 0.9% 10-20 ml/kg en 1a hora, luego SF 0.45% + reposicion), 2) INSULINA en infusion 0.05-0.1 UI/kg/h SIN BOLO (bolo aumenta riesgo edema cerebral en pediatricos), iniciar 1h despues de liquidos, 3) POTASIO agregar KCl 20-40 mEq/L cuando K menor a 5.5 con diuresis presente (insulina baja K), 4) DEXTROSA agregar cuando glucosa menor a 250 (evitar hipoglucemia), 5) BICARBONATO solo si pH menor a 6.9 (controversial). EVITAR descenso rapido de osmolaridad (riesgo EDEMA CEREBRAL, complicacion mas temida).',
ARRAY['IMSS-302-10']::text[], TRUE)
ON CONFLICT DO NOTHING;

-- ---------- MCQ 2: DM2 tratamiento ----------
INSERT INTO mcq_bank (specialty, difficulty, topic, vignette, question, options_json, correct_answer, explanation_correct, guidelines_ids, is_active)
VALUES ('Endocrinologia', 'intermediate', 'DM2',
'Mujer 55 anos con DM2 diagnostico reciente. HbA1c 8.2%, IMC 32, sin nefropatia. TFG 90. Sin ECV. Poco motivada para inyecciones.',
'Cual es el tratamiento INICIAL de eleccion?',
'{"A":"Insulina basal","B":"METFORMINA 500-1000 mg VO cada 12h (primera linea por: reduccion mortalidad CV, sin hipoglucemia, perdida de peso modesta) + cambios de estilo de vida","C":"Sulfonilureas","D":"Solo dieta"}'::jsonb,
'B',
'La respuesta correcta es B. METFORMINA es PRIMERA LINEA en DM2 (ADA/EASD 2023) por: 1) Reduccion mortalidad CV (UKPDS), 2) SIN hipoglucemia, 3) Perdida de peso modesta, 4) Bajo costo, 5) Contraindicada si TFG menor a 30. Titular gradualmente para minimizar GI. Meta HbA1c menor a 7% en general (menor a 6.5% en jovenes, menor a 8% en frailes). SEGUNDA LINEA (si HbA1c mayor a 1.5% arriba de meta o alto riesgo CV): SGLT2i (empagliflozina) o GLP-1RA (semaglutida) con beneficio CV/renal probado. Insulina (A) reservada si HbA1c mayor a 10% o descontrol severo.',
ARRAY['IMSS-093-08']::text[], TRUE)
ON CONFLICT DO NOTHING;

-- ---------- MCQ 3: Hipotiroidismo ----------
INSERT INTO mcq_bank (specialty, difficulty, topic, vignette, question, options_json, correct_answer, explanation_correct, guidelines_ids, is_active)
VALUES ('Endocrinologia', 'basic', 'Hipotiroidismo',
'Mujer 45 anos con fatiga, intolerancia al frio, aumento de peso 8 kg, estrenimiento, piel seca, bradicardia, cabello quebradizo. TSH 45 mUI/L (normal 0.4-4), T4 libre 0.4 (baja). Anti-TPO positivos.',
'Cual es el diagnostico y tratamiento?',
'{"A":"Hipotiroidismo primario por HASHIMOTO, LEVOTIROXINA 1.6 mcg/kg/dia VO en ayunas (ajustar por edad, embarazo, comorbilidades)","B":"Hipertiroidismo, propiltiouracilo","C":"Hipotiroidismo secundario, hidrocortisona","D":"Enfermedad de Graves, radioyodo"}'::jsonb,
'A',
'La respuesta correcta es A. Hipotiroidismo PRIMARIO (TSH ALTA + T4 BAJA) por TIROIDITIS DE HASHIMOTO (anti-TPO+). Tratamiento: LEVOTIROXINA VO en AYUNAS (30-60 min antes de desayuno, sin cafe, calcio, hierro). Dosis inicial 1.6 mcg/kg/dia en adultos jovenes. En mayores 65 anos, cardiopatia: iniciar 25-50 mcg/dia y titular lento. En EMBARAZO aumentar 25-30% (meta TSH menor a 2.5). Meta general TSH 0.5-2.5. Control cada 6 semanas hasta estable, luego anual.',
ARRAY['IMSS-093-08']::text[], TRUE)
ON CONFLICT DO NOTHING;

-- ---------- MCQ 4: Hipertiroidismo Graves ----------
INSERT INTO mcq_bank (specialty, difficulty, topic, vignette, question, options_json, correct_answer, explanation_correct, guidelines_ids, is_active)
VALUES ('Endocrinologia', 'intermediate', 'Graves',
'Mujer 32 anos con perdida de peso, palpitaciones, insomnio, temblor, exoftalmos, bocio difuso con soplo. TSH suprimida (0.01), T4 libre elevada 3.5, T3 elevada. Anti-TRAb positivos.',
'Cual es el tratamiento MEDICO de primera linea?',
'{"A":"Levotiroxina","B":"METIMAZOL 20-40 mg/dia VO (menor hepatotoxicidad que propiltiouracilo) + BETABLOQUEADOR (propranolol) para sintomas simpaticos","C":"Solo yodo radioactivo","D":"Tiroidectomia inmediata"}'::jsonb,
'B',
'La respuesta correcta es B. Enfermedad de Graves (hipertiroidismo autoinmune por anti-TRAb): 3 opciones: 1) TIONAMIDAS (METIMAZOL de eleccion, 20-40 mg/dia, menor hepatotoxicidad. PROPILTIOURACILO solo en 1er trimestre embarazo o crisis tirotoxica), duracion 12-18 meses. 2) YODO RADIOACTIVO (I-131) definitivo, contraindicado en embarazo y oftalmopatia severa. 3) TIROIDECTOMIA total en bocio muy grande, sospecha malignidad, embarazo con intolerancia farmaco. SIEMPRE BETABLOQUEADORES (propranolol) para sintomas simpaticos mientras alcanza eutiroidismo.',
ARRAY['IMSS-093-08']::text[], TRUE)
ON CONFLICT DO NOTHING;

-- ---------- MCQ 5: Sindrome de Cushing ----------
INSERT INTO mcq_bank (specialty, difficulty, topic, vignette, question, options_json, correct_answer, explanation_correct, guidelines_ids, is_active)
VALUES ('Endocrinologia', 'advanced', 'Cushing',
'Mujer 40 anos con obesidad central, cara de luna llena, estrias violaceas anchas en abdomen, HTA de novo, DM de novo, debilidad muscular proximal, hirsutismo.',
'Cual es el TAMIZAJE inicial mas apropiado?',
'{"A":"Solo cortisol matutino","B":"CORTISOL EN ORINA 24H (mayor a 3-4 veces normal es diagnostico) O CORTISOL SALIVAL NOCTURNO O TEST DE SUPRESION CON DEXAMETASONA 1mg NOCTURNO (cortisol matutino mayor a 1.8 sugiere Cushing)","C":"Solo ACTH plasmatica","D":"Test de estimulacion con ACTH"}'::jsonb,
'B',
'La respuesta correcta es B. Sospecha de sindrome de Cushing requiere 3 test de tamizaje (basta uno positivo, mejor 2): 1) CORTISOL LIBRE EN ORINA DE 24H mayor a 3-4x normal, 2) TEST DE SUPRESION CON DEXAMETASONA 1mg a las 23h y medir cortisol matutino (mayor a 1.8 mcg/dL positivo), 3) CORTISOL SALIVAL NOCTURNO (elevado si hay perdida del ritmo circadiano). Cortisol matutino solo (A) tiene variabilidad. Si tamizaje positivo, siguiente paso: medir ACTH para diferenciar Cushing dependiente ACTH (adenoma hipofisario o ectopico) vs INDEPENDIENTE (adenoma suprarrenal, iatrogenico).',
ARRAY['IMSS-093-08']::text[], TRUE)
ON CONFLICT DO NOTHING;

-- ---------- MCQ 6: Insuficiencia suprarrenal aguda ----------
INSERT INTO mcq_bank (specialty, difficulty, topic, vignette, question, options_json, correct_answer, explanation_correct, guidelines_ids, is_active)
VALUES ('Endocrinologia', 'advanced', 'Addison',
'Mujer 35 anos con enfermedad de Addison conocida, presenta 24h de vomitos por gastroenteritis. Ahora TA 78/40, taquicardia, deshidratacion severa, confusion, hiperpigmentacion, glucosa 55, Na 128, K 5.8.',
'Cual es el tratamiento INMEDIATO?',
'{"A":"Solo cristaloides","B":"HIDROCORTISONA 100 mg IV BOLO (luego 50-100 mg IV cada 6h) + SF 0.9% 1-2L bolo + DEXTROSA 50% si hipoglucemia + tratar causa desencadenante + reponer volumen segun deficit","C":"Adrenalina","D":"Metilprednisolona baja dosis"}'::jsonb,
'B',
'La respuesta correcta es B. CRISIS SUPRARRENAL: HIDROCORTISONA 100 mg IV BOLO inmediato (efecto glucocorticoide y mineralocorticoide en dosis altas), luego 50-100 mg IV cada 6h (200-400 mg/dia) por 24-48h. Simultaneamente: cristaloides SF 0.9% 1-2L bolo (choque hipovolemico + hiponatremia), dextrosa 50% IV si hipoglucemia, corregir electrolitos. IDENTIFICAR/TRATAR DESENCADENANTE (infeccion, cirugia, trauma). Prevencion: dosis STRESS de esteroides (triplicar en enfermedad, quintuplicar en cirugia). EDUCACION del paciente: brazalete medico + hidrocortisona autoinyectable.',
ARRAY['IMSS-093-08']::text[], TRUE)
ON CONFLICT DO NOTHING;

-- ---------- MCQ 7: Feocromocitoma ----------
INSERT INTO mcq_bank (specialty, difficulty, topic, vignette, question, options_json, correct_answer, explanation_correct, guidelines_ids, is_active)
VALUES ('Endocrinologia', 'advanced', 'Feocromocitoma',
'Hombre 45 anos con crisis paroxisticas de cefalea, palpitaciones, diaforesis, palidez, HTA severa (220/130), taquicardia. Entre episodios normal. Sospecha feocromocitoma.',
'Cual es el ESTUDIO diagnostico + preparacion PREQUIRURGICA?',
'{"A":"Solo cortisol","B":"METANEFRINAS PLASMATICAS FRACCIONADAS o METANEFRINAS URINARIAS 24H (S y E >95%) + TAC/RM abdominal para localizar + PREPARACION QUIRURGICA: alfa-bloqueo primero (fenoxibenzamina o doxazosina) 10-14 dias antes, DESPUES beta-bloqueo","C":"Renina","D":"TSH"}'::jsonb,
'B',
'La respuesta correcta es B. Feocromocitoma: TRIADA CLASICA (cefalea + palpitaciones + diaforesis). DIAGNOSTICO: METANEFRINAS PLASMATICAS FRACCIONADAS (metanefrina + normetanefrina) o METANEFRINAS URINARIAS 24H (sensibilidad y especificidad mayor a 95%). Localizacion: TAC o RM ABDOMINAL. PREPARACION PREQUIRURGICA CRUCIAL: 1) ALFA-BLOQUEO PRIMERO con fenoxibenzamina o doxazosina x 10-14 dias, 2) BETA-BLOQUEO DESPUES (nunca antes, causa crisis por vasoconstriccion sin oposicion alfa), 3) volumen. Tratamiento definitivo: adrenalectomia laparoscopica.',
ARRAY['IMSS-093-08']::text[], TRUE)
ON CONFLICT DO NOTHING;

-- ---------- MCQ 8: Hiperparatiroidismo primario ----------
INSERT INTO mcq_bank (specialty, difficulty, topic, vignette, question, options_json, correct_answer, explanation_correct, guidelines_ids, is_active)
VALUES ('Endocrinologia', 'intermediate', 'Hiperparatiroidismo',
'Mujer 55 anos asintomatica, calcio serico 11.8 mg/dL (normal 8.5-10.5) en analisis rutina. Repetido: 12.1. Fosforo bajo 2.2. PTH intacta ELEVADA 145 pg/mL (normal 10-65). 25-OH vitamina D normal.',
'Cual es el diagnostico + indicaciones de cirugia?',
'{"A":"Hipercalcemia por malignidad","B":"HIPERPARATIROIDISMO PRIMARIO (usualmente adenoma paratiroideo). Cirugia (paratiroidectomia) indicada si: sintomatico O calcio mayor a 1 mg arriba del limite O edad menor a 50 O TFG menor a 60 O osteoporosis (T-score menor a -2.5) O nefrolitiasis O calciuria mayor a 400","C":"Deficiencia vitamina D","D":"Metastasis oseas"}'::jsonb,
'B',
'La respuesta correcta es B. Hipercalcemia + PTH ALTA = HIPERPARATIROIDISMO PRIMARIO (85% adenoma solitario, 15% hiperplasia, menor a 1% carcinoma). Vs malignidad (PTH SUPRIMIDA + PTHrP alta). INDICACIONES QUIRURGICAS (paratiroidectomia): 1) Sintomatico (piedras, huesos, quejidos abdominales, tonos psiquicos), 2) Calcio mayor a 1 mg/dL arriba del normal, 3) Edad menor a 50 anos, 4) Osteoporosis (T-score menor a -2.5 o fractura por fragilidad), 5) TFG menor a 60, 6) Nefrolitiasis o calciuria mayor a 400 mg/24h. Estudios localizacion: sestamibi + USG cervical.',
ARRAY['IMSS-093-08']::text[], TRUE)
ON CONFLICT DO NOTHING;

-- ---------- MCQ 9: Nodulo tiroideo ----------
INSERT INTO mcq_bank (specialty, difficulty, topic, vignette, question, options_json, correct_answer, explanation_correct, guidelines_ids, is_active)
VALUES ('Endocrinologia', 'intermediate', 'Nodulo tiroideo',
'Mujer 42 anos, palpa nodulo tiroideo. USG: nodulo solido HIPOECOICO, 2.5 cm, con MICROCALCIFICACIONES, bordes irregulares, forma mas alta que ancha, vascularidad intranodal aumentada. TIRADS 5 (alta sospecha). TSH normal.',
'Cual es el siguiente paso?',
'{"A":"Solo observacion con USG anual","B":"BAAF (biopsia por aspiracion con aguja fina) del nodulo, guiada por USG (TIRADS 5 = biopsia si mayor o igual 1 cm)","C":"Yodo radioactivo","D":"Levotiroxina supresora"}'::jsonb,
'B',
'La respuesta correcta es B. Nodulo tiroideo TIRADS 5 (alta sospecha malignidad) requiere BAAF (biopsia aspiracion aguja fina) guiada por USG. Criterios de BAAF segun TIRADS: TR5 mayor o igual 1cm, TR4 mayor o igual 1.5cm, TR3 mayor o igual 2.5cm. Bethesda categoriza citologia: I (no diagnostico), II (benigno), III (atipia significado indeterminado, 5-15% malignidad), IV (folicular, 15-30%), V (sospechoso malignidad 60-75%), VI (maligno 97-99%). Papilar es el mas comun. Levotiroxina supresora (D) ya NO se recomienda para reducir tamano de nodulos.',
ARRAY['IMSS-093-08']::text[], TRUE)
ON CONFLICT DO NOTHING;

-- ---------- MCQ 10: Osteoporosis postmenopausica ----------
INSERT INTO mcq_bank (specialty, difficulty, topic, vignette, question, options_json, correct_answer, explanation_correct, guidelines_ids, is_active)
VALUES ('Endocrinologia', 'intermediate', 'Osteoporosis',
'Mujer 68 anos postmenopausica, sin fracturas, densitometria DEXA cadera: T-score -2.8. FRAX riesgo fractura cadera 10 anos 4%, mayor 12%. 25-OH vitamina D 22 ng/mL. Calcio y funcion renal normal.',
'Cual es el tratamiento?',
'{"A":"Solo calcio y vitamina D","B":"BIFOSFONATO (alendronato 70mg VO semanal en ayunas + agua + de pie 30 min) + CALCIO 1000-1200 mg/dia + VITAMINA D 800-2000 UI/dia + ejercicio de carga + prevencion caidas","C":"Terapia hormonal","D":"Denosumab primera linea"}'::jsonb,
'B',
'La respuesta correcta es B. Osteoporosis (T-score menor o igual -2.5) O fractura por fragilidad O FRAX cadera mayor o igual 3% O mayor mayor o igual 20% = TRATAMIENTO FARMACOLOGICO. PRIMERA LINEA: BIFOSFONATOS ORALES (alendronato 70mg semanal, risedronato 35mg semanal, ibandronato mensual). Instrucciones estrictas: EN AYUNAS, con AGUA (no jugo), MANTENERSE DE PIE 30 MIN (previene esofagitis). Duracion 5 anos (oral) o 3 anos IV, luego "vacaciones farmacologicas". CALCIO 1000-1200 mg + VITAMINA D 800-2000 UI diario. SEGUNDA LINEA: bifosfonato IV (zoledronato anual), denosumab (SC c/6m), teriparatida.',
ARRAY['IMSS-093-08']::text[], TRUE)
ON CONFLICT DO NOTHING;

COMMIT;

SELECT COUNT(*) AS total_endocrino FROM mcq_bank WHERE specialty = 'Endocrinologia';
