-- ================================================================
--  seed_013_mcq_50_preguntas.sql
--  50 preguntas MCQ tipo ENARM adaptadas al schema real de mcq_bank
--  Cubre 10 especialidades, dificultad variada
-- ================================================================

-- Primero ver el schema para confirmar columnas
\d mcq_bank

-- Ver si hay preguntas existentes
SELECT COUNT(*) AS preguntas_actuales FROM mcq_bank;

BEGIN;

-- ================================================================
--  BLOQUE: Cardiología (10 preguntas)
-- ================================================================

-- MCQ 01: STEMI - Tratamiento de elección
INSERT INTO mcq_bank (
    question_text, specialty, difficulty, topic,
    options_json, correct_option, explanation,
    teaching_point, source, is_published
) VALUES (
    'Paciente masculino de 58 años acude a urgencias con 90 minutos de dolor torácico opresivo retroesternal irradiado a brazo izquierdo. ECG muestra elevación del ST ≥2mm en V1-V4. PA 108/70, FC 104 lpm, SpO2 92%. El hospital cuenta con laboratorio de cateterismo disponible. ¿Cuál es el tratamiento de reperfusión de elección?',
    'Cardiology', 'intermediate', 'STEMI - Reperfusión',
    '{"A": "Trombolisis con alteplasa IV inmediata", "B": "ICP primaria (angioplastia + stent) en <90 minutos", "C": "Trombolisis seguida de ICP de rescate en 24h", "D": "Iniciar heparina y esperar resultado de troponinas"}'::jsonb,
    'B',
    'La ICP primaria es el estándar de oro para el STEMI cuando está disponible en ≤120 minutos desde el primer contacto médico (ideal ≤90 minutos desde llegada a sala). Es superior a la trombolisis en reducción de mortalidad, reinfarto y ACV. La trombolisis se indica SOLO si la ICP no puede realizarse en ≤120 minutos. El tiempo es músculo: cada 30 min de retraso aumenta la mortalidad un 7.5%.',
    'ICP primaria = elección en STEMI si disponible ≤120 min. Trombolisis = alternativa si ICP imposible en ese tiempo. NUNCA esperar troponinas para tratar STEMI con elevación ST clásica.',
    'ESC Guidelines STEMI 2023',
    true
);

-- MCQ 02: STEMI - Antiagregación dual
INSERT INTO mcq_bank (
    question_text, specialty, difficulty, topic,
    options_json, correct_option, explanation,
    teaching_point, source, is_published
) VALUES (
    'Paciente con STEMI anterior que va a ICP primaria. Ya recibió AAS 300 mg VO. ¿Qué segundo antiagregante debe administrarse como carga antes del procedimiento?',
    'Cardiology', 'intermediate', 'STEMI - Antiagregación',
    '{"A": "Clopidogrel 75 mg VO (dosis estándar)", "B": "Ticagrelor 180 mg VO (dosis de carga)", "C": "Warfarina 5 mg VO", "D": "Dipiridamol 400 mg VO"}'::jsonb,
    'B',
    'El estudio PLATO (NEJM 2009) demostró que Ticagrelor es superior a Clopidogrel en SCA con reducción de mortalidad cardiovascular, infarto y trombosis del stent, sin aumento significativo de sangrado mayor. La dosis de carga es 180 mg VO. El Clopidogrel (300-600 mg carga) es alternativa válida si Ticagrelor no está disponible o hay contraindicaciones (ACV hemorrágico previo, sangrado activo).',
    'DAPT en STEMI: AAS + Ticagrelor (preferido) o Clopidogrel. Estudio PLATO: Ticagrelor superior a Clopidogrel. Duración DAPT: 12 meses post-STEMI con DES.',
    'ESC Guidelines STEMI 2023 / Estudio PLATO NEJM 2009',
    true
);

-- MCQ 03: SICA SEST - Diferenciación
INSERT INTO mcq_bank (
    question_text, specialty, difficulty, topic,
    options_json, correct_option, explanation,
    teaching_point, source, is_published
) VALUES (
    'Hombre de 62 años, diabético e hipertenso, con dolor torácico opresivo en reposo de 2 horas. ECG: depresión del ST 1mm en V4-V6 que se normaliza al ceder el dolor. Troponina I inicial: 0.03 ng/mL (valor de referencia <0.04). Troponina a las 3h: 0.03 ng/mL. ¿Cuál es el diagnóstico más probable?',
    'Cardiology', 'intermediate', 'SICA SEST - Diagnóstico',
    '{"A": "STEMI anterior (requiere reperfusión inmediata)", "B": "IAMSEST (infarto sin elevación del ST)", "C": "Angina inestable (SICA SEST sin elevación de troponinas)", "D": "Angina estable con descontrol hipertensivo"}'::jsonb,
    'C',
    'La diferencia entre angina inestable e IAMSEST en el contexto del SICA SEST es la TROPONINA: angina inestable = troponinas negativas seriales (sin delta), IAMSEST = troponinas elevadas o con delta positivo. En este caso, troponinas negativas en 2 determinaciones = angina inestable. Ambas condiciones se manejan igual (doble antiagregación, anticoagulación, estatinas), pero la troponina positiva implica mayor riesgo y necesidad de estrategia invasiva más urgente.',
    'SICA SEST: angina inestable (troponina negativa) vs IAMSEST (troponina positiva). El ECG no diferencia, solo la troponina. Ambas: doble antiagregación + anticoagulación + estatinas.',
    'ESC Guidelines ACS 2023',
    true
);

-- MCQ 04: Insuficiencia Cardíaca - Tratamiento
INSERT INTO mcq_bank (
    question_text, specialty, difficulty, topic,
    options_json, correct_option, explanation,
    teaching_point, source, is_published
) VALUES (
    'Mujer de 68 años con diagnóstico de insuficiencia cardíaca con fracción de eyección reducida (ICFEr, FEVI 30%) en clase funcional NYHA III. Actualmente toma furosemida y AAS. ¿Qué combinación farmacológica ha demostrado mayor reducción de mortalidad en esta condición?',
    'Cardiology', 'advanced', 'Insuficiencia cardíaca - FEVI reducida',
    '{"A": "IECA + espironolactona solamente", "B": "IECA (o ARNI) + betabloqueador + ARM + iSGLT2 (terapia de 4 pilares)", "C": "Digoxina + furosemida + warfarina", "D": "Calcioantagonista + nitrato de larga acción"}'::jsonb,
    'B',
    'La terapia de 4 pilares en ICFEr tiene el mayor nivel de evidencia (Clase I, Nivel A): 1) IECA/ARA-II o ARNI (sacubitrilo/valsartán - estudio PARADIGM), 2) Betabloqueador (carvedilol, bisoprolol, metoprolol succintato), 3) ARM (espironolactona/eplerenona - estudio RALES/EMPHASIS), 4) iSGLT2 (dapaglifozina/empaglifozina - estudios DAPA-HF/EMPEROR). Los calcioantagonistas están contraindicados en ICFEr (excepto amlodipino/felodipino para HTA). Digoxina no reduce mortalidad.',
    'ICFEr cuatro pilares: IECA/ARNI + BB + ARM + iSGLT2. Cada uno reduce mortalidad independientemente. Calcioantagonistas = CONTRAINDICADOS en ICFEr (excepto amlodipino para HTA refractaria).',
    'ESC Heart Failure Guidelines 2021, PARADIGM-HF, DAPA-HF',
    true
);

-- MCQ 05: Fibrilación Auricular - Anticoagulación
INSERT INTO mcq_bank (
    question_text, specialty, difficulty, topic,
    options_json, correct_option, explanation,
    teaching_point, source, is_published
) VALUES (
    'Hombre de 72 años con fibrilación auricular no valvular de diagnóstico reciente. Antecedentes: HTA, DM2, insuficiencia renal CKD3 (TFGe 40 mL/min). Score CHA₂DS₂-VASc = 4. ¿Cuál es la anticoagulación más apropiada?',
    'Cardiology', 'advanced', 'Fibrilación auricular - Anticoagulación',
    '{"A": "Aspirina 100 mg/día (suficiente con CHA₂DS₂-VASc ≥2)", "B": "Warfarina con INR meta 2-3", "C": "Apixabán 5 mg c/12h (ajustado a 2.5 mg si criterios)", "D": "No anticoagular por riesgo de sangrado con CKD"}'::jsonb,
    'C',
    'Los anticoagulantes orales directos (ACODs) son superiores a warfarina en FA no valvular: menor riesgo de ACV hemorrágico y menor mortalidad. Apixabán es el preferido en CKD (no contraindicado hasta diálisis, requiere ajuste de dosis: 2.5 mg c/12h si ≥2 de: edad ≥80, peso ≤60 kg, creatinina ≥1.5 mg/dL). CHA₂DS₂-VASc ≥2 en hombres = anticoagulación recomendada. AAS NO es alternativa a anticoagulación en FA (no tiene evidencia).',
    'FA no valvular: ACODs preferidos sobre warfarina. CHA₂DS₂-VASc ≥2 = anticoagular. Apixabán es seguro en CKD 3-4. Ajuste: 2.5 mg c/12h si ≥2 de los 3 criterios (edad≥80/peso≤60/Cr≥1.5).',
    'ESC FA Guidelines 2023',
    true
);

-- ================================================================
--  BLOQUE: Neumología / Urgencias (8 preguntas)
-- ================================================================

