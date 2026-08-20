-- ================================================================
-- 021_gpcs_medint_cardio.sql
-- Sprint 7A.4.2: 12 GPCs de Medicina Interna, Cardiologia, Neumologia
-- ================================================================

BEGIN;

-- ========== IMSS-357-13: IAM STEMI (usada 4 veces) ==========
INSERT INTO clinical_guidelines (id, title, organization, year, url, specialty, summary, key_recommendations, tags)
VALUES ('IMSS-357-13',
'Intervencionismo Coronario Percutaneo en el Infarto Agudo del Miocardio con Elevacion del ST',
'IMSS', 2013, 'http://www.cenetec.salud.gob.mx',
'Cardiologia',
'GPC para el manejo del IAM CEST con enfasis en la reperfusion coronaria oportuna. Establece tiempos objetivo puerta-aguja (30 min para fibrinolisis) y puerta-balon (90 min para ICP primaria).',
'[
  {"level":"A","text":"Realizar ICP primaria en menos de 90 minutos desde primer contacto medico"},
  {"level":"A","text":"Doble antiagregacion plaquetaria (AAS + inhibidor P2Y12) inmediata"},
  {"level":"A","text":"Fibrinolisis si ICP no disponible en menos de 120 min (puerta-aguja <30 min)"},
  {"level":"A","text":"Anticoagulacion parenteral (HNF, enoxaparina o bivalirudina)"},
  {"level":"B","text":"Estatinas de alta intensidad al ingreso independiente del LDL"},
  {"level":"A","text":"Betabloqueadores en ausencia de contraindicacion"}
]'::jsonb,
ARRAY['IAM','STEMI','ICP','fibrinolisis','cardiologia','urgencias']::text[])
ON CONFLICT (id) DO UPDATE SET
  specialty = EXCLUDED.specialty,
  summary = EXCLUDED.summary,
  key_recommendations = EXCLUDED.key_recommendations,
  tags = EXCLUDED.tags;

-- ========== IMSS-191-10: SCA sin elevacion ST ==========
INSERT INTO clinical_guidelines (id, title, organization, year, url, specialty, summary, key_recommendations, tags)
VALUES ('IMSS-191-10',
'Diagnostico y Tratamiento del Sindrome Coronario Agudo sin Elevacion del ST',
'IMSS', 2019, 'http://www.cenetec.salud.gob.mx',
'Cardiologia',
'Manejo del SCA SEST con estratificacion de riesgo (TIMI, GRACE) para decidir estrategia invasiva vs conservadora. Recomienda angiografia temprana (<24h) en riesgo intermedio-alto.',
'[
  {"level":"A","text":"Estratificar riesgo con TIMI o GRACE score"},
  {"level":"A","text":"Estrategia invasiva temprana (<24h) si TIMI >=3 o troponina elevada"},
  {"level":"A","text":"Doble antiagregacion (AAS + ticagrelor/prasugrel/clopidogrel) por 12 meses"},
  {"level":"A","text":"Anticoagulacion (enoxaparina o fondaparinux) hasta ICP"},
  {"level":"B","text":"NO usar fibrinoliticos en SCASEST"}
]'::jsonb,
ARRAY['SCA','SCASEST','angina inestable','cardiologia']::text[])
ON CONFLICT (id) DO UPDATE SET
  specialty = EXCLUDED.specialty,
  summary = EXCLUDED.summary,
  key_recommendations = EXCLUDED.key_recommendations,
  tags = EXCLUDED.tags;

-- ========== IMSS-076-08: Emergencia hipertensiva ==========
INSERT INTO clinical_guidelines (id, title, organization, year, url, specialty, summary, key_recommendations, tags)
VALUES ('IMSS-076-08',
'Diagnostico y Tratamiento de la Crisis Hipertensiva',
'IMSS', 2018, 'http://www.cenetec.salud.gob.mx',
'Cardiologia',
'Manejo diferenciado de urgencia hipertensiva (sin dano organico) vs emergencia hipertensiva (con dano organico). Reduccion controlada de TA para evitar hipoperfusion.',
'[
  {"level":"A","text":"Emergencia HTA: reducir TAM 20-25% en 1a hora, luego a 160/100 en 2-6h"},
  {"level":"A","text":"NUNCA reducir TA a valores normales en 1a hora (riesgo isquemia)"},
  {"level":"B","text":"Nicardipino o labetalol IV son de eleccion en emergencia HTA"},
  {"level":"B","text":"Urgencia HTA: manejo VO con IECA, calcio antagonistas"}
]'::jsonb,
ARRAY['HTA','crisis hipertensiva','emergencia hipertensiva','cardiologia']::text[])
ON CONFLICT (id) DO UPDATE SET
  specialty = EXCLUDED.specialty,
  summary = EXCLUDED.summary,
  key_recommendations = EXCLUDED.key_recommendations,
  tags = EXCLUDED.tags;

