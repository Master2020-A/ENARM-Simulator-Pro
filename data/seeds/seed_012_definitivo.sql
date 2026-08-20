-- ================================================================
--  seed_012_casos_definitivo.sql
--  Schema real mapeado correctamente desde \d clinical_cases
--
--  Columnas obligatorias (NOT NULL):
--    title, slug (UNIQUE), chief_complaint, specialty,
--    difficulty (varchar), setting (varchar),
--    patient_json (jsonb), expected_diagnosis_json (jsonb)
--
--  Columnas opcionales usadas:
--    sub_specialty, history_present_illness, symptoms_json,
--    expected_studies_json, expected_treatments_json,
--    expected_followup, learning_objectives_json,
--    teaching_points, author_name, is_published
-- ================================================================

BEGIN;

-- ── CASO 004: Angina Inestable ───────────────────────────────────
INSERT INTO clinical_cases (
    title,
    slug,
    chief_complaint,
    specialty,
    sub_specialty,
    difficulty,
    setting,
    patient_json,
    history_present_illness,
    symptoms_json,
    expected_diagnosis_json,
    expected_studies_json,
    expected_treatments_json,
    expected_followup,
    learning_objectives_json,
    teaching_points,
    author_name,
    is_published
) VALUES (
    'Angina Inestable - SICA SEST en paciente con factores de riesgo cardiovascular',
    'angina-inestable-sica-sest-riesgo-cardiovascular',
    'Dolor en el pecho que me da en reposo desde ayer',
    'Cardiology',
    'Cardiología intervencionista',
    'intermediate',
    'emergency',
    '{
        "age": 62,
        "sex": "Male",
        "weight_kg": 88,
        "height_cm": 172,
        "occupation": "Chofer de camión jubilado",
        "full_name": "Carlos Mendoza Ríos",
        "lifestyle": {
            "smoker": true,
            "pack_years": 40,
            "exercise_frequency": "sedentary"
        },
        "medical_history": [
            {"condition": "Hipertensión arterial", "years": 10, "treatment": "Amlodipino 5 mg/día"},
            {"condition": "Diabetes mellitus tipo 2", "years": 8, "treatment": "Metformina 850 mg c/12h"},
            {"condition": "Dislipidemia", "treatment": "Sin tratamiento"}
        ],
        "family_history": [
            {"relationship": "Padre", "condition": "IAM", "deceased": true, "age_at_diagnosis": 58}
        ],
        "vital_signs_admission": {
            "systolic_bp": 158,
            "diastolic_bp": 96,
            "heart_rate": 94,
            "respiratory_rate": 18,
            "temperature_c": 36.8,
            "spo2": 97,
            "pain_scale": 7
        }
    }'::jsonb,
    'Masculino de 62 años que acude a urgencias por dolor retroesternal opresivo 7/10 irradiado a brazo izquierdo, de 18 horas de evolución, episódico en reposo, duración 15-20 min por episodio. Niega disnea o síncope actual. Refiere episodio similar hace 3 semanas con esfuerzo que cedió espontáneamente. Múltiples factores de riesgo cardiovascular: HAS, DM2, tabaquismo 40 paquetes-año, dislipidemia, antecedente familiar de IAM a los 58 años.',
    '[
        {"name": "Dolor torácico", "quality": "Opresivo", "location": "Retroesternal", "radiation": ["Brazo izquierdo"], "severity_scale": 7, "onset": "En reposo", "duration_minutes": 20, "pattern": "Episódico"},
        {"name": "Diaforesis", "severity": "Leve"}
    ]'::jsonb,
    '{
        "primary": "Síndrome coronario agudo sin elevación del ST (SICA SEST) - Angina inestable",
        "icd10": "I20.0",
        "key_findings": [
            "Dolor opresivo en reposo con irradiación típica",
            "Cambios dinámicos de ST sin elevación (depresión 1mm V4-V6 durante dolor)",
            "Troponinas negativas seriales (no IAMSEST)",
            "Múltiples factores de riesgo cardiovascular",
            "Episodio previo con esfuerzo hace 3 semanas (angina de inicio reciente)"
        ],
        "differentials": [
            "IAMCEST - excluido por ausencia de elevación ST persistente",
            "IAMSEST - excluido por troponinas negativas seriales",
            "Disección aórtica - dolor diferente, pulsos simétricos",
            "Espasmo coronario Prinzmetal - menos probable con múltiples FRC",
            "ERGE - sin relación con alimentos, características atípicas"
        ]
    }'::jsonb,
    '[
        {"name": "ECG 12 derivaciones seriado", "category": "diagnostic", "priority": "critical", "required": true, "score_weight": 20, "expected_finding": "Depresión ST dinámica 1mm V4-V6 durante dolor, se normaliza al ceder. Sin elevación. Sin ondas Q."},
        {"name": "Troponina I ultrasensible seriada", "category": "lab", "priority": "critical", "required": true, "score_weight": 15, "time_limit_min": 180, "expected_finding": "0.04 ng/mL basal y 3h - negativa, sin delta significativo"},
        {"name": "CPK-MB", "category": "lab", "priority": "high", "required": true, "score_weight": 5, "expected_finding": "18 U/L - normal"},
        {"name": "Perfil lipídico", "category": "lab", "priority": "high", "required": true, "score_weight": 5, "expected_finding": "LDL 162 mg/dL, CT 238 mg/dL"},
        {"name": "Glucemia", "category": "lab", "priority": "high", "required": true, "score_weight": 3, "expected_finding": "148 mg/dL"},
        {"name": "RX tórax", "category": "imaging", "priority": "high", "required": true, "score_weight": 5, "expected_finding": "Normal, ICT 0.52, sin cardiomegalia"},
        {"name": "Score TIMI o GRACE", "category": "diagnostic", "priority": "high", "required": true, "score_weight": 10, "justification": "Estratificación de riesgo para decidir invasiva vs conservadora"},
        {"name": "Ecocardiograma", "category": "imaging", "priority": "medium", "required": false, "score_weight": 5, "justification": "Función ventricular, motilidad segmentaria"}
    ]'::jsonb,
    '[
        {"order": 1, "phase": "immediate", "intervention": "Monitorización continua ECG + PA + SpO2 + acceso venoso x2", "required": true, "score_weight": 5},
        {"order": 2, "phase": "immediate", "intervention": "Aspirina 300 mg VO masticado (carga), luego 100 mg/día", "required": true, "score_weight": 15, "evidence": "Clase I nivel A"},
        {"order": 3, "phase": "immediate", "intervention": "Clopidogrel 300 mg VO carga (o Ticagrelor 180 mg si disponible)", "required": true, "score_weight": 15, "evidence": "Doble antiagregación Clase I"},
        {"order": 4, "phase": "immediate", "intervention": "Enoxaparina 1 mg/kg SC c/12h", "required": true, "score_weight": 10},
        {"order": 5, "phase": "immediate", "intervention": "Metoprolol 25-50 mg VO (si FC>60 y sin broncoespasmo)", "required": true, "score_weight": 5},
        {"order": 6, "phase": "immediate", "intervention": "Nitroglicerina SL 0.4 mg c/5min hasta 3 dosis PRN dolor", "required": false, "score_weight": 5},
        {"order": 7, "phase": "immediate", "intervention": "Atorvastatina 80 mg/noche (alta intensidad)", "required": true, "score_weight": 5},
        {"order": 8, "phase": "stratification", "intervention": "Calcular score TIMI/GRACE - estratificar alto/bajo riesgo", "required": true, "score_weight": 10},
        {"order": 9, "phase": "invasive", "intervention": "Coronariografía en 24-72h según score de riesgo (estrategia invasiva temprana)", "required": false, "score_weight": 10}
    ]'::jsonb,
    'Seguimiento cardiología en 2 semanas. DAPT (doble antiagregación) por 12 meses si se coloca stent. Control glucémico: HbA1c <7%. Control lipídico: LDL <55 mg/dL. Cese de tabaquismo. Rehabilitación cardíaca. Monitorizar PA meta <130/80 mmHg.',
    '["Distinguir angina inestable de IAMSEST e IAMCEST por ECG y troponinas", "Interpretar cambios dinámicos de ST (depresión sin elevación)", "Aplicar doble antiagregación correctamente en SICA SEST", "Calcular score TIMI/GRACE para estratificación de riesgo", "Indicar estrategia invasiva vs conservadora según riesgo"]'::jsonb,
    'SICA SEST incluye: angina inestable (troponinas negativas) e IAMSEST (troponinas positivas). La diferencia es la troponina, no el ECG. Doble antiagregación siempre: AAS + Clopidogrel/Ticagrelor. Ticagrelor superior a Clopidogrel (estudio PLATO). Score GRACE >140 = alto riesgo → invasiva en <24h. Score GRACE <109 = bajo riesgo → conservador.',
    'Equipo ENARM Simulator',
    true
);

