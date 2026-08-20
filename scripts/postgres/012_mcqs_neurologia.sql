-- ================================================================
-- 012_mcqs_neurologia.sql
-- Sprint 7A.3: 10 MCQs de Neurologia estilo ENARM
-- ================================================================

BEGIN;

-- ---------- MCQ 1: ACV isquemico ventana trombolisis ----------
INSERT INTO mcq_bank (specialty, difficulty, topic, vignette, question, options_json, correct_answer, explanation_correct, guidelines_ids, is_active)
VALUES ('Neurologia', 'intermediate', 'ACV isquemico',
'Hombre 68 anos, hipertenso, presenta hemiparesia derecha, afasia global de inicio hace 2 horas exactas. TA 175/95, glucemia 118. TAC craneo sin hemorragia. NIHSS 14.',
'Cual es el tratamiento MAS APROPIADO?',
'{"A":"Alteplase IV 0.9 mg/kg (max 90mg), 10% en bolo + 90% infusion 1h","B":"Aspirina 300mg VO","C":"Heparina IV en bolo","D":"Esperar TAC de control en 24h"}'::jsonb,
'A',
'La respuesta correcta es A. Ventana para trombolisis IV con alteplase es de 4.5 HORAS desde el inicio. Este paciente esta en 2h, sin contraindicaciones (TA menor a 185/110, glucemia >50, sin sangrado, NIHSS 4-25). Dosis: 0.9 mg/kg (max 90mg), 10% en bolo + 90% en infusion 1h. Aspirina (B) se inicia 24h post-trombolisis. Heparina (C) esta CONTRAINDICADA en fase aguda. Esperar (D) pierde ventana terapeutica.',
ARRAY['IMSS-102-08']::text[], TRUE)
ON CONFLICT DO NOTHING;

-- ---------- MCQ 2: Estatus epileptico ----------
INSERT INTO mcq_bank (specialty, difficulty, topic, vignette, question, options_json, correct_answer, explanation_correct, guidelines_ids, is_active)
VALUES ('Neurologia', 'advanced', 'Estatus epileptico',
'Mujer 45 anos con epilepsia, convulsiones tonico-clonicas continuas de 8 minutos sin recuperar conciencia entre crisis. Acceso IV colocado. Glucosa 92.',
'Cual es el tratamiento de PRIMERA linea?',
'{"A":"Fenitoina IV 20 mg/kg","B":"Lorazepam 4mg IV o diazepam 10mg IV","C":"Levetiracetam 60 mg/kg IV","D":"Propofol en infusion"}'::jsonb,
'B',
'La respuesta correcta es B. En estatus epileptico (crisis mayor a 5 min o crisis recurrentes sin recuperacion) la PRIMERA linea son BENZODIACEPINAS: lorazepam 0.1 mg/kg IV (max 4mg) o diazepam 0.15 mg/kg IV (max 10mg) o midazolam 10mg IM si no hay via IV. Si persiste 5 min mas: 2a linea fenitoina, valproato o levetiracetam. 3a linea: anestesia general con propofol/midazolam en UCI. Fenitoina sola (A) es 2a linea, mas lenta.',
ARRAY['IMSS-244-10']::text[], TRUE)
ON CONFLICT DO NOTHING;

-- ---------- MCQ 3: Cefalea primaria ----------
INSERT INTO mcq_bank (specialty, difficulty, topic, vignette, question, options_json, correct_answer, explanation_correct, guidelines_ids, is_active)
VALUES ('Neurologia', 'intermediate', 'Migrana',
'Mujer 32 anos con cefalea pulsatil unilateral izquierda, moderada-severa, 8 horas de duracion, con fotofobia, fonofobia y nausea. Empeora con actividad. 3er episodio este mes. Exploracion neurologica normal.',
'Cual es el tratamiento MAS APROPIADO?',
'{"A":"Paracetamol 500mg VO","B":"Sumatriptan 50-100mg VO + AINE (naproxeno)","C":"Morfina IV","D":"Diazepam VO"}'::jsonb,
'B',
'La respuesta correcta es B. Migrana moderada-severa se trata con TRIPTANES (sumatriptan, rizatriptan, zolmitriptan) idealmente en primeras 2h de la crisis, mas efectivo con AINE (naproxeno 500mg). Paracetamol (A) es para dolor leve. Opioides (C) NO se recomiendan por riesgo de dependencia y cefalea de rebote. Diazepam (D) no tiene rol. Profilaxis (si >4 crisis/mes): propranolol, topiramato, amitriptilina.',
ARRAY['IMSS-105-08']::text[], TRUE)
ON CONFLICT DO NOTHING;

