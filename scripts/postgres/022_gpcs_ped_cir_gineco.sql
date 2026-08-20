-- ================================================================
-- 022_gpcs_ped_cir_gineco.sql
-- Sprint 7A.4.3: 12 GPCs pediatricas, cirugia, gineco, endocrinologia
-- ================================================================

BEGIN;

-- ========== IMSS-063-08: Pediatria General ==========
INSERT INTO clinical_guidelines (id, title, organization, year, url, specialty, summary, key_recommendations, tags)
VALUES ('IMSS-063-08',
'Control del Nino Sano y Atencion Integrada',
'IMSS', 2020, 'http://www.cenetec.salud.gob.mx',
'Pediatria',
'Guia de atencion pediatrica integrada. Cubre vacunacion, alimentacion, deteccion oportuna de displasia de cadera, tamizaje neonatal, anemia, sobrepeso infantil.',
'[
  {"level":"A","text":"Tamizaje universal DDC al nacer con Ortolani/Barlow, USG si factores riesgo"},
  {"level":"A","text":"Vacunacion segun esquema nacional (BCG y HepB al nacer, hexavalente + rotavirus + neumococo a 2m/4m/6m)"},
  {"level":"A","text":"Lactancia materna exclusiva 6 meses, complementaria hasta 2 anos"},
  {"level":"A","text":"Tamizaje anemia ferropenica 12 meses (Hb <11 = anemia)"},
  {"level":"B","text":"Tamizaje neonatal ampliado (TSH, PKU, hipoplasia adrenal, fibrosis quistica)"}
]'::jsonb,
ARRAY['pediatria','nino sano','vacunas','DDC','anemia','lactancia']::text[])
ON CONFLICT (id) DO UPDATE SET
  specialty = EXCLUDED.specialty, summary = EXCLUDED.summary,
  key_recommendations = EXCLUDED.key_recommendations, tags = EXCLUDED.tags;

-- ========== IMSS-156-08: Diarrea aguda pediatrica ==========
INSERT INTO clinical_guidelines (id, title, organization, year, url, specialty, summary, key_recommendations, tags)
VALUES ('IMSS-156-08',
'Prevencion, Diagnostico y Tratamiento de la Diarrea Aguda en Pediatricos',
'IMSS', 2019, 'http://www.cenetec.salud.gob.mx',
'Pediatria',
'Diarrea aguda es principalmente viral (rotavirus). Rehidratacion oral con SRO es piedra angular. Zinc reduce duracion. Evitar antidiarreicos en pediatricos.',
'[
  {"level":"A","text":"Rehidratacion oral con SRO/vida suero oral (75 mL/kg en 4h) en deshidratacion algun grado"},
  {"level":"A","text":"Zinc 20 mg/dia (10 mg si <6m) por 10-14 dias reduce duracion e intensidad"},
  {"level":"A","text":"Alimentacion CONTINUA (no ayuno), incluida leche materna"},
  {"level":"A","text":"NO usar loperamida en pediatricos (contraindicada)"},
  {"level":"B","text":"Antibioticos SOLO en disenteria, colera, inmunocomprometido, sepsis"}
]'::jsonb,
ARRAY['diarrea','rehidratacion','SRO','zinc','pediatria']::text[])
ON CONFLICT (id) DO UPDATE SET
  specialty = EXCLUDED.specialty, summary = EXCLUDED.summary,
  key_recommendations = EXCLUDED.key_recommendations, tags = EXCLUDED.tags;

