-- ================================================================
-- 011_mcqs_infectologia.sql
-- Sprint 7A.3: 10 MCQs de Infectologia estilo ENARM
-- ================================================================

BEGIN;

-- ---------- MCQ 1: Sepsis 1h bundle ----------
INSERT INTO mcq_bank (specialty, difficulty, topic, vignette, question, options_json, correct_answer, explanation_correct, guidelines_ids, is_active)
VALUES ('Infectologia', 'intermediate', 'Sepsis',
'Mujer 68 anos con DM2, fiebre 39C, TA 82/50, FC 128, FR 28, confusion, oliguria. Lactato 4.5 mmol/L. Sospecha foco urinario.',
'Cual es el orden CORRECTO del bundle de la primera hora?',
'{"A":"Antibiotico primero, luego hemocultivos y liquidos","B":"Hemocultivos, luego liquidos, luego antibiotico","C":"Hemocultivos + lactato + antibiotico de amplio espectro + cristaloides 30 ml/kg + vasopresores si persiste hipotension","D":"Solo liquidos y esperar cultivos"}'::jsonb,
'C',
'La respuesta correcta es C. El bundle SEP-1 de la primera hora incluye: 1) Medir lactato, 2) Tomar hemocultivos ANTES del antibiotico, 3) Antibiotico de amplio espectro (dentro de la 1a hora), 4) Cristaloides 30 ml/kg en hipotension o lactato >4, 5) Vasopresores (norepinefrina) si TA no mejora tras liquidos para meta PAM mayor o igual 65. NO retrasar antibiotico esperando cultivos si no se pueden obtener rapido.',
ARRAY['IMSS-283-13']::text[], TRUE)
ON CONFLICT DO NOTHING;

-- ---------- MCQ 2: Meningitis bacteriana ----------
INSERT INTO mcq_bank (specialty, difficulty, topic, vignette, question, options_json, correct_answer, explanation_correct, guidelines_ids, is_active)
VALUES ('Infectologia', 'advanced', 'Meningitis',
'Adulto 35 anos con cefalea intensa, fiebre 40C, rigidez de nuca, confusion. Punto lumbar: LCR turbio, glucosa 20 mg/dL (serica 100), proteinas 250, leucocitos 3,500 con 90% PMN. Gram: diplococos gram positivos.',
'Cual es el tratamiento antibiotico empirico + adyuvante MAS APROPIADO?',
'{"A":"Ceftriaxona 2g IV cada 12h + vancomicina 15-20 mg/kg cada 8-12h + dexametasona 0.15 mg/kg cada 6h","B":"Solo penicilina G","C":"Meropenem monoterapia","D":"Aciclovir por sospecha herpes"}'::jsonb,
'A',
'La respuesta correcta es A. Meningitis bacteriana en adulto joven-adulto: cobertura empirica ceftriaxona (S. pneumoniae, N. meningitidis, H. influenzae) + vancomicina (S. pneumoniae resistente). DEXAMETASONA 15-20 min ANTES o con la 1a dosis de ATB reduce mortalidad y secuelas en meningitis neumococica. Penicilina sola (B) no cubre resistencia. Meropenem (C) es alternativa. Aciclovir (D) es para encefalitis herpetica (LCR con linfocitos, no PMN).',
ARRAY['IMSS-102-08']::text[], TRUE)
ON CONFLICT DO NOTHING;

-- ---------- MCQ 3: ITU complicada ----------
INSERT INTO mcq_bank (specialty, difficulty, topic, vignette, question, options_json, correct_answer, explanation_correct, guidelines_ids, is_active)
VALUES ('Infectologia', 'intermediate', 'Pielonefritis',
'Mujer 45 anos con DM2 mal controlada, fiebre 39C, dolor lumbar derecho, disuria, nausea. Puno percusion positiva. Urocultivo pendiente. Creatinina 1.4. Sin sepsis.',
'Cual es el tratamiento inicial mas adecuado?',
'{"A":"Nitrofurantoina VO 5 dias","B":"Fosfomicina dosis unica","C":"Ceftriaxona 1g IV cada 24h + hospitalizacion","D":"TMP-SMX ambulatorio 3 dias"}'::jsonb,
'C',
'La respuesta correcta es C. Pielonefritis en paciente DIABETICO MAL CONTROLADO es ITU COMPLICADA que requiere hospitalizacion + antibiotico IV (ceftriaxona 1g/24h, quinolona IV, o piperacilina-tazobactam). Nitrofurantoina (A) NO alcanza niveles parenquimatosos renales. Fosfomicina (B) es para cistitis no complicada. TMP-SMX ambulatorio (D) inadecuado por riesgo bacteriemia y falla terapeutica en diabetico.',
ARRAY['IMSS-078-08']::text[], TRUE)
ON CONFLICT DO NOTHING;