-- ---------- MCQ 4: Guillain-Barre ----------
INSERT INTO mcq_bank (specialty, difficulty, topic, vignette, question, options_json, correct_answer, explanation_correct, guidelines_ids, is_active)
VALUES ('Neurologia', 'advanced', 'GBS',
'Hombre 40 anos con debilidad ASCENDENTE simetrica de 5 dias post-diarrea. Arreflexia, sin nivel sensitivo, funciones cognitivas normales. LCR: proteinas 120 (disociacion albumino-citologica), 2 celulas. Capacidad vital 40%.',
'Cual es el tratamiento?',
'{"A":"Corticoides IV altas dosis","B":"Inmunoglobulina IV 0.4 g/kg/dia por 5 dias O plasmaferesis","C":"Solo observacion","D":"Piridostigmina VO"}'::jsonb,
'B',
'La respuesta correcta es B. Sindrome de Guillain-Barre requiere INMUNOTERAPIA con IGIV 0.4 g/kg/dia x 5 dias O plasmaferesis (5 sesiones). Ambos igual eficacia. Corticoides (A) NO son efectivos en GBS. Observacion (C) es peligrosa: 25% requieren ventilacion. VIGILAR CAPACIDAD VITAL, si menor a 20 ml/kg = INTUBACION. Piridostigmina (D) es para miastenia gravis (dx diferencial).',
ARRAY['IMSS-089-08']::text[], TRUE)
ON CONFLICT DO NOTHING;

-- ---------- MCQ 5: Parkinson tratamiento inicial ----------
INSERT INTO mcq_bank (specialty, difficulty, topic, vignette, question, options_json, correct_answer, explanation_correct, guidelines_ids, is_active)
VALUES ('Neurologia', 'intermediate', 'Parkinson',
'Hombre 62 anos con temblor de reposo mano derecha, bradicinesia, rigidez, marcha lenta desde hace 1 ano. Diagnostico de enfermedad de Parkinson temprano. Independiente para AVD.',
'Cual es el tratamiento inicial mas apropiado?',
'{"A":"Levodopa/carbidopa 250/25 cada 6h","B":"Agonista dopaminergico (pramipexol o rotigotina) o IMAO-B (rasagilina) segun edad y sintomas","C":"Solo terapia fisica","D":"Toxina botulinica"}'::jsonb,
'B',
'La respuesta correcta es B. En Parkinson TEMPRANO, la eleccion depende de EDAD: <65 anos con sintomas leves-moderados se prefieren AGONISTAS DOPAMINERGICOS (pramipexol, rotigotina) o IMAO-B (rasagilina) para RETRASAR complicaciones motoras de levodopa (disquinesias). Levodopa (A) reservada para >70 anos o discapacidad significativa (es la mas efectiva pero causa complicaciones a largo plazo). Fisioterapia (C) es adyuvante.',
ARRAY['IMSS-305-10']::text[], TRUE)
ON CONFLICT DO NOTHING;

-- ---------- MCQ 6: Hemorragia subaracnoidea ----------
INSERT INTO mcq_bank (specialty, difficulty, topic, vignette, question, options_json, correct_answer, explanation_correct, guidelines_ids, is_active)
VALUES ('Neurologia', 'advanced', 'HSA',
'Mujer 55 anos con cefalea EXPLOSIVA "la peor de mi vida", vomito, rigidez de nuca. TA 180/100. TAC craneo: sangre en cisternas basales y valle silviano.',
'Cual es la conducta INMEDIATA?',
'{"A":"AngioTAC o panangiografia + neurocirugia + nimodipino 60mg VO cada 4h","B":"Trombolisis IV","C":"Puncion lumbar diagnostica","D":"Corticoides IV"}'::jsonb,
'A',
'La respuesta correcta es A. HSA por aneurisma requiere: 1) IDENTIFICAR aneurisma con angioTAC o panangiografia, 2) TRATAMIENTO ANEURISMA (clipaje quirurgico o embolizacion endovascular con coils) en las primeras 24-72h, 3) NIMODIPINO 60mg VO cada 4h por 21 dias para prevenir VASOESPASMO (la principal causa de morbimortalidad tardia), 4) Control TA (meta sistolica menor a 160), 5) Reposo, analgesia, antiemeticos. NUNCA trombolisis (B). Puncion lumbar (C) solo si TAC negativo con alta sospecha.',
ARRAY['IMSS-372-10']::text[], TRUE)
ON CONFLICT DO NOTHING;

-- ---------- MCQ 7: Miastenia gravis ----------
INSERT INTO mcq_bank (specialty, difficulty, topic, vignette, question, options_json, correct_answer, explanation_correct, guidelines_ids, is_active)
VALUES ('Neurologia', 'advanced', 'Miastenia',
'Mujer 30 anos con debilidad FATIGABLE (empeora en el dia), ptosis palpebral, diplopia, disfagia. Anticuerpos anti-receptor acetilcolina positivos.',
'Cual es el tratamiento INICIAL?',
'{"A":"Piridostigmina 60mg VO cada 6h","B":"Timectomia inmediata","C":"Plasmaferesis","D":"Metotrexate"}'::jsonb,
'A',
'La respuesta correcta es A. Miastenia gravis se inicia con PIRIDOSTIGMINA (inhibidor colinesterasa) 60mg VO cada 4-6h como tratamiento sintomatico. Si insuficiente, agregar CORTICOIDES (prednisona) o inmunosupresor (azatioprina, micofenolato). Timectomia (B) esta indicada en TIMOMA o pacientes generalizados menores a 60 anos. Plasmaferesis/IGIV (C) es para CRISIS miastenica o pre-quirurgico. TAC de torax OBLIGATORIO para descartar timoma.',
ARRAY['IMSS-322-10']::text[], TRUE)
ON CONFLICT DO NOTHING;