-- ========== IMSS-524-11: Invaginacion intestinal ==========
INSERT INTO clinical_guidelines (id, title, organization, year, url, specialty, summary, key_recommendations, tags)
VALUES ('IMSS-524-11',
'Diagnostico y Tratamiento de la Invaginacion Intestinal en Pediatricos',
'IMSS', 2016, 'http://www.cenetec.salud.gob.mx',
'Cirugia Pediatrica',
'Triada clasica: llanto paroxistico + masa palpable + heces en jalea de grosella. USG es diagnostico. Reduccion no operatoria (hidrostatica o neumatica) es primera linea si estable.',
'[
  {"level":"A","text":"USG abdominal es diagnostico (signo de dona, target sign)"},
  {"level":"A","text":"Reduccion HIDROSTATICA/NEUMATICA guiada por USG/fluoroscopia si estable, <48h, sin peritonitis"},
  {"level":"A","text":"Cirugia si fracasa reduccion no operatoria, peritonitis, perforacion, choque"},
  {"level":"B","text":"Recurrencia 10-15%, mayoria en primeras 24h post-reduccion"}
]'::jsonb,
ARRAY['invaginacion','intususcepcion','jalea grosella','pediatria','cirugia']::text[])
ON CONFLICT (id) DO UPDATE SET
  specialty = EXCLUDED.specialty, summary = EXCLUDED.summary,
  key_recommendations = EXCLUDED.key_recommendations, tags = EXCLUDED.tags;

-- ========== IMSS-345-10: Cirugia pediatrica general ==========
INSERT INTO clinical_guidelines (id, title, organization, year, url, specialty, summary, key_recommendations, tags)
VALUES ('IMSS-345-10',
'Cirugia Pediatrica: Estenosis Hipertrofica del Piloro, Hernia Inguinal, Malrotacion, Onfalocele/Gastrosquisis, Hirschsprung',
'IMSS', 2018, 'http://www.cenetec.salud.gob.mx',
'Cirugia Pediatrica',
'Compendio de patologias quirurgicas pediatricas mas frecuentes. Enfasis en reconocimiento temprano de emergencias (volvulo, gastrosquisis).',
'[
  {"level":"A","text":"EHP: corregir alcalosis metabolica hipocloremica ANTES de piloromiotomia de Ramstedt"},
  {"level":"A","text":"Hernia inguinal en pediatricos SIEMPRE es indicacion quirurgica electiva"},
  {"level":"A","text":"VOMITO BILIOSO en neonato = emergencia (descartar volvulo con procedimiento de Ladd)"},
  {"level":"A","text":"Hirschsprung: biopsia rectal diagnostica (ausencia de celulas ganglionares)"},
  {"level":"B","text":"Gastrosquisis: cubrir con bolsa esteril + SOG + liquidos abundantes + silo o cierre por etapas"}
]'::jsonb,
ARRAY['EHP','piloro','Ramstedt','hernia','volvulo','Ladd','Hirschsprung','cirugia pediatrica']::text[])
ON CONFLICT (id) DO UPDATE SET
  specialty = EXCLUDED.specialty, summary = EXCLUDED.summary,
  key_recommendations = EXCLUDED.key_recommendations, tags = EXCLUDED.tags;

-- ========== IMSS-347-10: Criptorquidia y testiculo agudo ==========
INSERT INTO clinical_guidelines (id, title, organization, year, url, specialty, summary, key_recommendations, tags)
VALUES ('IMSS-347-10',
'Diagnostico y Tratamiento de Criptorquidia y Escroto Agudo',
'IMSS', 2018, 'http://www.cenetec.salud.gob.mx',
'Cirugia Pediatrica',
'Criptorquidia: descenso testicular fuera de escroto. Orquidopexia 6-18 meses reduce infertilidad y cancer. Torsion testicular es emergencia con ventana de oro <6h.',
'[
  {"level":"A","text":"Orquidopexia entre 6-18 meses (idealmente 12) en criptorquidia"},
  {"level":"A","text":"TORSION TESTICULAR: exploracion quirurgica URGENTE <6h para salvar testiculo"},
  {"level":"A","text":"Ventana oro <6h: viabilidad 90%. >24h: necrosis casi segura"},
  {"level":"A","text":"Orquidopexia BILATERAL (40% anomalia contralateral)"},
  {"level":"C","text":"USG Doppler apoya diagnostico pero NO retrasar cirugia si alta sospecha"}
]'::jsonb,
ARRAY['criptorquidia','torsion testicular','orquidopexia','escroto agudo','pediatria']::text[])
ON CONFLICT (id) DO UPDATE SET
  specialty = EXCLUDED.specialty, summary = EXCLUDED.summary,
  key_recommendations = EXCLUDED.key_recommendations, tags = EXCLUDED.tags;