-- ── CASO 005: NAC Grave ──────────────────────────────────────────
INSERT INTO clinical_cases (
    title, slug, chief_complaint, specialty, sub_specialty,
    difficulty, setting, patient_json, history_present_illness,
    symptoms_json, expected_diagnosis_json, expected_studies_json,
    expected_treatments_json, expected_followup,
    learning_objectives_json, teaching_points, author_name, is_published
) VALUES (
    'Neumonía Adquirida en la Comunidad Grave con Criterios de UCI',
    'nac-grave-criterios-uci-neumococo-epoc',
    'Fiebre alta y me cuesta respirar desde hace 4 días',
    'Pulmonology',
    'Neumología crítica',
    'advanced',
    'emergency',
    '{
        "age": 71, "sex": "Female", "weight_kg": 62, "height_cm": 158,
        "full_name": "María Elena Vázquez Ruiz",
        "occupation": "Pensionada, ex-maestra",
        "medical_history": [
            {"condition": "EPOC GOLD II", "years": 5, "treatment": "Tiotropio + Salbutamol PRN"},
            {"condition": "Hipertensión arterial", "treatment": "Enalapril 10 mg/día"}
        ],
        "lifestyle": {"smoker": false, "pack_years": 20, "ex_smoker": true},
        "vital_signs_admission": {
            "systolic_bp": 102, "diastolic_bp": 64, "heart_rate": 118,
            "respiratory_rate": 28, "temperature_c": 39.4, "spo2": 88, "pain_scale": 4
        }
    }'::jsonb,
    'Femenino de 71 años con EPOC GOLD II. Cuatro días de fiebre hasta 40°C, tos productiva con expectoración herrumbrosa, disnea progresiva ahora de reposo. Escalofríos, mialgia, anorexia. En las últimas 12 horas confusión leve referida por familiar. Sin viajes recientes ni hospitalización previa en el año. Sin vacuna de neumococo ni influenza en últimos 5 años. Antígeno de neumococo en orina positivo.',
    '[
        {"name": "Fiebre", "severity": "Severe", "value": "39.4°C"},
        {"name": "Tos productiva", "quality": "Expectoración herrumbrosa", "duration_days": 4},
        {"name": "Disnea", "severity": "Severe", "onset": "Progresiva, ahora de reposo"},
        {"name": "Confusión", "severity": "Mild", "duration_hours": 12},
        {"name": "Escalofríos y mialgia", "severity": "Moderate"}
    ]'::jsonb,
    '{
        "primary": "Neumonía adquirida en la comunidad (NAC) grave por Streptococcus pneumoniae - CURB-65: 4 puntos",
        "icd10": "J13",
        "key_findings": [
            "CURB-65: C=1 (confusión) + U=1 (urea >7 mmol/L) + R=1 (FR≥30) + B=1 (PAS<90 o PAD≤60) + 5=0 = 4 puntos",
            "SpO2 88% con FiO2 0.21 = insuficiencia respiratoria hipoxémica",
            "Antígeno neumococo en orina positivo",
            "Consolidación lobar LID en RX",
            "Procalcitonina 4.8 ng/mL = infección bacteriana grave",
            "EPOC de base: riesgo de hipercapnia con O2 liberal"
        ],
        "differentials": [
            "Agudización de EPOC con neumonía sobreagregada",
            "NAC por Legionella (antígeno negativo en orina)",
            "TEP con infarto pulmonar",
            "Carcinoma broncogénico con neumonía obstructiva"
        ]
    }'::jsonb,
    '[
        {"name": "RX tórax PA", "category": "imaging", "priority": "critical", "required": true, "score_weight": 15, "expected_finding": "Consolidación lobar LID 2/3 campo pulmonar derecho. Hiperinsuflación bilateral (EPOC base)."},
        {"name": "Gasometría arterial", "category": "lab", "priority": "critical", "required": true, "score_weight": 15, "expected_finding": "pH 7.46, PaO2 52 mmHg, PaCO2 32 mmHg, HCO3 22, SaO2 88% (FiO2 0.21) - alcalosis respiratoria por hiperventilación"},
        {"name": "BH con diferencial", "category": "lab", "priority": "critical", "required": true, "score_weight": 5, "expected_finding": "Leucocitos 22,400 con 89% neutrófilos, bandas 12%. Hb 11.8. Plaquetas 142,000"},
        {"name": "PCR y Procalcitonina", "category": "lab", "priority": "high", "required": true, "score_weight": 5, "expected_finding": "PCR 284 mg/L, PCT 4.8 ng/mL"},
        {"name": "Hemocultivos x2", "category": "lab", "priority": "high", "required": true, "score_weight": 10, "justification": "Antes de antibiótico. Identificar bacteriemia"},
        {"name": "Antígeno neumococo en orina", "category": "lab", "priority": "high", "required": true, "score_weight": 5, "expected_finding": "Positivo"},
        {"name": "Antígeno Legionella en orina", "category": "lab", "priority": "high", "required": true, "score_weight": 5, "expected_finding": "Negativo"},
        {"name": "Score CURB-65", "category": "diagnostic", "priority": "critical", "required": true, "score_weight": 10, "expected_finding": "4/5 = alto riesgo, indicación de UCI"},
        {"name": "Lactato sérico", "category": "lab", "priority": "high", "required": true, "score_weight": 5, "expected_finding": "2.8 mmol/L"}
    ]'::jsonb,
    '[
        {"order": 1, "phase": "immediate", "intervention": "Ingreso a UCI por CURB-65 ≥3 y criterios ATS/IDSA de NAC grave", "required": true, "score_weight": 10},
        {"order": 2, "phase": "immediate", "intervention": "O2 mascarilla Venturi 40% meta SpO2 88-92% (EPOC: riesgo hipercapnia con O2 liberal)", "required": true, "score_weight": 15, "warning": "NO dar O2 libre a ciegas en EPOC - puede suprimir drive hipóxico"},
        {"order": 3, "phase": "immediate", "intervention": "Ceftriaxona 2g IV c/24h + Azitromicina 500mg IV c/24h (cobertura dual, antes de 1 hora)", "required": true, "score_weight": 20, "evidence": "ATS/IDSA recomiendan cobertura de atípicos en NAC grave"},
        {"order": 4, "phase": "immediate", "intervention": "Hemocultivos x2 + antígenos en orina ANTES de antibiótico", "required": true, "score_weight": 10},
        {"order": 5, "phase": "supportive", "intervention": "Líquidos IV cristaloides 500 mL en 30 min si MAP <65 o hipotensión", "required": true, "score_weight": 5},
        {"order": 6, "phase": "supportive", "intervention": "Norepinefrina si MAP <65 tras reanimación con fluidos", "required": false, "score_weight": 5},
        {"order": 7, "phase": "supportive", "intervention": "Salbutamol + Ipratropio nebulizados c/4-6h (EPOC de base)", "required": true, "score_weight": 5},
        {"order": 8, "phase": "supportive", "intervention": "Profilaxis TVP: Enoxaparina 40 mg SC/día", "required": true, "score_weight": 5},
        {"order": 9, "phase": "monitoring", "intervention": "Gasometría arterial c/2-4h, monitorización respiratoria continua", "required": true, "score_weight": 5},
        {"order": 10, "phase": "escalation", "intervention": "Si falla ventilación no invasiva: intubación orotraqueal y VM", "required": false, "score_weight": 5}
    ]'::jsonb,
    'Duración antibiótico 5-7 días si respuesta adecuada. Control clínico y radiológico en 4-6 semanas. Vacunación: neumococo + influenza. Optimización de EPOC: tripleagia inhalatoria. Suspender tabaquismo si aplica. Rehabilitación pulmonar.',
    '["Calcular CURB-65 correctamente e identificar criterios de UCI", "Interpretar gasometría arterial en contexto de EPOC + NAC (hipercapnia)", "Seleccionar antibiótico empírico para NAC grave (cobertura dual)", "Manejar O2 en EPOC sin suprimir drive hipóxico (meta SpO2 88-92%)", "Identificar criterios ATS/IDSA de NAC grave"]'::jsonb,
    'CURB-65: ≥3 puntos = UCI. C=confusión, U=urea>7mmol, R=FR≥30, B=PAS<90 o PAD≤60, 65=edad. En EPOC: SpO2 meta 88-92%, NO 95-100% (riesgo hipercapnia y supresión de drive hipóxico). Cobertura dual (betalactámico + macrólido) superior a monoterapia en NAC grave. Tiempo hasta antibiótico <1 hora reduce mortalidad.',
    'Equipo ENARM Simulator',
    true
);

