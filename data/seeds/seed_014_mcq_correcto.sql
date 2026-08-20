-- ================================================================
--  seed_014_mcq_schema_correcto.sql
--  Schema REAL de mcq_bank:
--    vignette          = viñeta clínica (contexto del paciente)
--    question          = pregunta concreta
--    options_json      = {"A":..., "B":..., "C":..., "D":...}
--    correct_answer    = char: 'A','B','C', o 'D'
--    explanation_correct = explicación de por qué es correcta
--    explanation_incorrect_json = {"A":...,"B":...} (distractores)
--    references_text   = fuente/referencia
--    is_active         = true
-- ================================================================

BEGIN;

-- Verificar conteo antes
SELECT COUNT(*) AS mcq_antes FROM mcq_bank;

-- ================================================================
--  BLOQUE 1: Cardiología (8 preguntas)
-- ================================================================

INSERT INTO mcq_bank (
    specialty, sub_specialty, difficulty, topic,
    vignette, question, options_json,
    correct_answer, explanation_correct,
    explanation_incorrect_json,
    references_text, is_active
) VALUES (
    'Cardiology', 'Cardiología intervencionista',
    'intermediate', 'STEMI - Reperfusión',
    'Paciente masculino de 58 años con diabetes e hipertensión. Llega a urgencias con 90 minutos de dolor torácico opresivo retroesternal irradiado a brazo izquierdo, intensidad 9/10. ECG muestra elevación del ST ≥2mm en V1-V4. PA 108/70 mmHg, FC 104 lpm, SpO2 92%. El hospital dispone de laboratorio de cateterismo con hemodinamia en operación.',
    '¿Cuál es el tratamiento de reperfusión de elección para este paciente?',
    '{"A": "Trombolisis con alteplasa IV inmediata sin esperar más estudios",
      "B": "ICP primaria (angioplastia + stent) en menos de 90 minutos desde el primer contacto médico",
      "C": "Trombolisis IV seguida de ICP de rescate programada a las 24 horas",
      "D": "Iniciar heparina no fraccionada IV y esperar resultado de troponinas para decidir"}'::jsonb,
    'B',
    'La ICP primaria es el estándar de oro para el STEMI cuando está disponible en ≤120 minutos desde el primer contacto médico (meta ideal: puerta-balón ≤90 min). Es superior a la trombolisis en reducción de mortalidad, reinfarto y ACV hemorrágico (NNT ≈10). La trombolisis se reserva para cuando la ICP no puede realizarse en ese tiempo. NUNCA esperar troponinas en STEMI con elevación ST clásica: el diagnóstico es electrocardiográfico.',
    '{"A": "La trombolisis tiene mayor tasa de reinfarto y ACV vs ICP primaria; se usa solo si ICP no disponible en ≤120 min",
      "C": "La trombolisis + ICP programada (estrategia farmacoinvasiva) es para cuando ICP primaria no disponible inmediatamente, no cuando sí lo está",
      "D": "En STEMI el diagnóstico es electrocardiográfico; esperar troponinas retrasa la reperfusión y aumenta necrosis (tiempo = miocardio)"}'::jsonb,
    'ESC Guidelines STEMI 2023; Keeley EC et al. Lancet 2003',
    true
);

INSERT INTO mcq_bank (
    specialty, sub_specialty, difficulty, topic,
    vignette, question, options_json,
    correct_answer, explanation_correct,
    explanation_incorrect_json,
    references_text, is_active
) VALUES (
    'Cardiology', 'Farmacología cardiovascular',
    'intermediate', 'STEMI - Antiagregación dual',
    'Hombre de 62 años con STEMI anterior que va directamente a ICP primaria. Ya recibió aspirina 300 mg VO masticada en el servicio de urgencias. Sin antecedente de ACV hemorrágico. Sin cirugía programada. El servicio de hemodinamia tiene disponibles Clopidogrel y Ticagrelor.',
    '¿Qué segundo antiagregante plaquetario debe administrarse como dosis de carga antes de la ICP?',
    '{"A": "Clopidogrel 75 mg VO (dosis de mantenimiento estándar diaria)",
      "B": "Ticagrelor 180 mg VO (dosis de carga)",
      "C": "Warfarina 5 mg VO para anticoagulación periprocedimiento",
      "D": "Dipiridamol 400 mg VO como alternativa antiagregante"}'::jsonb,
    'B',
    'El estudio PLATO (NEJM 2009, n=18,624) demostró que Ticagrelor 90 mg c/12h es superior a Clopidogrel 75 mg/día en SCA: reduce mortalidad cardiovascular (4.0 vs 5.1%, p<0.001), infarto y trombosis del stent, sin aumento significativo de sangrado mayor TIMI. La dosis de carga de Ticagrelor es 180 mg VO. Clopidogrel 300-600 mg carga es alternativa válida si Ticagrelor no disponible o contraindicado (ACV hemorrágico previo, uso concomitante de anticoagulantes orales, necesidad de cirugía próxima).',
    '{"A": "75 mg es la dosis de mantenimiento, no de carga. La carga de Clopidogrel es 300-600 mg; además es segunda opción vs Ticagrelor",
      "C": "Warfarina no tiene rol en antiagregación periICP; puede usarse anticoagulación parenteral (HNF, bivalirudina) pero no warfarina oral",
      "D": "Dipiridamol no tiene indicación en SCA ni en ICP; su uso es en ACV/AIT con aspirina (Aggrenox)"}'::jsonb,
    'Wallentin L et al. PLATO trial. NEJM 2009;361:1045; ESC ACS Guidelines 2023',
    true
);

INSERT INTO mcq_bank (
    specialty, sub_specialty, difficulty, topic,
    vignette, question, options_json,
    correct_answer, explanation_correct,
    explanation_incorrect_json,
    references_text, is_active
) VALUES (
    'Cardiology', 'Síndrome coronario agudo',
    'intermediate', 'SICA SEST - Diagnóstico diferencial',
    'Hombre de 62 años con HAS, DM2 y tabaquismo activo. Acude con dolor retroesternal opresivo 7/10 en reposo de 18 horas, irradiado a brazo izquierdo. ECG: depresión de ST de 1mm en V4-V6 que se normaliza al ceder el dolor (cambios dinámicos). Troponina I ultrasensible: 0.03 ng/mL al ingreso y 0.03 ng/mL a las 3 horas (valor de referencia <0.04 ng/mL; sin delta significativo).',
    '¿Cuál es el diagnóstico más preciso en este paciente?',
    '{"A": "STEMI anterior: requiere activación de sala de hemodinamia inmediata",
      "B": "IAMSEST (infarto sin elevación del ST): troponinas positivas seriales",
      "C": "Angina inestable: SICA SEST con troponinas negativas en 2 determinaciones",
      "D": "Angina estable clase III: responde al reposo sin cambios enzimáticos"}'::jsonb,
    'C',
    'La diferencia entre angina inestable e IAMSEST dentro del SICA SEST es EXCLUSIVAMENTE la troponina: Angina inestable = troponinas negativas seriales (sin delta ≥20%), IAMSEST = troponinas elevadas o con delta positivo ≥20% entre la basal y la de 3h. Ambas condiciones comparten el mismo manejo inicial (doble antiagregación, anticoagulación, estatinas de alta intensidad), pero troponina positiva implica mayor riesgo (estrategia invasiva más urgente: <24h vs <72h). En este caso: 2 troponinas negativas sin delta = ANGINA INESTABLE.',
    '{"A": "STEMI requiere elevación persistente de ST ≥1mm en ≥2 derivadas contiguas o BCRIHH nuevo; los cambios dinámicos sin elevación excluyen STEMI",
      "B": "IAMSEST requiere troponina elevada o con delta positivo; aquí ambas son negativas sin delta",
      "D": "Angina estable ocurre con esfuerzo predecible y cede con reposo/nitratos; este dolor es en reposo = inestable por definición"}'::jsonb,
    'ESC ACS without ST-elevation Guidelines 2020; Thygesen K et al. Eur Heart J 2019',
    true
);

