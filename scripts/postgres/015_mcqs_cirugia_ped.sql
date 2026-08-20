-- ================================================================
-- 015_mcqs_cirugia_ped.sql
-- Sprint 7A.3: 10 MCQs de Cirugia Pediatrica estilo ENARM
-- ================================================================

BEGIN;

-- ---------- MCQ 1: Apendicitis aguda pediatrica ----------
INSERT INTO mcq_bank (specialty, difficulty, topic, vignette, question, options_json, correct_answer, explanation_correct, guidelines_ids, is_active)
VALUES ('Cirugia Pediatrica', 'intermediate', 'Apendicitis',
'Escolar 8 anos con dolor abdominal 12h, inicio periumbilical y migro a fosa iliaca derecha. Vomito, hiporexia, fiebre 38.2. McBurney (+), Blumberg (+), Rovsing (+). Leucocitos 16,200 con neutrofilia. Score de Alvarado 8.',
'Cual es el estudio de imagen INICIAL de eleccion?',
'{"A":"Radiografia simple abdomen","B":"Ultrasonido abdominal (apendice mayor a 6mm no compresible)","C":"TAC contrastada de abdomen","D":"Resonancia magnetica"}'::jsonb,
'B',
'La respuesta correcta es B. En pediatricos con sospecha de apendicitis, el ULTRASONIDO ABDOMINAL es el estudio de PRIMERA LINEA por: no irradia, alta especificidad (apendice mayor a 6mm no compresible, con liquido periapendicular). Sensibilidad 85-90%. TAC (C) se reserva si USG no concluyente o sospecha de complicacion. RM (D) alternativa en embarazadas o USG dudoso. Rx (A) solo detecta datos indirectos (fecalito, aire libre en perforacion).',
ARRAY['IMSS-031-08']::text[], TRUE)
ON CONFLICT DO NOTHING;

-- ---------- MCQ 2: Invaginacion intestinal ----------
INSERT INTO mcq_bank (specialty, difficulty, topic, vignette, question, options_json, correct_answer, explanation_correct, guidelines_ids, is_active)
VALUES ('Cirugia Pediatrica', 'advanced', 'Invaginacion',
'Lactante 8 meses con episodios paroxisticos de llanto intenso, encogimiento de piernas, vomitos, y evacuaciones en "jalea de grosella". Masa abdominal palpable en cuadrante superior derecho. Sin signos peritoneales.',
'Cual es el manejo INICIAL de eleccion?',
'{"A":"Laparotomia exploradora inmediata","B":"Reduccion HIDROSTATICA o NEUMATICA guiada por USG/fluoroscopia (si estable, sin peritonitis, menor a 24-48h)","C":"Solo antibioticos","D":"Manejo expectante"}'::jsonb,
'B',
'La respuesta correcta es B. Invaginacion intestinal (ileocolica mas frecuente) en lactantes ESTABLES sin peritonitis: REDUCCION HIDROSTATICA (con solucion salina o contraste) o NEUMATICA (con aire) guiada por USG o fluoroscopia. Exito 70-90%. CONTRAINDICACIONES: peritonitis, perforacion, choque, mayor a 48h evolucion, edad menor a 3 meses o mayor a 5 anos. Cirugia (A) si fallo la reduccion no operatoria o hay complicacion. Recurrencia 10-15%.',
ARRAY['IMSS-524-11']::text[], TRUE)
ON CONFLICT DO NOTHING;

