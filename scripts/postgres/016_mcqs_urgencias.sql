-- ================================================================
-- 016_mcqs_urgencias.sql
-- Sprint 7A.3: 10 MCQs de Urgencias / Trauma estilo ENARM
-- ================================================================

BEGIN;

-- ---------- MCQ 1: TCE severo ----------
INSERT INTO mcq_bank (specialty, difficulty, topic, vignette, question, options_json, correct_answer, explanation_correct, guidelines_ids, is_active)
VALUES ('Urgencias', 'advanced', 'TCE severo',
'Hombre 32 anos post-accidente automovilistico, inconsciente. Glasgow 7 (O2 V2 M3). Anisocoria (D 5mm arreactiva, I 3mm). Triada de Cushing (TA 178/98, FC 58, respiracion irregular). Signo de Battle.',
'Cual es el manejo INICIAL prioritario?',
'{"A":"TAC craneal antes de intubar","B":"INTUBACION OROTRAQUEAL (Glasgow menor a 8) con secuencia rapida + estabilizacion cervical + TAC urgente + interconsulta neurocirugia + manitol o SSH","C":"Solo O2 mascarilla y observacion","D":"Corticoides IV altas dosis"}'::jsonb,
'B',
'La respuesta correcta es B. TCE severo con Glasgow menor a 8 = INTUBACION ENDOTRAQUEAL con secuencia rapida (etomidato, ketamina, rocuronio). Prioridades: 1) ABC + estabilizacion cervical (collarin), 2) INTUBAR, 3) VM normocapnia (PaCO2 35-40), 4) TAC craneo URGENTE (hematoma epidural probable por anisocoria y Battle), 5) OSMOTERAPIA: manitol 0.5-1 g/kg IV o SSH 3%, 6) Cabecera 30 grados, 7) Meta PAM mayor o igual 80, PPC mayor a 60, 8) NEUROCIRUGIA para posible craniectomia. CORTICOIDES (D) CONTRAINDICADOS en TCE (estudio CRASH).',
ARRAY['IMSS-604-13']::text[], TRUE)
ON CONFLICT DO NOTHING;

-- ---------- MCQ 2: Shock hipovolemico ----------
INSERT INTO mcq_bank (specialty, difficulty, topic, vignette, question, options_json, correct_answer, explanation_correct, guidelines_ids, is_active)
VALUES ('Urgencias', 'intermediate', 'Shock hipovolemico',
'Hombre 25 anos por herida por arma blanca abdominal. TA 78/40, FC 138, palidez, diaforesis, alteracion mental. Estimacion perdida sanguinea 2,000 mL. Sin acceso IV aun.',
'Cual es el manejo INICIAL?',
'{"A":"Solo cristaloides 4L rapidos","B":"2 vias IV calibre 14-16 + CRISTALOIDES tibios en bolo 1-2L + TRANSFUSION concentrados eritrocitarios O NEG + plasma + plaquetas (protocolo transfusion masiva 1:1:1) + CIRUGIA URGENTE","C":"Vasopresores primero","D":"Torniquete abdominal"}'::jsonb,
'B',
'La respuesta correcta es B. Shock hemorragico CLASE IV (perdida mayor a 40% volumen) requiere reposicion agresiva y CONTROL DEL SANGRADO. Manejo: 1) 2 vias IV gruesas (14-16G) o intraosea, 2) CRISTALOIDES tibios 1-2L bolo pero PREFERIR TRANSFUSION temprana, 3) PROTOCOLO TRANSFUSION MASIVA 1:1:1 (concentrados: plasma: plaquetas), 4) ACIDO TRANEXAMICO 1g IV (CRASH-2), 5) CONTROL DE HEMORRAGIA definitivo = CIRUGIA URGENTE (no retrasar). Vasopresores (C) NO en shock hipovolemico (empeoran perfusion). NO cristaloides masivos solos (aumentan coagulopatia dilucional).',
ARRAY['IMSS-604-13']::text[], TRUE)
ON CONFLICT DO NOTHING;