-- ========== IMSS-186-09: Endocarditis ==========
INSERT INTO clinical_guidelines (id, title, organization, year, url, specialty, summary, key_recommendations, tags)
VALUES ('IMSS-186-09',
'Diagnostico y Tratamiento de la Endocarditis Infecciosa',
'IMSS', 2019, 'http://www.cenetec.salud.gob.mx',
'Cardiologia',
'Criterios de Duke modificados para diagnostico. Tratamiento antibiotico prolongado (4-6 semanas) guiado por cultivo. Cirugia valvular indicada en IC, embolismo recurrente o infeccion no controlada.',
'[
  {"level":"A","text":"Hemocultivos x3 antes de antibiotico en pacientes estables"},
  {"level":"A","text":"S. aureus meticilino-sensible: nafcilina/oxacilina 12g/dia IV 4-6 sem"},
  {"level":"A","text":"MRSA: vancomicina + rifampicina + gentamicina (valvula protesica)"},
  {"level":"B","text":"Cirugia si IC severa, embolismo recurrente, infeccion no controlada, absceso"}
]'::jsonb,
ARRAY['endocarditis','infeccion valvular','cardiologia','infectologia']::text[])
ON CONFLICT (id) DO UPDATE SET
  specialty = EXCLUDED.specialty,
  summary = EXCLUDED.summary,
  key_recommendations = EXCLUDED.key_recommendations,
  tags = EXCLUDED.tags;

-- ========== IMSS-478-11: Valvulopatia aortica ==========
INSERT INTO clinical_guidelines (id, title, organization, year, url, specialty, summary, key_recommendations, tags)
VALUES ('IMSS-478-11',
'Diagnostico y Tratamiento de la Estenosis Aortica',
'IMSS', 2018, 'http://www.cenetec.salud.gob.mx',
'Cardiologia',
'Estenosis aortica severa se define por area <1cm2 o gradiente >40 mmHg. Sintomatica tiene mortalidad 50% a 2 anos sin cirugia. Indicacion clase I de reemplazo valvular (SAVR o TAVI).',
'[
  {"level":"A","text":"EA severa sintomatica: reemplazo valvular es indicacion CLASE I"},
  {"level":"A","text":"TAVI es alternativa a SAVR en riesgo intermedio-alto o >65 anos"},
  {"level":"C","text":"EA asintomatica severa con FEVI <50%: considerar cirugia"},
  {"level":"B","text":"NO usar vasodilatadores en EA severa (riesgo hipotension)"}
]'::jsonb,
ARRAY['estenosis aortica','valvulopatia','TAVI','SAVR','cardiologia']::text[])
ON CONFLICT (id) DO UPDATE SET
  specialty = EXCLUDED.specialty,
  summary = EXCLUDED.summary,
  key_recommendations = EXCLUDED.key_recommendations,
  tags = EXCLUDED.tags;

-- ========== IMSS-484-11: Bradiarritmia ==========
INSERT INTO clinical_guidelines (id, title, organization, year, url, specialty, summary, key_recommendations, tags)
VALUES ('IMSS-484-11',
'Diagnostico y Tratamiento de las Bradiarritmias y Bloqueos AV',
'IMSS', 2016, 'http://www.cenetec.salud.gob.mx',
'Cardiologia',
'BAV completo sintomatico o pausas >3 seg es indicacion clase I de marcapasos definitivo. Manejo agudo con marcapasos transcutaneo/transvenoso como puente al implante.',
'[
  {"level":"A","text":"BAV completo sintomatico: marcapasos definitivo CLASE I"},
  {"level":"B","text":"Marcapasos transcutaneo como puente al definitivo"},
  {"level":"C","text":"Atropina 1mg IV en bradicardia sintomatica (raramente efectiva en BAV infrahisiano)"},
  {"level":"B","text":"Isoproterenol o dopamina si no responde a atropina"}
]'::jsonb,
ARRAY['bradiarritmia','BAV','marcapasos','cardiologia']::text[])
ON CONFLICT (id) DO UPDATE SET
  specialty = EXCLUDED.specialty,
  summary = EXCLUDED.summary,
  key_recommendations = EXCLUDED.key_recommendations,
  tags = EXCLUDED.tags;