-- ── CASO 006: Pancreatitis Aguda Severa ─────────────────────────
INSERT INTO clinical_cases (
    title, slug, chief_complaint, specialty, sub_specialty,
    difficulty, setting, patient_json, history_present_illness,
    symptoms_json, expected_diagnosis_json, expected_studies_json,
    expected_treatments_json, expected_followup,
    learning_objectives_json, teaching_points, author_name, is_published
) VALUES (
    'Pancreatitis Aguda Severa por Litiasis Biliar',
    'pancreatitis-aguda-severa-litiasis-biliar-balthazar',
    'Dolor de panza que no se me quita y vomité muchas veces',
    'Gastroenterology',
    'Gastroenterología y cirugía hepato-biliar',
    'advanced',
    'emergency',
    '{
        "age": 45, "sex": "Female", "weight_kg": 82, "height_cm": 160,
        "full_name": "Sandra Moreno Jiménez",
        "occupation": "Comerciante",
        "bmi": 32.0,
        "medical_history": [
            {"condition": "Colelitiasis", "years": 2, "notes": "No operada por decisión propia"},
            {"condition": "Obesidad grado I", "notes": "IMC 32"},
            {"condition": "Hipertrigliceridemia", "treatment": "Fibratos"}
        ],
        "lifestyle": {"alcohol_use": false},
        "vital_signs_admission": {
            "systolic_bp": 96, "diastolic_bp": 60, "heart_rate": 124,
            "respiratory_rate": 22, "temperature_c": 38.6, "spo2": 94, "pain_scale": 9
        }
    }'::jsonb,
    'Femenino de 45 años con colelitiasis conocida. 24 horas de dolor epigástrico intenso 9/10, inicio súbito tras cena copiosa, irradiado en cinturón hacia dorso. Náusea y vómito 8-10 veces sin mejoría. Distensión abdominal progresiva. En últimas 6 horas escalofríos y fiebre. Refiere alivio breve hace 12 horas seguido de empeoramiento (posible perforación). No consume alcohol.',
    '[
        {"name": "Dolor epigástrico", "quality": "Intenso, urente", "radiation": ["Dorso en cinturón"], "severity_scale": 9, "onset": "Súbito post-prandial"},
        {"name": "Vómito", "severity": "Severe", "frequency": "8-10 veces sin mejoría"},
        {"name": "Distensión abdominal", "severity": "Moderate", "progressive": true},
        {"name": "Fiebre", "value": "38.6°C", "onset_hours": 6},
        {"name": "Ictericia", "severity": "Mild", "description": "Piel ictérica leve"}
    ]'::jsonb,
    '{
        "primary": "Pancreatitis aguda severa por litiasis biliar (Ranson ≥3, CTSI 7/10, BISAP 2)",
        "icd10": "K85.1",
        "key_findings": [
            "Lipasa 1840 U/L (>3x normal = criterio diagnóstico)",
            "TAC Balthazar D-E con necrosis >30% y colección peripancreática",
            "CTSI (CT Severity Index) 7/10 = pancreatitis severa",
            "Criterios de Ranson al ingreso ≥3 (edad, leucocitos, glucosa, LDH, AST)",
            "Colelitiasis + colédoco dilatado 10mm = etiología biliar probable",
            "Hipocalcemia: calcio 7.8 mg/dL (necrosis grasa peripancreática)"
        ],
        "differentials": [
            "Úlcera péptica perforada - sin neumoperitoneo en RX, lipasa muy elevada",
            "Colangitis aguda - tríada de Charcot pero lipasa diagnóstica de pancreatitis",
            "Isquemia mesentérica aguda - dolor diferente, sin elevación enzimática",
            "CAD con dolor abdominal - glucosa 218 pero cetonas ausentes"
        ]
    }'::jsonb,
    '[
        {"name": "Lipasa sérica", "category": "lab", "priority": "critical", "required": true, "score_weight": 20, "expected_finding": "1840 U/L (>3x normal confirma diagnóstico)"},
        {"name": "Amilasa sérica", "category": "lab", "priority": "high", "required": true, "score_weight": 5, "expected_finding": "920 U/L"},
        {"name": "BH", "category": "lab", "priority": "critical", "required": true, "score_weight": 5, "expected_finding": "Leucocitos 18,600, Hb 15.8 (hemoconcentración), Plaquetas 198,000"},
        {"name": "Calcio sérico", "category": "lab", "priority": "critical", "required": true, "score_weight": 10, "expected_finding": "7.8 mg/dL (hipocalcemia por saponificación)"},
        {"name": "PFH completo", "category": "lab", "priority": "high", "required": true, "score_weight": 5, "expected_finding": "ALT 186, AST 142, BT 3.2 mg/dL, FA 320 - patrón colestásico"},
        {"name": "Gasometría arterial", "category": "lab", "priority": "high", "required": true, "score_weight": 5, "expected_finding": "pH 7.32, PaO2 68, PaCO2 30, HCO3 15 - acidosis metabólica leve"},
        {"name": "BUN, Creatinina", "category": "lab", "priority": "high", "required": true, "score_weight": 5, "expected_finding": "BUN 32, Creatinina 1.8 mg/dL - LRA prerrenal"},
        {"name": "USG abdominal", "category": "imaging", "priority": "critical", "required": true, "score_weight": 10, "expected_finding": "Colelitiasis múltiple, colédoco 10mm dilatado, líquido peripancreático"},
        {"name": "TAC abdomen con contraste", "category": "imaging", "priority": "critical", "required": true, "score_weight": 15, "expected_finding": "Balthazar D-E, necrosis ≥30%, CTSI 7/10", "timing": "A las 48-72h del inicio si no mejora"}
    ]'::jsonb,
    '[
        {"order": 1, "phase": "immediate", "intervention": "Ingreso a UCI o cuidados intermedios", "required": true, "score_weight": 5},
        {"order": 2, "phase": "immediate", "intervention": "NPO absoluto", "required": true, "score_weight": 5},
        {"order": 3, "phase": "fluid_resuscitation", "intervention": "Ringer Lactato 250-500 mL/h primeras 12-24h (preferible sobre SF 0.9%)", "required": true, "score_weight": 20, "evidence": "Ringer Lactato reduce PCR y complicaciones vs SF - AGA 2024"},
        {"order": 4, "phase": "pain", "intervention": "Morfina 0.05 mg/kg IV c/4h PRN o hidromorfona (NO meperidina)", "required": true, "score_weight": 5},
        {"order": 5, "phase": "pain", "intervention": "Ondansetrón 4-8 mg IV c/8h", "required": true, "score_weight": 5},
        {"order": 6, "phase": "monitoring", "intervention": "Diuresis horaria, balance hídrico, PA, FC, BUN/Cr/calcio/glucosa c/12-24h", "required": true, "score_weight": 5},
        {"order": 7, "phase": "biliary", "intervention": "CPRE urgente en primeras 24-72h si coledocolitiasis o colangitis asociada", "required": false, "score_weight": 10},
        {"order": 8, "phase": "antibiotic", "intervention": "NO antibiótico profiláctico en pancreatitis sin evidencia de infección. Solo si necrosis infectada: Imipenem o Meropenem", "required": true, "score_weight": 10},
        {"order": 9, "phase": "nutrition", "intervention": "Nutrición enteral (sonda nasoyeyunal) en primeras 24-48h. Evitar NPT si tolera enteral", "required": true, "score_weight": 10},
        {"order": 10, "phase": "electrolytes", "intervention": "Calcio IV si hipocalcemia sintomática (<7.5 mg/dL)", "required": true, "score_weight": 5},
        {"order": 11, "phase": "surgical", "intervention": "Colecistectomía diferida 6-8 semanas post-resolución del episodio", "required": true, "score_weight": 10}
    ]'::jsonb,
    'Control en 6-8 semanas para colecistectomía electiva. Dieta baja en grasas. Control de triglicéridos. Abstinencia alcohólica. Control de peso. Vigilar complicaciones tardías: pseudoquiste, fístula, estenosis pancreática.',
    '["Diagnosticar pancreatitis aguda con criterios de lipasa ≥3x normal + imagen", "Calcular Ranson/BISAP/CTSI para clasificar severidad", "Indicar Ringer Lactato sobre SF 0.9% para reanimación", "Decidir cuándo indicar antibiótico (solo necrosis infectada documentada)", "Indicar CPRE urgente solo en coledocolitiasis/colangitis asociada"]'::jsonb,
    'Diagnóstico: lipasa >3x normal = suficiente (sin necesidad de amilasa). Severidad: CTSI en TAC (Balthazar + necrosis) mejor predictor que Ranson. Fluidos: Ringer Lactato preferible a SF 0.9% (reduce acidosis hiperclorémica). Antibiótico: NUNCA profiláctico; solo si necrosis infectada (aspiración con aguja fina guiada por TAC o TAC con gas). Nutrición enteral: superior a NPT (preserva barrera intestinal, reduce translocación bacteriana). CPRE: solo si coledocolitiasis o colangitis, NO de rutina.',
    'Equipo ENARM Simulator',
    true
);

