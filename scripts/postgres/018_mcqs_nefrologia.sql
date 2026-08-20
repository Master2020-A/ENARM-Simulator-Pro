-- ================================================================
-- 018_mcqs_nefrologia.sql
-- Sprint 7A.3: 10 MCQs de Nefrologia estilo ENARM
-- ================================================================

BEGIN;

-- ---------- MCQ 1: LRA por AINEs ----------
INSERT INTO mcq_bank (specialty, difficulty, topic, vignette, question, options_json, correct_answer, explanation_correct, guidelines_ids, is_active)
VALUES ('Nefrologia', 'intermediate', 'LRA',
'Hombre 71 anos con HTA (enalapril) y DM2 (metformina), acude por oliguria (300 mL/24h) y edema MI de 5 dias. Automedico ibuprofeno 600mg cada 8h por 10 dias por gonalgia. Creatinina 3.8 (basal 1.1), urea 128, K 6.2. USG renal sin obstruccion.',
'Cual es el manejo INICIAL prioritario?',
'{"A":"Iniciar dialisis inmediata","B":"SUSPENDER nefrotoxicos (ibuprofeno, enalapril, metformina) + manejo hiperkalemia (gluconato Ca + insulina/glucosa + resinas) + balance hidrico estricto + furosemida si sobrecarga + evitar contrastes","C":"Continuar medicamentos y observar","D":"Solo hidratacion IV agresiva"}'::jsonb,
'B',
'La respuesta correcta es B. LRA AKIN 3 de causa MIXTA (prerrenal por AINE-IECA + intrinseca por NIA/NTA por AINE). Prioridades: 1) SUSPENDER inmediatamente TODOS los nefrotoxicos (AINE, IECA/ARA-II, metformina por riesgo acidosis lactica), 2) MANEJO HIPERKALEMIA (K 6.2 amenaza vida): gluconato calcio 10% 10mL IV (estabiliza miocardio) + insulina 10U + glucosa 50% + resinas (kayexalate) o furosemida, 3) Balance hidrico + peso diario, 4) EVITAR contrastes/aminoglucosidos. Dialisis (A) si: hiperK refractaria, acidosis severa, sobrecarga refractaria, uremia sintomatica (regla AEIOU).',
ARRAY['IMSS-395-10']::text[], TRUE)
ON CONFLICT DO NOTHING;

-- ---------- MCQ 2: ERC estadios ----------
INSERT INTO mcq_bank (specialty, difficulty, topic, vignette, question, options_json, correct_answer, explanation_correct, guidelines_ids, is_active)
VALUES ('Nefrologia', 'intermediate', 'ERC',
'Mujer 62 anos con DM2 20 anos + HTA. Creatinina 2.1, TFG estimada (CKD-EPI) 32 ml/min/1.73m2. Proteinuria 1.5 g/24h. USG: rinones pequenos y ecogenicos.',
'Cual es el estadio KDIGO + tratamiento nefroprotector?',
'{"A":"ERC estadio 2, sin tratamiento especial","B":"ERC ESTADIO 3B (TFG 30-44) con proteinuria A3. Tratamiento: IECA o ARA-II (proteinuria), META TA menor a 130/80, CONTROL glucemico HbA1c 7%, SGLT2i (empagliflozina/dapagliflozina) con beneficio renal probado, dieta hipoproteica moderada 0.8 g/kg, evitar nefrotoxicos, control anemia, control mineral-osea","C":"ERC terminal, dialisis inmediata","D":"Solo control anual"}'::jsonb,
'B',
'La respuesta correcta es B. KDIGO ERC estadios por TFG: G1 mayor o igual 90, G2 60-89, G3a 45-59, G3b 30-44, G4 15-29, G5 menor a 15. Esta paciente G3b + A3 (proteinuria mayor a 300 mg/g). Nefroproteccion: 1) IECA/ARA-II (reduce proteinuria y progresion), 2) META TA menor a 130/80 (menor a 120/80 con proteinuria), 3) HbA1c 7%, 4) SGLT2i con beneficio renal en DM2 y no DM (estudios CREDENCE, DAPA-CKD, EMPA-KIDNEY), 5) DIETA 0.8 g/kg proteinas, 6) EVITAR AINE/contrastes, 7) Control anemia (Hb 10-11.5), calcio-fosforo-PTH, 8) Vacunacion (influenza, neumococo, hepB).',
ARRAY['IMSS-335-10']::text[], TRUE)
ON CONFLICT DO NOTHING;

