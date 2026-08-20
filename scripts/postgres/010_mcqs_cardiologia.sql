-- ================================================================
-- 010_mcqs_cardiologia.sql
-- Sprint 7A.3: 10 MCQs de Cardiologia estilo ENARM
-- ================================================================

BEGIN;

-- ---------- MCQ 1: IAM CEST manejo inicial ----------
INSERT INTO mcq_bank (specialty, difficulty, topic, vignette, question, options_json, correct_answer, explanation_correct, guidelines_ids, is_active)
VALUES ('Cardiologia', 'intermediate', 'IAM CEST',
'Hombre de 58 anos con dolor toracico opresivo de 90 minutos, irradiado a brazo izquierdo y mandibula, acompanado de diaforesis y disnea. ECG muestra elevacion del ST 3mm en V2-V5. TA 100/60, FC 108.',
'Cual es la conducta INICIAL mas apropiada?',
'{"A":"Trombolisis con alteplase","B":"Doble antiagregacion (AAS 300mg + clopidogrel 600mg) + anticoagulacion","C":"Nitroglicerina sublingual y observar","D":"Solicitar troponinas y esperar resultados"}'::jsonb,
'B',
'La respuesta correcta es B. En IAM CEST, el manejo inicial inmediato incluye doble antiagregacion plaquetaria (aspirina 162-325mg masticable + inhibidor P2Y12 como clopidogrel 600mg carga o ticagrelor 180mg) + anticoagulacion (HNF o enoxaparina) mientras se define reperfusion. La trombolisis (A) es opcion SI no hay disponibilidad de ICP en menos de 120 min. Nitroglicerina (C) es sintomatica pero no cambia mortalidad. Esperar troponinas (D) retrasa el tratamiento en emergencia con diagnostico electrocardiografico claro.',
ARRAY['IMSS-357-13']::text[], TRUE)
ON CONFLICT DO NOTHING;

-- ---------- MCQ 2: Insuficiencia cardiaca descompensada ----------
INSERT INTO mcq_bank (specialty, difficulty, topic, vignette, question, options_json, correct_answer, explanation_correct, guidelines_ids, is_active)
VALUES ('Cardiologia', 'intermediate', 'ICC descompensada',
'Mujer 68 anos con IC clase III, acude por disnea de reposo, ortopnea y edema bilateral +++. TA 145/90, FC 110, SpO2 88%, estertores crepitantes bibasales, ingurgitacion yugular. BNP 1,850.',
'Cual es el tratamiento farmacologico INICIAL prioritario?',
'{"A":"Furosemida 40mg IV en bolo","B":"Metoprolol 25mg VO","C":"Nitroprusiato en infusion","D":"Digoxina 0.25mg IV"}'::jsonb,
'A',
'La respuesta correcta es A. En ICC descompensada con congestion evidente, el tratamiento inicial es diuretico de asa IV (furosemida 40-80mg bolo o infusion segun dosis previa). Los betabloqueadores (B) estan CONTRAINDICADOS en descompensacion aguda hasta euvolemia. Nitroprusiato (C) se usa si hipertension severa refractaria, requiere monitorizacion invasiva. Digoxina (D) tiene rol limitado, no es primera linea salvo FA con RVR.',
ARRAY['IMSS-104-08']::text[], TRUE)
ON CONFLICT DO NOTHING;

-- ---------- MCQ 3: Fibrilacion auricular anticoagulacion ----------
INSERT INTO mcq_bank (specialty, difficulty, topic, vignette, question, options_json, correct_answer, explanation_correct, guidelines_ids, is_active)
VALUES ('Cardiologia', 'advanced', 'FA anticoagulacion',
'Hombre 72 anos con FA persistente, HTA controlada, DM2, ACV isquemico hace 2 anos. Creatinina 1.1 mg/dL. Sin sangrados previos. TA 135/80, FC irregular 88.',
'Cual es el tratamiento antitrombotico mas apropiado?',
'{"A":"Aspirina 100mg/dia","B":"Warfarina con meta INR 2-3","C":"Apixaban 5mg cada 12h","D":"Doble antiagregacion aspirina + clopidogrel"}'::jsonb,
'C',
'La respuesta correcta es C. CHA2DS2-VASc de 5 puntos (HTA 1, DM 1, ACV 2, edad >65 1) requiere anticoagulacion. Los DOAC (apixaban, rivaroxaban, dabigatran) son PRIMERA LINEA sobre warfarina en FA NO valvular por: menor riesgo sangrado intracraneal, no requieren INR, menos interacciones. Aspirina (A) NO es adecuada para FA. Warfarina (B) sigue siendo alternativa si contraindicacion DOAC. Doble antiagregacion (D) no reemplaza anticoagulacion.',
ARRAY['IMSS-014-08']::text[], TRUE)
ON CONFLICT DO NOTHING;