-- ── CASO 007: Hemorragia Intracerebral ──────────────────────────
INSERT INTO clinical_cases (
    title, slug, chief_complaint, specialty, sub_specialty,
    difficulty, setting, patient_json, history_present_illness,
    symptoms_json, expected_diagnosis_json, expected_studies_json,
    expected_treatments_json, expected_followup,
    learning_objectives_json, teaching_points, author_name, is_published
) VALUES (
    'Hemorragia Intracerebral Hipertensiva en Ganglio Basal',
    'hemorragia-intracerebral-hipertensiva-ganglio-basal',
    'Mi marido se desplomó y no puede mover el brazo derecho',
    'Neurology',
    'Neurología vascular y urgencias neurológicas',
    'advanced',
    'emergency',
    '{
        "age": 67, "sex": "Male", "weight_kg": 79, "height_cm": 168,
        "full_name": "Ernesto Ríos Contreras",
        "occupation": "Contador jubilado",
        "medical_history": [
            {"condition": "Hipertensión arterial", "years": 15, "treatment": "Amlodipino 5 mg (irregular)", "controlled": false},
            {"condition": "Tabaquismo activo", "notes": "30 paquetes-año"}
        ],
        "vital_signs_admission": {
            "systolic_bp": 210, "diastolic_bp": 118, "heart_rate": 88,
            "respiratory_rate": 20, "temperature_c": 37.1, "spo2": 95,
            "gcs": 12
        }
    }'::jsonb,
    'Masculino de 67 años con HTA mal controlada de 15 años. Estando en reposo presentó súbitamente cefalea 10/10 "la peor de su vida" (thunderclap), caída al piso con pérdida transitoria de alerta (5 minutos), al recuperarse: desviación de comisura labial izquierda, incapacidad para mover brazo y pierna derechos, lenguaje incomprensible. Tiempo de evolución al arribo: 45 minutos. No toma anticoagulantes ni antiagregantes. Sin TCE previo.',
    '[
        {"name": "Cefalea", "quality": "Thunderclap - la peor de su vida", "severity_scale": 10, "onset": "Súbito en reposo"},
        {"name": "Pérdida de conciencia", "duration_minutes": 5, "type": "Transitoria"},
        {"name": "Hemiparesia derecha", "severity": "Severe", "distribution": "Brazo 1/5, pierna 2/5"},
        {"name": "Afasia mixta", "severity": "Severe", "type": "Expresiva y receptiva"},
        {"name": "Desviación de mirada", "direction": "Izquierda", "type": "Conjugada"}
    ]'::jsonb,
    '{
        "primary": "Hemorragia intracerebral espontánea hipertensiva en ganglio basal izquierdo (putamen)",
        "icd10": "I61.0",
        "key_findings": [
            "TAC craneal: Hematoma hiperdenso en putamen izquierdo 3.2x2.8cm (~12 mL)",
            "HTA severa 210/118 como causa probable (localización típica)",
            "GCS 12 - deterioro neurológico moderado",
            "Hemiparesia derecha + afasia mixta = lesión hemisferio izquierdo",
            "INR 1.0 - no anticoagulado",
            "Sin herniación: manejo conservador (hematoma <30 mL)"
        ],
        "differentials": [
            "ACV isquémico - excluido por hiperdensidad espontánea en TAC",
            "Hemorragia subaracnoidea - no sangre en cisternas basales, sin hemorragia perimesencefálica",
            "Tumor cerebral con sangrado intratumoral - sin lesión previa, HTA severa como causa",
            "MAV rota - posible pero menos probable en adulto mayor con HTA severa"
        ]
    }'::jsonb,
    '[
        {"name": "TAC craneal simple URGENTE", "category": "imaging", "priority": "critical", "required": true, "score_weight": 25, "time_limit_min": 25, "expected_finding": "Hematoma hiperdenso putamen izquierdo 3.2x2.8cm 12mL, edema perilesional, DLM 2mm, sin herniación"},
        {"name": "BH, coagulación (TP, TTPa, INR)", "category": "lab", "priority": "critical", "required": true, "score_weight": 10, "expected_finding": "INR 1.0 - normal. Plaquetas 198,000"},
        {"name": "Glucemia", "category": "lab", "priority": "critical", "required": true, "score_weight": 5, "expected_finding": "138 mg/dL"},
        {"name": "ECG", "category": "diagnostic", "priority": "high", "required": true, "score_weight": 5, "expected_finding": "Ritmo sinusal, sin FA"},
        {"name": "Escala NIHSS", "category": "diagnostic", "priority": "high", "required": true, "score_weight": 10, "justification": "Cuantificar déficit neurológico: afasia+hemiparesia = NIHSS estimado 12-15"},
        {"name": "Escala ICH Score", "category": "diagnostic", "priority": "high", "required": true, "score_weight": 10, "justification": "Estratificar mortalidad a 30 días"},
        {"name": "TAC angiografía o RM con contraste", "category": "imaging", "priority": "medium", "required": false, "score_weight": 5, "justification": "Si <45 años, sin HTA clara o localización atípica → buscar MAV/tumor"}
    ]'::jsonb,
    '[
        {"order": 1, "phase": "immediate", "intervention": "ABC: vía aérea, respiración, circulación. Cabecera 30 grados.", "required": true, "score_weight": 5},
        {"order": 2, "phase": "immediate", "intervention": "NO trombolíticos - contraindicados en hemorragia cerebral", "required": true, "score_weight": 15, "warning": "Error crítico dar tPA en HIC"},
        {"order": 3, "phase": "bp_control", "intervention": "Labetalol 20mg IV en 2 min, repetir c/10 min (máx 300 mg). Meta PAS 130-150 mmHg (AHA 2022)", "required": true, "score_weight": 20, "alternatives": "Nicardipino IV 5-15 mg/h. EVITAR Nitroprusiato (aumenta PIC)"},
        {"order": 4, "phase": "neuroprotection", "intervention": "Glucemia meta 140-180 mg/dL (hipo e hiperglucemia empeoran pronóstico)", "required": true, "score_weight": 5},
        {"order": 5, "phase": "neuroprotection", "intervention": "Temperatura meta <37.5°C, antipiréticos PRN", "required": true, "score_weight": 5},
        {"order": 6, "phase": "neurosurgery", "intervention": "Valoración neuroquirúrgica urgente (hematoma 12mL sin herniación = manejo conservador actual)", "required": true, "score_weight": 10, "criteria_surgery": "Hematoma >30mL cerebelar, o >10mL con deterioro progresivo"},
        {"order": 7, "phase": "icu", "intervention": "Ingreso UCI neurológica o Unidad de Ictus", "required": true, "score_weight": 5},
        {"order": 8, "phase": "monitoring", "intervention": "TAC control a las 24h para verificar expansión del hematoma", "required": true, "score_weight": 10},
        {"order": 9, "phase": "anticoagulation", "intervention": "NO revertir coagulación (INR normal). Si anticoagulado: revertir urgente", "required": true, "score_weight": 5},
        {"order": 10, "phase": "rehabilitation", "intervention": "Rehabilitación temprana (fisioterapia, foniatría, terapia ocupacional) cuando estabilizado", "required": true, "score_weight": 5}
    ]'::jsonb,
    'TAC control 24-48h. Rehabilitación multidisciplinaria fase I intrahospitalaria. Iniciar antihipertensivo oral cuando estabilizado. Prevención secundaria: control estricto HTA (<130/80), cese de tabaquismo. Valorar anticoagulación si FA asociada (después de 4-8 semanas). Seguimiento neurología en 4 semanas.',
    '["Distinguir HIC de ACV isquémico por hiperdensidad en TAC sin contraste", "Reconocer cefalea thunderclap como signo de alarma", "Manejar urgencia hipertensiva en HIC (labetalol, meta PAS 130-150)", "Conocer criterios de manejo quirúrgico vs conservador de HIC", "Calcular ICH Score para estratificar mortalidad"]'::jsonb,
    'DIAGNÓSTICO: TAC simple = suficiente para HIC (hiperdensidad espontánea). IAMCEST tiene "man in barrel" = ACA; putamen = HIC hipertensiva. META PA en HIC aguda: PAS 130-150 mmHg (INTERACT2, ATACH-II). EVITAR Nitroprusiato. CIRUGÍA: hematoma cerebelar >3cm, lobar >30mL con deterioro o herniación. Putamen/tálamo <30mL = conservador. PREDICCIÓN: ICH Score (GCS + volumen + intraventricular + infratentorial + edad) = mortalidad 30 días.',
    'Equipo ENARM Simulator',
    true
);

