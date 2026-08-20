-- ================================================================
-- 024_gpcs_adicionales.sql
-- Sprint 7A.4.5: 7 GPCs adicionales (neuro especializada, dengue, C. difficile)
-- ================================================================

BEGIN;

-- ========== IMSS-105-08: Cefaleas ==========
INSERT INTO clinical_guidelines (id, title, organization, year, url, specialty, summary, key_recommendations, tags)
VALUES ('IMSS-105-08',
'Diagnostico y Tratamiento de las Cefaleas Primarias y Secundarias',
'IMSS', 2018, 'http://www.cenetec.salud.gob.mx',
'Neurologia',
'Migrana, tensional y cluster son primarias. Red flags (SNOOP4) obligan a descartar HSA, tumor, meningitis. Triptanes en migrana moderada-severa; profilaxis si >4 crisis/mes.',
'[
  {"level":"A","text":"Migrana moderada-severa: triptanes (sumatriptan) + AINE (naproxeno) primera linea"},
  {"level":"A","text":"Migrana leve: paracetamol o AINE"},
  {"level":"A","text":"Profilaxis migrana (>=4 crisis/mes): propranolol, topiramato, amitriptilina"},
  {"level":"A","text":"RED FLAGS cefalea (SNOOP4): edad >50 nueva, focales, subita/thunderclap, cambio patron, positional, papiledema"},
  {"level":"A","text":"Red flags: TAC craneo urgente (descartar HSA, tumor, HTIC)"},
  {"level":"B","text":"OPIOIDES CONTRAINDICADOS en cefalea primaria (dependencia, rebote)"}
]'::jsonb,
ARRAY['cefalea','migrana','triptanes','red flags','SNOOP4','neurologia']::text[])
ON CONFLICT (id) DO UPDATE SET
  specialty = EXCLUDED.specialty, summary = EXCLUDED.summary,
  key_recommendations = EXCLUDED.key_recommendations, tags = EXCLUDED.tags;

-- ========== IMSS-089-08: Guillain-Barre ==========
INSERT INTO clinical_guidelines (id, title, organization, year, url, specialty, summary, key_recommendations, tags)
VALUES ('IMSS-089-08',
'Diagnostico y Tratamiento del Sindrome de Guillain-Barre',
'IMSS', 2016, 'http://www.cenetec.salud.gob.mx',
'Neurologia',
'Debilidad ascendente simetrica arreflexica post-infeccion. LCR con disociacion albumino-citologica. IGIV o plasmaferesis igual eficacia. CORTICOIDES NO efectivos.',
'[
  {"level":"A","text":"IGIV 0.4 g/kg/dia x 5 dias O plasmaferesis (5 sesiones), igual eficacia"},
  {"level":"A","text":"CORTICOIDES NO SON EFECTIVOS (no usar)"},
  {"level":"A","text":"VIGILAR capacidad vital. Si <20 mL/kg o insuficiencia respiratoria = INTUBACION"},
  {"level":"A","text":"Profilaxis TROMBOSIS con HBPM (inmovilidad)"},
  {"level":"B","text":"Fisioterapia temprana previene contracturas"}
]'::jsonb,
ARRAY['Guillain-Barre','GBS','polineuropatia','IGIV','plasmaferesis','neurologia']::text[])
ON CONFLICT (id) DO UPDATE SET
  specialty = EXCLUDED.specialty, summary = EXCLUDED.summary,
  key_recommendations = EXCLUDED.key_recommendations, tags = EXCLUDED.tags;

