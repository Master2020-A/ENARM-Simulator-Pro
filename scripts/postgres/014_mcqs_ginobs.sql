-- ================================================================
-- 014_mcqs_ginobs.sql
-- Sprint 7A.3: 10 MCQs de Gineco-Obstetricia estilo ENARM
-- ================================================================

BEGIN;

-- ---------- MCQ 1: Preeclampsia severa ----------
INSERT INTO mcq_bank (specialty, difficulty, topic, vignette, question, options_json, correct_answer, explanation_correct, guidelines_ids, is_active)
VALUES ('Gineco-Obstetricia', 'advanced', 'Preeclampsia',
'Primigesta 32 SDG con cefalea, vision borrosa, dolor epigastrico. TA 170/115. Proteinuria +++. Plaquetas 85,000. AST 180. Hiperreflexia. Sin convulsiones.',
'Cual es el manejo COMPLETO inmediato?',
'{"A":"Solo antihipertensivo VO","B":"Sulfato de magnesio (impregnacion 4-6g IV + mantenimiento 1-2g/h) + antihipertensivo IV (hidralazina o labetalol) + betametasona + programar interrupcion","C":"Cesarea de urgencia inmediata sin estabilizar","D":"Reposo y observacion 24h"}'::jsonb,
'B',
'La respuesta correcta es B. Preeclampsia SEVERA con criterios (TA mayor o igual 160/110, plaquetas menor a 100k, ELEVACION transaminasas, cefalea, vision borrosa, epigastralgia) requiere: 1) SULFATO DE MAGNESIO (previene eclampsia), 2) ANTIHIPERTENSIVO IV (hidralazina 5-10mg IV, labetalol, nifedipino VO) meta TAS 140-155, 3) BETAMETASONA 12mg IM 2 dosis (maduracion pulmonar si menor a 34 SDG), 4) INTERRUPCION del embarazo tras estabilizacion. Tambien: HELLP presente (plaquetopenia + AST alta).',
ARRAY['IMSS-020-08']::text[], TRUE)
ON CONFLICT DO NOTHING;

-- ---------- MCQ 2: DPPNI ----------
INSERT INTO mcq_bank (specialty, difficulty, topic, vignette, question, options_json, correct_answer, explanation_correct, guidelines_ids, is_active)
VALUES ('Gineco-Obstetricia', 'advanced', 'DPPNI',
'Multipara 36 SDG post-caida, presenta dolor abdominal INTENSO CONTINUO, sangrado transvaginal moderado OSCURO, utero HIPERTONICO/leñoso. FCF 90 lpm con desaceleraciones tardias. TA 100/60.',
'Cual es el diagnostico y conducta?',
'{"A":"Placenta previa, cesarea electiva","B":"Desprendimiento prematuro de placenta (DPPNI) grado III, CESAREA URGENTE","C":"Ruptura uterina, laparotomia","D":"Inicio de trabajo de parto, esperar"}'::jsonb,
'B',
'La respuesta correcta es B. DPPNI: dolor abdominal continuo + sangrado OSCURO + utero LEÑOSO/HIPERTONICO + sufrimiento fetal. Trauma es factor de riesgo. GRADO III (compromiso fetal severo) requiere CESAREA URGENTE ademas de estabilizacion materna (2 vias IV, cruzar sangre por CID). Placenta previa (A) tiene sangrado ROJO INDOLORO. Ruptura uterina (C) presenta cese de dinamica + partes fetales palpables. Complicaciones DPPNI: shock, CID, muerte fetal, insuficiencia renal.',
ARRAY['IMSS-162-09']::text[], TRUE)
ON CONFLICT DO NOTHING;