-- ── CASO 008: CAD ────────────────────────────────────────────────
INSERT INTO clinical_cases (
    title, slug, chief_complaint, specialty, sub_specialty,
    difficulty, setting, patient_json, history_present_illness,
    symptoms_json, expected_diagnosis_json, expected_studies_json,
    expected_treatments_json, expected_followup,
    learning_objectives_json, teaching_points, author_name, is_published
) VALUES (
    'Cetoacidosis Diabética Severa como Debut de DM Tipo 1',
    'cetoacidosis-diabetica-severa-debut-dm-tipo1',
    'Mi hijo lleva 3 días muy mal, no come y respira raro',
    'Endocrinology',
    'Endocrinología y Diabetes',
    'intermediate',
    'emergency',
    '{
        "age": 19, "sex": "Male", "weight_kg": 65, "height_cm": 178,
        "full_name": "Diego Fuentes Soto",
        "occupation": "Estudiante universitario",
        "medical_history": [],
        "recent_history": [
            {"symptom": "Pérdida de peso", "amount_kg": 8, "duration_weeks": 4},
            {"symptom": "Poliuria y polidipsia", "duration_weeks": 3},
            {"symptom": "Astenia progresiva"}
        ],
        "vital_signs_admission": {
            "systolic_bp": 104, "diastolic_bp": 68, "heart_rate": 116,
            "respiratory_rate": 30, "temperature_c": 37.2, "spo2": 98,
            "gcs": 13, "pain_scale": 5
        }
    }'::jsonb,
    'Masculino de 19 años sin antecedentes previos. Traído por su madre por 3 días de náusea, vómito múltiple, dolor abdominal difuso, debilidad intensa y confusión progresiva. La madre refiere que respira "muy profundo y rápido" y tiene aliento con olor a fruta (acetona). Antecedente de 3 semanas previas con poliuria, polidipsia intensa y pérdida de 8 kg. Sin traumatismo, sin fiebre. Sin medicamentos previos. Sin familiares con DM tipo 1 conocidos.',
    '[
        {"name": "Náusea y vómito", "duration_days": 3, "frequency": "Múltiple"},
        {"name": "Dolor abdominal", "location": "Difuso", "severity": "Moderate"},
        {"name": "Confusión progresiva", "severity": "Mild"},
        {"name": "Respiración de Kussmaul", "description": "Profunda y rápida FR 30"},
        {"name": "Aliento a acetona", "type": "Signo clínico clave"},
        {"name": "Poliuria y polidipsia", "duration_weeks": 3, "type": "Antecedente"},
        {"name": "Pérdida de peso", "amount_kg": 8, "duration_weeks": 4}
    ]'::jsonb,
    '{
        "primary": "Cetoacidosis Diabética severa (pH <7.2, HCO3 <10 mEq/L, glucosa >500 mg/dL) como debut de Diabetes Mellitus tipo 1",
        "icd10": "E10.10",
        "key_findings": [
            "Triada diagnóstica: hiperglucemia (518 mg/dL) + cetonemia (beta-HB 6.2 mmol/L) + acidosis metabólica AG elevado (AG=26)",
            "pH 7.18 = CAD severa (clasificación: leve pH 7.25-7.30, moderada 7.0-7.24, severa <7.0)",
            "HCO3 8 mEq/L - bicarbonato muy bajo",
            "Potasio 5.8 mEq/L - falsamente elevado (deplección real - vigilar al dar insulina)",
            "Péptido C 0.1 ng/mL + Anti-GAD positivos = confirma DM tipo 1 autoinmune",
            "Anión gap 26 = Na - (Cl+HCO3) = 132-98-8 = 26 (elevado, normal 8-12)"
        ],
        "differentials": [
            "Estado hiperosmolar hiperglucémico (EHH): glucosa >600, osmolalidad >320, SIN cetosis significativa, pH normal",
            "Acidosis láctica: sin causa de hipoperfusión, cetonas ausentes",
            "Intoxicación por salicilatos: AG elevado pero historia negativa",
            "Acidosis urémica: creatinina muy elevada (1.4 mg/dL es mínimamente elevada)"
        ]
    }'::jsonb,
    '[
        {"name": "Glucemia capilar y venosa", "category": "lab", "priority": "critical", "required": true, "score_weight": 10, "expected_finding": "524 mg/dL capilar, 518 mg/dL venosa"},
        {"name": "Gasometría venosa o arterial", "category": "lab", "priority": "critical", "required": true, "score_weight": 20, "expected_finding": "pH 7.18, HCO3 8 mEq/L, PCO2 22 mmHg (compensación respiratoria)"},
        {"name": "Electrolitos: Na, K, Cl, HCO3", "category": "lab", "priority": "critical", "required": true, "score_weight": 15, "expected_finding": "Na 132 (corregido 138.7), K 5.8 (falso), Cl 98, AG=26"},
        {"name": "Cetonas en orina (tira reactiva)", "category": "lab", "priority": "critical", "required": true, "score_weight": 10, "expected_finding": "4+ (strongly positive)"},
        {"name": "Beta-hidroxibutirato sérico", "category": "lab", "priority": "high", "required": true, "score_weight": 5, "expected_finding": "6.2 mmol/L (normal <0.6)"},
        {"name": "BUN, Creatinina", "category": "lab", "priority": "high", "required": true, "score_weight": 5, "expected_finding": "BUN 28, Cr 1.4 mg/dL"},
        {"name": "HbA1c", "category": "lab", "priority": "high", "required": true, "score_weight": 5, "expected_finding": "13.8% - descontrol severo previo"},
        {"name": "Péptido C e Insulina basal", "category": "lab", "priority": "medium", "required": true, "score_weight": 5, "expected_finding": "Péptido C 0.1, Insulina 2 mU/L - confirma DM tipo 1"},
        {"name": "Anti-GAD, Anti-IA2", "category": "lab", "priority": "medium", "required": false, "score_weight": 5, "expected_finding": "Anti-GAD positivos = etiología autoinmune confirmada"},
        {"name": "ECG", "category": "diagnostic", "priority": "high", "required": true, "score_weight": 5, "expected_finding": "Taquicardia sinusal. Sin cambios por hiperpotasemia (K 5.8 no crítico aún)"},
        {"name": "Na corregido", "category": "diagnostic", "priority": "high", "required": true, "score_weight": 5, "expected_finding": "Na corregido = 132 + 1.6×(518-100)/100 = 132 + 6.7 = 138.7 mEq/L - normal"}
    ]'::jsonb,
    '[
        {"order": 1, "phase": "fluids_phase1", "intervention": "SF 0.9% 1000 mL en 1 hora (15-20 mL/kg). Corregir deshidratación antes de insulina.", "required": true, "score_weight": 15},
        {"order": 2, "phase": "potassium_check", "intervention": "VERIFICAR K+ antes de insulina. NO iniciar insulina si K+ <3.5 mEq/L (riesgo arritmia)", "required": true, "score_weight": 15, "warning": "K 5.8 = puede dar insulina. Si K<3.5: dar KCl IV primero."},
        {"order": 3, "phase": "insulin", "intervention": "Insulina regular en infusión: 0.1 U/kg/h (NO bolo inicial en CAD). Meta: reducir glucosa 50-70 mg/dL/hora.", "required": true, "score_weight": 15, "evidence": "ADA 2024: bolo no mejora outcomes"},
        {"order": 4, "phase": "fluids_phase2", "intervention": "SF 0.9% o 0.45% 250-500 mL/h según Na corregido. Cuando glucosa <200: cambiar a Dextrosa 5% + SF 0.45%", "required": true, "score_weight": 10},
        {"order": 5, "phase": "potassium_replacement", "intervention": "Agregar KCl 20-40 mEq/L a sueros (meta K+ 4.0-5.0). Monitorizar ECG si K<3 o >6", "required": true, "score_weight": 10},
        {"order": 6, "phase": "bicarbonate", "intervention": "NO bicarbonato (pH 7.18 ≥ 6.9). Solo si pH <6.9: NaHCO3 100 mEq en 2h", "required": true, "score_weight": 10, "warning": "Error frecuente: dar bicarbonato con pH 7.18. Aumenta riesgo edema cerebral"},
        {"order": 7, "phase": "monitoring", "intervention": "Glucemia capilar c/1h, Gasometría c/2-4h, Electrolitos c/2-4h, Balance hídrico estricto", "required": true, "score_weight": 5},
        {"order": 8, "phase": "resolution", "intervention": "Criterios de resolución: Glucosa <200 + HCO3 ≥15 + pH >7.3 + AG normal (<12)", "required": true, "score_weight": 5},
        {"order": 9, "phase": "transition", "intervention": "Iniciar insulina basal SC 30-60 min ANTES de suspender infusión IV (evitar rebote)", "required": true, "score_weight": 5},
        {"order": 10, "phase": "education", "intervention": "Educación diabetológica, seguimiento endocrinología, plan de insulinoterapia a largo plazo", "required": true, "score_weight": 5}
    ]'::jsonb,
    'Seguimiento endocrinología en 1-2 semanas. Insulinoterapia intensiva: basal + bolos (análogos). Automonitoreo glucémico. Identificar factores precipitantes. Educación sobre síntomas de hipo e hiperglucemia. Carnet diabético. Objetivos: HbA1c <7%, glucemia 80-130 mg/dL preprandial.',
    '["Diagnosticar CAD con triada: hiperglucemia + cetonemia + acidosis metabólica con AG elevado", "Calcular anión gap y Na corregido en hiperosmolaridad", "Iniciar secuencia correcta: fluidos → verificar K+ → insulina", "Conocer cuándo NO usar bicarbonato en CAD (pH ≥6.9)", "Distinguir CAD de estado hiperosmolar hiperglucémico (EHH)"]'::jsonb,
    'TRIADA CAD: glucosa>250 + cetonas+ + acidosis (pH<7.3, HCO3<15) + AG elevado. SECUENCIA: fluidos PRIMERO → verificar K+ → insulina (nunca bolo). K+ en CAD: siempre bajo a pesar de valor aparentemente elevado (acidosis saca K al extracelular). BICARBONATO: solo pH<6.9. Edema cerebral con bicarbonato. RESOLUCIÓN: glucosa<200 + HCO3≥15 + pH>7.3 (NO esperar glucosa normal para suspender insulina IV). TRANSICIÓN: insulina basal SC 30-60min antes de suspender infusión. CAD vs EHH: CAD=cetosis+acidosis (glucosa 250-600), EHH=sin cetosis+hiperosmolaridad (glucosa>600, osm>320).',
    'Equipo ENARM Simulator',
    true
);

