-- ================================================================
-- 013_mcqs_neumologia.sql
-- Sprint 7A.3: 10 MCQs de Neumologia estilo ENARM
-- ================================================================

BEGIN;

-- ---------- MCQ 1: NAC CURB-65 ----------
INSERT INTO mcq_bank (specialty, difficulty, topic, vignette, question, options_json, correct_answer, explanation_correct, guidelines_ids, is_active)
VALUES ('Neumologia', 'intermediate', 'NAC',
'Hombre 72 anos con tos productiva, fiebre 39C, disnea. TA 88/58, FC 102, FR 30, confusion, urea 45 mg/dL. Rx: infiltrado alveolar en LID.',
'Cual es el nivel de atencion apropiado segun CURB-65?',
'{"A":"Ambulatorio con amoxicilina","B":"Hospitalizacion en piso","C":"UCI (CURB-65 4-5 puntos)","D":"Alta con azitromicina"}'::jsonb,
'C',
'La respuesta correcta es C. CURB-65: Confusion (1) + Urea >42 mg/dL (1) + FR mayor o igual 30 (1) + BP sistolica menor a 90 (1) + edad mayor o igual 65 (1) = 5 puntos. Mortalidad 27-57%. Puntaje 3-5 = UCI. Puntaje 2 = hospitalizacion. Puntaje 0-1 = ambulatorio. Tratamiento UCI: beta-lactamico (ceftriaxona) + macrolido (azitromicina) o quinolona respiratoria (levofloxacino).',
ARRAY['IMSS-234-09']::text[], TRUE)
ON CONFLICT DO NOTHING;

-- ---------- MCQ 2: EPOC exacerbacion ----------
INSERT INTO mcq_bank (specialty, difficulty, topic, vignette, question, options_json, correct_answer, explanation_correct, guidelines_ids, is_active)
VALUES ('Neumologia', 'intermediate', 'EPOC',
'Hombre 68 anos con EPOC, exacerbacion con aumento de disnea, tos y expectoracion PURULENTA. SpO2 86% aire ambiente. Sin fiebre alta. FEV1 basal 40%.',
'Cual es el tratamiento COMPLETO?',
'{"A":"Solo salbutamol nebulizado","B":"O2 meta SpO2 88-92% + broncodilatador (SABA+SAMA) + prednisona 40mg VO 5 dias + antibiotico (amoxi-clav o azitro)","C":"Ventilacion mecanica invasiva","D":"Corticoide inhalado nuevo"}'::jsonb,
'B',
'La respuesta correcta es B. Exacerbacion moderada-severa de EPOC (criterios Anthonisen tipo 1: DISNEA + TOS + PURULENCIA) requiere: 1) O2 meta 88-92% (evitar hiperoxia), 2) Broncodilatadores nebulizados (salbutamol + ipratropio), 3) CORTICOIDE SISTEMICO (prednisona 40mg VO x 5 dias), 4) ANTIBIOTICO si purulencia (amoxi-clav, azitromicina, o levofloxacino x 5-7 dias), 5) VNI si acidosis respiratoria pH menor a 7.35.',
ARRAY['IMSS-037-08']::text[], TRUE)
ON CONFLICT DO NOTHING;