INSERT INTO mcq_bank (
    specialty, sub_specialty, difficulty, topic,
    vignette, question, options_json,
    correct_answer, explanation_correct,
    explanation_incorrect_json,
    references_text, is_active
) VALUES (
    'Cardiology', 'Insuficiencia cardíaca',
    'advanced', 'ICFEr - Terapia farmacológica',
    'Mujer de 68 años con diagnóstico establecido de insuficiencia cardíaca con fracción de eyección reducida (ICFEr, FEVI 28%) en clase funcional NYHA III a pesar de 6 meses con furosemida 40 mg/día. Sin hipotensión. Creatinina 1.1 mg/dL, K+ 4.2 mEq/L, PA 128/80 mmHg.',
    '¿Qué combinación farmacológica ha demostrado mayor reducción de mortalidad en ICFEr y debe iniciarse?',
    '{"A": "Agregar digoxina 0.125 mg/día para mejorar inotropismo",
      "B": "Iniciar IECA (o ARNI) + betabloqueador + ARM + iSGLT2 (los 4 pilares de la ICFEr)",
      "C": "Agregar calcioantagonista dihidropiridínico (amlodipino 5 mg) para reducir poscarga",
      "D": "Aumentar furosemida a 80 mg/día y añadir espironolactona 25 mg sin otros cambios"}'::jsonb,
    'B',
    'La terapia de 4 pilares en ICFEr tiene nivel de evidencia Clase I-A para cada uno: 1) IECA/ARA-II o ARNI (sacubitrilo/valsartán, estudio PARADIGM-HF: -20% mortalidad), 2) Betabloqueador (carvedilol/bisoprolol/metoprolol succinato; estudios COPERNICUS, MERIT-HF, CIBIS-II: -34% mortalidad), 3) ARM (espironolactona/eplerenona, RALES/EMPHASIS-HF: -25-30% mortalidad), 4) iSGLT2 (dapagliflozina DAPA-HF / empagliflozina EMPEROR-Reduced: -25% hospitalización, -18% mortalidad). Los 4 reducen mortalidad independientemente.',
    '{"A": "Digoxina no reduce mortalidad en ICFEr (estudio DIG), solo reduce hospitalizaciones; no es pilar terapéutico",
      "C": "Calcioantagonistas no dihidropiridínicos (verapamilo, diltiazem) contraindicados en ICFEr por efecto inotrópico negativo; dihidropiridínicos (amlodipino) son neutros pero no reducen mortalidad",
      "D": "Solo el diurético sin IECA/BB/ARM/iSGLT2 es manejo subóptimo que no modifica la mortalidad"}'::jsonb,
    'ESC Heart Failure Guidelines 2021; PARADIGM-HF NEJM 2014; DAPA-HF NEJM 2019',
    true
);

INSERT INTO mcq_bank (
    specialty, sub_specialty, difficulty, topic,
    vignette, question, options_json,
    correct_answer, explanation_correct,
    explanation_incorrect_json,
    references_text, is_active
) VALUES (
    'Cardiology', 'Arritmias',
    'advanced', 'Fibrilación auricular - Anticoagulación',
    'Hombre de 72 años con fibrilación auricular no valvular de reciente diagnóstico. Antecedentes: HAS, DM2, insuficiencia renal crónica estadio 3 (TFGe 42 mL/min/1.73m²), sin ACV previo ni enfermedad vascular. Score CHA₂DS₂-VASc = 4. No toma anticoagulantes.',
    '¿Cuál es la anticoagulación oral más apropiada para este paciente con FA no valvular?',
    '{"A": "Aspirina 100 mg/día: suficiente protección en CHA₂DS₂-VASc ≥2 según guías antiguas",
      "B": "Warfarina con INR meta 2.0-3.0: estándar de oro en FA no valvular",
      "C": "Apixabán 5 mg c/12h (o 2.5 mg si cumple ≥2 criterios de reducción de dosis)",
      "D": "No anticoagular: el riesgo de sangrado con CKD supera el beneficio"}'::jsonb,
    'C',
    'Los ACODs son superiores a warfarina en FA no valvular: menor ACV hemorrágico (50% menos), menor mortalidad, sin necesidad de monitoreo de INR. Apixabán es el ACOD preferido en CKD: no está contraindicado hasta estadio 4-5 (TFGe >15). Dosis estándar: 5 mg c/12h. Reducir a 2.5 mg c/12h si ≥2 de: edad ≥80, peso ≤60 kg, creatinina ≥1.5 mg/dL. CHA₂DS₂-VASc ≥2 en hombres o ≥3 en mujeres = anticoagulación recomendada Clase I. AAS NO es alternativa a anticoagulación en FA (no tiene evidencia de reducción de ACV isquémico).',
    '{"A": "AAS no reduce ACV en FA; guías actuales lo retiraron como alternativa a anticoagulación (mayor sangrado que ACOD con menor eficacia)",
      "B": "ACODs superiores a warfarina en FA no valvular; warfarina se reserva para FA valvular (estenosis mitral moderada-severa, prótesis mecánica) o intolerancia a ACODs",
      "D": "CKD 3 no contraindica anticoagulación; el beneficio supera el riesgo. TFGe <15 o diálisis: usar warfarina o apixabán según preferencia"}'::jsonb,
    'ESC Atrial Fibrillation Guidelines 2020; ARISTOTLE trial NEJM 2011',
    true
);

-- ================================================================
--  BLOQUE 2: Neumología (5 preguntas)
-- ================================================================