-- ---------- MCQ 4: Crisis hipertensiva ----------
INSERT INTO mcq_bank (specialty, difficulty, topic, vignette, question, options_json, correct_answer, explanation_correct, guidelines_ids, is_active)
VALUES ('Cardiologia', 'intermediate', 'Emergencia hipertensiva',
'Mujer 55 anos con TA 220/130, cefalea intensa, vision borrosa, papiledema en fondo de ojo, sin foco neurologico. Sin dolor toracico. Creatinina 1.3.',
'Cual es la meta de reduccion de TA en la PRIMERA hora?',
'{"A":"Reducir a menos de 140/90 en 1 hora","B":"Reducir 25% de TA media inicial","C":"Reducir a menos de 180/110 en 24 horas","D":"Mantener TA actual y observar"}'::jsonb,
'B',
'La respuesta correcta es B. En emergencia hipertensiva (encefalopatia HTA con papiledema) la meta es reducir la TA media (PAM) 20-25% en la PRIMERA hora, luego a 160/100 en 2-6 horas, y llegar a normotension en 24-48h. Reduccion agresiva (A) causa hipoperfusion cerebral, renal, coronaria. Reduccion muy lenta (C) permite dano organico. No tratar (D) es negligencia. Farmaco de eleccion: nicardipino o labetalol IV.',
ARRAY['IMSS-076-08']::text[], TRUE)
ON CONFLICT DO NOTHING;

-- ---------- MCQ 5: Endocarditis infecciosa ----------
INSERT INTO mcq_bank (specialty, difficulty, topic, vignette, question, options_json, correct_answer, explanation_correct, guidelines_ids, is_active)
VALUES ('Cardiologia', 'advanced', 'Endocarditis',
'Hombre 45 anos usuario de drogas IV, fiebre de 2 semanas, soplo tricuspideo nuevo, embolias septicas pulmonares en TAC. Hemocultivos positivos para S. aureus meticilino-sensible.',
'Cual es el tratamiento antibiotico de eleccion?',
'{"A":"Vancomicina 15 mg/kg IV cada 12h por 6 semanas","B":"Nafcilina/oxacilina 12g/dia IV por 4-6 semanas","C":"Ceftriaxona 2g IV cada 24h por 4 semanas","D":"Ampicilina + gentamicina por 4 semanas"}'::jsonb,
'B',
'La respuesta correcta es B. Endocarditis por S. aureus METICILINO-SENSIBLE se trata con nafcilina u oxacilina 12g/dia IV por 4-6 semanas (2 semanas si es tricuspidea no complicada en usuario IV). Vancomicina (A) es para MRSA o alergicos a penicilina. Ceftriaxona (C) es para Streptococcus viridans. Ampicilina + gentamicina (D) es esquema para Enterococcus. La eleccion se basa en cultivo/sensibilidad.',
ARRAY['IMSS-186-09']::text[], TRUE)
ON CONFLICT DO NOTHING;

-- ---------- MCQ 6: Angina inestable ----------
INSERT INTO mcq_bank (specialty, difficulty, topic, vignette, question, options_json, correct_answer, explanation_correct, guidelines_ids, is_active)
VALUES ('Cardiologia', 'intermediate', 'SCA sin elevacion ST',
'Hombre 62 anos con dolor toracico opresivo de 20 min hace 2 horas, ya resuelto. ECG normal. Troponina I: 0.08 ng/mL (normal <0.04). Sin cambios dinamicos. TIMI 4 puntos.',
'Cual es la conducta mas apropiada?',
'{"A":"Alta con nitratos y control ambulatorio","B":"Trombolisis inmediata","C":"Estrategia invasiva temprana (angiografia menos de 24h) + antiagregacion dual + anticoagulacion","D":"Ergometria de esfuerzo antes del alta"}'::jsonb,
'C',
'La respuesta correcta es C. SCA sin elevacion ST de riesgo INTERMEDIO-ALTO (TIMI mayor o igual 3, troponina elevada) requiere estrategia INVASIVA TEMPRANA (angiografia coronaria en menos de 24h) + doble antiagregacion (AAS + clopidogrel/ticagrelor) + anticoagulacion (enoxaparina o fondaparinux). Alta (A) es peligrosa. Trombolisis (B) NO se usa en SCASEST. Ergometria (D) se contraindica en SCA activo.',
ARRAY['IMSS-191-10']::text[], TRUE)
ON CONFLICT DO NOTHING;

-- ---------- MCQ 7: Shock cardiogenico ----------
INSERT INTO mcq_bank (specialty, difficulty, topic, vignette, question, options_json, correct_answer, explanation_correct, guidelines_ids, is_active)
VALUES ('Cardiologia', 'advanced', 'Shock cardiogenico',
'Hombre 65 anos post-IAM anterior extenso hace 6 horas. TA 78/50, FC 118, oliguria, extremidades frias, estertores hasta apices. Lactato 4.2. Ecocardiograma FEVI 20%.',
'Cual es el vasopresor/inotropico de PRIMERA linea?',
'{"A":"Dopamina 5-10 mcg/kg/min","B":"Norepinefrina + dobutamina","C":"Adrenalina 0.1 mcg/kg/min","D":"Vasopresina 0.03 U/min"}'::jsonb,
'B',
'La respuesta correcta es B. En shock cardiogenico, la combinacion norepinefrina (para presion perfusion) + dobutamina (inotropico) es la mas usada. Dopamina (A) esta EN DESUSO por mayor arritmogenicidad y mortalidad vs norepinefrina (estudio SOAP II). Adrenalina (C) aumenta lactato y mortalidad. Vasopresina (D) es adyuvante en shock septico refractario, no primera linea aqui. Considerar soporte mecanico (BCIA, ECMO, Impella) si refractario.',
ARRAY['IMSS-357-13']::text[], TRUE)
ON CONFLICT DO NOTHING;