-- ---------- MCQ 3: Estenosis hipertrofica del piloro ----------
INSERT INTO mcq_bank (specialty, difficulty, topic, vignette, question, options_json, correct_answer, explanation_correct, guidelines_ids, is_active)
VALUES ('Cirugia Pediatrica', 'intermediate', 'EHP',
'Lactante masculino 5 semanas con vomitos EN PROYECTIL post-prandiales de 1 semana, no biliosos, con perdida de peso. Masa palpable en epigastrio (oliva pilorica). Onda peristaltica visible.',
'Cual es el diagnostico + alteracion hidroelectrolitica clasica + tratamiento?',
'{"A":"Reflujo GE, sin alteraciones, procineticos","B":"Estenosis hipertrofica del piloro, ALCALOSIS METABOLICA HIPOCLOREMICA HIPOKALEMICA, piloromiotomia de Ramstedt (tras correccion HE)","C":"Atresia esofagica, acidosis, cirugia urgente","D":"Alergia proteina de leche, formula elemental"}'::jsonb,
'B',
'La respuesta correcta es B. EHP tipica en varon primogenito 3-6 semanas, vomito EN PROYECTIL NO BILIOSO + oliva pilorica palpable. La alteracion HE clasica es ALCALOSIS METABOLICA HIPOCLOREMICA HIPOKALEMICA (por perdida de HCl y K en el vomito). USG confirma: espesor muscular pilorico mayor a 3mm, longitud mayor a 15mm. Tratamiento: CORREGIR HE PRIMERO (SF 0.9% + KCl) y luego PILOROMIOTOMIA DE RAMSTEDT (extramucosa). NO operar antes de corregir alcalosis (riesgo apnea postoperatoria).',
ARRAY['IMSS-345-10']::text[], TRUE)
ON CONFLICT DO NOTHING;

-- ---------- MCQ 4: Hernia inguinal pediatrica ----------
INSERT INTO mcq_bank (specialty, difficulty, topic, vignette, question, options_json, correct_answer, explanation_correct, guidelines_ids, is_active)
VALUES ('Cirugia Pediatrica', 'intermediate', 'Hernia inguinal',
'Lactante 6 meses con tumoracion inguinal derecha que aparece con el llanto y se reduce con la calma. Actualmente reductible. Sin dolor. Testiculos descendidos.',
'Cual es la conducta correcta?',
'{"A":"Observacion hasta los 5 anos","B":"Reparacion QUIRURGICA electiva (herniorrafia con seccion del saco herniario) en las siguientes semanas","C":"Fajas herniarias","D":"Solo si se complica"}'::jsonb,
'B',
'La respuesta correcta es B. Hernia inguinal en pediatricos SIEMPRE es indicacion quirurgica ELECTIVA (una vez diagnosticada) por RIESGO DE INCARCERACION/ESTRANGULACION alto en primer ano de vida. Cirugia: HERNIORRAFIA CON LIGADURA ALTA DEL SACO HERNIARIO (no requiere malla como en adultos, es hernia indirecta por persistencia del proceso vaginal). Fajas (C) SIN evidencia y peligrosas. En hernia INCARCERADA irreducible o estrangulada: cirugia URGENTE.',
ARRAY['IMSS-345-10']::text[], TRUE)
ON CONFLICT DO NOTHING;

-- ---------- MCQ 5: Malrotacion intestinal ----------
INSERT INTO mcq_bank (specialty, difficulty, topic, vignette, question, options_json, correct_answer, explanation_correct, guidelines_ids, is_active)
VALUES ('Cirugia Pediatrica', 'advanced', 'Vovulo',
'Recien nacido de 3 semanas con VOMITO BILIOSO subito, distension abdominal, hematoquezia. Estable pero taquicardico. Rx: pocas asas con imagen de "doble burbuja" y ausencia de aire distal.',
'Cual es el diagnostico y conducta?',
'{"A":"Estenosis pilorica, cirugia electiva","B":"MALROTACION INTESTINAL CON VOLVULO DEL INTESTINO MEDIO - LAPAROTOMIA URGENTE (procedimiento de LADD)","C":"Reflujo, procineticos","D":"Alergia alimentaria, cambio de formula"}'::jsonb,
'B',
'La respuesta correcta es B. VOMITO BILIOSO en neonato es EMERGENCIA quirurgica hasta demostrar lo contrario. Sospecha MALROTACION con VOLVULO (riesgo de isquemia intestinal masiva en horas). Diagnostico: transito intestinal (union duodenoyeyunal a la derecha de linea media) o USG (whirlpool sign). NO retrasar: LAPAROTOMIA URGENTE con PROCEDIMIENTO DE LADD (destorsion volvulo, seccion bandas de Ladd, ampliar base del mesenterio, apendicectomia). Retraso mayor a 6h = necrosis masiva y muerte.',
ARRAY['IMSS-345-10']::text[], TRUE)
ON CONFLICT DO NOTHING;