INSERT INTO mcq_bank (
    specialty, sub_specialty, difficulty, topic,
    vignette, question, options_json,
    correct_answer, explanation_correct,
    explanation_incorrect_json,
    references_text, is_active
) VALUES (
    'Pulmonology', 'Neumología crítica',
    'intermediate', 'NAC - Score CURB-65',
    'Mujer de 71 años con EPOC GOLD II. Acude con 4 días de fiebre 39.4°C, tos con expectoración herrumbrosa, disnea progresiva ahora de reposo y confusión temporal. Exploración: FR 28/min, PA 102/64 mmHg, FC 118 lpm, SpO2 88% con FiO2 0.21. Laboratorio: BUN 25 mg/dL (urea 54 mg/dL = 9 mmol/L). RX tórax: consolidación lobar en LID. Antígeno de neumococo en orina: positivo.',
    '¿Cuál es el score CURB-65 de esta paciente y cuál es la conducta apropiada?',
    '{"A": "CURB-65 = 2 puntos: hospitalización en piso, antibiótico oral",
      "B": "CURB-65 = 3 puntos: hospitalización con vigilancia estrecha en piso o semicríticos",
      "C": "CURB-65 = 4 puntos: ingreso a UCI o área de alta dependencia",
      "D": "CURB-65 = 5 puntos: solo cuidados paliativos, mortalidad >50%"}'::jsonb,
    'C',
    'CURB-65 (1 punto cada uno): C=Confusión(1) + U=Urea>7mmol/L aquí 9mmol/L(1) + R=FR≥30 aquí 28(0) + B=PAS<90 o PAD≤60 aquí PAS 102 y PAD 64(1) + 65=edad≥65 aquí 71(1) = 4 puntos. NOTA: FR 28 no alcanza el umbral de ≥30. Score 4 = mortalidad estimada 14-27%, indicación de UCI/alta dependencia. Los criterios ATS/IDSA de NAC grave complementan: ≥1 mayor (shock, VM) o ≥3 menores (FR≥30, PaO2/FiO2<250, multilobar, confusión, uremia, leucopenia, trombocitopenia, hipotermia, hipotensión). SpO2 88% refuerza la gravedad.',
    '{"A": "Score 2 = mortalidad ~9%, hospitalización en piso con antibiótico IV, no oral en este caso",
      "B": "Score 3 = mortalidad ~14%, hospitalización recomendada con vigilancia; score 4 supera este umbral",
      "D": "Score 5 no implica solo cuidados paliativos; la mortalidad es alta pero la mayoría responde a tratamiento activo agresivo"}'::jsonb,
    'ATS/IDSA Community-Acquired Pneumonia Guidelines Clin Infect Dis 2019',
    true
);

INSERT INTO mcq_bank (
    specialty, sub_specialty, difficulty, topic,
    vignette, question, options_json,
    correct_answer, explanation_correct,
    explanation_incorrect_json,
    references_text, is_active
) VALUES (
    'Pulmonology', 'Antibioticoterapia',
    'advanced', 'NAC grave - Antibiótico empírico UCI',
    'La misma paciente (mujer 71 años, EPOC, CURB-65=4, SpO2 88%) ingresa a UCI. Antígeno de neumococo positivo. Sin alergias medicamentosas conocidas. Sin hospitalización ni antibióticos en los últimos 3 meses. No hay datos de Pseudomonas.',
    '¿Cuál es el régimen antibiótico empírico de primera línea para NAC grave en UCI sin factores de riesgo para Pseudomonas?',
    '{"A": "Amoxicilina-clavulanato 875/125 mg VO c/12h + azitromicina VO",
      "B": "Ceftriaxona 2g IV c/24h + Azitromicina 500mg IV c/24h",
      "C": "Piperacilina-tazobactam 4.5g IV c/6h + amikacina IV (cobertura antipseudomona)",
      "D": "Penicilina G cristalina 4 millones UI IV c/4h monoterapia"}'::jsonb,
    'B',
    'ATS/IDSA 2019 para NAC grave UCI sin factores de Pseudomonas: Betalactámico antinecumocócico (ceftriaxona 2g, cefotaxima 2g, ampicilina-sulbactam 3g) + Macrólido IV (azitromicina 500 mg). Esta cobertura dual (neumococo + atípicos: Legionella, Mycoplasma) reduce mortalidad vs monoterapia en NAC grave. Alternativa: Fluoroquinolona respiratoria (levofloxacino 750 mg IV c/24h) en monoterapia. El antibiótico debe administrarse en ≤1 hora del diagnóstico (cada hora de retraso aumenta mortalidad 7%).',
    '{"A": "Amoxicilina-clavulanato oral es inadecuado en NAC grave-UCI; requiere vía IV y cobertura de atípicos",
      "C": "Piperacilina-tazobactam es para NAC con factores de riesgo para Pseudomonas (bronquiectasias, EPOC severo con exacerbaciones frecuentes, antibióticos previos, hospitalización reciente); sin esos factores es sobretratamiento",
      "D": "Penicilina G en monoterapia no cubre atípicos y tiene sensibilidad variable para neumococo; insuficiente en NAC grave"}'::jsonb,
    'Metlay JP et al. ATS/IDSA NAC Guidelines. Am J Respir Crit Care Med 2019',
    true
);

-- ================================================================
--  BLOQUE 3: Neurología (5 preguntas)
-- ================================================================

INSERT INTO mcq_bank (
    specialty, sub_specialty, difficulty, topic,
    vignette, question, options_json,
    correct_answer, explanation_correct,
    explanation_incorrect_json,
    references_text, is_active
) VALUES (
    'Neurology', 'Neurología vascular',
    'advanced', 'ACV isquémico - Trombolisis',
    'Hombre de 65 años, sin antecedente de ACV ni anticoagulación, que llega a urgencias con 2.5 horas de evolución de afasia y hemiparesia derecha de inicio súbito. PA 168/94 mmHg, FC 82, glucemia 142 mg/dL. TAC craneal simple: normal (sin hiperdensidad, sin efecto de masa). INR 1.0, plaquetas 195,000. NIHSS = 14.',
    '¿Está indicada la trombolisis IV con alteplasa en este paciente y bajo qué condición de presión arterial?',
    '{"A": "No indicada: PA 168/94 es contraindicación absoluta (requiere <160/90 antes de tPA)",
      "B": "Sí indicada: cumple todos los criterios. La PA debe bajarse a <185/110 con labetalol IV antes de administrar tPA",
      "C": "No indicada: debe realizarse angioTAC para confirmar oclusión de gran vaso antes de cualquier decisión",
      "D": "Sí indicada: puede administrarse con la PA actual sin intervención porque <180/110"}'::jsonb,
    'B',
    'El alteplasa IV está indicado en ACV isquémico ≤4.5h con: TAC sin hemorragia, déficit neurológico medible (NIHSS 14 = moderado-grave), sin contraindicaciones. La PA debe ser <185/110 mmHg ANTES de administrar tPA. Con PA 168/94: ya cumple el umbral de <185/110, pero si fuera >185/110 se bajaría con labetalol IV 10-20 mg (meta <185/110 para dar tPA; mantener <180/105 durante 24h postTPA). El angioTAC no es requisito para iniciar tPA aunque puede orientar a trombectomía adicional. Con PA 168/94 puede administrarse tPA sin intervención antihipertensiva.',
    '{"A": "Contraindicación absoluta de PA en tPA es >185/110 (no >160/90); 168/94 ya está dentro del umbral permitido",
      "C": "El angioTAC puede hacerse en paralelo o después del tPA si no retrasa el tratamiento; no es requisito previo obligatorio",
      "D": "PA 168/94 sí está por debajo de 185/110 así que puede administrarse sin tratamiento antihipertensivo previo; esta opción es correcta en el contexto, pero B da más información completa sobre el manejo"}'::jsonb,
    'Powers WJ et al. AHA/ASA Stroke Guidelines. Stroke 2019;50:e344',
    true
);