-- ---------- MCQ 4: TB pulmonar tratamiento ----------
INSERT INTO mcq_bank (specialty, difficulty, topic, vignette, question, options_json, correct_answer, explanation_correct, guidelines_ids, is_active)
VALUES ('Infectologia', 'intermediate', 'Tuberculosis',
'Hombre 40 anos con tos productiva 2 meses, hemoptisis, perdida de peso 8 kg, sudoracion nocturna. Rx torax: infiltrado apical derecho con cavernas. BAAR en esputo positivo (2+). VIH negativo.',
'Cual es el esquema de tratamiento inicial?',
'{"A":"Isoniazida + rifampicina por 6 meses","B":"HRZE (isoniazida, rifampicina, pirazinamida, etambutol) por 2 meses, luego HR por 4 meses","C":"Solo isoniazida por 9 meses","D":"Estreptomicina + isoniazida por 12 meses"}'::jsonb,
'B',
'La respuesta correcta es B. Esquema estandar TB pulmonar sensible: FASE INTENSIVA 2 meses con HRZE (isoniazida 5 mg/kg + rifampicina 10 mg/kg + pirazinamida 25 mg/kg + etambutol 15 mg/kg) SEGUIDO de FASE DE SOSTEN 4 meses con HR. Total 6 meses. Monoterapia (C) causa resistencia. Regimenes no estandar (A, D) son inadecuados. Vigilar hepatotoxicidad, neuritis optica (etambutol), acido urico (pirazinamida).',
ARRAY['IMSS-070-08']::text[], TRUE)
ON CONFLICT DO NOTHING;

-- ---------- MCQ 5: VIH inicio TARV ----------
INSERT INTO mcq_bank (specialty, difficulty, topic, vignette, question, options_json, correct_answer, explanation_correct, guidelines_ids, is_active)
VALUES ('Infectologia', 'advanced', 'VIH',
'Hombre 32 anos con diagnostico reciente de VIH. Carga viral 250,000 copias/mL, CD4 320 cel/uL. Asintomatico. Sin coinfecciones. Sin embarazo (heterosexual).',
'Cual es la conducta correcta sobre inicio de TARV?',
'{"A":"Iniciar TARV solo cuando CD4 menos de 200","B":"Iniciar TARV inmediatamente independiente de CD4","C":"Esperar sintomatologia","D":"Solo profilaxis con TMP-SMX"}'::jsonb,
'B',
'La respuesta correcta es B. Desde 2015 (estudios START y TEMPRANO) se recomienda TARV a TODO paciente con VIH independiente de CD4. Beneficios: reduce transmision, previene progresion, mejora sobrevida. Esquemas de eleccion 1a linea: bictegravir/tenofovir alafenamida/emtricitabina O dolutegravir + tenofovir + lamivudina. Esperar CD4<200 (A) ya no se recomienda. TMP-SMX (D) es profilaxis contra PJP solo si CD4<200.',
ARRAY['IMSS-067-08']::text[], TRUE)
ON CONFLICT DO NOTHING;

-- ---------- MCQ 6: Celulitis vs erisipela ----------
INSERT INTO mcq_bank (specialty, difficulty, topic, vignette, question, options_json, correct_answer, explanation_correct, guidelines_ids, is_active)
VALUES ('Infectologia', 'basic', 'Infeccion tejidos blandos',
'Mujer 55 anos con lesion cutanea en pierna derecha: eritema brillante, edema, bordes elevados bien definidos, dolor y fiebre 38.5C de 24h.',
'Cual es el diagnostico y tratamiento?',
'{"A":"Celulitis por S. aureus, dicloxacilina","B":"Erisipela por Streptococcus pyogenes, penicilina o cefalosporina","C":"Fascitis necrosante, cirugia urgente","D":"Dermatitis por contacto, corticoides"}'::jsonb,
'B',
'La respuesta correcta es B. Erisipela: eritema BRILLANTE con BORDES ELEVADOS BIEN DEFINIDOS (piel de naranja), fiebre, mayormente por S. pyogenes. Tratamiento: penicilina VO/IV o cefalexina. Celulitis (A) tiene bordes MAL definidos y compromete tejido subcutaneo mas profundo, mas frecuente S. aureus. Fascitis necrosante (C) tiene dolor DESPROPORCIONADO, crepitos, toxicidad sistemica. Dermatitis (D) no cursa con fiebre.',
ARRAY['IMSS-263-13']::text[], TRUE)
ON CONFLICT DO NOTHING;

-- ---------- MCQ 7: Neumonia nosocomial ----------
INSERT INTO mcq_bank (specialty, difficulty, topic, vignette, question, options_json, correct_answer, explanation_correct, guidelines_ids, is_active)
VALUES ('Infectologia', 'advanced', 'NAV',
'Hombre 65 anos, dia 8 de ventilacion mecanica por SDRA, presenta fiebre 39C, aumento de secreciones purulentas, infiltrado nuevo en Rx, leucocitosis. Sospecha NAV.',
'Cual es la cobertura antibiotica empirica MAS APROPIADA?',
'{"A":"Ceftriaxona + azitromicina","B":"Amoxicilina-clavulanico","C":"Piperacilina-tazobactam + vancomicina + amikacina","D":"Cefalotina monoterapia"}'::jsonb,
'C',
'La respuesta correcta es C. Neumonia asociada a ventilacion (NAV) TARDIA (mayor o igual 5 dias) requiere cobertura de Pseudomonas + MRSA + BGN multirresistentes: antipseudomonico (piperacilina-tazobactam, cefepime, meropenem) + vancomicina/linezolid (MRSA) + aminoglucosido o quinolona (doble cobertura Gram negativos). Ceftriaxona (A) es para NAC. Cobertura estrecha (B, D) inadecuada en nosocomial.',
ARRAY['IMSS-234-09']::text[], TRUE)
ON CONFLICT DO NOTHING;