-- ========== IMSS-305-10: Parkinson ==========
INSERT INTO clinical_guidelines (id, title, organization, year, url, specialty, summary, key_recommendations, tags)
VALUES ('IMSS-305-10',
'Diagnostico y Tratamiento de la Enfermedad de Parkinson',
'IMSS', 2019, 'http://www.cenetec.salud.gob.mx',
'Neurologia',
'Diagnostico clinico (bradicinesia + temblor de reposo o rigidez). Levodopa mas efectiva pero causa disquinesias a largo plazo; en jovenes preferir agonistas dopaminergicos o IMAO-B.',
'[
  {"level":"A","text":"<65 anos sintomas leves: agonistas dopaminergicos (pramipexol, rotigotina) o IMAO-B (rasagilina)"},
  {"level":"A","text":">70 anos o discapacidad significativa: LEVODOPA/CARBIDOPA es mas efectiva"},
  {"level":"A","text":"Fluctuaciones motoras: agregar entacapona, IMAO-B, agonista dopaminergico"},
  {"level":"B","text":"DBS (estimulacion cerebral profunda) en Parkinson avanzado con fluctuaciones refractarias"},
  {"level":"A","text":"Fisioterapia + ejercicio (tai chi, danza) mejoran movilidad y equilibrio"}
]'::jsonb,
ARRAY['Parkinson','levodopa','agonistas dopaminergicos','DBS','neurologia']::text[])
ON CONFLICT (id) DO UPDATE SET
  specialty = EXCLUDED.specialty, summary = EXCLUDED.summary,
  key_recommendations = EXCLUDED.key_recommendations, tags = EXCLUDED.tags;

-- ========== IMSS-322-10: Miastenia gravis ==========
INSERT INTO clinical_guidelines (id, title, organization, year, url, specialty, summary, key_recommendations, tags)
VALUES ('IMSS-322-10',
'Diagnostico y Tratamiento de la Miastenia Gravis',
'IMSS', 2017, 'http://www.cenetec.salud.gob.mx',
'Neurologia',
'Debilidad fatigable + anticuerpos anti-AChR positivos. Piridostigmina primera linea sintomatica. Corticoides + inmunosupresor. Timectomia en timoma o generalizada <60 anos.',
'[
  {"level":"A","text":"Piridostigmina 60 mg VO cada 4-6h primera linea sintomatica"},
  {"level":"A","text":"Corticoides (prednisona) + inmunosupresor (azatioprina, micofenolato) si insuficiente"},
  {"level":"A","text":"TAC de torax OBLIGATORIO (descartar timoma en 10-15%)"},
  {"level":"A","text":"Timectomia si TIMOMA (siempre) o generalizada <60 anos"},
  {"level":"A","text":"CRISIS miastenica (insuficiencia respiratoria): IGIV o plasmaferesis + intubacion si necesario"}
]'::jsonb,
ARRAY['miastenia gravis','piridostigmina','timoma','anti-AChR','neurologia']::text[])
ON CONFLICT (id) DO UPDATE SET
  specialty = EXCLUDED.specialty, summary = EXCLUDED.summary,
  key_recommendations = EXCLUDED.key_recommendations, tags = EXCLUDED.tags;

-- ========== IMSS-144-08: Demencia Alzheimer ==========
INSERT INTO clinical_guidelines (id, title, organization, year, url, specialty, summary, key_recommendations, tags)
VALUES ('IMSS-144-08',
'Diagnostico y Tratamiento de la Enfermedad de Alzheimer y Otras Demencias',
'IMSS', 2018, 'http://www.cenetec.salud.gob.mx',
'Neurologia',
'Deterioro cognitivo progresivo afectando funcionalidad. Descartar causas reversibles (B12, TSH, depresion). Inhibidores acetilcolinesterasa mejoran cognicion modestamente.',
'[
  {"level":"A","text":"Descartar causas REVERSIBLES: deficit B12, hipotiroidismo, depresion, drogas, hidrocefalia normotensiva"},
  {"level":"A","text":"Alzheimer LEVE-MODERADA (MMSE 10-24): donepezilo, rivastigmina o galantamina"},
  {"level":"A","text":"MODERADA-SEVERA (MMSE <15): agregar MEMANTINA (antagonista NMDA)"},
  {"level":"B","text":"ANTIPSICOTICOS solo si sintomas psicoticos severos (aumentan mortalidad, uso restringido)"},
  {"level":"A","text":"Apoyo al cuidador es fundamental (agotamiento del cuidador es riesgo)"}
]'::jsonb,
ARRAY['Alzheimer','demencia','donepezilo','memantina','neurologia']::text[])
ON CONFLICT (id) DO UPDATE SET
  specialty = EXCLUDED.specialty, summary = EXCLUDED.summary,
  key_recommendations = EXCLUDED.key_recommendations, tags = EXCLUDED.tags;