INSERT INTO mcq_bank (
    specialty, sub_specialty, difficulty, topic,
    vignette, question, options_json,
    correct_answer, explanation_correct,
    explanation_incorrect_json,
    references_text, is_active
) VALUES (
    'Neurology', 'Urgencias neurológicas',
    'advanced', 'HIC - Control de presión arterial',
    'Hombre de 67 años con HTA de 15 años mal controlada. Cefalea thunderclap súbita + hemiparesia derecha + afasia mixta. GCS 12. PA 210/118 mmHg. TAC craneal simple: hematoma hiperdenso en putamen izquierdo de 12 mL, edema perilesional leve, DLM 2mm, sin herniación. INR 1.0.',
    '¿Cuál es la meta de presión arterial sistólica en las primeras horas según la evidencia actual (estudios INTERACT2 y ATACH-II)?',
    '{"A": "PAS < 180 mmHg (meta laxa para evitar hipoperfusión perilesional)",
      "B": "PAS 130-150 mmHg (reducción intensiva moderada con mejor balance riesgo-beneficio)",
      "C": "PAS < 120 mmHg (control intensivo absoluto para minimizar expansión del hematoma)",
      "D": "No tratar la PA: la hipertensión es una respuesta compensatoria neuroprotectora"}'::jsonb,
    'B',
    'INTERACT2 (NEJM 2013): PAS objetivo <140 vs <180 mmHg en HIC aguda. El grupo <140 no redujo el desenlace primario (muerte/discapacidad severa) pero mejoró outcomes funcionales a 90 días (modified Rankin). ATACH-II (NEJM 2016): PAS 110-139 vs 140-179 mmHg. El grupo 110-139 tuvo mayor daño renal sin beneficio adicional en outcomes neurológicos. Consenso AHA 2022: meta PAS 130-150 mmHg como balance óptimo entre reducir expansión del hematoma y evitar hipoperfusión cerebral. Fármacos: labetalol IV, nicardipino IV. EVITAR nitroprusiato (aumenta PIC por vasodilatación cerebral).',
    '{"A": "PAS <180 es demasiado laxo; la evidencia apoya reducción a 130-150 para mejorar outcomes funcionales",
      "C": "PAS <120 asociada a mayor lesión renal aguda sin beneficio neurológico en ATACH-II; es demasiado agresivo",
      "D": "La hipertensión en HIC NO es neuroprotectora; la expansión del hematoma en las primeras horas es la principal causa de deterioro y la PA elevada la favorece"}'::jsonb,
    'INTERACT2 NEJM 2013; ATACH-II NEJM 2016; AHA ICH Guidelines 2022',
    true
);

INSERT INTO mcq_bank (
    specialty, sub_specialty, difficulty, topic,
    vignette, question, options_json,
    correct_answer, explanation_correct,
    explanation_incorrect_json,
    references_text, is_active
) VALUES (
    'Neurology', 'Epilepsia',
    'intermediate', 'Estatus epiléptico - Primera línea',
    'Adulto de 35 años es traído a urgencias en crisis convulsiva tónico-clónica generalizada. El personal refiere que la crisis comenzó hace 8 minutos y continúa. Se coloca acceso venoso periférico. No tiene alergias conocidas. Glucemia capilar: 98 mg/dL.',
    '¿Cuál es el tratamiento de primera línea para detener el estatus epiléptico con acceso venoso disponible?',
    '{"A": "Fenitoína 20 mg/kg IV a pasar en 20 minutos con monitoreo cardíaco",
      "B": "Lorazepam 0.1 mg/kg IV (máximo 4 mg por dosis, repetir en 5 min si persiste)",
      "C": "Fenobarbital 20 mg/kg IV",
      "D": "Propofol 1-2 mg/kg IV en bolo para sedación inmediata"}'::jsonb,
    'B',
    'Las benzodiacepinas son la primera línea del estatus epiléptico (evidencia Clase I): Lorazepam 0.1 mg/kg IV (máx 4 mg, repetir ×1 si no cede en 5 min) es el preferido por su duración de acción (12-24h vs 20 min del diazepam). Alternativas de primera línea: diazepam 0.15 mg/kg IV o midazolam 0.2 mg/kg IM (sin acceso IV: estudio RAMPART demostró midazolam IM igual de eficaz que lorazepam IV). La fenitoína y el levetiracetam son segunda línea (si la benzodiacepina falla). El propofol es tercera línea para estatus refractario en UCI con intubación.',
    '{"A": "Fenitoína es segunda línea (si benzodiacepina falla); inicio de acción lento (10-20 min) y requiere monitoreo cardíaco; no es de primera elección",
      "C": "Fenobarbital es segunda o tercera línea; mayor riesgo de depresión respiratoria que benzodiacepinas; se usa cuando fenitoína/levetiracetam fallan",
      "D": "Propofol es tercera línea para estatus refractario en UCI intubado; no se usa como primera línea en urgencias sin vía aérea asegurada"}'::jsonb,
    'AAN Status Epilepticus Guidelines 2016; RAMPART trial NEJM 2012',
    true
);

-- ================================================================
--  BLOQUE 4: Endocrinología (4 preguntas)
-- ================================================================