-- ---------- MCQ 8: Estenosis aortica severa ----------
INSERT INTO mcq_bank (specialty, difficulty, topic, vignette, question, options_json, correct_answer, explanation_correct, guidelines_ids, is_active)
VALUES ('Cardiologia', 'advanced', 'Valvulopatia aortica',
'Mujer 78 anos con disnea de esfuerzo, sincope y dolor toracico. Soplo sistolico eyectivo grado IV/VI en foco aortico irradiado a carotidas. Ecocardiograma: area valvular 0.7 cm2, gradiente medio 55 mmHg, FEVI 60%.',
'Cual es el manejo definitivo?',
'{"A":"Restriccion de sal y diureticos","B":"Reemplazo valvular aortico (quirurgico o TAVI)","C":"Valvuloplastia con balon","D":"Betabloqueadores y estatinas"}'::jsonb,
'B',
'La respuesta correcta es B. Estenosis aortica SEVERA SINTOMATICA (triada: disnea + sincope + angina) tiene mortalidad 50% a 2 anos sin tratamiento. Indicacion CLASE I de reemplazo valvular (quirurgico si bajo riesgo, TAVI si riesgo intermedio-alto o edad avanzada). Diureticos (A) son sintomaticos, no cambian pronostico. Valvuloplastia (C) es puente en pediatricos o inestables. Farmacos (D) no revierten la lesion mecanica.',
ARRAY['IMSS-478-11']::text[], TRUE)
ON CONFLICT DO NOTHING;

-- ---------- MCQ 9: Prevencion secundaria post-IAM ----------
INSERT INTO mcq_bank (specialty, difficulty, topic, vignette, question, options_json, correct_answer, explanation_correct, guidelines_ids, is_active)
VALUES ('Cardiologia', 'intermediate', 'Prevencion secundaria',
'Hombre 55 anos post-IAMCEST anterior con angioplastia primaria + stent farmacoactivo hace 2 semanas. FEVI 45%. TA 130/80, LDL 145 mg/dL, HbA1c 6.8%.',
'Cual esquema de prevencion secundaria es MAS APROPIADO?',
'{"A":"Aspirina + IECA + estatina + betabloqueador","B":"Aspirina + clopidogrel (por 12 meses) + IECA + estatina alta intensidad + betabloqueador","C":"Solo aspirina + estatina","D":"Warfarina + IECA + betabloqueador"}'::jsonb,
'B',
'La respuesta correcta es B. Post-IAM con stent farmacoactivo requiere DOBLE ANTIAGREGACION 12 meses (AAS + inhibidor P2Y12), IECA/ARA-II (proteccion miocardica y renal), ESTATINA DE ALTA INTENSIDAD (atorvastatina 40-80mg meta LDL <70), BETABLOQUEADOR (reduccion mortalidad). Opcion A omite antiagregacion dual necesaria por el stent. Opcion C es insuficiente. Warfarina (D) solo si FA concomitante o trombo VI.',
ARRAY['IMSS-357-13']::text[], TRUE)
ON CONFLICT DO NOTHING;

-- ---------- MCQ 10: Bradiarritmia sintomatica ----------
INSERT INTO mcq_bank (specialty, difficulty, topic, vignette, question, options_json, correct_answer, explanation_correct, guidelines_ids, is_active)
VALUES ('Cardiologia', 'intermediate', 'Bradiarritmia',
'Hombre 75 anos con sincope de repeticion. ECG: bloqueo AV de tercer grado con FC ventricular 32 lpm, sin respuesta a atropina. TA 90/60.',
'Cual es el manejo definitivo?',
'{"A":"Adrenalina en infusion continua","B":"Marcapasos transcutaneo temporal como puente + implante marcapasos definitivo","C":"Isoproterenol IV","D":"Observacion en UCI"}'::jsonb,
'B',
'La respuesta correcta es B. BAV completo SINTOMATICO es indicacion CLASE I de marcapasos definitivo. Manejo agudo: marcapasos transcutaneo (o transvenoso temporal) como puente al implante definitivo. Adrenalina/isoproterenol (A, C) son medidas temporales mientras se coloca marcapasos, no manejo definitivo. Observacion sola (D) es negligencia con riesgo asistolia. Atropina 1mg IV puede intentarse primero pero rara vez efectiva en bloqueo infrahisiano.',
ARRAY['IMSS-484-11']::text[], TRUE)
ON CONFLICT DO NOTHING;

COMMIT;

-- Verificacion
SELECT COUNT(*) AS total_cardiologia FROM mcq_bank WHERE specialty = 'Cardiologia';