-- ---------- MCQ 3: Hemorragia postparto ----------
INSERT INTO mcq_bank (specialty, difficulty, topic, vignette, question, options_json, correct_answer, explanation_correct, guidelines_ids, is_active)
VALUES ('Gineco-Obstetricia', 'intermediate', 'HPP',
'Primipara postparto vaginal hace 30 min, sangrado activo 1,200 mL. Utero blando (no contraido), placenta completa. TA 85/50, FC 128.',
'Cual es el manejo INICIAL sistematico?',
'{"A":"Solo transfundir","B":"MASAJE uterino bimanual + OXITOCINA 40 UI en 1L + acceso 2 vias + cristaloides + tipificar sangre + preparar 2do escalon (metilergonovina, misoprostol, carbetocina)","C":"Histerectomia inmediata","D":"Legrado uterino"}'::jsonb,
'B',
'La respuesta correcta es B. HPP por ATONIA UTERINA (causa mas frecuente 70%). Manejo escalonado: 1) MASAJE UTERINO + OXITOCINA 40 UI/L IV, 2) 2 vias + cristaloides + tipificar/cruzar, 3) 2do UTEROTONICO: metilergonovina 0.2mg IM (contraind HTA), carbetocina, misoprostol 800-1000mcg rectal, 4) BALON HIDROSTATICO intrauterino (Bakri), 5) SUTURAS COMPRESIVAS (B-Lynch), 6) LIGADURA ARTERIAS uterinas/hipogastricas, 7) HISTERECTOMIA (ultimo recurso). Regla 4T: Tono, Trauma, Tejido, Trombina.',
ARRAY['IMSS-103-08']::text[], TRUE)
ON CONFLICT DO NOTHING;

-- ---------- MCQ 4: Diabetes gestacional ----------
INSERT INTO mcq_bank (specialty, difficulty, topic, vignette, question, options_json, correct_answer, explanation_correct, guidelines_ids, is_active)
VALUES ('Gineco-Obstetricia', 'intermediate', 'DM gestacional',
'Embarazada 26 SDG, CTOG 75g: basal 98, 1h 195, 2h 165. Sin diabetes previa. IMC 32.',
'Cual es el diagnostico y manejo INICIAL?',
'{"A":"Diabetes gestacional + insulina inmediata","B":"Diabetes gestacional + dieta y ejercicio 2 semanas, si persiste insulina","C":"Metformina","D":"Normal, repetir en 6 semanas"}'::jsonb,
'B',
'La respuesta correcta es B. Criterios IADPSG (OMS 2013) para DG con CTOG 75g: BASAL mayor o igual 92, 1h mayor o igual 180, 2h mayor o igual 153. Basta UNO alterado. Esta paciente cumple criterios. Manejo INICIAL: DIETA (carbohidratos 40-45%, 3 comidas + 2-3 colaciones) + EJERCICIO (30 min/dia). Si mayor a 30% mediciones fuera de meta en 1-2 semanas (basal <95, 1h <140, 2h <120): INSULINA (de eleccion). Metformina (C) alternativa en algunos casos. NUNCA hipoglucemiantes orales excepto metformina.',
ARRAY['IMSS-320-10']::text[], TRUE)
ON CONFLICT DO NOTHING;

-- ---------- MCQ 5: Amenaza de parto pretermino ----------
INSERT INTO mcq_bank (specialty, difficulty, topic, vignette, question, options_json, correct_answer, explanation_correct, guidelines_ids, is_active)
VALUES ('Gineco-Obstetricia', 'intermediate', 'APP',
'Embarazada 30 SDG con contracciones regulares cada 5 min, dolorosas. Cervix 2 cm dilatado, borrado 60%. Membranas integras. Sin sangrado. FCF normal.',
'Cual es el manejo?',
'{"A":"Cesarea de urgencia","B":"Nifedipino 20mg VO carga (tocolisis) + betametasona 12mg IM cada 24h x 2 dosis (maduracion pulmonar) + sulfato de magnesio (neuroproteccion si menor a 32 SDG)","C":"Solo reposo","D":"Oxitocina para acelerar parto"}'::jsonb,
'B',
'La respuesta correcta es B. Amenaza de parto pretermino (24-34 SDG) con dinamica y cambios cervicales: 1) TOCOLISIS 48h para permitir maduracion pulmonar (nifedipino 1a linea, atosiban, indometacina), 2) CORTICOIDES: BETAMETASONA 12mg IM 2 dosis (reduce SDR neonatal, HIV, ECN), 3) SULFATO DE MAGNESIO si menor a 32 SDG (neuroproteccion, reduce paralisis cerebral), 4) ATB si sospecha RPM o corioamnionitis. NO cesarea (A) salvo indicacion obstetrica.',
ARRAY['IMSS-063-08']::text[], TRUE)
ON CONFLICT DO NOTHING;