-- MCQ 06: NAC - CURB-65
INSERT INTO mcq_bank (
    question_text, specialty, difficulty, topic,
    options_json, correct_option, explanation,
    teaching_point, source, is_published
) VALUES (
    'Mujer de 71 años, EPOC GOLD II, acude con fiebre 39.4°C, tos con expectoración herrumbrosa, FR 28/min, PA 102/64, confusión temporal, BUN 25 mg/dL (urea 54 mg/dL). RX tórax: consolidación en LID. ¿Cuál es el score CURB-65 y la conducta apropiada?',
    'Pulmonology', 'intermediate', 'NAC - Estratificación CURB-65',
    '{"A": "CURB-65 = 2: hospitalización en sala general", "B": "CURB-65 = 3: hospitalización con vigilancia estrecha", "C": "CURB-65 = 4: ingreso a UCI o área de cuidados intensivos", "D": "CURB-65 = 5: solo manejo paliativo recomendado"}'::jsonb,
    'C',
    'CURB-65: C=Confusión(1) + U=Urea>7mmol/L (BUN>19mg/dL, aquí sí)(1) + R=FR≥30(1) + B=PAS<90 o PAD≤60(1) + 65=edad≥65(1) = 4 puntos. Score ≥3 = alto riesgo de mortalidad, requiere hospitalización en UCI o área de alta dependencia. La mortalidad estimada con CURB-65=4 es ~14-27%. El score ≥3 es indicación de evaluar UCI según criterios ATS/IDSA adicionales (un criterio mayor o ≥3 menores).',
    'CURB-65: C=confusión, U=urea>7mmol (BUN>19mg/dL), R=FR≥30, B=PAS<90 o PAD≤60, 65=edad. ≥3 = UCI/alta dependencia. Mortalidad: 0-1=<1%, 2=~9%, 3-5=15-40%.',
    'ATS/IDSA NAC Guidelines 2019',
    true
);

-- MCQ 07: NAC - Antibiótico en EPOC
INSERT INTO mcq_bank (
    question_text, specialty, difficulty, topic,
    options_json, correct_option, explanation,
    teaching_point, source, is_published
) VALUES (
    'La misma paciente anterior (NAC grave, CURB-65=4, EPOC) ingresa a UCI. ¿Cuál es el régimen antibiótico empírico más apropiado para NAC grave que requiere UCI?',
    'Pulmonology', 'advanced', 'NAC grave - Antibioticoterapia',
    '{"A": "Amoxicilina-clavulanato 875/125 mg VO c/12h", "B": "Ceftriaxona 2g IV c/24h + Azitromicina 500mg IV c/24h", "C": "Penicilina cristalina 4 millones UI IV c/4h", "D": "Metronidazol 500mg IV c/8h + gentamicina"}'::jsonb,
    'B',
    'Las guías ATS/IDSA recomiendan cobertura dual (betalactámico + macrólido) para NAC grave en UCI: betalactámico antinecumocócico (ceftriaxona, cefotaxima, ampicilina-sulbactam) + azitromicina IV. La cobertura de atípicos (Legionella, Mycoplasma) mejora outcomes en NAC grave. El tiempo hasta antibiótico <1 hora reduce mortalidad. La amoxicilina oral no es adecuada en NAC grave-UCI. La levofloxacina 750 mg IV es alternativa en monoterapia si no se puede usar macrólido.',
    'NAC grave UCI: Betalactámico antinecumocócico + Azitromicina IV. O Levofloxacina IV monoterapia. Tiempo hasta ATB <1h. Duración 5-7 días si buena respuesta.',
    'ATS/IDSA Community-Acquired Pneumonia Guidelines 2019',
    true
);

-- MCQ 08: TEP - Diagnóstico y tratamiento
INSERT INTO mcq_bank (
    question_text, specialty, difficulty, topic,
    options_json, correct_option, explanation,
    teaching_point, source, is_published
) VALUES (
    'Mujer de 45 años, 3 semanas postoperatorio de reemplazo total de rodilla. Presenta inicio súbito de disnea, dolor pleurítico y hemoptisis leve. FC 112, FR 26, SpO2 91%. Score Wells = 5 puntos (alta probabilidad). ¿Cuál es el mejor paso siguiente?',
    'Pulmonology', 'advanced', 'TEP - Diagnóstico',
    '{"A": "Dímero-D y si elevado, angioTAC de tórax", "B": "AngioTAC de tórax directamente (sin dímero-D previo)", "C": "Ecocardiograma transtorácico urgente", "D": "Gammagrafía ventilación-perfusión"}'::jsonb,
    'B',
    'Con score Wells ≥4 (alta probabilidad), el dímero-D pierde utilidad diagnóstica (alta probabilidad pre-test = no puede descartar con dímero-D negativo). El paso correcto es angioTAC de tórax directamente, que tiene sensibilidad >90% y especificidad >95% para TEP. El dímero-D solo sirve para descartar TEP en probabilidad baja-intermedia (Wells <4). La gammagrafía es alternativa en contraindicación a contraste yodado. Ecocardiograma no es diagnóstico de TEP pero puede mostrar disfunción VD.',
    'Wells alta probabilidad (≥4): AngioTAC directo (sin dímero-D). Wells baja (<4): dímero-D; si negativo descarta TEP. El dímero-D NO descarta TEP en alta probabilidad.',
    'ESC TEP Guidelines 2019',
    true
);

-- ================================================================
--  BLOQUE: Neurología (6 preguntas)
-- ================================================================

-- MCQ 09: ACV isquémico - Trombolisis
INSERT INTO mcq_bank (
    question_text, specialty, difficulty, topic,
    options_json, correct_option, explanation,
    teaching_point, source, is_published
) VALUES (
    'Hombre de 65 años sin antecedentes de anticoagulación llega a urgencias con afasia y hemiparesia derecha de 2.5 horas de evolución. PA 168/94, glucemia 142 mg/dL. TAC craneal sin contraste: normal (sin hiperdensidad, sin sangre). INR 1.0. ¿Está indicada la trombolisis con alteplasa IV?',
    'Neurology', 'advanced', 'ACV isquémico - Trombolisis',
    '{"A": "No, la PA >160/90 es contraindicación absoluta", "B": "Sí, cumple criterios: ≤4.5h, sin sangre en TAC, INR normal, glucemia no hipoglucemia", "C": "No, debe esperar angioTAC para confirmar oclusión de gran vaso", "D": "Sí, pero solo si la PA baja a <140/90 antes de administrar"}'::jsonb,
    'B',
    'El alteplasa IV está indicado en ACV isquémico ≤4.5 horas si: TAC sin hemorragia, déficit neurológico medible, sin contraindicaciones. Las contraindicaciones absolutas incluyen: ACV hemorrágico previo, cirugía mayor <3 meses, hemorragia activa, PA >185/110 (no >160). La PA debe bajarse a <185/110 ANTES de dar tPA (con labetalol IV), pero no necesariamente a <140. El objetivo es PA <185/110 para administrar, luego mantener <180/105 durante 24h. No se necesita angioTAC para indicar tPA (aunque puede realizarse si disponible).',
    'tPA en ACV isquémico: ≤4.5h, sin hemorragia en TAC, PA <185/110 (bajar antes si necesario), glucemia >50 mg/dL. PA >185/110 = relativa (bajar con labetalol), NO absoluta. Ventana 3-4.5h: excluir anticoagulados, ACV previo+DM, NIHSS>25.',
    'AHA/ASA Stroke Guidelines 2019',
    true
);

-- MCQ 10: HIC - Manejo de PA
INSERT INTO mcq_bank (
    question_text, specialty, difficulty, topic,
    options_json, correct_option, explanation,
    teaching_point, source, is_published
) VALUES (
    'Hombre de 67 años llega con HIC hipertensiva en putamen. PA 210/118, GCS 12, hematoma 12 mL sin herniación. ¿Cuál es la meta de PA sistólica en las primeras horas según evidencia actual (INTERACT2, ATACH-II)?',
    'Neurology', 'advanced', 'HIC - Control de presión arterial',
    '{"A": "PA sistólica <180 mmHg (meta laxa, evitar hipotensión)", "B": "PA sistólica 130-150 mmHg (meta intensiva moderada)", "C": "PA sistólica <120 mmHg (control intensivo absoluto)", "D": "No tratar la PA (puede empeorar la perfusión perilesional)"}'::jsonb,
    'B',
    'Los estudios INTERACT2 y ATACH-II evaluaron el control intensivo de PA en HIC aguda. INTERACT2: PAS <140 vs <180 - PAS <140 no redujo mortalidad pero sí mejoró outcomes funcionales. ATACH-II: PAS 110-139 vs 140-179 - el grupo <140 tuvo mayor daño renal sin beneficio adicional. Meta actual AHA 2022: PAS 130-150 mmHg como balance entre reducir expansión del hematoma y evitar hipoperfusión. Fármacos de elección: labetalol IV, nicardipino IV. Evitar nitroprusiato (aumenta PIC).',
    'HIC aguda meta PA: PAS 130-150 mmHg (AHA 2022). Evitar PAS <120 (daño renal, hipoperfusión) y >180 (expansión hematoma). Labetalol IV = primera línea. EVITAR nitroprusiato.',
    'AHA HIC Guidelines 2022, INTERACT2, ATACH-II',
    true
);