-- ---------- MCQ 3: Hiperkalemia ----------
INSERT INTO mcq_bank (specialty, difficulty, topic, vignette, question, options_json, correct_answer, explanation_correct, guidelines_ids, is_active)
VALUES ('Nefrologia', 'advanced', 'Hiperkalemia',
'Paciente en dialisis con K serico 7.2 mEq/L. ECG: ondas T PICUDAS, PR prolongado, QRS ensanchado.',
'Cual es la SECUENCIA correcta de tratamiento?',
'{"A":"Solo resinas VO","B":"1) GLUCONATO DE CALCIO 10% 10 mL IV en 2-3 min (ESTABILIZA MIOCARDIO, primera medida) + 2) INSULINA 10U IV + GLUCOSA 50% 50 mL (redistribuye K a intracelular) + 3) SALBUTAMOL nebulizado + 4) RESINAS (kayexalate) o FUROSEMIDA (eliminan K del cuerpo) + 5) HEMODIALISIS URGENTE","C":"Solo dialisis, no medicar","D":"Bicarbonato solo"}'::jsonb,
'B',
'La respuesta correcta es B. Hiperkalemia SEVERA (K mayor a 6.5) con cambios ECG amenaza muerte por arritmia. SECUENCIA: 1) GLUCONATO CALCIO 10% 10 mL IV en 2-3 min = PRIMERA medida (no baja K pero ESTABILIZA MIOCARDIO por 30-60 min), 2) REDISTRIBUCION intracelular con insulina 10U IV + dextrosa 50% 50 mL + salbutamol NEB (bajan K 0.5-1 mEq en 15-30 min, efecto temporal), 3) ELIMINACION del cuerpo: kayexalate/patiromer/zirconio, furosemida IV si no anurico, HEMODIALISIS URGENTE si refractario o anurico (definitivo). Bicarbonato solo util en acidosis metabolica concomitante.',
ARRAY['IMSS-395-10']::text[], TRUE)
ON CONFLICT DO NOTHING;

-- ---------- MCQ 4: Glomerulonefritis postestreptococica ----------
INSERT INTO mcq_bank (specialty, difficulty, topic, vignette, question, options_json, correct_answer, explanation_correct, guidelines_ids, is_active)
VALUES ('Nefrologia', 'intermediate', 'GNPE',
'Nino 8 anos con edema periorbitario, orina color COCA-COLA (hematuria macroscopica), HTA 145/95, oliguria. Antecedente de faringitis hace 2 semanas. Complemento C3 BAJO, antiestreptolisina O elevada.',
'Cual es el diagnostico + manejo?',
'{"A":"Sindrome nefrotico, corticoides","B":"GLOMERULONEFRITIS POSTESTREPTOCOCICA (sindrome nefritico agudo). Manejo: RESTRICCION Na y agua + antihipertensivos (nifedipino o hidralazina) + diuretico (furosemida) si sobrecarga + tratar infeccion residual con penicilina + observacion (autolimitada 4-8 semanas)","C":"Nefritis lupica, ciclofosfamida","D":"Sindrome de Alport, IECA"}'::jsonb,
'B',
'La respuesta correcta es B. GNPE clasica: NINO + edema + HEMATURIA MACROSCOPICA color coca-cola + HTA + oliguria = SINDROME NEFRITICO AGUDO. C3 BAJO + ASO alta post-faringitis. Manejo: SOPORTE (autolimitada, resuelve 4-8 semanas): 1) Restringir Na (menor a 2g/dia) y liquidos, 2) DIURETICO (furosemida) si sobrecarga/HTA, 3) ANTIHIPERTENSIVOS (nifedipino, hidralazina, evitar IECA en fase aguda por riesgo hiperK), 4) PENICILINA si infeccion residual (no cambia curso), 5) Observacion complemento (normaliza 6-8 semanas). Biopsia SOLO si atipico o no mejora. Pronostico bueno en ninos.',
ARRAY['IMSS-241-12']::text[], TRUE)
ON CONFLICT DO NOTHING;

