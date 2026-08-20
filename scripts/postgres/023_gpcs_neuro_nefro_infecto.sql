-- ================================================================
-- 023_gpcs_neuro_nefro_infecto.sql
-- Sprint 7A.4.4: 12 GPCs neurologia, nefrologia, infectologia, urgencias
-- ================================================================

BEGIN;

-- ========== IMSS-102-08: ACV isquemico ==========
INSERT INTO clinical_guidelines (id, title, organization, year, url, specialty, summary, key_recommendations, tags)
VALUES ('IMSS-102-08',
'Diagnostico y Tratamiento del Evento Vascular Cerebral Isquemico Agudo',
'IMSS', 2019, 'http://www.cenetec.salud.gob.mx',
'Neurologia',
'ACV isquemico requiere reperfusion urgente. Ventana trombolisis IV con alteplase: 4.5h. Trombectomia mecanica hasta 6-24h en oclusion grande. NIHSS estratifica gravedad.',
'[
  {"level":"A","text":"Alteplase IV 0.9 mg/kg (max 90mg) en ventana <=4.5h desde inicio sintomas"},
  {"level":"A","text":"Trombectomia mecanica hasta 6-24h en oclusion arteria grande (M1, ACI)"},
  {"level":"A","text":"TAC craneo SIN CONTRASTE urgente para descartar hemorragia"},
  {"level":"A","text":"TA <185/110 antes de trombolisis (labetalol/nicardipino IV si necesario)"},
  {"level":"A","text":"Aspirina 300mg VO 24h POST-trombolisis (o inmediato si no trombolisis)"},
  {"level":"B","text":"NO usar heparina en fase aguda (excepto casos seleccionados)"}
]'::jsonb,
ARRAY['ACV','EVC isquemico','alteplase','trombolisis','trombectomia','neurologia']::text[])
ON CONFLICT (id) DO UPDATE SET
  specialty = EXCLUDED.specialty, summary = EXCLUDED.summary,
  key_recommendations = EXCLUDED.key_recommendations, tags = EXCLUDED.tags;

-- ========== IMSS-244-10: Urgencias y toxicologia (mas usada 16 veces) ==========
INSERT INTO clinical_guidelines (id, title, organization, year, url, specialty, summary, key_recommendations, tags)
VALUES ('IMSS-244-10',
'Manejo de Urgencias Medicas: RCP, Intoxicaciones, Trauma, Anafilaxia, Termorregulacion',
'IMSS', 2018, 'http://www.cenetec.salud.gob.mx',
'Urgencias',
'Guia integral de urgencias medicas. Cubre soporte vital avanzado, intoxicaciones frecuentes con antidotos, anafilaxia, hipertermia/hipotermia, RCP con protocolos actualizados AHA.',
'[
  {"level":"A","text":"FV/TV sin pulso: desfibrilar INMEDIATO (200J bifasico), RCP 2 min entre choques"},
  {"level":"A","text":"Adrenalina 1mg IV c/3-5 min en PCR, amiodarona 300mg tras 3er choque"},
  {"level":"A","text":"Anafilaxia: ADRENALINA IM 0.3-0.5mg EN MUSLO (no diferir por antihistaminicos)"},
  {"level":"A","text":"Organofosforados: atropina hasta atropinizacion + pralidoxima"},
  {"level":"A","text":"Paracetamol toxico: N-acetilcisteina (nomograma Rumack-Matthew)"},
  {"level":"A","text":"Golpe de calor: enfriamiento AGRESIVO (inmersion agua fria), meta <39C en 30 min"},
  {"level":"A","text":"Hipotermia severa: recalentamiento ACTIVO CENTRAL, NO declarar muerte hasta caliente y muerto"}
]'::jsonb,
ARRAY['RCP','desfibrilacion','anafilaxia','intoxicacion','antidotos','hipotermia','golpe calor','urgencias']::text[])
ON CONFLICT (id) DO UPDATE SET
  specialty = EXCLUDED.specialty, summary = EXCLUDED.summary,
  key_recommendations = EXCLUDED.key_recommendations, tags = EXCLUDED.tags;