-- ── CASO 009: Apendicitis Complicada ────────────────────────────
INSERT INTO clinical_cases (
    title, slug, chief_complaint, specialty, sub_specialty,
    difficulty, setting, patient_json, history_present_illness,
    symptoms_json, expected_diagnosis_json, expected_studies_json,
    expected_treatments_json, expected_followup,
    learning_objectives_json, teaching_points, author_name, is_published
) VALUES (
    'Apendicitis Aguda Complicada con Perforación y Absceso Periapendicular',
    'apendicitis-aguda-complicada-perforacion-absceso',
    'Dolor en el lado derecho del abdomen que empeoró mucho hoy',
    'Surgery',
    'Cirugía General y de Urgencias',
    'intermediate',
    'emergency',
    '{
        "age": 28, "sex": "Female", "weight_kg": 58, "height_cm": 162,
        "full_name": "Karla Sánchez Romo",
        "occupation": "Secretaria",
        "allergies": ["Penicilina (rash cutáneo)"],
        "gynecological": {"g": 1, "p": 1, "lmp_days_ago": 10},
        "medical_history": [],
        "vital_signs_admission": {
            "systolic_bp": 112, "diastolic_bp": 72, "heart_rate": 108,
            "respiratory_rate": 20, "temperature_c": 38.8, "spo2": 98, "pain_scale": 8
        }
    }'::jsonb,
    'Femenino de 28 años sin antecedentes relevantes. 72 horas de evolución: inició con dolor periumbilical hace 3 días, migró a fosa ilíaca derecha hace 48 horas, intensidad progresiva ahora 8/10. Náusea, vómito ×3, anorexia desde el inicio. Fiebre 38.8°C desde hace 24 horas. Diarrea líquida ×2. Refiere que el dolor alivió brevemente hace 12 horas y luego se incrementó nuevamente (signo sugestivo de perforación). Beta-hCG sérica negativa. Alergia a penicilina (rash cutáneo).',
    '[
        {"name": "Dolor abdominal", "location": "Periumbilical → FID", "migration": true, "severity_scale": 8, "duration_hours": 72},
        {"name": "Náusea y vómito", "frequency": "×3", "onset": "Inicio del cuadro"},
        {"name": "Anorexia", "duration_hours": 72},
        {"name": "Fiebre", "value": "38.8°C", "duration_hours": 24},
        {"name": "Alivio transitorio", "onset_hours_ago": 12, "interpretation": "Posible perforación con descompresión momentánea"},
        {"name": "Diarrea", "frequency": "×2", "type": "Líquida"}
    ]'::jsonb,
    '{
        "primary": "Apendicitis aguda complicada - Perforación con peritonitis localizada y absceso periapendicular (Score de Alvarado 9/10)",
        "icd10": "K35.2",
        "key_findings": [
            "Score de Alvarado 9/10: migración (2) + anorexia (1) + náusea (1) + dolor FID (2) + rebote (1) + temperatura >37.3°C (1) + leucocitosis (2) = 9",
            "TAC: apéndice 12mm, colección 2cm periapendicular (absceso)",
            "Alivio transitorio hace 12h = signo de perforación (descompresión)",
            "Signos físicos: Blumberg+, Rovsing+, Psoas+, Obturador+",
            "Beta-hCG negativa: descarta embarazo ectópico y EPI",
            "Alergia a penicilina: usar alternativa (ciprofloxacino + metronidazol)"
        ],
        "differentials": [
            "Enfermedad pélvica inflamatoria (EPI) - descartada: beta-hCG negativa, TAC sin salpingitis",
            "Quiste ovárico complicado - descartado por imagen",
            "Diverticulitis de Meckel - raro en adultos, localización similar",
            "Ileítis terminal (Crohn) - más crónica, diarrea prolongada"
        ]
    }'::jsonb,
    '[
        {"name": "Score de Alvarado", "category": "diagnostic", "priority": "critical", "required": true, "score_weight": 15, "expected_finding": "9/10 = muy alta probabilidad de apendicitis"},
        {"name": "BH con diferencial", "category": "lab", "priority": "critical", "required": true, "score_weight": 10, "expected_finding": "Leucocitos 18,200 con 91% neutrófilos, bandas 8%"},
        {"name": "PCR y Procalcitonina", "category": "lab", "priority": "high", "required": true, "score_weight": 5, "expected_finding": "PCR 168 mg/L, PCT 1.8 ng/mL"},
        {"name": "Beta-hCG sérica", "category": "lab", "priority": "critical", "required": true, "score_weight": 10, "expected_finding": "Negativa - descarta embarazo ectópico"},
        {"name": "Urianálisis", "category": "lab", "priority": "high", "required": true, "score_weight": 5, "expected_finding": "Normal - descarta ITU y cólico ureteral"},
        {"name": "USG abdominopélvico", "category": "imaging", "priority": "high", "required": true, "score_weight": 10, "expected_finding": "Apéndice no visualizable por gas. Líquido libre periapendicular en FID."},
        {"name": "TAC abdomen y pelvis con contraste", "category": "imaging", "priority": "critical", "required": true, "score_weight": 15, "expected_finding": "Apéndice 12mm, stranding periapendicular, colección 2cm en FID (absceso). Sin neumoperitoneo franco."}
    ]'::jsonb,
    '[
        {"order": 1, "phase": "initial", "intervention": "NPO + hidratación IV + analgesia: Ketorolaco 30mg IV c/8h + Tramadol 100mg IV PRN", "required": true, "score_weight": 5},
        {"order": 2, "phase": "antibiotics", "intervention": "Ciprofloxacino 400mg IV c/12h + Metronidazol 500mg IV c/8h (ALERGIA A PENICILINA)", "required": true, "score_weight": 20, "note": "Si NO alergia: Ampicilina-sulbactam o Piperacilina-tazobactam. En alergia: Ciprofloxacino+Metronidazol o Clindamicina+Amikacina"},
        {"order": 3, "phase": "surgery", "intervention": "Apendicectomía laparoscópica urgente + lavado de cavidad + drenaje periapendicular", "required": true, "score_weight": 25, "note": "Laparoscópica preferida incluso en perforada si cirujano con experiencia"},
        {"order": 4, "phase": "intraoperative", "intervention": "Cultivo intraoperatorio del líquido periapendicular", "required": true, "score_weight": 5},
        {"order": 5, "phase": "postop_antibiotics", "intervention": "Continuar antibiótico 4-7 días postoperatorio según hallazgos intraoperatorios", "required": true, "score_weight": 10},
        {"order": 6, "phase": "alternative", "intervention": "Si absceso >3cm sin peritonitis franca: drenaje percutáneo + ATB + apendicectomía de intervalo (6-8 semanas)", "required": false, "score_weight": 10, "note": "Decisión individualizada según estado del paciente"}
    ]'::jsonb,
    'Cita cirugía en 2 semanas. Si drenaje percutáneo: apendicectomía de intervalo en 6-8 semanas. Histopatología del apéndice. Vigilar absceso residual. Antibiótico oral de completamiento según cultivo.',
    '["Reconocer signos físicos de apendicitis: Blumberg, Rovsing, Psoas, Obturador", "Calcular e interpretar Score de Alvarado correctamente", "Distinguir apendicitis simple de complicada (perforada, absceso, peritonitis)", "Seleccionar antibiótico adecuado en alergia a penicilina", "Conocer cuándo optar por apendicectomía de intervalo vs cirugía inmediata"]'::jsonb,
    'ALVARADO: Migración(2)+Anorexia(1)+Náusea/Vómito(1)+Dolor FID(2)+Rebote(1)+Fiebre(1)+Leucocitosis(2)=10. ≥7=alta probabilidad quirúrgica. PERFORACIÓN: alivio transitorio del dolor = signo clásico (descompresión por perforación). ANTIBIÓTICO en ALERGIA Penicilina: Ciprofloxacino+Metronidazol O Clindamicina+Gentamicina. NO ampicilina, NO amoxicilina. LAPAROSCOPIA: preferida incluso en perforada (menos dolor, recuperación rápida, menos complicaciones). ABSCESO >3cm maduro: drenaje percutáneo + ATB + apendicectomía de intervalo (6-8 sem). BETA-hCG: SIEMPRE en mujer en edad fértil antes de cirugía.',
    'Equipo ENARM Simulator',
    true
);