-- ---------- MCQ 3: Quemaduras extensas ----------
INSERT INTO mcq_bank (specialty, difficulty, topic, vignette, question, options_json, correct_answer, explanation_correct, guidelines_ids, is_active)
VALUES ('Urgencias', 'advanced', 'Quemaduras',
'Hombre 40 anos, 70 kg, quemaduras 2do y 3er grado en 40% SCT por incendio hace 1 hora. Consciente, disneico, esputo carbonaceo, cejas quemadas.',
'Cual es el manejo INICIAL prioritario incluyendo formula de reposicion?',
'{"A":"Solo curaciones locales","B":"Intubacion PRECOZ (via aerea comprometida) + reanimacion hidrica formula PARKLAND (4 ml x kg x %SCT quemada = 11,200 mL Ringer lactato en 24h, 50% en primeras 8h) + analgesia + ATB topico + valoracion centro quemados","C":"Corticoides IV","D":"Vasopresores"}'::jsonb,
'B',
'La respuesta correcta es B. Quemadura mayor a 20% SCT + signos de lesion inhalatoria (esputo carbonaceo, cejas quemadas, disnea) = INTUBACION PRECOZ antes de edema (dificil despues). FORMULA PARKLAND: 4 ml x kg x %SCT quemada = REPOSICION en 24h con Ringer lactato. La MITAD en primeras 8h (desde la quemadura, no llegada), otra mitad en 16h siguientes. Meta: diuresis 0.5-1 ml/kg/h. Otros: analgesia opioide, ATB topico (sulfadiazina plata), profilaxis TT, transferir CENTRO ESPECIALIZADO. Corticoides (C) CONTRAINDICADOS.',
ARRAY['IMSS-041-08']::text[], TRUE)
ON CONFLICT DO NOTHING;

-- ---------- MCQ 4: Intoxicacion organofosforados ----------
INSERT INTO mcq_bank (specialty, difficulty, topic, vignette, question, options_json, correct_answer, explanation_correct, guidelines_ids, is_active)
VALUES ('Urgencias', 'intermediate', 'Intoxicaciones',
'Agricultor 35 anos con exposicion a pesticida hace 2h. Presenta miosis, sialorrea, sudoracion profusa, broncorrea, diarrea, fasciculaciones. Bradicardia 45. Confuso.',
'Cual es el sindrome + antidoto?',
'{"A":"Sindrome opioide, naloxona","B":"Sindrome COLINERGICO por ORGANOFOSFORADOS: ATROPINA IV en dosis crecientes (meta ausencia de secreciones bronquiales) + PRALIDOXIMA (regenera acetilcolinesterasa)","C":"Sindrome simpaticomimetico, propranolol","D":"Sindrome anticolinergico, fisostigmina"}'::jsonb,
'B',
'La respuesta correcta es B. Sindrome COLINERGICO (SLUDGE: Salivacion, Lagrimeo, Uresis, Defecacion, GI, Emesis) + MIOSIS + broncorrea + bradicardia + FASCICULACIONES = intoxicacion por ORGANOFOSFORADOS/CARBAMATOS (inhiben acetilcolinesterasa). Antidoto: 1) ATROPINA 2-5mg IV cada 5-10 min, duplicando dosis hasta ATROPINIZACION (secreciones bronquiales secas), 2) PRALIDOXIMA (2-PAM) 1-2g IV: regenera enzima (efectiva antes de 24-48h), 3) DESCONTAMINACION (retirar ropa, lavar piel). Diazepam para convulsiones.',
ARRAY['IMSS-244-10']::text[], TRUE)
ON CONFLICT DO NOTHING;

-- ---------- MCQ 5: Reaccion anafilactica ----------
INSERT INTO mcq_bank (specialty, difficulty, topic, vignette, question, options_json, correct_answer, explanation_correct, guidelines_ids, is_active)
VALUES ('Urgencias', 'intermediate', 'Anafilaxia',
'Mujer 30 anos, 5 min post-inyeccion penicilina IM presenta urticaria generalizada, edema labial, disnea con sibilancias, TA 80/50, taquicardia 120.',
'Cual es el tratamiento INMEDIATO?',
'{"A":"Difenhidramina 25mg IV","B":"ADRENALINA IM 0.3-0.5 mg (1:1000) en muslo cara anterolateral, repetir cada 5-15 min si necesario + O2 + cristaloides + antihistaminicos + corticoides","C":"Corticoides IV solos","D":"Salbutamol nebulizado unico"}'::jsonb,
'B',
'La respuesta correcta es B. Anafilaxia (compromiso 2+ sistemas: piel, respiratorio, CV, GI) requiere ADRENALINA IM INMEDIATA como primer paso. Dosis: 0.3-0.5 mg IM (1:1000) EN MUSLO cara anterolateral (mejor absorcion que deltoides o SC). Repetir cada 5-15 min. NUNCA retrasar por antihistaminicos o corticoides (SEGUNDA LINEA). Ademas: O2 alto flujo, cristaloides IV 1-2L (shock distributivo), difenhidramina 25-50mg IV + ranitidina 50mg IV, metilprednisolona 125mg IV (previene fase tardia), salbutamol NEB si broncoespasmo. Observar 4-8h por reaccion bifasica.',
ARRAY['IMSS-244-10']::text[], TRUE)
ON CONFLICT DO NOTHING;