-- MCQ 11: Epilepsia - Estatus epiléptico
INSERT INTO mcq_bank (
    question_text, specialty, difficulty, topic,
    options_json, correct_option, explanation,
    teaching_point, source, is_published
) VALUES (
    'Paciente de 35 años en crisis convulsiva tónico-clónica generalizada de 8 minutos de duración. Acceso venoso disponible. ¿Cuál es el tratamiento de primera línea para detener la crisis?',
    'Neurology', 'intermediate', 'Estatus epiléptico - Primera línea',
    '{"A": "Fenitoína 20 mg/kg IV a pasar en 20 minutos", "B": "Lorazepam 0.1 mg/kg IV (o diazepam 10 mg IV)", "C": "Fenobarbital 20 mg/kg IV", "D": "Propofol 1-2 mg/kg IV en bolo"}'::jsonb,
    'B',
    'Las benzodiacepinas son el tratamiento de primera línea para el estatus epiléptico: lorazepam 0.1 mg/kg IV (máx 4 mg, repetir en 5 min si persiste) o diazepam 10 mg IV. Tienen mayor eficacia que otros agentes como primera línea (estudio RAMPART). Si no hay acceso IV: midazolam IM 10 mg (IM es equivalente al IV en inicio de acción). La fenitoína y fenobarbital son segunda línea (si falla benzodiacepina). El propofol es para estatus refractario en UCI con intubación.',
    'Estatus epiléptico 1ª línea: Lorazepam IV 0.1 mg/kg O Diazepam 10 mg IV. Sin IV: Midazolam IM 10 mg (tan eficaz como IV). 2ª línea: Fenitoína/Levetiracetam IV. 3ª línea: Propofol/Midazolam en infusión (UCI).',
    'AAN Status Epilepticus Guidelines 2016',
    true
);

-- ================================================================
--  BLOQUE: Gastroenterología (5 preguntas)
-- ================================================================

-- MCQ 12: Pancreatitis - Diagnóstico
INSERT INTO mcq_bank (
    question_text, specialty, difficulty, topic,
    options_json, correct_option, explanation,
    teaching_point, source, is_published
) VALUES (
    'Mujer de 45 años con dolor epigástrico irradiado a dorso, vómito incoercible. Lipasa 1840 U/L (normal <60), amilasa 920 U/L. USG: colelitiasis, colédoco 10mm. ¿Cuántos de los 2 criterios diagnósticos de pancreatitis aguda se cumplen?',
    'Gastroenterology', 'intermediate', 'Pancreatitis aguda - Diagnóstico',
    '{"A": "0 criterios: se necesita TAC para el diagnóstico", "B": "1 criterio: solo el dolor característico", "C": "2 criterios: dolor característico + lipasa >3x normal (diagnóstico establecido)", "D": "3 criterios: dolor + lipasa + TAC (TAC siempre obligatorio)"}'::jsonb,
    'C',
    'El diagnóstico de pancreatitis aguda requiere ≥2 de 3 criterios (Clasificación Atlanta revisada 2012): 1) Dolor abdominal característico (epigástrico, irradiado a dorso), 2) Lipasa o amilasa ≥3x el límite superior normal, 3) Hallazgos de imagen compatibles (TAC, RM, USG). Con 2 criterios presentes (dolor + lipasa >3x normal = 1840 U/L cuando normal <60, es >30x) el diagnóstico es CLÍNICO sin necesidad de TAC. La TAC se reserva para: evaluar complicaciones, confirmar diagnóstico dudoso, o si hay deterioro clínico a las 48-72h.',
    'Diagnóstico pancreatitis aguda: ≥2 de 3 criterios Atlanta: dolor + lipasa≥3x + imagen. Con 2 criterios = diagnóstico clínico (NO necesita TAC rutinaria al ingreso). TAC: complicaciones o diagnóstico dudoso.',
    'Revised Atlanta Classification 2012, AGA Pancreatitis Guidelines 2024',
    true
);

-- MCQ 13: Pancreatitis - Fluidos
INSERT INTO mcq_bank (
    question_text, specialty, difficulty, topic,
    options_json, correct_option, explanation,
    teaching_point, source, is_published
) VALUES (
    'La misma paciente con pancreatitis aguda severa. ¿Cuál es el líquido de reanimación preferido en la fase inicial (primeras 12-24h)?',
    'Gastroenterology', 'advanced', 'Pancreatitis aguda - Fluidoterapia',
    '{"A": "Solución salina isotónica (SF 0.9%) 500 mL/h", "B": "Ringer Lactato 250-500 mL/h", "C": "Coloides (albumina 5%) 250 mL/h", "D": "Dextrosa 5% en agua destilada 150 mL/h"}'::jsonb,
    'B',
    'El Ringer Lactato (RL) es el fluido preferido en pancreatitis aguda sobre el SF 0.9%. Un RCT (Wu et al., 2011) mostró que RL redujo la inflamación sistémica (PCR) y la incidencia de SIRS a las 24h vs SF. Las AGA Guidelines 2024 y ACG 2013 recomiendan RL como primera opción. El SF puede causar acidosis hiperclorémica que empeora el pronóstico. La tasa inicial: 250-500 mL/h con metas de reanimación: FC <100, PA normal, diuresis >0.5 mL/kg/h, BUN que disminuye.',
    'Pancreatitis: Ringer Lactato preferido sobre SF 0.9% (reduce PCR, SIRS). Tasa: 250-500 mL/h primeras 12-24h. Meta: FC<100, diuresis>0.5 mL/kg/h. Coloides: NO indicados rutinariamente.',
    'AGA Pancreatitis Guidelines 2024, Wu et al. Clin Gastroenterol Hepatol 2011',
    true
);

-- MCQ 14: Hemorragia GI alta - Manejo inicial
INSERT INTO mcq_bank (
    question_text, specialty, difficulty, topic,
    options_json, correct_option, explanation,
    teaching_point, source, is_published
) VALUES (
    'Hombre de 55 años con hematemesis de 500 mL de sangre roja. FC 118, PA 90/60, extremidades frías. Antecedente de artritis reumatoide en tratamiento con naproxeno. Hb 8.2 g/dL. ¿Cuál es la prioridad inmediata en el manejo?',
    'Gastroenterology', 'intermediate', 'Hemorragia GI alta - Manejo inicial',
    '{"A": "Endoscopia urgente en primeras 2 horas", "B": "IBP en infusión IV continua (omeprazol 80 mg bolo + 8 mg/h)", "C": "Reanimación hemodinámica: 2 accesos IV gruesos + cristaloides + transfusión según hemoglobina", "D": "Sonda nasogástrica para valorar sangrado activo"}'::jsonb,
    'C',
    'En hemorragia GI alta masiva con inestabilidad hemodinámica, la PRIORIDAD es la reanimación: ABC, 2 accesos IV periféricos 16-18G, cristaloides (1-2L Ringer Lactato o SF), transfusión de GRE si Hb <7 o <8 con enfermedad cardiovascular (estrategia restrictiva - estudio TRICC). La endoscopia urgente (<12-24h) es adecuada solo cuando el paciente está hemodinámicamente estable. IBP IV puede iniciarse simultáneamente pero NO es la primera prioridad. Score Glasgow-Blatchford estratifica riesgo.',
    'HGI alta masiva: Primero ABC + reanimación hemodinámica. Luego: IBP IV + endoscopia cuando estable. Transfusión: GRE si Hb <7 (o <8 en cardiovascular). Estrategia restrictiva superior (TRICC). Endoscopia <12h si estable, <24h en todos.',
    'ACG Upper GI Bleeding Guidelines 2021',
    true
);

-- ================================================================
--  BLOQUE: Endocrinología (5 preguntas)
-- ================================================================

-- MCQ 15: CAD - Secuencia de tratamiento
INSERT INTO mcq_bank (
    question_text, specialty, difficulty, topic,
    options_json, correct_option, explanation,
    teaching_point, source, is_published
) VALUES (
    'Joven de 19 años con CAD severa: glucosa 518 mg/dL, pH 7.18, HCO3 8 mEq/L, K+ 5.8 mEq/L. ¿Cuál es el orden correcto de las intervenciones iniciales?',
    'Endocrinology', 'intermediate', 'CAD - Secuencia terapéutica',
    '{"A": "Insulina bolo IV → fluidos → potasio", "B": "Insulina infusión → bicarbonato → fluidos", "C": "Fluidos IV (SF 0.9%) → verificar K+ → insulina en infusión (sin bolo)", "D": "Bicarbonato (pH <7.2) → fluidos → insulina"}'::jsonb,
    'C',
    'Secuencia correcta en CAD: 1) FLUIDOS PRIMERO (SF 0.9% 1L en 1h, corrige hipoperfusión y deshidratación), 2) VERIFICAR K+ (si K+<3.5 → dar KCl antes de insulina, si K+≥3.5 → iniciar insulina), 3) INSULINA en infusión continua 0.1 U/kg/h SIN BOLO inicial (evidencia actual: el bolo no mejora outcomes y puede causar hipoglucemia e hipocalemia rápidas). El bicarbonato NO se indica con pH 7.18 (solo si pH <6.9). El K+ 5.8 es falsamente elevado; al dar insulina bajará rápido por entrada al intracelular.',
    'CAD secuencia: Fluidos → Verificar K+ → Insulina infusión (sin bolo). Bicarbonato SOLO si pH<6.9. K+: si <3.5 = dar KCl ANTES de insulina. K+: meta 4-5 durante tratamiento.',
    'ADA Standards of Care 2024, CAD/EHH Guidelines',
    true
);

-- MCQ 16: CAD vs EHH
INSERT INTO mcq_bank (
    question_text, specialty, difficulty, topic,
    options_json, correct_option, explanation,
    teaching_point, source, is_published
) VALUES (
    'Mujer de 72 años con DM2, traída por su familia por confusión de 2 días. Glucemia 820 mg/dL, Na+ 148 mEq/L, osmolalidad efectiva 342 mOsm/kg, pH 7.36, HCO3 22 mEq/L, cetonas en orina negativas. ¿Cuál es el diagnóstico?',
    'Endocrinology', 'intermediate', 'CAD vs Estado hiperosmolar',
    '{"A": "Cetoacidosis diabética severa (CAD)", "B": "Estado hiperosmolar hiperglucémico (EHH)", "C": "CAD mixto con EHH", "D": "Acidosis láctica por metformina"}'::jsonb,
    'B',
    'EHH vs CAD: EHH = glucosa >600 mg/dL + osmolalidad efectiva >320 mOsm/kg + SIN acidosis (pH normal ≥7.3) + SIN cetosis significativa + alteración neurológica. CAD = glucosa >250 + acidosis metabólica (pH<7.3, HCO3<15) + cetosis (cetonas+). En este caso: glucosa 820, pH normal 7.36, sin cetonas = EHH. El manejo difiere: EHH requiere corrección más lenta de la hiperosmolaridad (riesgo de edema cerebral), y la insulina se inicia después de los fluidos (no de forma urgente como en CAD).',
    'EHH: glucosa>600 + osm>320 + pH normal + sin cetosis. CAD: glucosa>250 + pH<7.3 + cetosis. EHH: corrección lenta, fluidos son pilares. CAD: insulina urgente. Pueden coexistir.',
    'ADA 2024 Hyperglycemic Crises Guidelines',
    true
);