-- ========== IMSS-009-08: Asma ==========
INSERT INTO clinical_guidelines (id, title, organization, year, url, specialty, summary, key_recommendations, tags)
VALUES ('IMSS-009-08',
'Diagnostico y Tratamiento del Asma en Ninos y Adultos',
'IMSS', 2019, 'http://www.cenetec.salud.gob.mx',
'Neumologia',
'Manejo escalonado segun GINA. Crisis asmatica se trata con SABA + O2 + corticoide sistemico. Nunca dar de alta solo con SABA. Educacion sobre uso correcto de inhaladores fundamental.',
'[
  {"level":"A","text":"Crisis: SABA nebulizado + O2 (SpO2 >94%) + corticoide sistemico en 1a hora"},
  {"level":"A","text":"Ipratropio agregado a SABA en crisis moderada-severa"},
  {"level":"B","text":"Sulfato de magnesio 2g IV si crisis severa refractaria"},
  {"level":"A","text":"Corticoide inhalado a largo plazo es piedra angular del tratamiento"},
  {"level":"A","text":"NUNCA usar SABA como monoterapia (aumenta mortalidad)"}
]'::jsonb,
ARRAY['asma','crisis asmatica','SABA','neumologia']::text[])
ON CONFLICT (id) DO UPDATE SET
  specialty = EXCLUDED.specialty,
  summary = EXCLUDED.summary,
  key_recommendations = EXCLUDED.key_recommendations,
  tags = EXCLUDED.tags;

-- ========== IMSS-437-11: TEP ==========
INSERT INTO clinical_guidelines (id, title, organization, year, url, specialty, summary, key_recommendations, tags)
VALUES ('IMSS-437-11',
'Diagnostico y Tratamiento del Tromboembolismo Pulmonar Agudo',
'IMSS', 2018, 'http://www.cenetec.salud.gob.mx',
'Neumologia',
'AngioTAC pulmonar es el estudio de eleccion. Escala Wells guia probabilidad pre-test. Anticoagulacion inmediata si sospecha alta. Trombolisis en TEP masivo con inestabilidad hemodinamica.',
'[
  {"level":"A","text":"Wells >4 + D-dimero elevado: angioTAC pulmonar"},
  {"level":"A","text":"Anticoagulacion INMEDIATA con HBPM/HNF/DOAC si sospecha alta"},
  {"level":"A","text":"Trombolisis (alteplase) en TEP masivo con inestabilidad hemodinamica"},
  {"level":"B","text":"DOAC (apixaban, rivaroxaban) preferidos sobre warfarina para tratamiento cronico"}
]'::jsonb,
ARRAY['TEP','embolismo pulmonar','D-dimero','anticoagulacion','neumologia']::text[])
ON CONFLICT (id) DO UPDATE SET
  specialty = EXCLUDED.specialty,
  summary = EXCLUDED.summary,
  key_recommendations = EXCLUDED.key_recommendations,
  tags = EXCLUDED.tags;

-- ========== IMSS-354-10: Neumotorax ==========
INSERT INTO clinical_guidelines (id, title, organization, year, url, specialty, summary, key_recommendations, tags)
VALUES ('IMSS-354-10',
'Diagnostico y Tratamiento del Neumotorax Espontaneo',
'IMSS', 2017, 'http://www.cenetec.salud.gob.mx',
'Neumologia',
'Neumotorax primario en jovenes altos y delgados. Manejo segun tamano: <2cm observacion, >2cm drenaje. Recurrencia 30%, considerar pleurodesis en 2do episodio.',
'[
  {"level":"A","text":"Neumotorax <2cm y asintomatico: observacion + O2"},
  {"level":"A","text":"Neumotorax >2cm o sintomatico: sonda pleural 20-28 Fr"},
  {"level":"A","text":"Neumotorax A TENSION: descompresion con aguja INMEDIATA (2do EIC LMC o 5to LAM), sin esperar Rx"},
  {"level":"B","text":"Pleurodesis quimica o quirurgica en recurrencia"}
]'::jsonb,
ARRAY['neumotorax','sonda pleural','descompresion','neumologia']::text[])
ON CONFLICT (id) DO UPDATE SET
  specialty = EXCLUDED.specialty,
  summary = EXCLUDED.summary,
  key_recommendations = EXCLUDED.key_recommendations,
  tags = EXCLUDED.tags;