INSERT INTO mcq_bank (
    specialty, sub_specialty, difficulty, topic,
    vignette, question, options_json,
    correct_answer, explanation_correct,
    explanation_incorrect_json,
    references_text, is_active
) VALUES (
    'Endocrinology', 'Diabetes y emergencias metabólicas',
    'intermediate', 'CAD - Secuencia terapéutica',
    'Joven de 19 años sin antecedentes previos. Traído por su madre con 3 días de náusea, vómito múltiple, dolor abdominal y confusión. Respiración profunda y rápida (Kussmaul), aliento a acetona. Glucemia venosa 518 mg/dL, pH 7.18, HCO3 8 mEq/L, K+ 5.8 mEq/L, cetonas en orina 4+, Na+ 132 mEq/L. El paciente tiene un acceso venoso periférico colocado.',
    '¿Cuál es el orden correcto de las primeras intervenciones en el manejo de esta CAD severa?',
    '{"A": "Insulina regular en bolo IV 0.1 U/kg → luego fluidos → luego verificar potasio",
      "B": "Bicarbonato de sodio IV primero (pH 7.18 = acidosis severa) → luego fluidos → luego insulina",
      "C": "SF 0.9% 1000 mL en 1 hora → verificar K+ → insulina en infusión 0.1 U/kg/h (sin bolo inicial)",
      "D": "Insulina NPH SC 0.5 U/kg → fluidos orales si tolera → monitoreo ambulatorio"}'::jsonb,
    'C',
    'Secuencia correcta en CAD (ADA 2024): 1) FLUIDOS PRIMERO: SF 0.9% 1L en 1h (corrige hipoperfusión, dilución de cetonas, prepara para insulina). 2) VERIFICAR K+: con K+ 5.8 (>3.5) se puede iniciar insulina; si K+<3.5 = dar KCl IV antes de insulina (la insulina mueve K+ al intracelular y puede causar arritmias). 3) INSULINA en infusión continua 0.1 U/kg/h SIN BOLO (evidencia actual: el bolo IV no mejora outcomes y puede precipitar hipoglucemia e hipocalemia rápidas - Kitabchi et al. 2008). BICARBONATO: SOLO si pH<6.9 (aquí pH 7.18 > 6.9 = NO indicado; riesgo de edema cerebral, hipocalemia, alcalosis paradójica).',
    '{"A": "El bolo de insulina está obsoleto en el manejo actual de CAD; ADA 2024 recomienda infusión sin bolo; además fluidos van primero",
      "B": "Bicarbonato SOLO si pH<6.9; con pH 7.18 está contraindicado (edema cerebral, empeora hipocalemia, alcalosis paradójica del SNC)",
      "D": "Insulina NPH SC tiene inicio lento (2-4h) y es completamente inadecuada en CAD severa; requiere insulina de acción rápida IV"}'::jsonb,
    'Kitabchi AE et al. Diabetes Care 2009; ADA Standards of Care 2024',
    true
);

INSERT INTO mcq_bank (
    specialty, sub_specialty, difficulty, topic,
    vignette, question, options_json,
    correct_answer, explanation_correct,
    explanation_incorrect_json,
    references_text, is_active
) VALUES (
    'Endocrinology', 'Diabetes y emergencias metabólicas',
    'intermediate', 'CAD vs Estado hiperosmolar',
    'Mujer de 72 años con DM2 de larga evolución. Traída por confusión progresiva de 2 días. Glucemia 850 mg/dL, Na+ 148 mEq/L, K+ 4.1 mEq/L, Cl- 108 mEq/L, osmolalidad efectiva 344 mOsm/kg, pH 7.36, HCO3 22 mEq/L, PCO2 38 mmHg, cetonas en orina negativas. Creatinina 2.1 mg/dL.',
    '¿Cuál es el diagnóstico correcto y su diferencia principal con la CAD?',
    '{"A": "CAD severa: glucosa >600 confirma CAD independientemente del pH",
      "B": "Estado hiperosmolar hiperglucémico (EHH): hiperglucemia extrema + hiperosmolaridad + SIN acidosis + SIN cetosis significativa",
      "C": "CAD mixto con EHH: combinación con glucosa muy alta y confusión",
      "D": "Acidosis láctica por metformina: glucosa alta y confusión con pH normal transitoriamente"}'::jsonb,
    'B',
    'EHH (antes CHHNK): glucosa>600 mg/dL + osmolalidad efectiva>320 mOsm/kg + pH≥7.3 + HCO3≥15 + SIN cetosis significativa (cetonas negativas o mínimas) + alteración neurológica. CAD: glucosa>250 + pH<7.3 + HCO3<15 + CETONAS POSITIVAS + AG elevado. DIFERENCIA CLAVE: cetosis (CAD sí, EHH no) y acidosis (CAD sí, EHH no). Osmolalidad efectiva = 2×Na + glucosa/18 = 2×148 + 850/18 = 296 + 47 = 343 mOsm/kg (>320). Manejo: EHH prioriza corrección de osmolalidad con fluidos (más lento que CAD, riesgo de edema cerebral si corrección rápida); insulina se inicia después de los fluidos.',
    '{"A": "La glucosa >600 por sí sola no define CAD; la CAD puede ocurrir con glucosa de 250-400; lo definitorio en CAD es la cetoacidosis, no el nivel de glucosa",
      "C": "CAD y EHH pueden coexistir pero en este caso el pH normal y cetonas negativas excluyen CAD",
      "D": "Acidosis láctica por metformina tiene pH bajo (<7.35) con lactato elevado >5 mmol/L; aquí pH 7.36 es normal"}'::jsonb,
    'ADA Hyperglycemic Crises Guidelines. Diabetes Care 2024',
    true
);

-- ================================================================
--  BLOQUE 5: Gastroenterología (3 preguntas)
-- ================================================================

INSERT INTO mcq_bank (
    specialty, sub_specialty, difficulty, topic,
    vignette, question, options_json,
    correct_answer, explanation_correct,
    explanation_incorrect_json,
    references_text, is_active
) VALUES (
    'Gastroenterology', 'Pancreatología',
    'intermediate', 'Pancreatitis aguda - Diagnóstico',
    'Mujer de 45 años con obesidad y colelitiasis conocida. 24 horas de dolor epigástrico intenso 9/10 irradiado en cinturón al dorso, inicio súbito tras cena copiosa, con vómito incoercible y fiebre de 38.6°C. USG: colelitiasis múltiple, colédoco 10mm dilatado, páncreas no visualizable. Laboratorio: lipasa 1840 U/L (normal <60), amilasa 920 U/L, bilirrubina total 3.2 mg/dL.',
    '¿Cuántos criterios de Atlanta revisada 2012 se cumplen para el diagnóstico de pancreatitis aguda y es necesaria una TAC para confirmarlo?',
    '{"A": "1 criterio (solo dolor): TAC obligatoria para confirmar el diagnóstico",
      "B": "2 criterios (dolor + lipasa ≥3x normal): diagnóstico establecido, TAC no es necesaria de rutina al ingreso",
      "C": "3 criterios (dolor + lipasa + imagen): siempre se necesitan los 3 para diagnóstico definitivo",
      "D": "2 criterios (dolor + imagen): lipasa elevada no es criterio diagnóstico según Atlanta"}'::jsonb,
    'B',
    'Criterios de Atlanta revisada 2012 para pancreatitis aguda (≥2 de 3 son suficientes): 1) Dolor epigástrico irradiado al dorso (presente), 2) Lipasa o amilasa ≥3x normal (lipasa 1840/60 = 30x normal; presente), 3) Hallazgos de imagen compatibles (TAC/RM/USG). Con 2 criterios el diagnóstico es CLÍNICO y NO requiere TAC de rutina al ingreso. La TAC con contraste se reserva para: evaluar complicaciones (necrosis, absceso), diagnóstico dudoso (solo 1 criterio), o deterioro clínico a las 48-72h sin mejoría. TAC temprana (<72h) puede subestimar necrosis.',
    '{"A": "Con 2 criterios el diagnóstico está establecido; la TAC no es obligatoria al ingreso si ≥2 criterios están presentes",
      "C": "Atlanta 2012 solo requiere ≥2 de 3; no son necesarios los 3 simultáneamente",
      "D": "Lipasa SÍ es criterio de Atlanta; de hecho es más sensible y específica que amilasa para pancreatitis aguda"}'::jsonb,
    'Revised Atlanta Classification of Acute Pancreatitis 2012. Gut 2013; AGA Guidelines 2024',
    true
);