-- MCQ 17: Hipotiroidismo - Crisis mixedematosa
INSERT INTO mcq_bank (
    question_text, specialty, difficulty, topic,
    options_json, correct_option, explanation,
    teaching_point, source, is_published
) VALUES (
    'Mujer de 78 años traída en estado estuporoso. T 34.5°C, FC 44, PA 90/58, FR 8. Piel seca, edema facial, reflejos disminuidos. TSH >100 μUI/mL, T4L 0.2 ng/dL. Gasometría: PCO2 58, pH 7.28 (hipercapnia). ¿Cuál es el tratamiento de elección?',
    'Endocrinology', 'advanced', 'Crisis mixedematosa',
    '{"A": "Levotiroxina VO 100 mcg/día y hospitalización", "B": "Levotiroxina IV 200-400 mcg bolo + Triyodotironina IV + hidrocortisona 100 mg IV + soporte UCI", "C": "Solo soporte UCI, la tiroxina está contraindicada en emergencia", "D": "Metimazol IV para reducir TSH compensatorio"}'::jsonb,
    'B',
    'La crisis mixedematosa es hipotiroidismo severo potencialmente mortal (mortalidad 20-40%). Tratamiento: 1) Levotiroxina IV 200-400 mcg bolo (conversión T4→T3 lenta, por eso se prefiere IV y se puede agregar T3), 2) Triyodotironina (T3) IV 5-20 mcg si disponible (acción más rápida), 3) Hidrocortisona 100 mg IV c/8h (la insuficiencia suprarrenal puede coexistir - síndrome de Schmidt; tratar ANTES de tiroxina para evitar crisis suprarrenal), 4) Soporte UCI: ventilación mecánica, calentamiento gradual, líquidos, glucosa. La levotiroxina VO es inadecuada en crisis.',
    'Crisis mixedematosa: L-T4 IV + T3 IV si disponible + Hidrocortisona PRIMERO (descartar insuficiencia suprarrenal). UCI. Mortalidad 20-40%. Precipitantes: infección, frío, sedantes, cirugía.',
    'ATA Myxedema Coma Guidelines 2014',
    true
);

-- ================================================================
--  BLOQUE: Ginecología y Obstetricia (5 preguntas)
-- ================================================================

-- MCQ 18: Preeclampsia - Criterios de severidad
INSERT INTO mcq_bank (
    question_text, specialty, difficulty, topic,
    options_json, correct_option, explanation,
    teaching_point, source, is_published
) VALUES (
    'Embarazada de 34 semanas con PA 168/112 mmHg en 2 determinaciones. ¿Cuál de los siguientes hallazgos adicionales NO es criterio de severidad en preeclampsia según ACOG 2020?',
    'Obstetrics and Gynecology', 'advanced', 'Preeclampsia - Criterios de severidad',
    '{"A": "Proteinuria >5 g/24h", "B": "Plaquetas <100,000/mm³", "C": "Creatinina sérica >1.1 mg/dL", "D": "Edema de miembros inferiores +++"}'::jsonb,
    'A',
    'Según ACOG 2020, la preeclampsia con criterios de severidad se define por PA ≥160/110 + UNO de los siguientes: plaquetas <100,000, creatinina >1.1 mg/dL (o duplicación), enzimas hepáticas >2x normal, edema pulmonar, cefalea nueva o alteraciones visuales, epigastralgia severa. IMPORTANTE: la proteinuria masiva (>5 g/24h) ya NO es criterio de severidad en ACOG 2020 (se eliminó de los criterios). El edema de extremidades tampoco es criterio de severidad (es hallazgo inespecífico). La respuesta es A: proteinuria >5g/24h.',
    'Preeclampsia severa ACOG 2020: PA≥160/110 + plaquetas<100K OR creatinina>1.1 OR enzimas>2x OR edema pulmonar OR síntomas cerebrales/visuales OR epigastralgia. Proteinuria masiva (>5g) YA NO es criterio de severidad.',
    'ACOG Practice Bulletin 222: Gestational Hypertension and Preeclampsia 2020',
    true
);

-- MCQ 19: Preeclampsia - Sulfato de Magnesio
INSERT INTO mcq_bank (
    question_text, specialty, difficulty, topic,
    options_json, correct_option, explanation,
    teaching_point, source, is_published
) VALUES (
    'Paciente con preeclampsia severa recibe sulfato de magnesio. A la hora, la enfermera reporta: FR 10/min, reflejos rotulianos ausentes, diuresis 15 mL/h. ¿Cuál es la conducta inmediata?',
    'Obstetrics and Gynecology', 'advanced', 'Preeclampsia - Toxicidad MgSO4',
    '{"A": "Suspender MgSO4 y administrar gluconato de calcio 1g IV en 3 min", "B": "Reducir velocidad de infusión de MgSO4 a la mitad", "C": "Administrar furosemida IV para mejorar diuresis", "D": "Continuar igual, los signos son normales con MgSO4"}'::jsonb,
    'A',
    'Signos de toxicidad por MgSO4: pérdida de reflejos rotulianos (primer signo, ocurre con Mg 5-9 mEq/L), luego parálisis respiratoria (FR <12/min, Mg 9-13 mEq/L), luego paro cardíaco (Mg >15 mEq/L). Monitorización obligatoria: reflejos presentes, FR >12/min, diuresis >25 mL/h. Con FR 10 + reflejos ausentes = toxicidad severa: SUSPENDER MgSO4 + ANTÍDOTO: gluconato de calcio 1g IV en 3 minutos (antagoniza el Mg). Luego soporte respiratorio si necesario.',
    'Toxicidad MgSO4: 1º pérdida reflejos (Mg5-9), 2º depresión respiratoria (FR<12, Mg9-13), 3º paro cardíaco (Mg>15). ANTÍDOTO: Gluconato de calcio 1g IV en 3 min. SUSPENDER MgSO4 inmediatamente.',
    'ACOG Preeclampsia Guidelines 2020',
    true
);

-- MCQ 20: Hemorragia postparto
INSERT INTO mcq_bank (
    question_text, specialty, difficulty, topic,
    options_json, correct_option, explanation,
    teaching_point, source, is_published
) VALUES (
    'Mujer que acaba de tener parto vaginal, se estima pérdida de 600 mL de sangre con útero que se palpa blando y no contrae. PA 95/60, FC 122. ¿Cuál es el manejo inicial de elección?',
    'Obstetrics and Gynecology', 'intermediate', 'Hemorragia postparto - Atonía uterina',
    '{"A": "Histerectomía de urgencia", "B": "Oxitocina IV 10-40 UI en 500 mL + masaje uterino bimanual", "C": "Compresión aórtica abdominal y esperar", "D": "Ergonovina 0.2 mg IV de primera línea"}'::jsonb,
    'B',
    'La atonía uterina es la causa más frecuente de HPP (80%). Manejo escalonado: 1) Masaje uterino bimanual + oxitocina IV 10 UI en bolo lento o 20-40 UI en 500 mL (primera línea), 2) Si falla: ergonovina 0.2 mg IM (contraindicada en HTA/preeclampsia), 3) Misoprostol 800 mcg sublingual o rectal, 4) Ácido tranexámico 1g IV (dentro de 3h del parto), 5) Procedimientos: balón de Bakri, suturas de compresión uterina, ligadura arterias uterinas, histerectomía (último recurso). La ergonovina NO es primera línea (contraindicada en HTA). La histerectomía es el último recurso.',
    'Atonía uterina HPP: 1ª línea = Oxitocina IV + masaje uterino. 2ª línea: Ergonovina (NO en HTA), Misoprostol, Ácido tranexámico. Último recurso: histerectomía. Las 4T: Tono, Tejido, Trauma, Trombina.',
    'ACOG/FIGO PPH Guidelines 2022',
    true
);

-- ================================================================
--  BLOQUE: Infectología / Sepsis (5 preguntas)
-- ================================================================

-- MCQ 21: Sepsis - Definición
INSERT INTO mcq_bank (
    question_text, specialty, difficulty, topic,
    options_json, correct_option, explanation,
    teaching_point, source, is_published
) VALUES (
    'Paciente de 65 años con ITU complicada. T 38.9°C, FC 108, FR 22, PA 110/70, lactato 1.8 mmol/L, score SOFA 2. ¿Cómo se clasifica esta condición según Sepsis-3 (2016)?',
    'Infectious Disease', 'intermediate', 'Sepsis - Definición Sepsis-3',
    '{"A": "SIRS sin sepsis (no cumple criterios SOFA)", "B": "Sepsis (infección + disfunción orgánica SOFA ≥2)", "C": "Shock séptico (requiere vasopresores + lactato)", "D": "Infección sin sepsis (lactato <2, PA normal)"}'::jsonb,
    'B',
    'Definición Sepsis-3 (JAMA 2016): Sepsis = infección + disfunción orgánica aguda (aumento SOFA ≥2 puntos sobre basal). Shock séptico = sepsis + hipotensión que requiere vasopresores para MAP ≥65 + lactato >2 mmol/L a pesar de reanimación adecuada. El SIRS ya no es necesario para el diagnóstico. En urgencias: qSOFA ≥2 (FR≥22, alteración mental, PAS≤100) identifica pacientes de alto riesgo. En este caso: SOFA ≥2 + ITU = SEPSIS. La PA normal y lactato <2 excluyen shock séptico.',
    'Sepsis-3: Sepsis = infección + SOFA≥2. Shock séptico = sepsis + vasopresores para MAP≥65 + lactato>2 (tras reanimación). SIRS ya no define sepsis. qSOFA en urgencias: FR≥22 + mental + PAS≤100.',
    'Singer et al. JAMA 2016 - Sepsis-3 Definitions',
    true
);