-- ---------- MCQ 6: Criptorquidia ----------
INSERT INTO mcq_bank (specialty, difficulty, topic, vignette, question, options_json, correct_answer, explanation_correct, guidelines_ids, is_active)
VALUES ('Cirugia Pediatrica', 'intermediate', 'Criptorquidia',
'Nino de 14 meses con testiculo derecho no palpable en escroto ni canal inguinal. Nunca descendio. Sin otras anomalias.',
'Cual es la conducta apropiada?',
'{"A":"Observacion hasta pubertad","B":"ORQUIDOPEXIA laparoscopica o abierta entre 6-18 meses de edad","C":"Testosterona","D":"Extraccion (orquiectomia)"}'::jsonb,
'B',
'La respuesta correcta es B. Criptorquidia NO desciende espontaneamente despues de los 6 meses. ORQUIDOPEXIA se recomienda entre 6-18 meses (idealmente 12 meses) para: 1) Reducir riesgo INFERTILIDAD, 2) Reducir riesgo CANCER TESTICULAR (aunque no lo elimina), 3) Prevencion de torsion, 4) Estetica y psicologia. Si NO PALPABLE: laparoscopia diagnostica y descenso. Hormonoterapia (C) obsoleta. Orquiectomia (D) solo si testiculo atrofico intraabdominal o post-puberal.',
ARRAY['IMSS-347-10']::text[], TRUE)
ON CONFLICT DO NOTHING;

-- ---------- MCQ 7: Onfalocele vs gastrosquisis ----------
INSERT INTO mcq_bank (specialty, difficulty, topic, vignette, question, options_json, correct_answer, explanation_correct, guidelines_ids, is_active)
VALUES ('Cirugia Pediatrica', 'advanced', 'Defectos pared',
'Recien nacido con defecto de pared abdominal a la DERECHA del cordon umbilical, sin membrana que cubra las asas intestinales evisceradas. Asas edematizadas.',
'Cual es el diagnostico + conducta inicial?',
'{"A":"Onfalocele, con membrana, riesgo cardiopatia","B":"GASTROSQUISIS (sin membrana, lateral al cordon), cubrir con bolsa esteril + descompresion gastrica + liquidos + cirugia por etapas o silo","C":"Hernia umbilical, cirugia electiva","D":"Extrofia vesical"}'::jsonb,
'B',
'La respuesta correcta es B. GASTROSQUISIS: defecto DERECHO del cordon, SIN membrana, aislada, sin anomalias asociadas (usualmente). ONFALOCELE: defecto CENTRAL, CON membrana amniotica que cubre viscera, ASOCIADO a otras anomalias (cardiacas 30%, trisomias, sindrome Beckwith-Wiedemann). Manejo INICIAL gastrosquisis: 1) cubrir asas con bolsa esteril tibia (evitar hipotermia y perdida agua), 2) SOG a descompresion, 3) LIQUIDOS abundantes 2-3 veces requerimiento (grandes perdidas), 4) ATB, 5) cierre primario o SILO por etapas.',
ARRAY['IMSS-345-10']::text[], TRUE)
ON CONFLICT DO NOTHING;

-- ---------- MCQ 8: Enterocolitis necrotizante ----------
INSERT INTO mcq_bank (specialty, difficulty, topic, vignette, question, options_json, correct_answer, explanation_correct, guidelines_ids, is_active)
VALUES ('Cirugia Pediatrica', 'advanced', 'ECN',
'Prematuro 32 SDG, 15 dias de vida, con distension abdominal, sangre en heces, letargia, apneas. Rx abdomen: NEUMATOSIS INTESTINAL. Sin neumoperitoneo. Plaquetas 80,000.',
'Cual es el manejo?',
'{"A":"Alimentacion enteral aumentada","B":"NPO + SOG descompresion + ATB triple (ampicilina + gentamicina + metronidazol) + NPT + monitor cercano; cirugia si neumoperitoneo o deterioro","C":"Cirugia inmediata sin descompresion","D":"Solo probioticos"}'::jsonb,
'B',
'La respuesta correcta es B. ECN estadio II (Bell) por NEUMATOSIS INTESTINAL. Manejo MEDICO agresivo: 1) NPO, 2) SOG descompresion, 3) ATB triple amplio espectro (amp+genta+metronidazol o pip-tazo), 4) NPT, 5) Reanimacion hidroelectrolitica, 6) Vigilancia clinica y radiologica c/6-8h. INDICACIONES QUIRURGICAS: NEUMOPERITONEO (perforacion), deterioro clinico refractario, masa palpable con obstruccion, asa fija en Rx, acidosis metabolica persistente. Mortalidad 20-30%.',
ARRAY['IMSS-244-10']::text[], TRUE)
ON CONFLICT DO NOTHING;