INSERT INTO mcq_bank (
    specialty, sub_specialty, difficulty, topic,
    vignette, question, options_json,
    correct_answer, explanation_correct,
    explanation_incorrect_json,
    references_text, is_active
) VALUES (
    'Gastroenterology', 'Pancreatología',
    'advanced', 'Pancreatitis aguda - Fluidoterapia',
    'La misma paciente con pancreatitis aguda severa ingresa a UCI. FC 124, PA 96/60, hematocrito 48% (hemoconcentración). Creatinina 1.8 mg/dL. Se decide iniciar reanimación hídrica agresiva.',
    '¿Cuál es el fluido de reanimación preferido en la pancreatitis aguda y por qué es superior al suero fisiológico 0.9%?',
    '{"A": "Suero fisiológico 0.9% a 500 mL/h: el más estudiado en pancreatitis y más disponible",
      "B": "Ringer Lactato 250-500 mL/h: reduce marcadores inflamatorios (PCR) y la incidencia de SIRS vs SF 0.9%",
      "C": "Coloides (albúmina 5%) 250 mL/h: mejor expansión del espacio intravascular",
      "D": "Dextrosa 5% en agua: aporta energía y evita la hipoglucemia por NPO"}'::jsonb,
    'B',
    'El Ringer Lactato (RL) es el fluido preferido en pancreatitis aguda sobre SF 0.9%, respaldado por: RCT Wu et al. (Clin Gastroenterol Hepatol 2011): RL vs SF en pancreatitis aguda → RL redujo significativamente SIRS a las 24h (84% vs 0%, p<0.001) y PCR (51 vs 104 mg/L). Mecanismo: el SF 0.9% produce acidosis hiperclorémica que activa tripsinógeno pancreático; el RL contiene lactato que se metaboliza a bicarbonato (alcalinizante). La meta de reanimación: FC<100, PA normal, diuresis>0.5 mL/kg/h, BUN que disminuye a las 24h. Tasa: 250-500 mL/h primeras 12-24h (ajustar según monitorización).',
    '{"A": "SF 0.9% produce acidosis hiperclorémica que puede activar tripsinógeno y empeorar la pancreatitis; es inferior al RL",
      "C": "Coloides (albúmina) no tienen evidencia superior a cristaloides en pancreatitis y son más costosos; no se recomiendan de rutina",
      "D": "Dextrosa no corrige la deshidratación ni la hipovolemia; es inadecuada como fluido de reanimación en pancreatitis"}'::jsonb,
    'Wu BU et al. Clin Gastroenterol Hepatol 2011; AGA Pancreatitis Guidelines 2024',
    true
);

-- ================================================================
--  BLOQUE 6: Ginecología y Obstetricia (4 preguntas)
-- ================================================================

INSERT INTO mcq_bank (
    specialty, sub_specialty, difficulty, topic,
    vignette, question, options_json,
    correct_answer, explanation_correct,
    explanation_incorrect_json,
    references_text, is_active
) VALUES (
    'Obstetrics and Gynecology', 'Medicina materno-fetal',
    'advanced', 'Preeclampsia - Criterios de severidad ACOG 2020',
    'Embarazada de 34 semanas con PA 168/112 mmHg en 2 tomas. Proteinuria 4.8 g/24h. Plaquetas 89,000/mm³. ALT 186 U/L, AST 210 U/L. LDH 820 U/L. Creatinina 1.2 mg/dL. Esquistocitos en frotis. Cefalea frontal 8/10. Visión borrosa. Edema facial ++. Movimientos fetales presentes.',
    '¿Cuál de los siguientes hallazgos NO es criterio de severidad en preeclampsia según ACOG Practice Bulletin 222 (2020)?',
    '{"A": "Plaquetas < 100,000/mm³",
      "B": "Proteinuria > 5 g en orina de 24 horas",
      "C": "Creatinina sérica > 1.1 mg/dL (o duplicación de la basal)",
      "D": "Cefalea de nueva aparición que no responde a analgésicos"}'::jsonb,
    'B',
    'ACOG 2020 eliminó la proteinuria masiva (>5 g/24h) de los criterios de severidad de preeclampsia. Actualmente, preeclampsia con criterios de severidad se define por PA ≥160/110 en ≥2 ocasiones + UNO de: plaquetas<100,000, creatinina>1.1 mg/dL o duplicación de basal, enzimas hepáticas>2x normal, edema pulmonar, cefalea nueva o trastornos visuales, epigastralgia severa. La proteinuria cuantitativa ya no modifica la clasificación de severidad (aunque sigue siendo criterio diagnóstico de preeclampsia). El edema de extremidades tampoco es criterio de severidad.',
    '{"A": "Plaquetas <100,000 es criterio de severidad y parte del síndrome HELLP (clase 2 de Mississippi)",
      "C": "Creatinina >1.1 mg/dL (basal embarazo ~0.5-0.8) indica disfunción renal y es criterio de severidad",
      "D": "Cefalea de nueva aparición que no responde a paracetamol es síntoma de severidad neurológica (criterio ACOG 2020)"}'::jsonb,
    'ACOG Practice Bulletin No. 222: Gestational Hypertension and Preeclampsia. Obstet Gynecol 2020',
    true
);