-- ---------- MCQ 6: Cancer cervicouterino tamizaje ----------
INSERT INTO mcq_bank (specialty, difficulty, topic, vignette, question, options_json, correct_answer, explanation_correct, guidelines_ids, is_active)
VALUES ('Gineco-Obstetricia', 'intermediate', 'CaCU tamizaje',
'Mujer 30 anos, asintomatica, activa sexualmente desde los 18 anos, 3 parejas. Nunca vacunada contra VPH.',
'Cual es la recomendacion de TAMIZAJE de CaCU?',
'{"A":"Citologia cervical anual desde los 21","B":"Prueba de VPH (o co-test VPH+citologia) cada 5 anos de 30-65 anos, O citologia sola cada 3 anos si <30","C":"Solo colposcopia","D":"Legrado endocervical"}'::jsonb,
'B',
'La respuesta correcta es B. Recomendaciones actualizadas (ACS 2020, USPSTF 2018): 21-29 anos citologia cada 3 anos. 30-65 anos: prueba de VPH cada 5 anos (preferida) O co-test (VPH+citologia) cada 5 anos O citologia cada 3 anos. Suspender si mayor a 65 con 3 negativos previos. Colposcopia (C) es DIAGNOSTICA (post-anormalidad), no tamizaje. Vacunacion VPH ideal 9-14 anos, hasta 26 anos.',
ARRAY['IMSS-146-08']::text[], TRUE)
ON CONFLICT DO NOTHING;

-- ---------- MCQ 7: EPI ----------
INSERT INTO mcq_bank (specialty, difficulty, topic, vignette, question, options_json, correct_answer, explanation_correct, guidelines_ids, is_active)
VALUES ('Gineco-Obstetricia', 'intermediate', 'EPI',
'Mujer 25 anos con dolor pelvico bilateral, flujo purulento, dispareunia, fiebre 38.5, dolor a la movilizacion cervical. Sin embarazo (BhCG negativo). Sexualmente activa multiple parejas.',
'Cual es el tratamiento ambulatorio de EPI?',
'{"A":"Ceftriaxona 500mg IM DU + doxiciclina 100mg VO cada 12h por 14 dias + metronidazol 500mg VO cada 12h por 14 dias","B":"Solo azitromicina 1g VO","C":"Amoxicilina 500mg cada 8h","D":"Fluconazol 150mg DU"}'::jsonb,
'A',
'La respuesta correcta es A. EPI (enfermedad pelvica inflamatoria) requiere cobertura polimicrobiana: N. GONORRHOEAE (ceftriaxona), C. TRACHOMATIS (doxiciclina), ANAEROBIOS (metronidazol). Duracion 14 dias. HOSPITALIZACION si: embarazo, absceso tubo-ovarico, sepsis, intolerancia VO, falla ambulatorio. Estudiar y tratar TAMBIEN a parejas sexuales. Serologia VIH/VDRL. Sin tratamiento: infertilidad, embarazo ectopico, dolor cronico.',
ARRAY['IMSS-081-08']::text[], TRUE)
ON CONFLICT DO NOTHING;

-- ---------- MCQ 8: Menopausia ----------
INSERT INTO mcq_bank (specialty, difficulty, topic, vignette, question, options_json, correct_answer, explanation_correct, guidelines_ids, is_active)
VALUES ('Gineco-Obstetricia', 'intermediate', 'Menopausia',
'Mujer 52 anos, amenorrea 14 meses, bochornos severos que interfieren con sueno, sequedad vaginal, artralgias. Sin cancer mama o utero. Sin antecedente TEV.',
'Cual es la conducta apropiada?',
'{"A":"Solo terapia no farmacologica","B":"Terapia hormonal (estrogeno + progestagenos ya que tiene utero) menor dosis efectiva menor tiempo, con reevaluacion anual","C":"Antidepresivos siempre","D":"Testosterona"}'::jsonb,
'B',
'La respuesta correcta es B. Sintomas VASOMOTORES moderados-severos + genitourinarios en menopausia (menor a 60 anos o menor a 10 anos post-menopausia): TERAPIA HORMONAL de eleccion. Estrogeno + PROGESTAGENO (si utero presente para evitar hiperplasia). MENOR DOSIS EFECTIVA por MENOR TIEMPO POSIBLE, con reevaluacion anual. Contraindicaciones: Ca mama, Ca endometrio, TEV/TVP, hepatopatia grave, sangrado vaginal no diagnosticado. Alternativas: ISRS (venlafaxina), gabapentina, estrogenos topicos vaginales.',
ARRAY['IMSS-020-08']::text[], TRUE)
ON CONFLICT DO NOTHING;