-- ---------- MCQ 9: Enfermedad de Hirschsprung ----------
INSERT INTO mcq_bank (specialty, difficulty, topic, vignette, question, options_json, correct_answer, explanation_correct, guidelines_ids, is_active)
VALUES ('Cirugia Pediatrica', 'advanced', 'Hirschsprung',
'Neonato masculino que NO EXPULSO MECONIO en las primeras 48h. Distension abdominal, vomitos biliosos. Tacto rectal: ampolla vacia con salida explosiva de gas y meconio al retirar el dedo.',
'Cual es el diagnostico y estudio confirmatorio?',
'{"A":"Ileo meconial (fibrosis quistica), enema Gastrografin","B":"ENFERMEDAD DE HIRSCHSPRUNG (aganglionosis colonica): colon por enema (zona transicion) + BIOPSIA RECTAL (ausencia celulas ganglionares)","C":"Atresia anal, exploracion perineal","D":"Sepsis, hemocultivos"}'::jsonb,
'B',
'La respuesta correcta es B. Retraso en eliminacion de meconio (>48h) + distension + tacto rectal con salida explosiva de gas (signo del "guante") = ENFERMEDAD DE HIRSCHSPRUNG (aganglionosis del plexo mienterico). Estudios: 1) COLON POR ENEMA con contraste hidrosoluble muestra ZONA DE TRANSICION rectosigmoidea, 2) BIOPSIA RECTAL (aspirativa o quirurgica) es DIAGNOSTICO DEFINITIVO (ausencia de celulas ganglionares + aumento acetilcolinesterasa). Manejo: colostomia + procedimiento definitivo (Duhamel, Soave, Swenson).',
ARRAY['IMSS-345-10']::text[], TRUE)
ON CONFLICT DO NOTHING;

-- ---------- MCQ 10: Testiculo agudo ----------
INSERT INTO mcq_bank (specialty, difficulty, topic, vignette, question, options_json, correct_answer, explanation_correct, guidelines_ids, is_active)
VALUES ('Cirugia Pediatrica', 'advanced', 'Torsion testicular',
'Adolescente 14 anos con dolor testicular DERECHO SUBITO hace 3 horas, nausea. Testiculo ASCENDIDO, horizontalizado, muy doloroso. Reflejo cremasterico AUSENTE derecho. USG Doppler: ausencia de flujo.',
'Cual es la conducta INMEDIATA?',
'{"A":"Antibioticos por sospecha orquitis","B":"EXPLORACION QUIRURGICA URGENTE (ventana de oro menor a 6h) con destorsion + orquidopexia bilateral","C":"AINE y observacion 24h","D":"Solo USG de control"}'::jsonb,
'B',
'La respuesta correcta es B. TORSION TESTICULAR: dolor subito + testiculo ascendido/horizontalizado + reflejo cremasterico AUSENTE + Doppler sin flujo = EMERGENCIA QUIRURGICA. Ventana de "oro" menor a 6h para salvar testiculo (viabilidad 90%). Mayor a 24h = necrosis casi segura. Procedimiento: exploracion escrotal + DESTORSION + evaluar viabilidad + ORQUIDOPEXIA BILATERAL (fijar ambos testiculos, 40% de anomalia contralateral). Diagnostico diferencial: orquiepididimitis (dolor progresivo, fiebre, disuria, Doppler AUMENTADO), torsion apendice testicular (dolor localizado polo superior, "punto azul").',
ARRAY['IMSS-347-10']::text[], TRUE)
ON CONFLICT DO NOTHING;

COMMIT;

SELECT COUNT(*) AS total_cirugia_ped FROM mcq_bank WHERE specialty = 'Cirugia Pediatrica';