-- ---------- MCQ 5: Sindrome nefrotico ----------
INSERT INTO mcq_bank (specialty, difficulty, topic, vignette, question, options_json, correct_answer, explanation_correct, guidelines_ids, is_active)
VALUES ('Nefrologia', 'intermediate', 'Sindrome nefrotico',
'Nino 4 anos con edema generalizado (anasarca), proteinuria masiva 5g/24h, hipoalbuminemia 1.8 g/dL, hipercolesterolemia. Sin HTA. Sin hematuria. Complemento normal.',
'Cual es el diagnostico mas probable + tratamiento?',
'{"A":"Nefropatia IgA","B":"ENFERMEDAD DE CAMBIOS MINIMOS (mas comun en pediatricos 90%). Tratamiento: PREDNISONA 60 mg/m2/dia (o 2 mg/kg/dia) VO por 4-6 semanas, luego reduccion progresiva. Respuesta esperada mayor a 90%.","C":"Glomerulonefritis membranosa, corticoides + ciclosporina","D":"Nefropatia diabetica"}'::jsonb,
'B',
'La respuesta correcta es B. Sindrome nefrotico en NINO 1-8 anos = ENFERMEDAD DE CAMBIOS MINIMOS (90%). Criterios: proteinuria mayor a 3.5 g/24h o mayor a 40 mg/m2/h + hipoalbuminemia menor a 3 + edema + hiperlipidemia. Tratamiento EMPIRICO SIN BIOPSIA (alta probabilidad): PREDNISONA 60 mg/m2/dia (o 2 mg/kg/dia, max 60mg) VO x 4-6 semanas, respuesta mayor a 90% (remision proteinuria en 2-3 semanas). Reduccion gradual. BIOPSIA si: no responde a esteroides 4-6 semanas, edad menor a 1 o mayor a 12, HTA, hematuria, complemento bajo, IRA. Complicaciones: infeccion (peritonitis por neumococo), TROMBOSIS (albumina baja, PROFILAXIS si menor a 2).',
ARRAY['IMSS-241-12']::text[], TRUE)
ON CONFLICT DO NOTHING;

-- ---------- MCQ 6: Nefrolitiasis ----------
INSERT INTO mcq_bank (specialty, difficulty, topic, vignette, question, options_json, correct_answer, explanation_correct, guidelines_ids, is_active)
VALUES ('Nefrologia', 'intermediate', 'Litiasis',
'Hombre 40 anos con dolor lumbar tipo COLICO irradiado a genitales, hematuria, nausea. USG: litiasis ureteral derecha de 6mm. Hidronefrosis leve. Creatinina normal, sin fiebre.',
'Cual es el manejo?',
'{"A":"Litotricia inmediata","B":"MANEJO CONSERVADOR (litos menor a 10mm expulsan espontaneamente 80%): analgesia (AINE ketorolaco IV o VO, opioide si severo) + hidratacion + TAMSULOSINA 0.4 mg VO/dia (alfa-bloqueador facilita expulsion, terapia expulsiva medica) + tamizar orina para recuperar lito","C":"Cirugia abierta","D":"Nefrostomia"}'::jsonb,
'B',
'La respuesta correcta es B. Litiasis ureteral menor a 5mm expulsan 90%, 5-10mm 50-70%, mayor a 10mm menor a 20%. Sin complicaciones (fiebre, IRA, sepsis, obstruccion severa) = MANEJO CONSERVADOR: 1) ANALGESIA con AINE (ketorolaco IV 30mg o diclofenaco VO, primera linea, efectivos) + opioides si dolor severo, 2) HIDRATACION oral (2-3 L/dia), 3) TAMSULOSINA 0.4 mg VO/dia (relaja musculo liso ureteral, aumenta expulsion), 4) Tamizar orina para analizar composicion del lito. INTERVENCION (litotricia extracorporea LEO o ureteroscopia) si: mayor a 10mm, obstruccion 4-6 semanas sin expulsar, complicaciones. URGENCIA: pielonefritis obstructiva (nefrostomia o cateter JJ).',
ARRAY['IMSS-244-10']::text[], TRUE)
ON CONFLICT DO NOTHING;