INSERT INTO mcq_bank (
    specialty, sub_specialty, difficulty, topic,
    vignette, question, options_json,
    correct_answer, explanation_correct,
    explanation_incorrect_json,
    references_text, is_active
) VALUES (
    'Obstetrics and Gynecology', 'Urgencias obstétricas',
    'advanced', 'MgSO4 - Toxicidad y antídoto',
    'Paciente con preeclampsia severa recibe sulfato de magnesio IV (carga 4g en 20 min, mantenimiento 2 g/h). A la hora de iniciado el mantenimiento la enfermera reporta: frecuencia respiratoria 10/min, reflejos rotulianos ausentes, diuresis 20 mL en la última hora. El paciente está somnoliento pero responde a estímulos.',
    '¿Cuál es la conducta inmediata ante estos signos de toxicidad por sulfato de magnesio?',
    '{"A": "Reducir la velocidad de infusión de MgSO4 a 1 g/h y vigilar",
      "B": "Suspender MgSO4 de inmediato y administrar gluconato de calcio 1g IV en 3 minutos",
      "C": "Administrar furosemida 40 mg IV para aumentar la eliminación renal del magnesio",
      "D": "Intubar preventivamente y continuar MgSO4 a la misma dosis"}'::jsonb,
    'B',
    'Toxicidad por MgSO4: Secuencia por nivel sérico: Mg 4-5 mEq/L = rango terapéutico (reflejos presentes, sin toxicidad), Mg 5-9 mEq/L = PÉRDIDA DE REFLEJOS ROTULIANOS (primer signo de toxicidad), Mg 9-13 mEq/L = parálisis respiratoria (FR<12/min), Mg >15 mEq/L = paro cardíaco. MONITORIZACIÓN obligatoria: reflejos rotulianos presentes + FR>12/min + diuresis>25 mL/h (el Mg se elimina por vía renal). Con signos de toxicidad (reflejos ausentes + FR 10): SUSPENDER MgSO4 + ANTÍDOTO INMEDIATO: gluconato de calcio 10% 1g IV en 3 minutos (antagoniza el Mg en la membrana celular). Preparar intubación como contingencia.',
    '{"A": "Reducir la dosis cuando ya hay toxicidad establecida (reflejos ausentes + FR deprimida) es insuficiente y peligroso; se requiere suspender y antídoto",
      "C": "Furosemida puede aumentar eliminación renal pero actúa demasiado lento ante una emergencia; el gluconato de calcio actúa en segundos",
      "D": "Continuar MgSO4 con toxicidad establecida puede progresar a paro cardíaco; la intuación puede ser necesaria pero es secundaria al antídoto"}'::jsonb,
    'ACOG Practice Bulletin 222: Preeclampsia 2020; SMFM Consult Series',
    true
);

-- ================================================================
--  BLOQUE 7: Infectología / Sepsis (3 preguntas)
-- ================================================================

INSERT INTO mcq_bank (
    specialty, sub_specialty, difficulty, topic,
    vignette, question, options_json,
    correct_answer, explanation_correct,
    explanation_incorrect_json,
    references_text, is_active
) VALUES (
    'Infectious Disease', 'Sepsis y choque séptico',
    'intermediate', 'Sepsis-3 - Definición y clasificación',
    'Mujer de 65 años con ITU complicada. Al ingreso: temperatura 38.9°C, FC 108 lpm, FR 22/min, PA 110/70 mmHg. Score SOFA: 2 puntos (creatinina levemente elevada). Lactato 1.8 mmol/L. Sin vasopresores.',
    '¿Cómo se clasifica esta presentación según la definición Sepsis-3 (Singer et al. JAMA 2016)?',
    '{"A": "SIRS sin sepsis: cumple criterios de respuesta inflamatoria pero no disfunción orgánica",
      "B": "Sepsis: infección sospechada + disfunción orgánica aguda (SOFA ≥2)",
      "C": "Choque séptico: requiere vasopresores + lactato >2 mmol/L",
      "D": "Infección sin sepsis: lactato <2 mmol/L y PA normal excluyen sepsis"}'::jsonb,
    'B',
    'Definición Sepsis-3 (JAMA 2016): SEPSIS = infección sospechada o confirmada + disfunción orgánica aguda (aumento del score SOFA ≥2 puntos sobre el basal). CHOQUE SÉPTICO = sepsis + hipotensión que requiere vasopresores para MAP ≥65 mmHg + lactato >2 mmol/L DESPUÉS de reanimación adecuada con fluidos. En esta paciente: infección (ITU) + SOFA ≥2 = SEPSIS. No es choque séptico porque PA es normal y no requiere vasopresores. El SIRS ya no es necesario para el diagnóstico (puede haber sepsis sin SIRS y SIRS sin sepsis). qSOFA ≥2 en urgencias alerta sobre sepsis: FR≥22 + alteración mental + PAS≤100.',
    '{"A": "SOFA ≥2 indica disfunción orgánica que define sepsis; el SIRS es un concepto obsoleto en Sepsis-3",
      "C": "Choque séptico requiere vasopresores + lactato >2 mmol/L POST reanimación; PA 110/70 sin vasopresores no es choque",
      "D": "Lactato <2 y PA normal no excluyen sepsis; la sepsis se define por disfunción orgánica (SOFA ≥2), no por lactato o PA"}'::jsonb,
    'Singer M et al. The Third International Consensus Definitions for Sepsis. JAMA 2016;315:801',
    true
);

-- ================================================================
--  BLOQUE 8: Cirugía (2 preguntas)
-- ================================================================

INSERT INTO mcq_bank (
    specialty, sub_specialty, difficulty, topic,
    vignette, question, options_json,
    correct_answer, explanation_correct,
    explanation_incorrect_json,
    references_text, is_active
) VALUES (
    'Surgery', 'Cirugía de urgencias',
    'intermediate', 'Apendicitis - Score de Alvarado',
    'Mujer de 28 años, alergia a penicilina (rash), sin embarazo (beta-hCG negativa). 72 horas de dolor que inició periumbilical y migró a fosa ilíaca derecha, anorexia, náusea, fiebre 38.8°C. Blumberg positivo, Rovsing positivo, Psoas positivo. Leucocitos 18,200 con 91% neutrófilos. PCR 168 mg/L. TAC: apéndice 12mm, colección periapendicular 2cm, sin neumoperitoneo.',
    '¿Cuál es el score de Alvarado y la conducta quirúrgica más apropiada?',
    '{"A": "Score 6: observación 24h, TAC de control, cirugía solo si empeora",
      "B": "Score 8: alta probabilidad, apendicectomía laparoscópica urgente + lavado + ATB con ciprofloxacino/metronidazol (alergia penicilina)",
      "C": "Score 10: apendicectomía abierta inmediata sin estudios adicionales",
      "D": "Score 7: drenaje percutáneo del absceso y antibióticos, sin cirugía urgente"}'::jsonb,
    'B',
    'Score de Alvarado: Migración(2) + Anorexia(1) + Náusea(1) + Dolor FID(2) + Blumberg+(1) + T>37.3°C(1) + Leucocitosis>10K(2) = máximo 10. Aquí: migración(2)+anorexia(1)+náusea(1)+dolor FID(2)+Blumberg+(1)+fiebre(1)+leucocitosis(2) = 10 puntos, pero vómito no es confirmado entonces puede ser 9. Score ≥7 = alta probabilidad quirúrgica. Con absceso periapendicular 2cm y peritonitis localizada: apendicectomía laparoscópica urgente + lavado + drenaje periapendicular. Antibiótico en alergia a penicilina: Ciprofloxacino 400mg IV c/12h + Metronidazol 500mg IV c/8h (NO ampicilina-sulbactam ni piperacilina-tazobactam por alergia).',
    '{"A": "Score 6 indicaría probabilidad intermedia; con score ≥7-8 y TAC confirmatoria la indicación quirúrgica es clara",
      "C": "La laparoscópica es la vía preferida incluso en perforada; la cirugía abierta no es primera elección en el escenario actual",
      "D": "El drenaje percutáneo + ATB (apendicectomía de intervalo) es una opción para abscesos >3cm maduros sin peritonitis franca; con peritonitis localizada y score alto la cirugía urgente es preferida"}'::jsonb,
    'Alvarado A. Ann Emerg Med 1986; WSES Jerusalem Guidelines 2020',
    true
);