-- ========== IMSS-604-13: TCE severo ==========
INSERT INTO clinical_guidelines (id, title, organization, year, url, specialty, summary, key_recommendations, tags)
VALUES ('IMSS-604-13',
'Manejo del Trauma Craneoencefalico Severo',
'IMSS', 2018, 'http://www.cenetec.salud.gob.mx',
'Urgencias',
'TCE severo (Glasgow <=8) requiere intubacion, TAC urgente, control HTIC con osmoterapia. Corticoides CONTRAINDICADOS. Objetivo: PPC >60, PAM >=80, PIC <22.',
'[
  {"level":"A","text":"Glasgow <=8: intubacion orotraqueal con secuencia rapida + estabilizacion cervical"},
  {"level":"A","text":"TAC craneo URGENTE sin contraste, luego TAC cervical/torax/abdomen si politrauma"},
  {"level":"A","text":"Osmoterapia si HTIC: MANITOL 0.5-1 g/kg IV O solucion salina hipertonica 3%"},
  {"level":"A","text":"CORTICOIDES CONTRAINDICADOS en TCE (estudio CRASH aumento mortalidad)"},
  {"level":"A","text":"Metas: PAM >=80, PPC >60, PIC <22, normocapnia (PaCO2 35-40)"},
  {"level":"A","text":"Levetiracetam profilaxis de convulsiones postraumaticas 7 dias"},
  {"level":"B","text":"Craniectomia descompresiva en HTIC refractaria"}
]'::jsonb,
ARRAY['TCE','trauma craneal','HTIC','manitol','Glasgow','urgencias']::text[])
ON CONFLICT (id) DO UPDATE SET
  specialty = EXCLUDED.specialty, summary = EXCLUDED.summary,
  key_recommendations = EXCLUDED.key_recommendations, tags = EXCLUDED.tags;

-- ========== IMSS-041-08: Quemaduras ==========
INSERT INTO clinical_guidelines (id, title, organization, year, url, specialty, summary, key_recommendations, tags)
VALUES ('IMSS-041-08',
'Atencion Integral al Paciente Quemado',
'IMSS', 2018, 'http://www.cenetec.salud.gob.mx',
'Urgencias',
'Reanimacion hidrica con formula Parkland (4 mL x kg x %SCT) las primeras 24h. Intubacion precoz si lesion inhalatoria. Referencia a centro de quemados si >20% SCT o quemaduras profundas.',
'[
  {"level":"A","text":"Formula PARKLAND: 4 mL x kg x %SCT Ringer lactato en 24h (50% en 8h)"},
  {"level":"A","text":"Meta diuresis: 0.5-1 mL/kg/h (adultos), 1-2 mL/kg/h (pediatricos)"},
  {"level":"A","text":"Intubacion PRECOZ si esputo carbonaceo, cejas quemadas, disfonia (antes edema)"},
  {"level":"A","text":"Analgesia opioide IV titulada"},
  {"level":"A","text":"CORTICOIDES CONTRAINDICADOS en quemados"},
  {"level":"B","text":"Referir a centro de quemados si >20% SCT, cara/manos/genitales, electricas, quimicas"}
]'::jsonb,
ARRAY['quemaduras','Parkland','reanimacion hidrica','urgencias']::text[])
ON CONFLICT (id) DO UPDATE SET
  specialty = EXCLUDED.specialty, summary = EXCLUDED.summary,
  key_recommendations = EXCLUDED.key_recommendations, tags = EXCLUDED.tags;

-- ========== IMSS-395-10: LRA ==========
INSERT INTO clinical_guidelines (id, title, organization, year, url, specialty, summary, key_recommendations, tags)
VALUES ('IMSS-395-10',
'Diagnostico y Tratamiento de la Lesion Renal Aguda',
'IMSS', 2019, 'http://www.cenetec.salud.gob.mx',
'Nefrologia',
'Clasificacion AKIN/KDIGO por creatinina o diuresis. Etiologia prerrenal/intrinseca/postrenal. Suspender nefrotoxicos (AINE, IECA, contraste). Dialisis por criterios AEIOU.',
'[
  {"level":"A","text":"Clasificar con AKIN/KDIGO por creatinina o diuresis"},
  {"level":"A","text":"SUSPENDER nefrotoxicos: AINE, IECA/ARA-II, aminoglucosidos, contraste"},
  {"level":"A","text":"Manejo hiperkalemia: gluconato calcio + insulina/glucosa + resinas"},
  {"level":"A","text":"Furosemida IV si sobrecarga hidrica (no cambia mortalidad)"},
  {"level":"A","text":"Criterios dialisis (AEIOU): Acidosis, Electrolitos (hiperK), Intoxicacion, Overload, Uremia"},
  {"level":"B","text":"SHR: terlipresina + albumina 1g/kg dia 1, luego 20-40 g/dia"}
]'::jsonb,
ARRAY['LRA','IRA','AKIN','KDIGO','nefrotoxicos','hiperkalemia','dialisis','nefrologia']::text[])
ON CONFLICT (id) DO UPDATE SET
  specialty = EXCLUDED.specialty, summary = EXCLUDED.summary,
  key_recommendations = EXCLUDED.key_recommendations, tags = EXCLUDED.tags;