-- ========== IMSS-031-08: Apendicitis ==========
INSERT INTO clinical_guidelines (id, title, organization, year, url, specialty, summary, key_recommendations, tags)
VALUES ('IMSS-031-08',
'Diagnostico y Tratamiento de la Apendicitis Aguda',
'IMSS', 2020, 'http://www.cenetec.salud.gob.mx',
'Cirugia General',
'Dolor migratorio periumbilical a fosa iliaca derecha es clasico. Score de Alvarado y USG orientan diagnostico. Apendicectomia laparoscopica es abordaje de eleccion.',
'[
  {"level":"A","text":"USG abdominal 1a linea en pediatricos y embarazadas (apendice >6mm no compresible)"},
  {"level":"A","text":"TAC contrastada si USG no concluyente en adultos"},
  {"level":"A","text":"Score de Alvarado o PAS estratifica riesgo"},
  {"level":"A","text":"Antibiotico preoperatorio: cef+metronidazol (adultos) o cef+metronidazol dosis kg (pediatricos)"},
  {"level":"A","text":"Apendicectomia LAPAROSCOPICA es abordaje de eleccion"}
]'::jsonb,
ARRAY['apendicitis','McBurney','Alvarado','cirugia','abdomen agudo']::text[])
ON CONFLICT (id) DO UPDATE SET
  specialty = EXCLUDED.specialty, summary = EXCLUDED.summary,
  key_recommendations = EXCLUDED.key_recommendations, tags = EXCLUDED.tags;

-- ========== IMSS-103-08: HPP ==========
INSERT INTO clinical_guidelines (id, title, organization, year, url, specialty, summary, key_recommendations, tags)
VALUES ('IMSS-103-08',
'Prevencion, Diagnostico y Tratamiento de la Hemorragia Postparto',
'IMSS', 2019, 'http://www.cenetec.salud.gob.mx',
'Gineco-Obstetricia',
'Atonia uterina es la causa mas frecuente de HPP. Manejo escalonado: masaje + uterotonicos + balon + suturas compresivas + histerectomia. Regla 4T: Tono, Trauma, Tejido, Trombina.',
'[
  {"level":"A","text":"Manejo activo del alumbramiento (oxitocina 10 UI IM post-parto) reduce HPP 50%"},
  {"level":"A","text":"Atonia: masaje uterino + OXITOCINA 40 UI/L IV + 2do uterotonico (metilergonovina, carbetocina, misoprostol)"},
  {"level":"B","text":"Balon hidrostatico Bakri si falla uterotonicos"},
  {"level":"B","text":"Suturas compresivas B-Lynch antes de histerectomia"},
  {"level":"A","text":"Acido tranexamico 1g IV en primera hora reduce mortalidad (WOMAN trial)"}
]'::jsonb,
ARRAY['HPP','hemorragia postparto','atonia','oxitocina','ginecologia']::text[])
ON CONFLICT (id) DO UPDATE SET
  specialty = EXCLUDED.specialty, summary = EXCLUDED.summary,
  key_recommendations = EXCLUDED.key_recommendations, tags = EXCLUDED.tags;