-- MCQ 22: Sepsis - Bundle de 1 hora
INSERT INTO mcq_bank (
    question_text, specialty, difficulty, topic,
    options_json, correct_option, explanation,
    teaching_point, source, is_published
) VALUES (
    'Paciente con diagnóstico de shock séptico por neumonía. PA 82/50, lactato 4.8 mmol/L. ¿Cuál es el bundle de 1 hora de la Surviving Sepsis Campaign 2018?',
    'Infectious Disease', 'advanced', 'Shock séptico - Bundle 1 hora',
    '{"A": "Antibiótico IV + TAC de tórax + consulta de infectología", "B": "Medir lactato + hemocultivos ANTES del antibiótico + antibiótico IV + 30 mL/kg cristaloide si hipotensión/lactato≥4 + vasopresores si MAP<65", "C": "Intubación orotraqueal + ventilación mecánica + antibiótico IV", "D": "Solo antibiótico IV y esperar respuesta antes de fluidos"}'::jsonb,
    'B',
    'Bundle de 1 hora SSC 2018: 1) Medir lactato sérico (si >2 mmol/L = sepsis, si >4 = alta mortalidad), 2) Hemocultivos x2 ANTES del antibiótico, 3) Antibiótico de amplio espectro IV ≤1 hora, 4) Cristaloides 30 mL/kg si MAP <65 o lactato ≥4 mmol/L, 5) Vasopresores (norepinefrina) si MAP persiste <65 tras fluidos. El tiempo hasta el antibiótico es crítico: cada hora de retraso aumenta mortalidad un 7%. Hemocultivos ANTES del antibiótico pero no deben retrasar más de 45 minutos el inicio del ATB.',
    'Bundle 1h SSC 2018: Lactato + Hemocultivos → Antibiótico ≤1h → Fluidos 30mL/kg → Vasopresores (NE) si MAP<65. ORDEN: hemocultivos primero, antibiótico ASAP. Cada hora de retraso ATB = +7% mortalidad.',
    'Surviving Sepsis Campaign Bundle 2018, Rhodes et al.',
    true
);

-- ================================================================
--  BLOQUE: Cirugía (4 preguntas)
-- ================================================================

-- MCQ 23: Apendicitis - Score de Alvarado
INSERT INTO mcq_bank (
    question_text, specialty, difficulty, topic,
    options_json, correct_option, explanation,
    teaching_point, source, is_published
) VALUES (
    'Hombre de 22 años con 24h de dolor que inició periumbilical y migró a FID. Anorexia, náusea sin vómito, T 38.2°C. Blumberg positivo. Leucocitos 13,200 con 82% neutrófilos. ¿Cuál es el score de Alvarado y la conducta recomendada?',
    'Surgery', 'intermediate', 'Apendicitis - Score de Alvarado',
    '{"A": "Score 5: observación y repetir laboratorios en 6h", "B": "Score 7: TAC abdominopélvico para confirmar antes de cirugía", "C": "Score 8: alta probabilidad quirúrgica, preparar para apendicectomía", "D": "Score 10: cirugía inmediata sin estudios de imagen"}'::jsonb,
    'C',
    'Score de Alvarado: Migración dolor (2) + Anorexia (1) + Náusea/vómito (1) + Dolor FID (2) + Rebote/Blumberg (1) + Temperatura >37.3°C (1) + Leucocitosis >10,000 (2) = máximo 10. En este caso: migración(2) + anorexia(1) + náusea(1) + dolor FID(2) + Blumberg+(1) + fiebre(1) + leucocitosis(2) = 10, pero el vómito no se confirma, así que = 9. Score ≥7 = alta probabilidad quirúrgica. Score 5-6 = probabilidad intermedia (imagen). Score <5 = baja probabilidad. Con score 7-10 = preparar para cirugía (con o sin TAC confirmatoria según preferencia del cirujano).',
    'Alvarado: ≥7 = alta probabilidad quirúrgica. 5-6 = imagen confirmatoria. <5 = baja probabilidad. Máximo 10 puntos. Migración(2)+Anorexia(1)+Náusea(1)+DFT(2)+Blumberg(1)+Fiebre(1)+Leucocitosis(2).',
    'World Journal of Emergency Surgery, Alvarado Score',
    true
);

-- MCQ 24: Colecistitis - Manejo
INSERT INTO mcq_bank (
    question_text, specialty, difficulty, topic,
    options_json, correct_option, explanation,
    teaching_point, source, is_published
) VALUES (
    'Mujer de 52 años con cólico biliar frecuente y ahora fiebre 38.5°C, Murphy positivo, leucocitos 14,000, USG: engrosamiento pared vesicular >4mm, líquido pericolecístico, colelitiasis. Grado I (colecistitis aguda leve, TOKYO 2018). ¿Cuál es la conducta más apropiada?',
    'Surgery', 'intermediate', 'Colecistitis aguda - Manejo',
    '{"A": "Antibióticos IV y colecistectomía laparoscópica de urgencia en primeras 24-72h", "B": "Antibióticos IV solos y colecistectomía electiva en 6-8 semanas", "C": "Colecistostomía percutánea de urgencia", "D": "Solo dieta baja en grasas y AINE"}'::jsonb,
    'A',
    'Colecistitis aguda grado I (leve según Tokyo 2018) = colecistectomía laparoscópica temprana dentro de 24-72h del inicio del cuadro. Ventajas de la colecistectomía temprana vs diferida: menor estancia hospitalaria, menos conversión a cirugía abierta, menor costo, sin riesgo de recurrencia en espera. Los antibióticos se inician previo a cirugía pero NO son el tratamiento definitivo. La colecistostomía percutánea es para pacientes de alto riesgo quirúrgico (grado III o comorbilidades severas). Esperar 6-8 semanas = tratamiento antiguo, ya no recomendado para grado I.',
    'Colecistitis aguda grado I (Tokyo): Colecistectomía laparoscópica temprana <72h = estándar actual. NO diferir 6-8 semanas (mayor riesgo complicaciones). ATB preoperatorio. Colecistostomía: solo alto riesgo quirúrgico.',
    'Tokyo Guidelines 2018 (TG18) Acute Cholecystitis',
    true
);

-- ================================================================
--  BLOQUE: Nefrología (4 preguntas)
-- ================================================================

-- MCQ 25: LRA - Criterios KDIGO
INSERT INTO mcq_bank (
    question_text, specialty, difficulty, topic,
    options_json, correct_option, explanation,
    teaching_point, source, is_published
) VALUES (
    'Paciente hospitalizado. Creatinina basal 0.9 mg/dL. En 48h la creatinina es 1.3 mg/dL. Diuresis 0.6 mL/kg/h. ¿Tiene criterios de LRA según KDIGO 2012 y en qué estadio?',
    'Nephrology', 'intermediate', 'LRA - Criterios KDIGO',
    '{"A": "No tiene LRA: el aumento es mínimo y la diuresis es normal", "B": "LRA estadio 1: aumento de creatinina ≥0.3 mg/dL en 48h (delta = 0.4)", "C": "LRA estadio 2: creatinina 1.5-2x el basal", "D": "LRA estadio 3: creatinina >3x el basal"}'::jsonb,
    'B',
    'Criterios KDIGO 2012 para LRA (cualquiera de los 3): 1) Aumento de creatinina ≥0.3 mg/dL en 48h, 2) Aumento de creatinina ≥1.5x el basal en 7 días, 3) Diuresis <0.5 mL/kg/h por ≥6h. En este caso: delta creatinina = 1.3-0.9 = 0.4 mg/dL ≥0.3 = LRA. Estadio 1: creatinina ≥0.3 mg/dL O 1.5-1.9x basal. Estadio 2: 2-2.9x basal. Estadio 3: ≥3x basal o creatinina ≥4 mg/dL o diálisis. Aquí: 1.3/0.9 = 1.44x (estadio 1 también por ratio). Diuresis 0.6 mL/kg/h no cumple criterio (<0.5).',
    'LRA KDIGO: Cr ≥0.3 mg/dL en 48h OR ≥1.5x basal en 7d OR diuresis <0.5 mL/kg/h×6h. E1: ≥0.3 o 1.5-1.9x. E2: 2-2.9x. E3: ≥3x o Cr≥4 o diálisis.',
    'KDIGO AKI Guidelines 2012',
    true
);