-- ========== IMSS-335-10: ERC ==========
INSERT INTO clinical_guidelines (id, title, organization, year, url, specialty, summary, key_recommendations, tags)
VALUES ('IMSS-335-10',
'Diagnostico y Tratamiento de la Enfermedad Renal Cronica',
'IMSS', 2020, 'http://www.cenetec.salud.gob.mx',
'Nefrologia',
'Estadificacion KDIGO por TFG y albuminuria. Nefroproteccion con IECA/ARA-II. SGLT2i con beneficio renal en DM2 y no-DM. Preparar terapia de reemplazo renal en estadio 4.',
'[
  {"level":"A","text":"Estadificacion KDIGO por TFG (G1-G5) y albuminuria (A1-A3)"},
  {"level":"A","text":"IECA o ARA-II en proteinuria >300 mg/g o HTA con ERC"},
  {"level":"A","text":"Meta TA <130/80 (<120/80 con proteinuria)"},
  {"level":"A","text":"SGLT2i (empagliflozina, dapagliflozina) con beneficio renal probado"},
  {"level":"A","text":"Dieta hipoproteica 0.8 g/kg + restriccion Na <2g/dia"},
  {"level":"B","text":"Preparar acceso vascular (FAVI) 6-12 meses antes de necesidad dialisis"},
  {"level":"A","text":"Vacunacion: influenza, neumococo, hepatitis B"}
]'::jsonb,
ARRAY['ERC','IRC','KDIGO','SGLT2i','nefroproteccion','dialisis','nefrologia']::text[])
ON CONFLICT (id) DO UPDATE SET
  specialty = EXCLUDED.specialty, summary = EXCLUDED.summary,
  key_recommendations = EXCLUDED.key_recommendations, tags = EXCLUDED.tags;

-- ========== IMSS-241-12: Glomerulonefritis y sindrome nefrotico/nefritico ==========
INSERT INTO clinical_guidelines (id, title, organization, year, url, specialty, summary, key_recommendations, tags)
VALUES ('IMSS-241-12',
'Diagnostico y Tratamiento de Glomerulonefritis y Sindrome Nefrotico',
'IMSS', 2015, 'http://www.cenetec.salud.gob.mx',
'Nefrologia',
'Sindrome nefritico (hematuria + HTA + oliguria + edema) vs nefrotico (proteinuria masiva + hipoalbuminemia + edema + hiperlipidemia). En ninos: cambios minimos con corticoides.',
'[
  {"level":"A","text":"Nefritico: GNPE es autolimitada, tratamiento sintomatico (restriccion Na, diuretico, antiHTA)"},
  {"level":"A","text":"Nefrotico en nino 1-8a: cambios minimos, PREDNISONA 60 mg/m2/dia sin biopsia"},
  {"level":"A","text":"Respuesta a corticoides 90% en 2-3 semanas"},
  {"level":"B","text":"Biopsia si no responde a corticoides 4-6 sem, <1a o >12a, HTA, hematuria, complemento bajo"},
  {"level":"A","text":"Profilaxis TROMBOSIS si albumina <2 g/dL (riesgo TVP/TEP)"}
]'::jsonb,
ARRAY['glomerulonefritis','sindrome nefrotico','sindrome nefritico','GNPE','nefrologia']::text[])
ON CONFLICT (id) DO UPDATE SET
  specialty = EXCLUDED.specialty, summary = EXCLUDED.summary,
  key_recommendations = EXCLUDED.key_recommendations, tags = EXCLUDED.tags;