-- ---------- MCQ 8: Dengue grave ----------
INSERT INTO mcq_bank (specialty, difficulty, topic, vignette, question, options_json, correct_answer, explanation_correct, guidelines_ids, is_active)
VALUES ('Infectologia', 'intermediate', 'Dengue',
'Mujer 28 anos, dia 5 de fiebre por dengue confirmado. Ahora afebril pero con dolor abdominal intenso, vomitos, letargia. TA 90/70, FC 115. Hematocrito 52% (previo 42), plaquetas 40,000.',
'Cual es la conducta INMEDIATA?',
'{"A":"Alta con paracetamol","B":"Transfundir plaquetas profilactica","C":"Reanimacion con cristaloides IV y hospitalizacion (dengue grave)","D":"Iniciar corticoides IV"}'::jsonb,
'C',
'La respuesta correcta es C. Estos son signos de ALARMA (dolor abdominal, vomitos, letargia, hemoconcentracion, trombocitopenia rapida) que indican FASE CRITICA de dengue con FUGA CAPILAR. Requiere hospitalizacion y REANIMACION CON CRISTALOIDES (Ringer o SF 10 ml/kg en 1h). Plaquetas (B) NO se transfunden profilacticamente, solo si sangrado activo o menos de 10,000. Corticoides (D) NO tienen evidencia. Alta (A) es negligente.',
ARRAY['IMSS-151-08']::text[], TRUE)
ON CONFLICT DO NOTHING;

-- ---------- MCQ 9: Profilaxis post-exposicion VIH ----------
INSERT INTO mcq_bank (specialty, difficulty, topic, vignette, question, options_json, correct_answer, explanation_correct, guidelines_ids, is_active)
VALUES ('Infectologia', 'intermediate', 'Profilaxis VIH',
'Enfermera se pincha con aguja usada en paciente VIH positivo con carga viral 50,000 copias hace 30 minutos. Herida profunda con sangre visible en la aguja. VIH basal de enfermera negativo.',
'Cual es la conducta correcta?',
'{"A":"Solo observacion y serologia a los 3 meses","B":"Iniciar PPE con esquema triple (tenofovir/emtricitabina + raltegravir o dolutegravir) por 28 dias, iniciar antes de 72h ideal en menos de 2h","C":"Vacuna anti-VIH","D":"Inmunoglobulina especifica"}'::jsonb,
'B',
'La respuesta correcta es B. Exposicion de ALTO RIESGO (herida profunda + carga viral alta) requiere PPE INMEDIATA (idealmente en menos de 2h, hasta 72h): tenofovir/emtricitabina + inhibidor integrasa (raltegravir/dolutegravir) por 28 dias. Seguimiento serologico a 6 semanas, 3 y 6 meses. Vacuna VIH (C) NO existe. Inmunoglobulina (D) es para hepatitis B, no VIH. Solo observar (A) es negligencia.',
ARRAY['IMSS-241-12']::text[], TRUE)
ON CONFLICT DO NOTHING;

-- ---------- MCQ 10: C. difficile ----------
INSERT INTO mcq_bank (specialty, difficulty, topic, vignette, question, options_json, correct_answer, explanation_correct, guidelines_ids, is_active)
VALUES ('Infectologia', 'intermediate', 'Colitis pseudomembranosa',
'Mujer 70 anos, 10 dias post-tratamiento con clindamicina por celulitis, presenta 8 evacuaciones diarreicas al dia con moco, dolor abdominal, fiebre 38.5. Leucocitos 18,000. Toxina C. difficile positiva.',
'Cual es el tratamiento de PRIMERA linea?',
'{"A":"Metronidazol VO 500mg cada 8h","B":"Vancomicina VO 125mg cada 6h por 10 dias","C":"Ceftriaxona IV","D":"Ciprofloxacino VO"}'::jsonb,
'B',
'La respuesta correcta es B. Desde 2018 (IDSA), VANCOMICINA VO 125mg cada 6h por 10 dias ES la PRIMERA linea para C. difficile (leve, moderada, severa). Metronidazol (A) ya NO es primera linea por menor eficacia. Fidaxomicina es alternativa (menos recurrencias). Ceftriaxona/quinolonas (C, D) EMPEORAN al perpetuar disbiosis. SUSPENDER el antibiotico causal. Recurrencia: vancomicina en pulsos o transplante fecal.',
ARRAY['IMSS-563-12']::text[], TRUE)
ON CONFLICT DO NOTHING;

COMMIT;

SELECT COUNT(*) AS total_infectologia FROM mcq_bank WHERE specialty = 'Infectologia';