-- ========== IMSS-284-13: Derrame pleural ==========
INSERT INTO clinical_guidelines (id, title, organization, year, url, specialty, summary, key_recommendations, tags)
VALUES ('IMSS-284-13',
'Diagnostico y Tratamiento del Derrame Pleural',
'IMSS', 2015, 'http://www.cenetec.salud.gob.mx',
'Neumologia',
'Criterios de Light diferencian exudado vs trasudado. Exudado obliga a buscar etiologia: neoplasia, TB, empiema, TEP. Empiema requiere drenaje + antibiotico.',
'[
  {"level":"A","text":"Criterios de LIGHT: prot LP/serica >0.5 O DHL LP/serica >0.6 O DHL LP >2/3 LSN serica = exudado"},
  {"level":"B","text":"Exudado: buscar neoplasia, TB, empiema, TEP, autoinmune"},
  {"level":"A","text":"pH pleural <7.2 en paraneumonico: sonda pleural + antibiotico"},
  {"level":"B","text":"ADA >40 U/L sugiere TB pleural"}
]'::jsonb,
ARRAY['derrame pleural','criterios light','empiema','neumologia']::text[])
ON CONFLICT (id) DO UPDATE SET
  specialty = EXCLUDED.specialty,
  summary = EXCLUDED.summary,
  key_recommendations = EXCLUDED.key_recommendations,
  tags = EXCLUDED.tags;

-- ========== IMSS-517-11: SAOS y FPI ==========
INSERT INTO clinical_guidelines (id, title, organization, year, url, specialty, summary, key_recommendations, tags)
VALUES ('IMSS-517-11',
'Diagnostico y Tratamiento de las Enfermedades Pulmonares Intersticiales y SAOS',
'IMSS', 2016, 'http://www.cenetec.salud.gob.mx',
'Neumologia',
'SAOS moderado-severo (IAH >15) requiere CPAP. FPI se diagnostica con patron UIP en TAC de alta resolucion. Antifibroticos (pirfenidona, nintedanib) enlentecen progresion.',
'[
  {"level":"A","text":"Polisomnografia nocturna es estandar oro para SAOS"},
  {"level":"A","text":"CPAP en SAOS moderado-severo (IAH >15) reduce mortalidad CV"},
  {"level":"A","text":"Perdida de peso >10% reduce IAH 30% en obesos"},
  {"level":"A","text":"FPI: antifibroticos (pirfenidona o nintedanib)"},
  {"level":"A","text":"CORTICOIDES EMPEORAN FPI (contraindicados)"}
]'::jsonb,
ARRAY['SAOS','apnea sueno','CPAP','FPI','fibrosis pulmonar','neumologia']::text[])
ON CONFLICT (id) DO UPDATE SET
  specialty = EXCLUDED.specialty,
  summary = EXCLUDED.summary,
  key_recommendations = EXCLUDED.key_recommendations,
  tags = EXCLUDED.tags;

-- ========== IMSS-030-13: Cancer pulmonar ==========
INSERT INTO clinical_guidelines (id, title, organization, year, url, specialty, summary, key_recommendations, tags)
VALUES ('IMSS-030-13',
'Prevencion, Diagnostico y Tratamiento del Cancer Pulmonar',
'IMSS', 2016, 'http://www.cenetec.salud.gob.mx',
'Neumologia',
'Tamizaje con TAC de baja dosis anual en 50-80 anos con >20 paq-ano de tabaquismo. Cesacion tabaquica es la intervencion mas costo-efectiva. Estadificacion TNM guia tratamiento.',
'[
  {"level":"A","text":"Tamizaje TAC baja dosis anual: 50-80 anos + >=20 paq-ano + fumador activo o exfumador <15 anos"},
  {"level":"A","text":"CESACION TABAQUICA es la intervencion mas costo-efectiva"},
  {"level":"A","text":"Estadificacion TNM guia tratamiento"},
  {"level":"B","text":"NO usar Rx torax como tamizaje (sin evidencia)"}
]'::jsonb,
ARRAY['cancer pulmonar','tamizaje','TAC baja dosis','tabaquismo','neumologia']::text[])
ON CONFLICT (id) DO UPDATE SET
  specialty = EXCLUDED.specialty,
  summary = EXCLUDED.summary,
  key_recommendations = EXCLUDED.key_recommendations,
  tags = EXCLUDED.tags;

COMMIT;

SELECT COUNT(*) AS total_guidelines FROM clinical_guidelines;