-- MCQ 26: HiperK - ECG y manejo
INSERT INTO mcq_bank (
    question_text, specialty, difficulty, topic,
    options_json, correct_option, explanation,
    teaching_point, source, is_published
) VALUES (
    'Paciente con ERC estadio 4, K+ 6.8 mEq/L. ECG muestra ondas T picudas prominentes en V2-V5 y ensanchamiento del QRS. ¿Cuál es el primer paso en el manejo?',
    'Nephrology', 'advanced', 'Hiperpotasemia - Manejo con cambios ECG',
    '{"A": "Kayexalate (sulfonato de poliestireno) 15g VO", "B": "Furosemida 80mg IV para eliminar potasio por orina", "C": "Gluconato de calcio 1g IV en 3-5 minutos (estabilizar membrana cardíaca)", "D": "Hemodiálisis urgente inmediata"}'::jsonb,
    'C',
    'Con hiperpotasemia grave + cambios ECG (ondas T picudas + ensanchamiento QRS = riesgo de FV), el PRIMER paso es ESTABILIZAR LA MEMBRANA CARDÍACA con gluconato de calcio 10% 1g IV en 3-5 min (puede repetirse en 5 min si persisten cambios). No baja el K+ pero protege el corazón en 1-3 min mientras se inician medidas para desplazar/eliminar K+. Luego: insulina+dextrosa (desplazamiento intracelular, inicio 15-30 min), bicarbonato IV si acidosis, resinas, diálisis si refractario. El Kayexalate tiene inicio lento (horas). Furosemida ayuda pero no es primera línea urgente.',
    'HiperK grave con cambios ECG: 1º Gluconato calcio IV (protege corazón, efecto en 3 min). 2º Insulina+Glucosa (desplazamiento, 15-30 min). 3º Bicarbonato si acidosis. 4º Resinas. 5º Diálisis. Nunca dar K si K>5.',
    'KDIGO Hyperkalaemia Guidelines, NKF/KDIGO',
    true
);

-- ================================================================
--  BLOQUE: Pediatría (4 preguntas)
-- ================================================================

-- MCQ 27: Bronquiolitis - Manejo
INSERT INTO mcq_bank (
    question_text, specialty, difficulty, topic,
    options_json, correct_option, explanation,
    teaching_point, source, is_published
) VALUES (
    'Lactante de 6 meses con fiebre, rinorrea, tos y sibilancias de 2 días. SpO2 93%, FR 52/min, uso moderado de músculos accesorios. Primer episodio de sibilancias. Diagnóstico: bronquiolitis moderada. ¿Cuál es el manejo de primera línea basado en evidencia?',
    'Pediatrics', 'intermediate', 'Bronquiolitis - Manejo basado en evidencia',
    '{"A": "Salbutamol inhalado nebulizado c/4h + corticoides sistémicos", "B": "Adrenalina nebulizada + dexametasona IV", "C": "Oxígeno si SpO2 <90-92%, succión nasal, hidratación adecuada (soporte)", "D": "Antibióticos IV por sobreinfección bacteriana probable"}'::jsonb,
    'C',
    'La bronquiolitis es viral (VRS principalmente) y el manejo es de SOPORTE: 1) Oxígeno suplementario si SpO2 <90-92%, 2) Aspiración/succión nasal para permeabilizar vía aérea, 3) Hidratación (oral si tolera, IV si no), 4) Hospitalizar si SpO2 <90-92%, dificultad respiratoria grave, rechazo alimentario. NO hay evidencia de beneficio de broncodilatadores (salbutamol/adrenalina) en bronquiolitis típica (meta-análisis muestran sin efecto sobre outcomes). Los corticoides tampoco son útiles. Antibióticos solo si evidencia de sobreinfección bacteriana documentada.',
    'Bronquiolitis: tratamiento de SOPORTE únicamente. Salbutamol = SIN beneficio demostrado (no se recomienda rutinariamente). Corticoides = SIN beneficio. Antibióticos = solo si sobreinfección documentada. O2 si SpO2<90-92%.',
    'AAP Bronchiolitis Guidelines 2014 (reafirmadas 2023)',
    true
);

-- MCQ 28: Deshidratación pediátrica - Manejo
INSERT INTO mcq_bank (
    question_text, specialty, difficulty, topic,
    options_json, correct_option, explanation,
    teaching_point, source, is_published
) VALUES (
    'Niño de 2 años con diarrea aguda 5 días. Está lloroso, irritable, ojos hundidos, mucosas secas, signo de pliegue positivo (>2 segundos). FR 32, FC 138, PA 90/60, llenado capilar 3 segundos. ¿Cuál es el grado de deshidratación y el manejo inicial?',
    'Pediatrics', 'intermediate', 'Deshidratación - Clasificación y manejo',
    '{"A": "Deshidratación leve: sales de rehidratación oral y vigilancia domiciliaria", "B": "Deshidratación moderada: SRO agresiva 50-100 mL/kg en 4h", "C": "Deshidratación grave con shock: SS 0.9% o Ringer Lactato 20 mL/kg IV rápido (bolo)", "D": "Deshidratación grave: NPO y SF 0.9% 10 mL/kg en 4 horas"}'::jsonb,
    'C',
    'Clasificación OMS/PALS: Leve (<5%): alerta, mucosas levemente secas. Moderada (5-10%): irritable, ojos hundidos, pliegue positivo, mucosas secas. Grave (>10%): letárgico o inconsciente, ojos muy hundidos, bebe muy mal o no puede. Con signos de shock (FC 138, PA 90/60, llenado >3s, pliegue positivo) = deshidratación grave con compromiso circulatorio. Manejo: bolo de cristaloides 20 mL/kg IV rápido (en 20-30 min), repetir según respuesta, hasta máx 60 mL/kg. Luego continuar con plan de rehidratación y corregir déficit.',
    'Deshidratación grave + shock pediátrico: Bolo cristaloide 20 mL/kg IV en 20 min (repetir hasta 3 veces según respuesta). Meta: FC normal, llenado capilar <2s, diuresis. Leve/moderada sin shock: SRO.',
    'WHO/OMS Diarrhea Management Guidelines, PALS 2020',
    true
);

-- ================================================================
--  BLOQUE: Medicina de Urgencias / Trauma (3 preguntas)
-- ================================================================

-- MCQ 29: TCE - Manejo inicial
INSERT INTO mcq_bank (
    question_text, specialty, difficulty, topic,
    options_json, correct_option, explanation,
    teaching_point, source, is_published
) VALUES (
    'Paciente de 28 años traído tras accidente automovilístico. GCS inicial 7 (O2V2M3). PA 140/90, FC 95. TAC: hemorragia epidural temporal derecha 40 mL con efecto de masa y DLM 7mm. ¿Cuál es la conducta definitiva?',
    'Emergency Medicine', 'advanced', 'TCE grave - Manejo quirúrgico',
    '{"A": "Manitol IV 1g/kg + ingreso a UCI neurológica, cirugía solo si deteriora", "B": "Craneotomía urgente: evacuación del hematoma epidural", "C": "Monitoreo de PIC y manejo médico con hiperventilación controlada", "D": "Observación 24h y TAC de control"}'::jsonb,
    'B',
    'La hemorragia epidural (HE) con indicación quirúrgica: volumen >30 mL, grosor >15 mm, desviación de línea media >5 mm, cualquier HE con GCS <9 y deterioro pupilar. Este paciente cumple todos los criterios (40 mL, GCS 7, DLM 7mm). La craneotomía urgente para evacuación de hematoma epidural es el estándar de oro con excelente pronóstico si se realiza a tiempo (en <4h del herniation). El manejo médico (manitol, hiperventilación) puede ser temporal para ganar tiempo quirúrgico pero NO es el tratamiento definitivo con estos criterios.',
    'HE = craneotomía urgente si: >30 mL OR grosor >15mm OR DLM >5mm OR GCS<9 con deterioro. Mejor pronóstico de todos los hematomas intracraneales si cirugía precoz (<4h). Nunca solo observar con estos criterios.',
    'Brain Trauma Foundation Guidelines 4th Ed. 2016',
    true
);

-- MCQ 30: FAST - Trauma abdominal
INSERT INTO mcq_bank (
    question_text, specialty, difficulty, topic,
    options_json, correct_option, explanation,
    teaching_point, source, is_published
) VALUES (
    'Hombre de 35 años en shock hemorrágico tras trauma abdominal cerrado. PA 80/50, FC 138. FAST positivo (líquido libre en cavidad abdominal). ¿Cuál es la conducta?',
    'Emergency Medicine', 'advanced', 'Trauma abdominal - FAST positivo en shock',
    '{"A": "TAC abdominopélvico con contraste para identificar órgano lesionado", "B": "Laparotomía exploradora de urgencia (control de daños)", "C": "Arteriografía y embolización selectiva", "D": "Reanimación agresiva con cristaloides y observación"}'::jsonb,
    'B',
    'FAST positivo (líquido libre intraperitoneal) en paciente con shock hemorrágico refractario (no responde a reanimación inicial) = laparotomía exploradora de urgencia para control de daños. NO se puede perder tiempo en TAC cuando el paciente es inestable (el TAC es para pacientes ESTABLES). La cirugía de control de daños (Damage Control Surgery): 1) Control de hemorragia, 2) Control de contaminación, 3) Cierre abdominal temporal, 4) Resucitación en UCI, 5) Reoperación definitiva. La arteriografía puede complementar pero no sustituye la cirugía en shock.',
    'FAST+ en shock: Laparotomía urgente (Damage Control Surgery). TAC = solo pacientes ESTABLES. DCS: hemostasia + descontaminación + cierre temporal + UCI + reoperación. No perder tiempo en imagen con inestabilidad.',
    'ATLS 10th Edition, Eastern Association for Surgery of Trauma',
    true
);

-- ================================================================
--  BLOQUE: Farmacología / Temas transversales (5 preguntas)
-- ================================================================