-- ---------- MCQ 3: Asma severa ----------
INSERT INTO mcq_bank (specialty, difficulty, topic, vignette, question, options_json, correct_answer, explanation_correct, guidelines_ids, is_active)
VALUES ('Neumologia', 'intermediate', 'Crisis asmatica',
'Mujer 24 anos con antecedente de asma, disnea severa que no cede con salbutamol MDI casero. FR 32, SpO2 89%, sibilancias difusas, uso de musculos accesorios, dificil hablar. PEF 45% predicho.',
'Cual es el manejo INICIAL en urgencias?',
'{"A":"Solo salbutamol MDI","B":"Salbutamol NEB continuo + ipratropio NEB + O2 + corticoide sistemico (metilprednisolona 40-80mg IV o prednisona 40-50mg VO)","C":"Adrenalina IM","D":"Aminofilina IV en bolo"}'::jsonb,
'B',
'La respuesta correcta es B. Crisis asmatica MODERADA-SEVERA: 1) O2 meta SpO2 mayor a 94%, 2) SABA NEB (salbutamol) cada 20 min x 3 dosis o CONTINUO, 3) Ipratropio NEB (mejora broncodilatacion en crisis severas), 4) CORTICOIDE SISTEMICO EN LA PRIMERA HORA (metilprednisolona 40-80mg IV o prednisona 40-50mg VO, igual eficacia). Si refractario: sulfato de magnesio 2g IV en 20 min. Adrenalina (C) solo en anafilaxia. Aminofilina (D) ya no se recomienda.',
ARRAY['IMSS-009-08']::text[], TRUE)
ON CONFLICT DO NOTHING;

-- ---------- MCQ 4: TEP diagnostico ----------
INSERT INTO mcq_bank (specialty, difficulty, topic, vignette, question, options_json, correct_answer, explanation_correct, guidelines_ids, is_active)
VALUES ('Neumologia', 'advanced', 'TEP',
'Mujer 55 anos post-cirugia ortopedica hace 5 dias, presenta disnea subita, dolor pleuritico, taquicardia FC 118. TA 110/70, SpO2 88%. D-dimero 3,500. Wells 6 puntos (alta probabilidad).',
'Cual es el ESTUDIO diagnostico de eleccion?',
'{"A":"Rx torax","B":"AngioTAC pulmonar (helicoidal)","C":"Ecocardiograma","D":"Gammagrafia V/Q"}'::jsonb,
'B',
'La respuesta correcta es B. Wells alto (mayor a 4) + D-dimero elevado obliga a IMAGEN. AngioTAC PULMONAR (helicoidal multidetector) es el ESTUDIO DE ELECCION: sensibilidad 96%, especificidad 89%, disponible 24/7. Gammagrafia V/Q (D) es alternativa si contraindicacion contraste (embarazo, IRA, alergia). Ecocardiograma (C) util para TEP MASIVO/SUBMASIVO (disfuncion VD). Rx (A) NO diagnostica pero descarta neumotorax. Iniciar ANTICOAGULACION empirica mientras se realiza estudio.',
ARRAY['IMSS-437-11']::text[], TRUE)
ON CONFLICT DO NOTHING;

-- ---------- MCQ 5: Neumotorax espontaneo ----------
INSERT INTO mcq_bank (specialty, difficulty, topic, vignette, question, options_json, correct_answer, explanation_correct, guidelines_ids, is_active)
VALUES ('Neumologia', 'intermediate', 'Neumotorax',
'Hombre 22 anos alto y delgado, fumador, dolor toracico subito y disnea leve. Rx: neumotorax derecho de 30% (>2cm apical). Sin desviacion mediastinica. Estable hemodinamicamente.',
'Cual es el tratamiento?',
'{"A":"Observacion y O2 alto flujo","B":"Aspiracion con aguja o sonda pleural (drenaje pleural) 20-28 Fr","C":"Toracotomia urgente","D":"Pleurodesis quimica inmediata"}'::jsonb,
'B',
'La respuesta correcta es B. Neumotorax espontaneo PRIMARIO grande (mayor a 2cm en apex o compromiso mayor a 30%) requiere DRENAJE: aspiracion con aguja o sonda pleural (20-28 Fr) conectada a sello de agua. Observacion + O2 (A) solo si menor a 2cm y asintomatico. Toracotomia (C) para casos refractarios, recurrentes o hemotorax masivo. Pleurodesis (D) para prevencion de recurrencia en 2do episodio o falla de tratamiento. Recurrencia 30% al ano.',
ARRAY['IMSS-354-10']::text[], TRUE)
ON CONFLICT DO NOTHING;