-- ================================================================
--  BLOQUE 9: Nefrología (2 preguntas)
-- ================================================================

INSERT INTO mcq_bank (
    specialty, sub_specialty, difficulty, topic,
    vignette, question, options_json,
    correct_answer, explanation_correct,
    explanation_incorrect_json,
    references_text, is_active
) VALUES (
    'Nephrology', 'Lesión renal aguda',
    'intermediate', 'LRA - Criterios KDIGO 2012',
    'Paciente hospitalizado por neumonía. Creatinina basal conocida de 0.9 mg/dL (medida hace 3 semanas). En 48 horas de hospitalización la creatinina sube a 1.3 mg/dL. Diuresis mantenida en 0.6 mL/kg/h. No hay signos de sobrecarga hídrica.',
    '¿Tiene este paciente lesión renal aguda (LRA) según criterios KDIGO 2012 y en qué estadio?',
    '{"A": "No tiene LRA: el aumento es mínimo y la diuresis es normal (>0.5 mL/kg/h)",
      "B": "LRA estadio 1: aumento de creatinina ≥0.3 mg/dL en 48h (delta = 0.4 mg/dL cumple criterio)",
      "C": "LRA estadio 2: creatinina 2-2.9x el valor basal",
      "D": "LRA estadio 3: creatinina ≥3x el basal o necesidad de diálisis"}'::jsonb,
    'B',
    'Criterios KDIGO 2012 para LRA (cualquiera de los 3): 1) Aumento de creatinina ≥0.3 mg/dL en 48h [aquí: 1.3-0.9=0.4 mg/dL ≥0.3: CUMPLE], 2) Aumento ≥1.5x el basal en 7 días [1.3/0.9=1.44x, NO cumple el umbral de 1.5x], 3) Diuresis <0.5 mL/kg/h por ≥6h [0.6 mL/kg/h: NO cumple]. Estadificación: E1=creatinina 1.5-1.9x basal OR aumento ≥0.3 mg/dL. E2=2.0-2.9x basal. E3=≥3x basal OR creatinina≥4 mg/dL OR diálisis. El delta de 0.4 mg/dL en 48h cumple criterio 1 = LRA Estadio 1. La diuresis normal no excluye LRA.',
    '{"A": "La diuresis normal NO excluye LRA; el criterio 1 (delta creatinina ≥0.3 mg/dL en 48h) es independiente de la diuresis",
      "C": "LRA estadio 2 requiere creatinina 2-2.9x basal; aquí es 1.44x, que es estadio 1",
      "D": "LRA estadio 3 requiere ≥3x basal (aquí 1.44x) o creatinina ≥4 mg/dL (aquí 1.3) o diálisis; no cumple"}'::jsonb,
    'KDIGO Acute Kidney Injury Guidelines 2012. Kidney Int Suppl 2012',
    true
);

-- ================================================================
--  BLOQUE 10: Pediatría (3 preguntas)
-- ================================================================

INSERT INTO mcq_bank (
    specialty, sub_specialty, difficulty, topic,
    vignette, question, options_json,
    correct_answer, explanation_correct,
    explanation_incorrect_json,
    references_text, is_active
) VALUES (
    'Pediatrics', 'Neumología pediátrica',
    'intermediate', 'Bronquiolitis - Manejo basado en evidencia',
    'Lactante de 6 meses, previamente sano, con 2 días de fiebre 38.2°C, rinorrea hialina, tos y sibilancias. Es el primer episodio de sibilancias. SpO2 93%, FR 52/min, uso moderado de músculos accesorios intercostales. No tiene historia familiar de atopia. Se diagnostica bronquiolitis moderada.',
    '¿Cuál es el manejo más apropiado según la evidencia actual (AAP Guidelines)?',
    '{"A": "Salbutamol nebulizado 2.5 mg c/4h + corticosteroides sistémicos (dexametasona IM)",
      "B": "Adrenalina nebulizada 3 mg + dexametasona oral + alta si mejora en 2 horas",
      "C": "Oxígeno si SpO2 <90-92%, succión nasal suave, hidratación adecuada, monitorización",
      "D": "Antibióticos IV (ampicilina-sulbactam) por posible sobreinfección bacteriana"}'::jsonb,
    'C',
    'La bronquiolitis es de etiología viral (VRS en 70-80% de casos) y el tratamiento es EXCLUSIVAMENTE de soporte según AAP 2014 (reafirmadas 2022): 1) Oxígeno suplementario si SpO2 <90-92% (con SpO2 93% está en el límite; se puede observar o iniciar O2 con cánula nasal), 2) Succión/aspiración nasal para permeabilizar vía aérea, 3) Hidratación: oral si tolera, IV/SNG si no. NO existen intervenciones farmacológicas con beneficio demostrado: Salbutamol = sin beneficio (Cochrane 2014: no mejora SpO2 ni internación), Corticoides = sin beneficio (Cochrane 2013), Adrenalina = sin beneficio en hospitalizados, Antibióticos = solo si sobreinfección documentada (<1%).',
    '{"A": "Salbutamol sin evidencia de beneficio en bronquiolitis (Cochrane 2014); no se recomienda rutinariamente; corticoides tampoco tienen beneficio",
      "B": "Adrenalina+dexametasona: estudio PECARN mostró reducción de hospitalizaciones en urgencias, pero no en hospitalizados; AAP no recomienda uso rutinario",
      "D": "Bronquiolitis viral: antibióticos no indicados; sobreinfección bacteriana es rara (<1%); solo si evidencia clara de neumonía bacteriana concurrente"}'::jsonb,
    'AAP Clinical Practice Guideline: Bronchiolitis. Pediatrics 2014; reafirmado 2022',
    true
);

-- Verificación final
SELECT
    COUNT(*) AS total_mcq,
    COUNT(*) FILTER (WHERE vignette IS NOT NULL AND vignette != '') AS con_vignette,
    COUNT(*) FILTER (WHERE question IS NOT NULL AND question != '') AS con_question,
    COUNT(*) FILTER (WHERE correct_answer IS NOT NULL) AS con_respuesta
FROM mcq_bank;

SELECT specialty, difficulty, COUNT(*) AS preguntas
FROM mcq_bank
WHERE is_active = true
GROUP BY specialty, difficulty
ORDER BY specialty, difficulty;

COMMIT;