-- ---------- MCQ 9: Sangrado uterino anormal ----------
INSERT INTO mcq_bank (specialty, difficulty, topic, vignette, question, options_json, correct_answer, explanation_correct, guidelines_ids, is_active)
VALUES ('Gineco-Obstetricia', 'intermediate', 'SUA postmenopausia',
'Mujer 60 anos, postmenopausia 8 anos, presenta sangrado vaginal escaso pero repetido en las ultimas 3 semanas. Nunca terapia hormonal. Sin traumatismo. USG endometrio 8 mm.',
'Cual es el ESTUDIO diagnostico prioritario?',
'{"A":"Solo repetir USG en 3 meses","B":"Biopsia endometrial (aspiracion o histeroscopia) - descartar cancer endometrial","C":"Anticonceptivos orales","D":"Colposcopia"}'::jsonb,
'B',
'La respuesta correcta es B. Sangrado postmenopausico + endometrio mayor a 4-5 mm es SOSPECHOSO DE CANCER ENDOMETRIAL (adenocarcinoma tipo endometrioide es el mas comun). BIOPSIA ENDOMETRIAL es OBLIGATORIA (aspiracion con canula Pipelle o histeroscopia con biopsia dirigida). Sensibilidad 90-95%. Factores riesgo: obesidad, DM, HTA, nuliparidad, menopausia tardia, tamoxifeno, Lynch. Observar (A) es NEGLIGENCIA. Anticonceptivos (C) en postmenopausia sin indicacion.',
ARRAY['IMSS-146-08']::text[], TRUE)
ON CONFLICT DO NOTHING;

-- ---------- MCQ 10: Ruptura prematura membranas ----------
INSERT INTO mcq_bank (specialty, difficulty, topic, vignette, question, options_json, correct_answer, explanation_correct, guidelines_ids, is_active)
VALUES ('Gineco-Obstetricia', 'intermediate', 'RPM pretermino',
'Embarazada 32 SDG con salida de liquido claro por vagina hace 12h. Sin dinamica uterina. Sin fiebre. FCF normal. Especulo: LA en fondo de saco, cristalografia +. Sin datos de corioamnionitis.',
'Cual es el manejo?',
'{"A":"Interrupcion inmediata","B":"Manejo expectante hasta 34 SDG + ATB profilactico (ampi+eritro o azitromicina 7 dias) + betametasona + vigilancia infecciosa","C":"Solo tocolisis prolongada","D":"Alta con observacion domiciliaria"}'::jsonb,
'B',
'La respuesta correcta es B. RPM pretermino (24-33+6 SDG) sin corioamnionitis: MANEJO EXPECTANTE hasta 34 SDG (balance riesgo prematurez vs infeccion). 1) HOSPITALIZAR, 2) ATB para latencia y profilaxis GBS (ampicilina 2g IV c/6h + eritromicina 250mg VO c/6h por 7 dias, o azitromicina), 3) CORTICOIDES betametasona 12mg IM x 2, 4) VIGILANCIA de corioamnionitis (fiebre, taquicardia materna/fetal, dolor uterino, PCR), 5) NO tocolisis prolongada (C). Si corioamnionitis: interrupcion inmediata + ATB IV amplio espectro.',
ARRAY['IMSS-063-08']::text[], TRUE)
ON CONFLICT DO NOTHING;

COMMIT;

SELECT COUNT(*) AS total_ginobs FROM mcq_bank WHERE specialty = 'Gineco-Obstetricia';