-- ---------- MCQ 6: PCR (paro cardiorrespiratorio) ----------
INSERT INTO mcq_bank (specialty, difficulty, topic, vignette, question, options_json, correct_answer, explanation_correct, guidelines_ids, is_active)
VALUES ('Urgencias', 'intermediate', 'RCP',
'Hombre 55 anos colapso subito. No responde, no respira, no pulso. Monitor: FIBRILACION VENTRICULAR. Un solo rescatador con desfibrilador.',
'Cual es la secuencia CORRECTA de RCP?',
'{"A":"5 ciclos de compresiones 30:2 antes de desfibrilar","B":"DESFIBRILAR primero (200J bifasico) + reanudar RCP 2 minutos (compresiones 100-120/min, profundidad 5-6cm, minimizar interrupciones) + reevaluar ritmo, si persiste FV: desfibrilar + adrenalina 1mg IV cada 3-5 min + amiodarona 300mg tras 3er choque","C":"Solo adrenalina IV","D":"Marcapasos transcutaneo"}'::jsonb,
'B',
'La respuesta correcta es B. En FV/TV SIN PULSO PRESENCIADA con desfibrilador disponible: DESFIBRILAR INMEDIATAMENTE (200J bifasico o 360J monofasico). Luego: RCP 2 minutos (30:2 sin via aerea avanzada, o continuas 100-120/min con intubado), Minimizar interrupciones, PROFUNDIDAD 5-6cm, permitir descompresion torax. ADRENALINA 1mg IV cada 3-5 min. AMIODARONA 300mg IV tras 3er choque (o lidocaina). Buscar causas H y T (hipoxia, hipovolemia, H+, hipo/hiperK, hipotermia, tension, taponamiento, toxinas, trombosis).',
ARRAY['IMSS-244-10']::text[], TRUE)
ON CONFLICT DO NOTHING;

-- ---------- MCQ 7: Intoxicacion paracetamol ----------
INSERT INTO mcq_bank (specialty, difficulty, topic, vignette, question, options_json, correct_answer, explanation_correct, guidelines_ids, is_active)
VALUES ('Urgencias', 'advanced', 'Toxicologia',
'Mujer 20 anos, ingiere 30 tabletas de paracetamol 500mg (15 gramos) hace 6 horas con intento suicida. Asintomatica. Nivel serico paracetamol tras 4h: 200 ug/mL (nomograma Rumack-Matthew en zona toxica).',
'Cual es el antidoto y ventana terapeutica?',
'{"A":"Naloxona","B":"N-ACETILCISTEINA (NAC) IV o VO, idealmente en las primeras 8-10 horas post-ingesta (efectiva hasta 24h y aun despues si hepatotoxicidad establecida)","C":"Flumazenil","D":"Fisostigmina"}'::jsonb,
'B',
'La respuesta correcta es B. Toxicidad paracetamol (mayor a 150 mg/kg o mayor a 7.5g adulto) mide nivel serico 4h post-ingesta y compara con NOMOGRAMA RUMACK-MATTHEW. Si zona toxica = NAC. Regimen IV (Prescott): 150 mg/kg bolo + 50 mg/kg en 4h + 100 mg/kg en 16h. VO: 140 mg/kg dosis carga + 70 mg/kg cada 4h por 17 dosis. Optimo antes de 8-10h. Efectivo hasta 24h y aun despues si dano establecido. Carbon activado si menor a 4h desde ingesta. Vigilar transaminasas, INR, creatinina.',
ARRAY['IMSS-244-10']::text[], TRUE)
ON CONFLICT DO NOTHING;

-- ---------- MCQ 8: Neumotorax a tension ----------
INSERT INTO mcq_bank (specialty, difficulty, topic, vignette, question, options_json, correct_answer, explanation_correct, guidelines_ids, is_active)
VALUES ('Urgencias', 'advanced', 'Neumotorax tension',
'Hombre 30 anos post-trauma toracico cerrado, disnea SEVERA, TA 70/40, FC 140, cianosis, ingurgitacion yugular, desviacion traqueal a la izquierda, hiperresonancia y AUSENCIA de murmullo vesicular en hemitorax DERECHO.',
'Cual es la conducta INMEDIATA?',
'{"A":"Rx torax portatil para confirmar","B":"TORACOCENTESIS DESCOMPRESIVA (aguja 14G) en 2do espacio intercostal linea medioclavicular o 4to-5to EIC linea axilar media + posterior sonda pleural","C":"Intubacion y VM","D":"Fibrinoliticos"}'::jsonb,
'B',
'La respuesta correcta es B. Neumotorax a TENSION es DIAGNOSTICO CLINICO (no esperar Rx). Hallazgos: shock + IY + desviacion traqueal contralateral + timpanismo + ausencia MV = TORACOCENTESIS DESCOMPRESIVA URGENTE. Sitios: 2do EIC linea medioclavicular O 5to EIC linea axilar media (recomendacion actual ATLS). Aguja gruesa 14G. Se convierte a neumotorax simple con "silbido" de aire. LUEGO colocar SONDA PLEURAL (28-32 Fr) conectada a sello agua. NO retrasar por imagen (paciente muere).',
ARRAY['IMSS-604-13']::text[], TRUE)
ON CONFLICT DO NOTHING;