-- ---------- MCQ 8: Demencia Alzheimer ----------
INSERT INTO mcq_bank (specialty, difficulty, topic, vignette, question, options_json, correct_answer, explanation_correct, guidelines_ids, is_active)
VALUES ('Neurologia', 'intermediate', 'Demencia',
'Mujer 74 anos con deterioro cognitivo progresivo 3 anos: perdida de memoria reciente, desorientacion temporal, apraxia. MMSE 18/30. TAC: atrofia cortical difusa. B12 y TSH normales.',
'Cual es el tratamiento farmacologico?',
'{"A":"Donepezilo 5-10mg/dia (inhibidor colinesterasa)","B":"Antipsicoticos","C":"Levodopa","D":"Ninguno, solo apoyo"}'::jsonb,
'A',
'La respuesta correcta es A. Enfermedad de Alzheimer LEVE-MODERADA (MMSE 10-24): inhibidores de acetilcolinesterasa (DONEPEZILO 5-10mg/dia, rivastigmina, galantamina) mejoran modestamente cognicion y funcionalidad. En MODERADA-SEVERA (MMSE menos de 15): agregar MEMANTINA (antagonista NMDA). Antipsicoticos (B) SOLO si sintomas psicoticos severos (aumentan mortalidad). Levodopa (C) es para Parkinson. Apoyo (D) es fundamental pero no unico tratamiento.',
ARRAY['IMSS-144-08']::text[], TRUE)
ON CONFLICT DO NOTHING;

-- ---------- MCQ 9: Esclerosis multiple brote ----------
INSERT INTO mcq_bank (specialty, difficulty, topic, vignette, question, options_json, correct_answer, explanation_correct, guidelines_ids, is_active)
VALUES ('Neurologia', 'advanced', 'EM brote',
'Mujer 28 anos con diagnostico previo de EM remitente-recurrente, presenta ceguera unilateral izquierda subaguda de 3 dias con dolor ocular al movimiento. Fondo de ojo: papila normal (neuritis retrobulbar).',
'Cual es el tratamiento de la RECAIDA aguda?',
'{"A":"Interferon beta subcutaneo","B":"Metilprednisolona 1g IV cada 24h por 3-5 dias","C":"Fingolimod VO","D":"Natalizumab IV"}'::jsonb,
'B',
'La respuesta correcta es B. RECAIDA aguda de EM (neuritis optica es tipica) se trata con METILPREDNISOLONA 1g IV cada 24h por 3-5 dias (acelera recuperacion, no cambia curso a largo plazo). Interferon (A), fingolimod (C), natalizumab (D) son MODIFICADORES de la ENFERMEDAD (previenen brotes futuros), NO se usan en el brote agudo. Plasmaferesis si refractaria a corticoides.',
ARRAY['IMSS-357-13']::text[], TRUE)
ON CONFLICT DO NOTHING;

-- ---------- MCQ 10: Cefalea red flags ----------
INSERT INTO mcq_bank (specialty, difficulty, topic, vignette, question, options_json, correct_answer, explanation_correct, guidelines_ids, is_active)
VALUES ('Neurologia', 'intermediate', 'Cefalea secundaria',
'Hombre 65 anos SIN cefaleas previas, presenta cefalea de inicio ABRUPTO hace 6h, "la peor de su vida", vomito, rigidez de nuca leve. TA 165/90. Sin foco motor.',
'Cual es la conducta INMEDIATA?',
'{"A":"Paracetamol y observacion","B":"TAC craneo urgente SIN contraste (si negativo puncion lumbar)","C":"Sumatriptan VO","D":"RMN cerebral electiva"}'::jsonb,
'B',
'La respuesta correcta es B. RED FLAGS de cefalea (edad >50 con NUEVA cefalea + inicio abrupto/thunderclap + rigidez de nuca) obliga a descartar HSA. TAC sin contraste sensibilidad 95% primeras 6h. Si TAC negativo pero alta sospecha: PUNCION LUMBAR (xantocromia). Otras red flags SNOOP4: Sistemicos, Neurologicos focales, Onset subito, Older (mayor a 50), Pattern change, Positional, Precipitada por Valsalva, Papiledema. Triptanes (C) CONTRAINDICADOS sin descartar organico.',
ARRAY['IMSS-105-08']::text[], TRUE)
ON CONFLICT DO NOTHING;

COMMIT;

SELECT COUNT(*) AS total_neurologia FROM mcq_bank WHERE specialty = 'Neurologia';