-- ---------- MCQ 6: Derrame pleural exudado ----------
INSERT INTO mcq_bank (specialty, difficulty, topic, vignette, question, options_json, correct_answer, explanation_correct, guidelines_ids, is_active)
VALUES ('Neumologia', 'intermediate', 'Derrame pleural',
'Mujer 60 anos con derrame pleural derecho de 4 semanas. Toracocentesis: proteinas LP 4.5 g/dL, proteinas serica 6.5. DHL LP 350, DHL serica 200. Cociente prot LP/serica 0.69.',
'Cual es la interpretacion CORRECTA?',
'{"A":"Trasudado por ICC","B":"Exudado (criterios de Light positivos): buscar etiologia (neoplasia, TB, empiema, TEP)","C":"Muestra contaminada","D":"Necesita nueva toracocentesis"}'::jsonb,
'B',
'La respuesta correcta es B. CRITERIOS DE LIGHT (basta UN criterio positivo = exudado): 1) Prot LP/serica mayor a 0.5 (0.69 SI cumple), 2) DHL LP/serica mayor a 0.6 (1.75 SI cumple), 3) DHL LP mayor a 2/3 del limite superior normal serico. Este es EXUDADO. Causas: NEOPLASIA (mas frecuente en adulto mayor), TB, empiema paraneumonico, TEP, autoinmune. Estudios adicionales: citologia, ADA (TB), Gram/cultivo, pH (menos a 7.2 sugiere empiema).',
ARRAY['IMSS-284-13']::text[], TRUE)
ON CONFLICT DO NOTHING;

-- ---------- MCQ 7: SAOS diagnostico ----------
INSERT INTO mcq_bank (specialty, difficulty, topic, vignette, question, options_json, correct_answer, explanation_correct, guidelines_ids, is_active)
VALUES ('Neumologia', 'intermediate', 'Apnea del sueno',
'Hombre 52 anos, obeso (IMC 34), ronquidos fuertes, apneas presenciadas por esposa, somnolencia diurna severa (Epworth 16/24), cefalea matutina. HTA de dificil control.',
'Cual es el ESTUDIO diagnostico de eleccion + tratamiento estandar?',
'{"A":"Espirometria + broncodilatadores","B":"Polisomnografia nocturna + CPAP","C":"Rx torax + O2 nocturno","D":"Gasometria diurna + traqueostomia"}'::jsonb,
'B',
'La respuesta correcta es B. Sospecha alta de SAOS (Epworth mayor a 10 + factores de riesgo). POLISOMNOGRAFIA NOCTURNA es el ESTANDAR: mide IAH (indice apnea-hipopnea). Leve 5-15, moderado 15-30, severo mayor a 30. Tratamiento SAOS moderado-severo: CPAP (presion positiva continua) titulado. Perdida de peso obligatoria (10% peso reduce IAH 30%). Otros: dispositivos orales, cirugia UPPP. NO tratar aumenta riesgo CV, HTA, ACV, muerte subita.',
ARRAY['IMSS-517-11']::text[], TRUE)
ON CONFLICT DO NOTHING;

-- ---------- MCQ 8: Fibrosis pulmonar ----------
INSERT INTO mcq_bank (specialty, difficulty, topic, vignette, question, options_json, correct_answer, explanation_correct, guidelines_ids, is_active)
VALUES ('Neumologia', 'advanced', 'FPI',
'Hombre 68 anos con disnea progresiva 8 meses, tos seca, acropaquia, estertores VELCRO bibasales. TAC: patron RETICULAR periferico basal, panal de abeja subpleural, sin vidrio esmerilado. Espirometria restrictiva DLCO 45%.',
'Cual es el diagnostico + tratamiento?',
'{"A":"Neumonia intersticial usual (UIP)/FPI + antifibrotico (pirfenidona o nintedanib)","B":"Sarcoidosis + corticoides","C":"Neumonia + antibioticos","D":"Asbestosis + esteroides"}'::jsonb,
'A',
'La respuesta correcta es A. Patron UIP TIPICO en TAC (reticular basal periferico, PANAL DE ABEJA, sin vidrio esmerilado) + clinica compatible = FIBROSIS PULMONAR IDIOPATICA (FPI). Tratamiento: ANTIFIBROTICOS (pirfenidona o nintedanib) que enlentecen declive de FEV1. Corticoides EMPEORAN la FPI (contraindicados). Considerar trasplante pulmonar en menores a 65 anos. Sobrevida media 3-5 anos. Sarcoidosis (B) tiene adenopatias hiliares + granulomas.',
ARRAY['IMSS-517-11']::text[], TRUE)
ON CONFLICT DO NOTHING;