-- ========== IMSS-078-08: ITU ==========
INSERT INTO clinical_guidelines (id, title, organization, year, url, specialty, summary, key_recommendations, tags)
VALUES ('IMSS-078-08',
'Diagnostico y Tratamiento de la Infeccion de Vias Urinarias',
'IMSS', 2019, 'http://www.cenetec.salud.gob.mx',
'Infectologia',
'ITU no complicada vs complicada. Nitrofurantoina y fosfomicina primera linea en cistitis. Pielonefritis en diabetico/embarazada requiere hospitalizacion y ATB IV.',
'[
  {"level":"A","text":"Cistitis no complicada: nitrofurantoina 100mg c/6h x 5d O fosfomicina 3g DU O TMP-SMX x 3d"},
  {"level":"A","text":"Pielonefritis: ceftriaxona 1g IV c/24h o quinolona VO/IV segun gravedad"},
  {"level":"A","text":"ITU complicada (DM, embarazo, uropatia, inmunosupresion): hospitalizar + ATB IV"},
  {"level":"A","text":"BACTERIURIA ASINTOMATICA EN EMBARAZO SI se trata (nitrofurantoina o cefalexina x 5-7d)"},
  {"level":"B","text":"Nitrofurantoina NO en 3er trimestre (riesgo hemolisis neonatal)"}
]'::jsonb,
ARRAY['ITU','cistitis','pielonefritis','embarazo','infectologia']::text[])
ON CONFLICT (id) DO UPDATE SET
  specialty = EXCLUDED.specialty, summary = EXCLUDED.summary,
  key_recommendations = EXCLUDED.key_recommendations, tags = EXCLUDED.tags;

-- ========== IMSS-070-08: TB pulmonar ==========
INSERT INTO clinical_guidelines (id, title, organization, year, url, specialty, summary, key_recommendations, tags)
VALUES ('IMSS-070-08',
'Prevencion, Diagnostico y Tratamiento de la Tuberculosis',
'IMSS', 2020, 'http://www.cenetec.salud.gob.mx',
'Infectologia',
'Esquema estandar HRZE (2 meses) seguido de HR (4 meses). Total 6 meses. Vigilar hepatotoxicidad, neuritis optica, hiperuricemia. TAES (tratamiento estrictamente supervisado).',
'[
  {"level":"A","text":"Esquema estandar TB sensible: HRZE 2 meses + HR 4 meses (total 6 meses)"},
  {"level":"A","text":"Isoniazida 5 mg/kg + Rifampicina 10 mg/kg + Pirazinamida 25 mg/kg + Etambutol 15 mg/kg"},
  {"level":"A","text":"Vigilancia mensual clinica + baciloscopia 2, 4, 6 meses"},
  {"level":"A","text":"TAES (tratamiento estrictamente supervisado) mejora adherencia"},
  {"level":"B","text":"Suplementar PIRIDOXINA (vitamina B6) 25-50 mg/dia para prevenir neuritis por isoniazida"},
  {"level":"A","text":"Estudio contactos: PPD/IGRA + Rx torax"}
]'::jsonb,
ARRAY['tuberculosis','TB','HRZE','isoniazida','rifampicina','infectologia']::text[])
ON CONFLICT (id) DO UPDATE SET
  specialty = EXCLUDED.specialty, summary = EXCLUDED.summary,
  key_recommendations = EXCLUDED.key_recommendations, tags = EXCLUDED.tags;

-- ========== IMSS-067-08: VIH ==========
INSERT INTO clinical_guidelines (id, title, organization, year, url, specialty, summary, key_recommendations, tags)
VALUES ('IMSS-067-08',
'Diagnostico y Tratamiento del VIH/SIDA',
'IMSS', 2020, 'http://www.cenetec.salud.gob.mx',
'Infectologia',
'TARV para TODO paciente con VIH independiente de CD4 (desde 2015, estudios START/TEMPRANO). Esquema 1a linea con inhibidor integrasa. Profilaxis oportunistas segun CD4.',
'[
  {"level":"A","text":"TARV para TODO paciente con VIH independiente de CD4 (desde 2015)"},
  {"level":"A","text":"Esquema 1a linea: bictegravir/tenofovir alafenamida/emtricitabina O dolutegravir + tenofovir + lamivudina"},
  {"level":"A","text":"Profilaxis PJP con TMP-SMX si CD4 <200"},
  {"level":"A","text":"Profilaxis MAC con azitromicina 1200mg/sem si CD4 <50"},
  {"level":"A","text":"PROFILAXIS POST-EXPOSICION (PPE): en <2h ideal, hasta 72h, esquema triple x 28 dias"}
]'::jsonb,
ARRAY['VIH','SIDA','TARV','PPE','infectologia']::text[])
ON CONFLICT (id) DO UPDATE SET
  specialty = EXCLUDED.specialty, summary = EXCLUDED.summary,
  key_recommendations = EXCLUDED.key_recommendations, tags = EXCLUDED.tags;