-- ── CASO 010: Preeclampsia + HELLP ──────────────────────────────
INSERT INTO clinical_cases (
    title, slug, chief_complaint, specialty, sub_specialty,
    difficulty, setting, patient_json, history_present_illness,
    symptoms_json, expected_diagnosis_json, expected_studies_json,
    expected_treatments_json, expected_followup,
    learning_objectives_json, teaching_points, author_name, is_published
) VALUES (
    'Preeclampsia con Criterios de Severidad y Síndrome HELLP a las 34 Semanas',
    'preeclampsia-severidad-hellp-34-semanas',
    'Dolor de cabeza muy fuerte y veo borroso, estoy embarazada',
    'Obstetrics and Gynecology',
    'Medicina materno-fetal y urgencias obstétricas',
    'advanced',
    'emergency',
    '{
        "age": 32, "sex": "Female", "weight_kg": 78, "height_cm": 163,
        "full_name": "Lucía Torres Vargas",
        "occupation": "Contadora",
        "obstetric": {
            "gestational_weeks": 34.2,
            "gravida": 2,
            "para": 1,
            "prior_births": [{"type": "vaginal", "complications": "none", "years_ago": 4}],
            "prenatal_visits": 3,
            "first_trimester_bp": "110/70"
        },
        "medical_history": [],
        "vital_signs_admission": {
            "systolic_bp": 168, "diastolic_bp": 112, "heart_rate": 96,
            "respiratory_rate": 18, "temperature_c": 37.0, "spo2": 97
        }
    }'::jsonb,
    'Femenino de 32 años, 34.2 semanas de gestación (G2P1). Control prenatal irregular (solo 3 consultas). TA basal normotensa en primer trimestre (110/70 mmHg). Acude a urgencias por cefalea frontal intensa 8/10 de 6 horas de evolución, visión borrosa y fotofobia. Edema de cara y manos notado hace 3 días. Epigastralgia moderada. Movimientos fetales presentes. PA medida por familiar en esta semana: 160/105 mmHg. Niega convulsiones. Sin sangrado vaginal.',
    '[
        {"name": "Cefalea", "location": "Frontal", "severity_scale": 8, "duration_hours": 6, "type": "Síntoma de severidad en preeclampsia"},
        {"name": "Visión borrosa y fotofobia", "severity": "Moderate", "type": "Síntoma de severidad"},
        {"name": "Edema", "location": "Cara y manos", "duration_days": 3, "severity": "Moderate"},
        {"name": "Epigastralgia", "severity": "Moderate", "type": "Distensión capsular hepática - criterio HELLP"}
    ]'::jsonb,
    '{
        "primary": "Preeclampsia con criterios de severidad complicada con Síndrome HELLP completo (Hemólisis + Enzimas hepáticas elevadas + Plaquetas bajas)",
        "icd10": "O14.12",
        "key_findings": [
            "Criterios de severidad: PAS≥160 o PAD≥110 (168/112), proteinuria masiva (4.8 g/24h), síntomas cerebrales (cefalea, visión borrosa), epigastralgia",
            "HELLP completo: Hemólisis (esquistocitos, LDH 820, Hb baja) + ALT/AST >2x normal (186/210) + Plaquetas <100,000 (89,000)",
            "RCIU: percentil 8 para EG = restricción severa del crecimiento",
            "Oligohidramnios: ILA 5.8 cm",
            "Doppler umbilical: flujo diastólico ausente = signo de alarma fetal"
        ],
        "differentials": [
            "Hipertensión gestacional - sin proteinuria (descartada: proteinuria 4.8 g/24h)",
            "HTA crónica con preeclampsia sobreagregada - TA basal normal en 1er trimestre",
            "Púrpura trombocitopénica trombótica (PTT) - similar pero sin proteinuria masiva ni HTA tan severa",
            "Hígado graso agudo del embarazo - hipoglucemia severa, coagulopatía marcada (aquí TP/TTP normales)"
        ]
    }'::jsonb,
    '[
        {"name": "TA seriada cada 15 min", "category": "diagnostic", "priority": "critical", "required": true, "score_weight": 5, "expected_finding": "168/112 mmHg = severidad"},
        {"name": "Proteinuria en orina de 24h o cociente P/Cr", "category": "lab", "priority": "critical", "required": true, "score_weight": 10, "expected_finding": "4.8 g/24h (rango nefrótico)"},
        {"name": "BH con frotis periférico", "category": "lab", "priority": "critical", "required": true, "score_weight": 10, "expected_finding": "Hb 11.2, Plaquetas 89,000, esquistocitos presentes (hemólisis microangiopática)"},
        {"name": "PFH: ALT, AST, LDH, Bilirrubinas", "category": "lab", "priority": "critical", "required": true, "score_weight": 10, "expected_finding": "ALT 186, AST 210 (>2x normal), LDH 820, BT 1.8 - HELLP"},
        {"name": "Creatinina y ácido úrico", "category": "lab", "priority": "high", "required": true, "score_weight": 5, "expected_finding": "Cr 1.2 mg/dL (elevada: basal 0.6), AU 7.8 mg/dL"},
        {"name": "TP, TTPa, fibrinógeno", "category": "lab", "priority": "high", "required": true, "score_weight": 5, "expected_finding": "Normales (descartar CID)"},
        {"name": "USG obstétrico con Doppler", "category": "imaging", "priority": "critical", "required": true, "score_weight": 10, "expected_finding": "RCIU percentil 8, ILA 5.8, flujo diastólico umbilical ausente"},
        {"name": "NST (cardiotocografía)", "category": "diagnostic", "priority": "critical", "required": true, "score_weight": 5, "expected_finding": "Reactivo, sin desaceleraciones (bienestar fetal momentáneo)"},
        {"name": "Fondo de ojo", "category": "diagnostic", "priority": "high", "required": true, "score_weight": 5, "expected_finding": "Edema de papila incipiente, espasmo arteriolar - hallazgo de severidad"},
        {"name": "Glucemia", "category": "lab", "priority": "high", "required": true, "score_weight": 5, "expected_finding": "92 mg/dL normal (descarta hígado graso agudo del embarazo)"}
    ]'::jsonb,
    '[
        {"order": 1, "phase": "immediate", "intervention": "UCI obstétrica / área de alto riesgo - monitorización continua materna y fetal", "required": true, "score_weight": 5},
        {"order": 2, "phase": "magnesium", "intervention": "Sulfato de magnesio: CARGA 4-6g IV en 15-20 min → MANTENIMIENTO 1-2 g/h infusión continua", "required": true, "score_weight": 20, "monitoring": "Diuresis >25mL/h, FR >12/min, reflejos presentes. ANTÍDOTO: Gluconato calcio 1g IV listo"},
        {"order": 3, "phase": "bp_control", "intervention": "Labetalol 20mg IV, repetir c/10 min (máx 300 mg). Meta PA 140-150/90-100 mmHg", "required": true, "score_weight": 15, "alternatives": "Hidralazina 5-10 mg IV c/20 min. Nifedipino 10 mg VO.", "contraindications": "IECAS y ARA-II contraindicados en embarazo (fetotoxicidad)"},
        {"order": 4, "phase": "fetal_lung_maturity", "intervention": "Betametasona 12 mg IM c/24h ×2 dosis (maduración pulmonar fetal a 34 SDG)", "required": true, "score_weight": 10, "evidence": "Clase I aunque beneficio marginal a 34 SDG, se indica"},
        {"order": 5, "phase": "delivery", "intervention": "INTERRUPCIÓN DEL EMBARAZO - indicación absoluta por HELLP completo. Cesárea urgente.", "required": true, "score_weight": 20, "justification": "HELLP + plaquetas <100,000 = mayor riesgo de sangrado en parto vaginal"},
        {"order": 6, "phase": "transfusion", "intervention": "Transfusión de plaquetas si <50,000/mm³ previo a cesárea", "required": false, "score_weight": 5},
        {"order": 7, "phase": "postpartum", "intervention": "Continuar MgSO4 24-48h postparto (riesgo de eclampsia persiste 24-48h posparto)", "required": true, "score_weight": 10},
        {"order": 8, "phase": "postpartum", "intervention": "Antihipertensivos VO: Nifedipino de liberación prolongada. Meta PA <150/100 postparto", "required": true, "score_weight": 5}
    ]'::jsonb,
    'Vigilancia estrecha plaquetas y enzimas hepáticas postparto (pueden deteriorarse en primeras 24-48h). Lactancia materna según medicamentos. Seguimiento TA: resolución en 6-12 semanas. Asesoría sobre riesgo en próximos embarazos (20-25% recurrencia). Aspirina 100-150 mg/día desde semana 12 en próximo embarazo (prevención preeclampsia).',
    '["Identificar criterios de severidad en preeclampsia (PA ≥160/110, proteinuria masiva, síntomas)", "Diagnosticar síndrome HELLP completo por laboratorio (hemólisis+LDH+plaquetas+enzimas)", "Administrar sulfato de magnesio correctamente (dosis carga/mantenimiento, monitorización, antídoto)", "Conocer antihipertensivos seguros y contraindicados en embarazo", "Decidir vía e indicación de interrupción del embarazo en preeclampsia severa + HELLP"]'::jsonb,
    'PREECLAMPSIA SEVERA: PA ≥160/110 en ≥2 ocasiones con 4h de diferencia, OR síntomas severos (cefalea, visión, epigastralgia) OR proteinuria ≥5g/24h OR plaquetas<100K OR creatinina>1.1 OR enzimas hepáticas elevadas. HELLP: H=Hemólisis (esquistocitos+LDH>600+BT>1.2) + EL=Enzimas hepáticas (ALT/AST>2x) + LP=Plaquetas<100K. MgSO4: anticonvulsivante de ELECCIÓN. MONITORIZAR: FR>12, diuresis>25mL/h, reflejos presentes. TOXICIDAD: FR<12, pérdida de reflejos → Gluconato calcio 1g IV. ANTIHIPERTENSIVOS SEGUROS: Labetalol, Hidralazina, Nifedipino, Metildopa. CONTRAINDICADOS: IECAS, ARA-II (fetotóxicos). INTERRUPCIÓN: HELLP completo = indicación absoluta independiente de EG.',
    'Equipo ENARM Simulator',
    true
);

-- ── Verificación final ───────────────────────────────────────────
SELECT 
    id,
    LEFT(title, 55) AS titulo,
    specialty,
    difficulty,
    setting,
    is_published
FROM clinical_cases 
ORDER BY created_at
LIMIT 20;

SELECT 
    specialty,
    difficulty,
    COUNT(*) as casos
FROM clinical_cases
GROUP BY specialty, difficulty
ORDER BY specialty, difficulty;

SELECT COUNT(*) AS total_casos FROM clinical_cases;

COMMIT;