-- ---------- MCQ 9: Cancer pulmonar tamizaje ----------
INSERT INTO mcq_bank (specialty, difficulty, topic, vignette, question, options_json, correct_answer, explanation_correct, guidelines_ids, is_active)
VALUES ('Neumologia', 'intermediate', 'Cancer pulmonar',
'Hombre 58 anos, fumador activo 40 paquetes-ano, asintomatico, acude a control anual. Sin sintomas respiratorios.',
'Cual es la recomendacion de TAMIZAJE de cancer pulmonar?',
'{"A":"Rx torax anual","B":"TAC de BAJA DOSIS anual (edad 50-80 + mayor o igual 20 paq-ano + fumador activo o exfumador menor a 15 anos)","C":"Broncoscopia anual","D":"No tamizaje, solo suspender tabaquismo"}'::jsonb,
'B',
'La respuesta correcta es B. USPSTF 2021: TAC de BAJA DOSIS anual para cancer pulmonar en pacientes 50-80 anos + historia de mayor o igual 20 paquetes-ano + fumador actual o exfumador menor a 15 anos. Reduce mortalidad por cancer pulmonar 20% (estudio NLST). Rx torax (A) NO tiene evidencia como tamizaje. Broncoscopia (C) es invasiva, no para tamizaje. Cesacion tabaquica (D) es fundamental pero se COMBINA con tamizaje.',
ARRAY['IMSS-030-13']::text[], TRUE)
ON CONFLICT DO NOTHING;

-- ---------- MCQ 10: Hemoptisis masiva ----------
INSERT INTO mcq_bank (specialty, difficulty, topic, vignette, question, options_json, correct_answer, explanation_correct, guidelines_ids, is_active)
VALUES ('Neumologia', 'advanced', 'Hemoptisis',
'Hombre 55 anos con TB pulmonar previa (secuelas cavitarias), presenta hemoptisis 600 mL en 24h. TA 100/60, FC 118, SpO2 91%. Sospecha aspergiloma.',
'Cual es la conducta INMEDIATA?',
'{"A":"Antibioticos y observacion","B":"Decubito lateral del lado SANGRANTE + O2 + broncoscopia + embolizacion arteria bronquial","C":"Corticoides IV altas dosis","D":"Alta con antifibrinoliticos VO"}'::jsonb,
'B',
'La respuesta correcta es B. Hemoptisis MASIVA (mayor a 500 mL/24h o mayor a 100 mL/h): 1) VIA AEREA prioridad: intubar con SELECTIVA de pulmon sano si severa, 2) DECUBITO LATERAL del lado SANGRANTE (evita anegar pulmon sano), 3) O2, 4) BRONCOSCOPIA localiza sitio y puede tratar (adrenalina topica, coagulacion), 5) EMBOLIZACION de arteria bronquial es el tratamiento DEFINITIVO en 90%. Cirugia si refractario o aspergiloma masivo. Antifibrinoliticos (D) solo en menor severidad. Corticoides (C) sin indicacion.',
ARRAY['IMSS-354-10']::text[], TRUE)
ON CONFLICT DO NOTHING;

COMMIT;

SELECT COUNT(*) AS total_neumologia FROM mcq_bank WHERE specialty = 'Neumologia';