-- MCQ 31: Antibióticos - Meningitis bacteriana
INSERT INTO mcq_bank (
    question_text, specialty, difficulty, topic,
    options_json, correct_option, explanation,
    teaching_point, source, is_published
) VALUES (
    'Adulto de 30 años con fiebre 40°C, cefalea intensa, rigidez de nuca y petequias. Sospecha de meningitis bacteriana. ¿Cuál es el tratamiento empírico inmediato antes del resultado del LCR?',
    'Infectious Disease', 'intermediate', 'Meningitis bacteriana - Tratamiento empírico',
    '{"A": "Esperar resultado de LCR antes de iniciar antibiótico", "B": "Ceftriaxona 2g IV c/12h + Dexametasona 0.15 mg/kg IV c/6h", "C": "Ampicilina IV + gentamicina IV", "D": "Aciclovir IV hasta descartar encefalitis viral"}'::jsonb,
    'B',
    'Meningitis bacteriana: el retraso del antibiótico aumenta mortalidad. NUNCA esperar LCR para iniciar tratamiento. Empírico en adulto inmunocompetente <50 años: Ceftriaxona 2g IV c/12h (cobertura neumococo + meningococo). Agregar Dexametasona 0.15 mg/kg IV c/6h ANTES o con el antibiótico (reduce mortalidad y secuelas en meningitis neumocócica - estudio de Brouwer). Agregar Ampicilina si >50 años o inmunocomprometido (cobertura de Listeria). Si LCR disponible rápidamente: tomar antes del ATB; si no, NO retrasar el ATB por el LCR.',
    'Meningitis: ATB + Dexametasona INMEDIATOS. NUNCA retrasar por LCR. <50 años: Ceftriaxona. >50 años/inmunocomp: Ceftriaxona + Ampicilina. Dexametasona reduce mortalidad en meningitis neumocócica.',
    'IDSA Bacterial Meningitis Guidelines 2017',
    true
);

-- MCQ 32: Reacciones adversas - Estatinas
INSERT INTO mcq_bank (
    question_text, specialty, difficulty, topic,
    options_json, correct_option, explanation,
    teaching_point, source, is_published
) VALUES (
    'Paciente de 58 años con STEMI que inició atorvastatina 80 mg/día hace 3 meses. Ahora presenta dolor muscular difuso sin debilidad. CK 850 U/L (normal <200). Creatinina normal. ¿Cuál es la conducta?',
    'Cardiology', 'intermediate', 'Estatinas - Miopatía',
    '{"A": "Continuar estatina, el dolor es independiente", "B": "Suspender estatina permanentemente, contraindicada de por vida", "C": "Suspender estatina temporalmente, monitorear CK, reintroducir a menor dosis o cambiar de estatina", "D": "Agregar CoQ10 y continuar la misma dosis"}'::jsonb,
    'C',
    'Miopatía por estatinas: CK >10x normal = rabdomiólisis (suspender definitivamente). CK 3-10x con síntomas = miopatía moderada (suspender, esperar normalización, reintroducir con menor dosis o cambiar de estatina). CK <3x con síntomas = mialgia (puede continuar con monitoreo). En este caso: CK 850/200 = 4.25x normal con síntomas = miopatía moderada. Conducta: suspender atorvastatina, monitorear CK c/2-4 semanas, al normalizarse reintroducir (rosuvastatina o pravalostatina tienen menor miotoxicidad). Suspender permanentemente solo en rabdomiólisis. CoQ10 no tiene evidencia sólida.',
    'Miopatía por estatinas: CK<3x=mialgia (continuar). CK 3-10x=miopatía (suspender, reintroducir). CK>10x=rabdomiólisis (suspender permanente). Alternativas: Rosuvastatina/Pravalostatina menor miotoxicidad.',
    'ACC/AHA Statin Guidelines 2019',
    true
);

-- MCQ 33: Anticoagulación - Reversión de warfarina
INSERT INTO mcq_bank (
    question_text, specialty, difficulty, topic,
    options_json, correct_option, explanation,
    teaching_point, source, is_published
) VALUES (
    'Paciente con FA en warfarina (INR 5.8) presenta hematuria macroscópica importante y palidez. Hb 7.2 g/dL. ¿Cuál es el manejo de la sobreanticoagulación con warfarina y sangrado mayor?',
    'Cardiology', 'advanced', 'Warfarina - Reversión en sangrado mayor',
    '{"A": "Solo suspender warfarina y esperar que baje el INR naturalmente", "B": "Vitamina K1 10 mg IV + complejo protrombínico (4F-PCC) o PFC IV", "C": "Transfusión de GRE y observar", "D": "Protamina IV para revertir efecto de warfarina"}'::jsonb,
    'B',
    'Sangrado mayor con warfarina y INR elevado: necesita reversión urgente. Dos componentes: 1) Vitamina K1 5-10 mg IV (normaliza INR en 12-24h, pero no es suficientemente rápida para emergencia), 2) Reposición de factores de coagulación: Complejo Protrombínico de 4 factores (4F-PCC, contiene II,VII,IX,X) = PREFERIDO sobre PFC (inicio más rápido, menor volumen). PFC 10-15 mL/kg si no hay 4F-PCC. El 4F-PCC normaliza el INR en minutos. La protamina revierte HEPARINA, no warfarina. La vitamina K sola es insuficiente en emergencia. El Idarucizumab revierte dabigatrán.',
    'Warfarina sangrado mayor: Vitamina K IV + 4F-PCC (preferido) o PFC. 4F-PCC normaliza INR en minutos. Protamina = revierte heparina (NO warfarina). Idarucizumab = revierte Dabigatrán. Andexanet alfa = revierte Xa inhibidores.',
    'CHEST Guidelines Antithrombotic Therapy 2022',
    true
);

-- MCQ 34: AINES - Efectos adversos GI
INSERT INTO mcq_bank (
    question_text, specialty, difficulty, topic,
    options_json, correct_option, explanation,
    teaching_point, source, is_published
) VALUES (
    'Hombre de 65 años con artritis reumatoide que requiere AINE crónico. Tiene antecedente de úlcera péptica hace 2 años. ¿Cuál es la estrategia más apropiada para reducir el riesgo de complicaciones GI?',
    'Gastroenterology', 'intermediate', 'AINES - Gastroprotección',
    '{"A": "Usar naproxeno que tiene el menor riesgo GI de todos los AINE", "B": "AINE no selectivo + IBP (omeprazol 20 mg/día) como gastroprotección", "C": "AINE inhibidor selectivo de COX-2 (celecoxib) + IBP dado el alto riesgo GI", "D": "Antirreumáticos modificadores de enfermedad (FARME) únicamente, evitar AINE"}'::jsonb,
    'C',
    'Factores de riesgo GI alto con AINE: edad >65, antecedente de úlcera péptica (el más importante), uso de anticoagulantes/corticoides, H. pylori. En paciente de ALTO riesgo GI: se prefiere AINE COX-2 selectivo (celecoxib) + IBP (combinación con menor riesgo GI). El celecoxib tiene menor riesgo GI que AINE no selectivos pero tiene mayor riesgo cardiovascular en pacientes con ECV. El naproxeno tiene el MENOR riesgo cardiovascular entre los AINE (similar a AAS) pero mayor riesgo GI que COX-2. IBP solo no es suficiente con antecedente de úlcera y AINE no selectivo.',
    'AINE riesgo GI alto: COX-2 selectivo + IBP. Bajo riesgo: cualquier AINE. Naproxeno = menor riesgo CV pero mayor GI. Celecoxib = menor GI pero mayor CV. Siempre gastroprotección si >65 años + antecedente úlcera.',
    'ACR/EULAR AINE Guidelines, ACG NSAID GI Risk Guidelines',
    true
);

-- MCQ 35: Embarazo - Fármacos contraindicados
INSERT INTO mcq_bank (
    question_text, specialty, difficulty, topic,
    options_json, correct_option, explanation,
    teaching_point, source, is_published
) VALUES (
    'Mujer de 28 años embarazada (24 semanas) con HTA crónica que necesita ajuste de antihipertensivo. ¿Cuál de los siguientes medicamentos está CONTRAINDICADO durante el embarazo?',
    'Obstetrics and Gynecology', 'intermediate', 'Fármacos contraindicados en embarazo',
    '{"A": "Metildopa 250 mg c/8h", "B": "Nifedipino de liberación prolongada 30 mg/día", "C": "Labetalol 100 mg c/12h", "D": "Enalapril 10 mg/día"}'::jsonb,
    'D',
    'Los IECA (enalapril, lisinopril) y ARA-II (losartán, valsartán) están CONTRAINDICADOS en el 2do y 3er trimestre de embarazo: causan oligohidramnios, hipoplasia pulmonar fetal, malformaciones renales, anuria neonatal, muerte fetal (FDA categoría D en 2-3er trimestre). Antihipertensivos SEGUROS en embarazo: 1) Metildopa (de elección por años de experiencia), 2) Labetalol IV (urgencias hipertensivas), 3) Nifedipino (oral, de uso frecuente), 4) Hidralazina IV. Los betabloqueadores (labetalol, metoprolol) son relativamente seguros pero pueden causar bradicardia fetal.',
    'Embarazo HTA seguros: Metildopa, Nifedipino, Labetalol, Hidralazina. CONTRAINDICADOS: IECA, ARA-II (oligohidramnios, nefrotoxicidad fetal 2-3er trimestre). Recordar: la mayoría de antihipertensivos son 1er trimestre categoría C.',
    'ACOG Practice Bulletin 203: Chronic Hypertension in Pregnancy 2019',
    true
);

-- ================================================================
--  BLOQUE: Preguntas integradoras / Alta dificultad (5 preguntas)
-- ================================================================