-- ========== IMSS-146-08: Cancer cervicouterino ==========
INSERT INTO clinical_guidelines (id, title, organization, year, url, specialty, summary, key_recommendations, tags)
VALUES ('IMSS-146-08',
'Prevencion, Deteccion, Diagnostico y Tratamiento del Cancer Cervicouterino',
'IMSS', 2018, 'http://www.cenetec.salud.gob.mx',
'Gineco-Obstetricia',
'Tamizaje con prueba de VPH o co-test (30-65a) cada 5 anos. Vacunacion VPH ideal 9-14 anos. Colposcopia dirigida en anormalidades.',
'[
  {"level":"A","text":"Tamizaje 21-29 anos: citologia cada 3 anos"},
  {"level":"A","text":"Tamizaje 30-65 anos: prueba VPH cada 5 anos (preferida) o co-test"},
  {"level":"A","text":"Vacunacion VPH ideal 9-14 anos (2 dosis), hasta 26 anos (3 dosis)"},
  {"level":"A","text":"Colposcopia + biopsia en anormalidades (ASC-US con VPH positivo, LSIL, HSIL)"},
  {"level":"B","text":"Suspender tamizaje >65 anos con 3 negativos previos"}
]'::jsonb,
ARRAY['cancer cervicouterino','CaCU','VPH','citologia','colposcopia','ginecologia']::text[])
ON CONFLICT (id) DO UPDATE SET
  specialty = EXCLUDED.specialty, summary = EXCLUDED.summary,
  key_recommendations = EXCLUDED.key_recommendations, tags = EXCLUDED.tags;

-- ========== IMSS-081-08: EPI ==========
INSERT INTO clinical_guidelines (id, title, organization, year, url, specialty, summary, key_recommendations, tags)
VALUES ('IMSS-081-08',
'Diagnostico y Tratamiento de la Enfermedad Pelvica Inflamatoria',
'IMSS', 2019, 'http://www.cenetec.salud.gob.mx',
'Gineco-Obstetricia',
'EPI es infeccion polimicrobiana del tracto genital superior. Tratamiento cubre N. gonorrhoeae + C. trachomatis + anaerobios. Estudiar y tratar parejas sexuales.',
'[
  {"level":"A","text":"Tratamiento ambulatorio: ceftriaxona 500mg IM DU + doxiciclina 100mg VO c/12h x 14d + metronidazol 500mg VO c/12h x 14d"},
  {"level":"A","text":"Hospitalizacion si: embarazo, absceso tubo-ovarico, sepsis, falla ambulatorio"},
  {"level":"A","text":"Estudiar/tratar parejas sexuales y serologia VIH/VDRL"},
  {"level":"B","text":"Sin tratamiento: infertilidad, embarazo ectopico, dolor cronico"}
]'::jsonb,
ARRAY['EPI','enfermedad pelvica inflamatoria','ETS','ginecologia']::text[])
ON CONFLICT (id) DO UPDATE SET
  specialty = EXCLUDED.specialty, summary = EXCLUDED.summary,
  key_recommendations = EXCLUDED.key_recommendations, tags = EXCLUDED.tags;

-- ========== IMSS-093-08: Endocrinologia integral ==========
INSERT INTO clinical_guidelines (id, title, organization, year, url, specialty, summary, key_recommendations, tags)
VALUES ('IMSS-093-08',
'Diagnostico y Tratamiento de Enfermedades Endocrinologicas (DM2, Tiroides, Suprarrenal, Osteoporosis)',
'IMSS', 2020, 'http://www.cenetec.salud.gob.mx',
'Endocrinologia',
'Guia integral endocrinologica. Metformina primera linea DM2. SGLT2i con beneficio CV/renal. Cushing tamizaje con 3 pruebas. Hiperparatiroidismo primario indicaciones quirurgicas.',
'[
  {"level":"A","text":"DM2 primera linea: METFORMINA (contraindicada TFG <30)"},
  {"level":"A","text":"DM2 con ECV o HFrEF: agregar SGLT2i (empagliflozina, dapagliflozina)"},
  {"level":"A","text":"Hipotiroidismo: levotiroxina 1.6 mcg/kg/dia VO en ayunas"},
  {"level":"A","text":"Cushing tamizaje: cortisol libre urinario 24h O supresion dexametasona 1mg O cortisol salival nocturno"},
  {"level":"A","text":"Feocromocitoma: alfa-bloqueo (fenoxibenzamina) 10-14 dias ANTES de beta-bloqueo"},
  {"level":"A","text":"Osteoporosis: bifosfonato oral (alendronato) primera linea + calcio 1200mg + vit D 800 UI"}
]'::jsonb,
ARRAY['DM2','tiroides','Cushing','feocromocitoma','osteoporosis','endocrinologia']::text[])
ON CONFLICT (id) DO UPDATE SET
  specialty = EXCLUDED.specialty, summary = EXCLUDED.summary,
  key_recommendations = EXCLUDED.key_recommendations, tags = EXCLUDED.tags;