-- ========== IMSS-151-08: Dengue ==========
INSERT INTO clinical_guidelines (id, title, organization, year, url, specialty, summary, key_recommendations, tags)
VALUES ('IMSS-151-08',
'Diagnostico y Tratamiento del Dengue',
'IMSS', 2020, 'http://www.cenetec.salud.gob.mx',
'Infectologia',
'Clasificacion OMS: dengue sin signos alarma, con signos alarma, grave. Fase critica dias 3-7 (fuga capilar). Reanimacion agresiva con cristaloides. NO transfundir plaquetas profilactico.',
'[
  {"level":"A","text":"Signos ALARMA: dolor abdominal, vomitos persistentes, hemoconcentracion + trombocitopenia rapida, letargia"},
  {"level":"A","text":"Dengue con signos alarma o grave: HOSPITALIZAR + cristaloides Ringer 10 mL/kg en 1h"},
  {"level":"A","text":"NO transfundir plaquetas profilactico (solo si sangrado activo o <10,000)"},
  {"level":"A","text":"NO AAS ni AINE (aumentan sangrado). Paracetamol para fiebre"},
  {"level":"B","text":"NS1 antigeno positivo dias 1-5, IgM/IgG despues dia 5"}
]'::jsonb,
ARRAY['dengue','fuga capilar','plaquetas','infectologia','tropical']::text[])
ON CONFLICT (id) DO UPDATE SET
  specialty = EXCLUDED.specialty, summary = EXCLUDED.summary,
  key_recommendations = EXCLUDED.key_recommendations, tags = EXCLUDED.tags;

-- ========== IMSS-563-12: C. difficile ==========
INSERT INTO clinical_guidelines (id, title, organization, year, url, specialty, summary, key_recommendations, tags)
VALUES ('IMSS-563-12',
'Diagnostico y Tratamiento de la Colitis Pseudomembranosa por Clostridioides difficile',
'IMSS', 2019, 'http://www.cenetec.salud.gob.mx',
'Infectologia',
'Diarrea post-antibiotico (clindamicina, quinolonas, cef). Vancomicina VO desde 2018 es 1a linea (metronidazol ya no). Recurrencia: pulsos, fidaxomicina o trasplante fecal.',
'[
  {"level":"A","text":"1a linea DESDE 2018 (IDSA): VANCOMICINA VO 125mg c/6h x 10 dias"},
  {"level":"A","text":"Alternativa: fidaxomicina 200mg VO c/12h x 10d (menos recurrencias)"},
  {"level":"A","text":"METRONIDAZOL YA NO ES 1A LINEA (menor eficacia)"},
  {"level":"A","text":"SUSPENDER antibiotico causal si posible"},
  {"level":"B","text":"Recurrencia: vancomicina en pulsos o TRASPLANTE FECAL"},
  {"level":"A","text":"NO usar antimotilidad (loperamida) por riesgo megacolon toxico"}
]'::jsonb,
ARRAY['C. difficile','colitis pseudomembranosa','vancomicina','antibioticos','infectologia']::text[])
ON CONFLICT (id) DO UPDATE SET
  specialty = EXCLUDED.specialty, summary = EXCLUDED.summary,
  key_recommendations = EXCLUDED.key_recommendations, tags = EXCLUDED.tags;

COMMIT;

SELECT COUNT(*) AS total_guidelines FROM clinical_guidelines;