-- ---------- MCQ 7: Hiponatremia ----------
INSERT INTO mcq_bank (specialty, difficulty, topic, vignette, question, options_json, correct_answer, explanation_correct, guidelines_ids, is_active)
VALUES ('Nefrologia', 'advanced', 'Hiponatremia',
'Mujer 65 anos con cancer pulmonar. Na serico 118 mEq/L. Osmolaridad plasmatica 245 (baja), osmolaridad urinaria 480 (alta), Na urinario 65 (alto). Euvolemica clinicamente. Uratos 2.1 (bajos). Confusion leve.',
'Cual es el diagnostico y manejo?',
'{"A":"Hiponatremia hipovolemica, SF 0.9%","B":"SIADH (secrecion inapropiada ADH) por cancer pulmonar. Manejo: RESTRICCION de agua libre (800-1000 mL/dia), tabletas de sal, DEMECLOCICLINA o TOLVAPTAN si refractario. Corregir Na LENTO (menor a 8-10 mEq/L en 24h para evitar MIELINOLISIS PONTINA)","C":"Hiperhidratacion, diureticos","D":"Insuficiencia adrenal, hidrocortisona"}'::jsonb,
'B',
'La respuesta correcta es B. Hiponatremia EUVOLEMICA + osm plasmatica baja + osm urinaria alta + Na urinario alto + acido urico bajo = SIADH. Causas: neoplasias (pulmon oat cell, mas frecuente), SNC (ACV, tumor, meningitis), pulmonar (neumonia, TB), farmacos (ISRS, carbamazepina, ciclofosfamida). MANEJO: 1) TRATAR CAUSA, 2) RESTRICCION LIQUIDOS 800-1000 mL/dia (base), 3) Tabletas sal + urea, 4) Antagonistas ADH: DEMECLOCICLINA, TOLVAPTAN (vaptanes) si refractario. CORRECCION LENTA (mayor a 24h): NO mayor a 8-10 mEq/L en 24h por RIESGO MIELINOLISIS PONTINA CENTRAL (desmielinizacion irreversible). Salina hipertonica 3% solo si sintomas neurologicos SEVEROS (convulsiones, coma).',
ARRAY['IMSS-244-10']::text[], TRUE)
ON CONFLICT DO NOTHING;

-- ---------- MCQ 8: ITU en embarazo ----------
INSERT INTO mcq_bank (specialty, difficulty, topic, vignette, question, options_json, correct_answer, explanation_correct, guidelines_ids, is_active)
VALUES ('Nefrologia', 'intermediate', 'ITU embarazo',
'Embarazada 22 SDG con urocultivo tamizaje positivo E. coli mayor a 10^5 UFC/mL. ASINTOMATICA. Sensible a nitrofurantoina, cefalexina, TMP-SMX.',
'Cual es la conducta correcta?',
'{"A":"Solo observacion","B":"TRATAR bacteriuria asintomatica en embarazo (riesgo pielonefritis, parto pretermino, RN bajo peso): NITROFURANTOINA 100 mg VO cada 6h por 5-7 dias (evitar tercer trimestre) O CEFALEXINA 500 mg cada 6h por 7 dias. UROCULTIVO CONTROL 1 semana post-tratamiento","C":"Solo si sintomatica","D":"Fluconazol"}'::jsonb,
'B',
'La respuesta correcta es B. BACTERIURIA ASINTOMATICA EN EMBARAZO (mayor a 10^5 UFC/mL en 1-2 urocultivos) SI SE TRATA (unico grupo donde el tratamiento asintomatico esta indicado por: 30% progresa a pielonefritis, riesgo parto pretermino, RN bajo peso). Tamizaje universal 12-16 SDG. Farmacos SEGUROS: NITROFURANTOINA (categoria B, EVITAR ultimas 4 semanas por riesgo hemolisis neonatal), CEFALEXINA, FOSFOMICINA, amoxicilina. EVITAR: fluoroquinolonas (cartilago fetal), TMP-SMX (kernicterus 3er trim, defectos tubo neural 1er trim), tetraciclinas (dientes). UROCULTIVO CONTROL 1-2 semanas post-tratamiento. Repetir mensual hasta parto.',
ARRAY['IMSS-078-08']::text[], TRUE)
ON CONFLICT DO NOTHING;