-- ========== IMSS-302-10: CAD pediatrica ==========
INSERT INTO clinical_guidelines (id, title, organization, year, url, specialty, summary, key_recommendations, tags)
VALUES ('IMSS-302-10',
'Diagnostico y Tratamiento de la Cetoacidosis Diabetica en Pediatricos',
'IMSS', 2018, 'http://www.cenetec.salud.gob.mx',
'Endocrinologia Pediatrica',
'CAD pediatrica requiere protocolo estricto: liquidos primero, luego insulina en infusion (SIN bolo por riesgo edema cerebral), potasio, dextrosa cuando glucosa <250.',
'[
  {"level":"A","text":"Hidratacion PRIMERO: SF 0.9% 10-20 mL/kg en 1a hora, luego SF 0.45% + reposicion"},
  {"level":"A","text":"Insulina EN INFUSION 0.05-0.1 UI/kg/h SIN BOLO en pediatricos (bolo aumenta riesgo edema cerebral)"},
  {"level":"A","text":"Iniciar insulina 1 hora DESPUES de comenzar liquidos"},
  {"level":"A","text":"Potasio (KCl 20-40 mEq/L) cuando K <5.5 y hay diuresis"},
  {"level":"B","text":"Dextrosa cuando glucosa <250 (evita hipoglucemia)"},
  {"level":"C","text":"Bicarbonato SOLO si pH <6.9 (controversial)"},
  {"level":"A","text":"EDEMA CEREBRAL: complicacion mas temida. Descenso lento de osmolaridad"}
]'::jsonb,
ARRAY['CAD','cetoacidosis','DM1','pediatria','endocrinologia','edema cerebral']::text[])
ON CONFLICT (id) DO UPDATE SET
  specialty = EXCLUDED.specialty, summary = EXCLUDED.summary,
  key_recommendations = EXCLUDED.key_recommendations, tags = EXCLUDED.tags;

-- ========== IMSS-320-10: Diabetes gestacional ==========
INSERT INTO clinical_guidelines (id, title, organization, year, url, specialty, summary, key_recommendations, tags)
VALUES ('IMSS-320-10',
'Diagnostico y Tratamiento de la Diabetes Gestacional',
'IMSS', 2019, 'http://www.cenetec.salud.gob.mx',
'Gineco-Obstetricia',
'Tamizaje universal 24-28 SDG con CTOG 75g. Criterios IADPSG: basta un valor alterado. Manejo escalonado: dieta+ejercicio, luego insulina si no controla.',
'[
  {"level":"A","text":"Tamizaje universal con CTOG 75g a 24-28 SDG"},
  {"level":"A","text":"Criterios IADPSG: basal >=92, 1h >=180, 2h >=153 (uno alterado = DG)"},
  {"level":"A","text":"Manejo inicial: DIETA + EJERCICIO 1-2 semanas"},
  {"level":"A","text":"Insulina si >30% mediciones fuera de meta (basal <95, 1h <140, 2h <120)"},
  {"level":"B","text":"Metformina alternativa en algunos casos, no sulfonilureas"}
]'::jsonb,
ARRAY['diabetes gestacional','CTOG','IADPSG','embarazo','endocrinologia']::text[])
ON CONFLICT (id) DO UPDATE SET
  specialty = EXCLUDED.specialty, summary = EXCLUDED.summary,
  key_recommendations = EXCLUDED.key_recommendations, tags = EXCLUDED.tags;

COMMIT;

SELECT COUNT(*) AS total_guidelines FROM clinical_guidelines;