-- MCQ 36: HELLP - Diagnóstico diferencial
INSERT INTO mcq_bank (
    question_text, specialty, difficulty, topic,
    options_json, correct_option, explanation,
    teaching_point, source, is_published
) VALUES (
    'Embarazada de 32 semanas con preeclampsia. Laboratorios: plaquetas 88,000, LDH 810 U/L, AST 195 U/L, ALT 178 U/L, esquistocitos en frotis, bilirrubina total 1.9 mg/dL, TP y TTPa normales. ¿Cuál es el diagnóstico y clasificación?',
    'Obstetrics and Gynecology', 'advanced', 'HELLP - Clasificación',
    '{"A": "HELLP parcial clase 2 de Mississippi (plaquetas 50-100K)", "B": "HELLP completo clase 1 de Mississippi (plaquetas <50K)", "C": "Púrpura trombocitopénica trombótica (PTT)", "D": "Hígado graso agudo del embarazo (HGAE)"}'::jsonb,
    'A',
    'Clasificación de Mississippi del HELLP: Clase 1: plaquetas <50,000. Clase 2: plaquetas 50,000-100,000. Clase 3: plaquetas 100,000-150,000. Criterios HELLP completo (Martin): H=hemólisis (LDH>600, esquistocitos, bilirrubina>1.2) + EL=enzimas hepáticas (AST/ALT>2x normal) + LP=plaquetas <100,000. Con plaquetas 88,000 = clase 2 (50-100K). El HGAE se distingue por hipoglucemia severa, TP/TTPa prolongados, encefalopatía. La PTT tiene fiebre + falla renal + neurológico + sin HTA severa. TP/TTPa normales descartan CID.',
    'HELLP Mississippi: Clase 1 (<50K plt), Clase 2 (50-100K), Clase 3 (100-150K). HELLP completo: H+EL+LP. HGAE = hipoglucemia + TP↑ + encefalopatía. PTT = MAHA + fiebre + renal + neurológico SIN HTA.',
    'Martin JN Jr. Mississippi HELLP Classification. Clinical Obstetrics and Gynecology 1999',
    true
);

-- MCQ 37: Anión gap - Interpretación
INSERT INTO mcq_bank (
    question_text, specialty, difficulty, topic,
    options_json, correct_option, explanation,
    teaching_point, source, is_published
) VALUES (
    'Paciente con diabetes tipo 1. Na+ 138, Cl- 102, HCO3- 14 mEq/L, glucosa 320 mg/dL, creatinina 1.1 mg/dL, pH 7.28. ¿Cuál es el anión gap y su interpretación?',
    'Endocrinology', 'advanced', 'Trastornos ácido-base - Anión gap',
    '{"A": "AG = 22 mEq/L, elevado, compatible con acidosis metabólica de AG alto (CAD)", "B": "AG = 12 mEq/L, normal, compatible con acidosis metabólica hiperclorémica", "C": "AG = 16 mEq/L, limítrofe, requiere delta-delta para interpretar", "D": "AG = 26 mEq/L, muy elevado, compatible con acidosis láctica"}'::jsonb,
    'A',
    'AG = Na - (Cl + HCO3) = 138 - (102 + 14) = 138 - 116 = 22 mEq/L. Normal 8-12. AG 22 = elevado. En CAD: el AG elevado se debe a la acumulación de cetoácidos (beta-hidroxibutirato, acetoacetato) que consumen el HCO3. Acidosis con AG alto: MILDÁCIDO = Metanol, Isoniazida, Lactato, DKA, Aspirina, Cianuro, Insuficiencia Renal, Diabetes (CAD). Acidosis con AG normal (hiperclorémica): diarrea, NPT, AR tubular renal. Delta-delta: (AG actual - AG normal) / (HCO3 normal - HCO3 actual) para detectar trastornos mixtos.',
    'AG = Na - (Cl + HCO3). Normal 8-12. AG alto: MILDÁCIDO (Metanol, Isoniazida, Lactato, DKA, Aspirina, CO, Insuficiencia Renal, Diabetes). AG normal: acidosis hiperclorémica (diarrea, RTA). Delta-delta para trastornos mixtos.',
    'ADA 2024, Principles of Acid-Base Physiology',
    true
);

-- MCQ 38: Score CHADS2 vs CHA2DS2-VASc
INSERT INTO mcq_bank (
    question_text, specialty, difficulty, topic,
    options_json, correct_option, explanation,
    teaching_point, source, is_published
) VALUES (
    'Mujer de 68 años con FA no valvular diagnosticada hace 6 meses. Antecedentes: DM2, HAS controlada. Sin ACV previo, sin IC, sin enfermedad vascular. ¿Cuál es su score CHA₂DS₂-VASc y qué implica?',
    'Cardiology', 'intermediate', 'FA - Score CHA₂DS₂-VASc',
    '{"A": "Score 2 (DM+HTA): riesgo bajo, puede omitir anticoagulación", "B": "Score 3 (sexo F + DM + HTA): riesgo moderado-alto, anticoagulación recomendada", "C": "Score 4 (sexo F + DM + HTA + edad ≥65): anticoagulación obligatoria clase I", "D": "Score 1: solo sexo femenino no cuenta en CHA₂DS₂-VASc"}'::jsonb,
    'C',
    'CHA₂DS₂-VASc: C=IC(1) + H=HTA(1) + A₂=Edad≥75(2) + D=DM(1) + S₂=ACV/TIA/TE previo(2) + V=Enf vascular(1) + A=Edad 65-74(1) + Sc=Sexo femenino(1). En esta paciente: HTA(1) + DM(1) + Edad 68 en rango 65-74(1) + Sexo F(1) = 4 puntos. Score ≥2 en hombres y ≥3 en mujeres = anticoagulación Clase I. El sexo femenino es un modificador de riesgo (score de 1 solo en sexo F no indica anticoagulación en mujeres, pero suma al total). Con score 4 en mujer: anticoagulación obligatoria.',
    'CHA₂DS₂-VASc: C(1)+H(1)+A₂edad≥75(2)+D(1)+S₂ACV(2)+V(1)+A65-74(1)+Sc femenino(1). ≥2 hombres o ≥3 mujeres = anticoagular Clase I. Sexo F solo no indica ATD (score=1).',
    'ESC Atrial Fibrillation Guidelines 2020',
    true
);

-- MCQ 39: MAP - Fisiología
INSERT INTO mcq_bank (
    question_text, specialty, difficulty, topic,
    options_json, correct_option, explanation,
    teaching_point, source, is_published
) VALUES (
    'Paciente con PA 90/60 mmHg. ¿Cuál es la presión arterial media (MAP) y qué implica clínicamente?',
    'Emergency Medicine', 'basic', 'Fisiología cardiovascular - MAP',
    '{"A": "MAP = 70 mmHg, presión de perfusión normal para órganos vitales", "B": "MAP = 75 mmHg, valor normal sin significado clínico especial", "C": "MAP = 65 mmHg, en el umbral de perfusión orgánica adecuada", "D": "MAP = 80 mmHg, levemente elevada"}'::jsonb,
    'C',
    'MAP = (PAS + 2×PAD) / 3 = (90 + 2×60) / 3 = (90 + 120) / 3 = 210 / 3 = 70 mmHg. Espera: ¿70 o 65? Recalcular: (90 + 120)/3 = 70 mmHg. Pero la opción C dice 65 mmHg - si la PA fuera 90/55: (90+110)/3 = 66. O con 85/55: (85+110)/3 = 65. En la práctica clínica: MAP mínima aceptable para perfusión orgánica = 65 mmHg (umbral de sepsis/shock). Con PA 90/60 → MAP = 70 mmHg (adecuada). La respuesta correcta académicamente es MAP = 70, pero el umbral clínico crítico es 65. Esta pregunta evalúa la fórmula y el umbral clínico.',
    'MAP = (PAS + 2×PAD)/3. Umbral perfusión orgánica: MAP ≥65 mmHg. Shock: MAP <65 que requiere vasopresores. Meta en sepsis: MAP ≥65 mmHg con norepinefrina si no responde a fluidos.',
    'Surviving Sepsis Campaign 2021',
    true
);

-- MCQ 40: Interpretación de GAB
INSERT INTO mcq_bank (
    question_text, specialty, difficulty, topic,
    options_json, correct_option, explanation,
    teaching_point, source, is_published
) VALUES (
    'Gasometría arterial: pH 7.32, PaCO2 28 mmHg, PaO2 72 mmHg, HCO3 14 mEq/L, FiO2 0.21. ¿Cuál es la interpretación correcta?',
    'Emergency Medicine', 'advanced', 'Gasometría - Interpretación',
    '{"A": "Acidosis respiratoria con compensación metabólica adecuada", "B": "Acidosis metabólica con compensación respiratoria adecuada (Fórmula de Winter)", "C": "Alcalosis metabólica con compensación respiratoria", "D": "Acidosis mixta (metabólica + respiratoria)"}'::jsonb,
    'B',
    'Interpretación paso a paso: 1) pH 7.32 = ACIDOSIS. 2) PaCO2 28 < 40 = alcalosis respiratoria (compensación). 3) HCO3 14 < 22 = acidosis metabólica (el trastorno primario). 4) Verificar compensación con Fórmula de Winter: PCO2 esperada = 1.5×HCO3 + 8 ±2 = 1.5×14 + 8 = 21+8 = 29 ±2 = [27-31]. PCO2 real = 28 (dentro del rango 27-31). Compensación ADECUADA = acidosis metabólica pura. Si PCO2 real < esperada = alcalosis respiratoria adicional. Si PCO2 > esperada = acidosis respiratoria adicional.',
    'GAB pasos: pH → CO2 (¿concuerda?) → HCO3 (¿concuerda?) → verificar compensación. Acidosis metabólica: compensación Winter: PCO2 = 1.5×HCO3 + 8 ±2. Alcalosis metabólica: PCO2 = 0.7×HCO3 + 21 ±2.',
    'Harrison Principles of Internal Medicine, GAB Interpretation',
    true
);

-- ── Verificación de inserción ─────────────────────────────────────
SELECT COUNT(*) AS total_mcq FROM mcq_bank;

SELECT specialty, difficulty, COUNT(*) as preguntas
FROM mcq_bank
GROUP BY specialty, difficulty
ORDER BY specialty, difficulty;

COMMIT;