-- ---------- MCQ 9: Golpe de calor ----------
INSERT INTO mcq_bank (specialty, difficulty, topic, vignette, question, options_json, correct_answer, explanation_correct, guidelines_ids, is_active)
VALUES ('Urgencias', 'intermediate', 'Golpe de calor',
'Corredor 25 anos en maraton clima caluroso, colapsa. Temperatura CENTRAL 41.5C, alterado mentalmente, sin sudoracion (piel seca caliente), taquicardia 140, TA 90/50.',
'Cual es el manejo?',
'{"A":"Solo hidratacion oral","B":"ENFRIAMIENTO AGRESIVO INMEDIATO (INMERSION en agua fria/hielo o rociado + ventiladores) meta menor a 39C rapidamente + reanimacion hidrica IV + monitor + evitar antipireticos","C":"Paracetamol IV","D":"Corticoides"}'::jsonb,
'B',
'La respuesta correcta es B. Golpe de calor CLASICO o POR ESFUERZO (temperatura CENTRAL mayor a 40C + alteracion SNC): ENFRIAMIENTO INMEDIATO reduce mortalidad. Metodos: INMERSION EN AGUA FRIA (mas eficaz), rociado + ventiladores, bolsas de hielo en axilas/ingles/cuello. Meta llegar a 39C en menos de 30 min. TAMBIEN: cristaloides IV, monitor cardiaco, glucemia, electrolitos, funcion renal (rabdomiolisis), coagulacion (CID). ANTIPIRETICOS (paracetamol) NO SIRVEN (hipertermia no es fiebre), pueden empeorar hepatotoxicidad. Corticoides (D) sin utilidad.',
ARRAY['IMSS-244-10']::text[], TRUE)
ON CONFLICT DO NOTHING;

-- ---------- MCQ 10: Hipotermia severa ----------
INSERT INTO mcq_bank (specialty, difficulty, topic, vignette, question, options_json, correct_answer, explanation_correct, guidelines_ids, is_active)
VALUES ('Urgencias', 'advanced', 'Hipotermia',
'Hombre rescatado tras 4h en agua helada. Temperatura central 27C. Inconsciente, bradicardia 32, fibrilacion auricular. Rigidez muscular. Sin pulso palpable pero monitor con actividad electrica.',
'Cual es el manejo?',
'{"A":"RCP convencional y desfibrilar","B":"HIPOTERMIA SEVERA (menor a 28C): recalentamiento ACTIVO CENTRAL (liquidos IV a 42C, lavado cavidades, ECMO/CIRCULACION EXTRACORPOREA), RCP con adaptaciones (aplazar farmacos y desfibrilacion hasta mayor a 30C), NO declarar muerte hasta que este CALIENTE (mayor a 32C) y MUERTO","C":"Recalentamiento externo pasivo con cobijas","D":"Adrenalina en dosis dobles"}'::jsonb,
'B',
'La respuesta correcta es B. HIPOTERMIA SEVERA (menor a 28C): RECALENTAMIENTO ACTIVO CENTRAL es prioridad. Metodos: liquidos IV tibios (40-42C), lavado peritoneal/pleural/gastrico con SF caliente, hemodialisis, y el mas efectivo ECMO/CIRCULACION EXTRACORPOREA. Adaptaciones RCP: compresiones lentas y suaves, LIMITAR DESFIBRILACION a 1 intento si menor a 30C, POSTPONER farmacos (adrenalina) hasta temperatura mayor a 30C. Regla clave: "NOT DEAD UNTIL WARM AND DEAD" - no declarar muerte hasta temperatura central mayor a 32-35C con paro persistente.',
ARRAY['IMSS-244-10']::text[], TRUE)
ON CONFLICT DO NOTHING;

COMMIT;

SELECT COUNT(*) AS total_urgencias FROM mcq_bank WHERE specialty = 'Urgencias';