-- ---------- MCQ 9: Sindrome hepatorrenal ----------
INSERT INTO mcq_bank (specialty, difficulty, topic, vignette, question, options_json, correct_answer, explanation_correct, guidelines_ids, is_active)
VALUES ('Nefrologia', 'advanced', 'SHR',
'Hombre 58 anos con cirrosis Child C y ascitis refractaria. Creatinina subio de 1.2 a 2.5 en 5 dias tras paracentesis (sin albumina). Oliguria. Na urinario 8 (bajo), sedimento urinario limpio, sin proteinuria. Sin nefrotoxicos. USG renal normal.',
'Cual es el diagnostico + tratamiento?',
'{"A":"Nefritis intersticial, corticoides","B":"SINDROME HEPATORRENAL tipo 1 (LRA en cirrotico sin causa alterna). Tratamiento: TERLIPRESINA 1-2 mg IV cada 4-6h + ALBUMINA 1 g/kg dia 1, luego 20-40 g/dia + suspender diureticos + TIPS o TRASPLANTE HEPATICO definitivo","C":"NTA por hipovolemia, cristaloides","D":"Uropatia obstructiva, cateterizar"}'::jsonb,
'B',
'La respuesta correcta es B. SHR criterios: cirrosis + ascitis + creatinina mayor a 1.5 (LRA en menos de 48h en tipo 1) + SIN causa alterna (sin shock, nefrotoxicos, sedimento activo, USG normal) + SIN mejoria tras retiro diureticos y expansion volumen con albumina 1g/kg x 2 dias. TIPO 1: rapido, mortalidad 80% sin tratamiento. TIPO 2: mas gradual con ascitis refractaria. TRATAMIENTO: 1) TERLIPRESINA 1-2 mg IV c/4-6h (vasoconstrictor esplacnico, aumenta presion perfusion renal), 2) ALBUMINA 1 g/kg dia 1, luego 20-40 g/dia, 3) SUSPENDER diureticos + betabloqueadores no selectivos, 4) TIPS (transjugular intrahepatico portosistemico) puente, 5) TRASPLANTE HEPATICO = unico definitivo.',
ARRAY['IMSS-395-10']::text[], TRUE)
ON CONFLICT DO NOTHING;

-- ---------- MCQ 10: Acidosis metabolica brecha aumentada ----------
INSERT INTO mcq_bank (specialty, difficulty, topic, vignette, question, options_json, correct_answer, explanation_correct, guidelines_ids, is_active)
VALUES ('Nefrologia', 'advanced', 'Acidosis metabolica',
'Mujer 45 anos con acidosis metabolica: pH 7.20, HCO3 10, PaCO2 22, Na 138, Cl 100. Calculo brecha anionica: 138 - (100 + 10) = 28 (elevada mayor a 12).',
'Cual es el diagnostico diferencial de acidosis metabolica con BRECHA ANIONICA AUMENTADA?',
'{"A":"Diarrea, acidosis tubular renal","B":"Nemotecnia MUDPILES/GOLD MARK: Metanol, Uremia, DKA (cetoacidosis diabetica), Paraldehido, Iron/Isoniazida, Lactato, Etilenglicol, Salicilatos. OTROS: acidosis lactica (sepsis, isquemia, metformina), cetoacidosis alcoholica","C":"Vomitos, hipercalemia","D":"Hiperventilacion primaria"}'::jsonb,
'B',
'La respuesta correcta es B. Acidosis metabolica CON BRECHA ANIONICA AUMENTADA (mayor a 12): nemotecnia MUDPILES o GOLD MARK. Causas: Metanol/etilenglicol (intoxicaciones, cristales oxalato en orina), Uremia (LRA/ERC avanzada), Diabetic Ketoacidosis (CAD), Paraldehido/propilenglicol, Iron/INH (isoniazida), LACTATO (sepsis, shock, isquemia mesenterica, METFORMINA, biguanidas), Etilenglicol, Salicilatos. Cetoacidosis ALCOHOLICA. Vs acidosis con brecha NORMAL (hiperclorémica): perdidas GI (DIARREA), acidosis tubular renal (ATR), inhibidores anhidrasa carbonica (acetazolamida), amonio/cloruro exogenos. Manejo: TRATAR CAUSA, bicarbonato solo si pH menor a 7.1-7.2 o especifico.',
ARRAY['IMSS-244-10']::text[], TRUE)
ON CONFLICT DO NOTHING;

COMMIT;

SELECT COUNT(*) AS total_nefrologia FROM mcq_bank WHERE specialty = 'Nefrologia';