-- ========== IMSS-263-13: Infecciones piel y tejidos blandos ==========
INSERT INTO clinical_guidelines (id, title, organization, year, url, specialty, summary, key_recommendations, tags)
VALUES ('IMSS-263-13',
'Diagnostico y Tratamiento de Infecciones de Piel y Tejidos Blandos',
'IMSS', 2016, 'http://www.cenetec.salud.gob.mx',
'Infectologia',
'Erisipela (bordes definidos, S. pyogenes) vs celulitis (bordes difusos, S. aureus). Fascitis necrosante es emergencia quirurgica (dolor desproporcionado, crepitos, toxicidad).',
'[
  {"level":"A","text":"Erisipela (bordes definidos): penicilina o cefalexina VO/IV"},
  {"level":"A","text":"Celulitis (bordes difusos): dicloxacilina, cefalexina o clindamicina"},
  {"level":"A","text":"FASCITIS NECROSANTE: cirugia URGENTE + ATB amplio espectro (piperacilina-tazobactam + clindamicina + vancomicina)"},
  {"level":"B","text":"MRSA comunitario si absceso: I&D + TMP-SMX o clindamicina o doxiciclina"},
  {"level":"C","text":"Elevar extremidad afectada, marcar borde con marcador"}
]'::jsonb,
ARRAY['erisipela','celulitis','fascitis necrosante','MRSA','infectologia']::text[])
ON CONFLICT (id) DO UPDATE SET
  specialty = EXCLUDED.specialty, summary = EXCLUDED.summary,
  key_recommendations = EXCLUDED.key_recommendations, tags = EXCLUDED.tags;

-- ========== IMSS-283-13: Sepsis (Surviving Sepsis) ==========
INSERT INTO clinical_guidelines (id, title, organization, year, url, specialty, summary, key_recommendations, tags)
VALUES ('IMSS-283-13',
'Diagnostico y Manejo Inicial de Sepsis y Shock Septico (Surviving Sepsis Campaign)',
'IMSS', 2021, 'http://www.cenetec.salud.gob.mx',
'Infectologia',
'Bundle SEP-1 en la 1a hora: lactato, hemocultivos ANTES de ATB, antibiotico amplio espectro, cristaloides 30 mL/kg si hipotension o lactato >4, vasopresores si TA no mejora.',
'[
  {"level":"A","text":"Bundle 1a hora: medir lactato + hemocultivos + antibiotico + cristaloides 30 mL/kg + vasopresor si necesario"},
  {"level":"A","text":"Antibiotico amplio espectro en <1h desde reconocimiento (hemocultivos ANTES)"},
  {"level":"A","text":"Cristaloides 30 mL/kg en hipotension o lactato >4 mmol/L"},
  {"level":"A","text":"Norepinefrina 1a linea si hipotension persiste, meta PAM >=65"},
  {"level":"B","text":"Hidrocortisona 200 mg/dia IV si shock septico refractario a vasopresores"},
  {"level":"A","text":"Control foco infeccioso <6-12h (drenaje, cirugia, retirar cateteres)"}
]'::jsonb,
ARRAY['sepsis','shock septico','bundle','norepinefrina','infectologia','urgencias']::text[])
ON CONFLICT (id) DO UPDATE SET
  specialty = EXCLUDED.specialty, summary = EXCLUDED.summary,
  key_recommendations = EXCLUDED.key_recommendations, tags = EXCLUDED.tags;

COMMIT;

SELECT COUNT(*) AS total_guidelines FROM clinical_guidelines;
