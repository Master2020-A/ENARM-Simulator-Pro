-- ================================================================
-- 019_mcqs_pediatria.sql
-- Sprint 7A.3: 10 MCQs de Pediatria estilo ENARM
-- ================================================================

BEGIN;

-- ---------- MCQ 1: Bronquiolitis ----------
INSERT INTO mcq_bank (specialty, difficulty, topic, vignette, question, options_json, correct_answer, explanation_correct, guidelines_ids, is_active)
VALUES ('Pediatria', 'intermediate', 'Bronquiolitis',
'Lactante 6 meses con tos y dificultad respiratoria 3 dias post-cuadro catarral. FR 62, SpO2 89%, tiraje subcostal, sibilancias y estertores crepitantes difusos. Panel viral: VSR positivo. Sin fiebre alta.',
'Cual es el tratamiento CORRECTO?',
'{"A":"Broncodilatador nebulizado + corticoides + antibiotico","B":"MANEJO DE SOPORTE: O2 suplementario meta SpO2 mayor a 92% + hidratacion + aspiracion de secreciones nasales + posicion semi-Fowler. NO broncodilatadores, NO corticoides, NO antibioticos de rutina","C":"Adrenalina subcutanea","D":"Alta domiciliaria"}'::jsonb,
'B',
'La respuesta correcta es B. Bronquiolitis (menor a 2 anos) es enfermedad VIRAL AUTOLIMITADA. Tratamiento SOLO DE SOPORTE con EVIDENCIA: 1) O2 suplementario si SpO2 menor a 90-92%, 2) Hidratacion (VO fraccionada, IV o SOG si dificultad severa), 3) Aspiracion suave de secreciones nasales con SF, 4) Posicion semi-Fowler. LA GPC IMSS Y AAP NO RECOMIENDAN: broncodilatadores (no cambian evolucion), corticoides (sin beneficio, pueden prolongar excrecion viral), antibioticos (viral, salvo sobreinfeccion documentada), mucoliticos, antihistaminicos. HOSPITALIZAR si: SpO2 menor a 92%, dificultad moderada-severa, apneas, deshidratacion, edad menor a 3 meses, comorbilidades. HIGIENE DE MANOS previene transmision.',
ARRAY['IMSS-032-08']::text[], TRUE)
ON CONFLICT DO NOTHING;

-- ---------- MCQ 2: Diarrea aguda ----------
INSERT INTO mcq_bank (specialty, difficulty, topic, vignette, question, options_json, correct_answer, explanation_correct, guidelines_ids, is_active)
VALUES ('Pediatria', 'intermediate', 'Diarrea aguda',
'Nino 18 meses con diarrea acuosa 8 evacuaciones al dia, sin sangre, 3 dias, vomitos ocasionales. Ojos hundidos, mucosas secas, retorno cutaneo lento, pero bebe con avidez y alerta (deshidratacion algun grado, plan B OMS).',
'Cual es el manejo?',
'{"A":"Antibiotico empirico ampicilina","B":"HIDRATACION ORAL con VIDA SUERO ORAL/SRO (75 mL/kg en 4 horas) + alimentacion continuada (NO ayuno) + ZINC 20 mg/dia por 10-14 dias + educacion signos alarma. Reevaluar tras 4h","C":"Antidiarreicos (loperamida)","D":"NPO 48 horas"}'::jsonb,
'B',
'La respuesta correcta es B. Diarrea aguda VIRAL (mas frecuente rotavirus) + deshidratacion algun grado (plan B OMS): TERAPIA DE REHIDRATACION ORAL (TRO) con SRO/vida suero oral 75 mL/kg en 4 horas. Continuar ALIMENTACION (leche materna, dieta habitual, NO ayuno, mejora recuperacion mucosa). ZINC 20 mg/dia (10 mg si menor a 6 meses) por 10-14 dias reduce duracion e intensidad. PROBIOTICOS opcionales (Lactobacillus rhamnosus, S. boulardii). NO usar: antidiarreicos (loperamida contraindicada en pediatricos), antibioticos rutinarios (solo si disenteria, colera, inmunocomprometido, sepsis), antiemeticos rutinarios (ondansetron dosis unica puede ayudar). IV solo si choque, ileo, deshidratacion severa (plan C).',
ARRAY['IMSS-156-08']::text[], TRUE)
ON CONFLICT DO NOTHING;

-- ---------- MCQ 3: Fiebre sin foco ----------
INSERT INTO mcq_bank (specialty, difficulty, topic, vignette, question, options_json, correct_answer, explanation_correct, guidelines_ids, is_active)
VALUES ('Pediatria', 'advanced', 'Fiebre sin foco',
'Lactante 45 dias con fiebre 39C sin foco clinico. Aspecto general bueno. Padres refieren irritabilidad.',
'Cual es la conducta ESTANDAR?',
'{"A":"Alta con paracetamol y observacion domiciliaria","B":"HOSPITALIZAR + estudios completos (BH, PCR, procalcitonina, EGO, urocultivo, hemocultivo, LCR con puncion lumbar, Rx torax) + ANTIBIOTICO EMPIRICO IV (ampicilina + cefotaxima o gentamicina) hasta descartar bacteriemia/meningitis","C":"Solo BH y observacion","D":"Solo antipireticos"}'::jsonb,
'B',
'La respuesta correcta es B. Fiebre sin foco en LACTANTE MENOR A 3 MESES es EMERGENCIA (alto riesgo infeccion bacteriana severa por inmadurez inmune y patogenos verticales como GBS, E. coli, Listeria, herpes). SIEMPRE HOSPITALIZAR + ESTUDIOS COMPLETOS ("sepsis workup"): BH, PCR, procalcitonina, EGO+urocultivo, HEMOCULTIVO, LCR (puncion lumbar), Rx torax si sintomas respiratorios, considerar coproculivo si diarrea. ANTIBIOTICO EMPIRICO IV inmediatamente sin esperar resultados: ampicilina (cubre Listeria y enterococo) + cefotaxima o gentamicina. Escalar segun cultivos. En MAYORES 3 meses: enfoque en apariencia clinica (escala YALE), si "bien lucen" puede manejarse ambulatorio con estudios selectivos.',
ARRAY['IMSS-244-10']::text[], TRUE)
ON CONFLICT DO NOTHING;

-- ---------- MCQ 4: Crup viral ----------
INSERT INTO mcq_bank (specialty, difficulty, topic, vignette, question, options_json, correct_answer, explanation_correct, guidelines_ids, is_active)
VALUES ('Pediatria', 'intermediate', 'Laringotraqueitis',
'Nino 2 anos con tos "PERRUNA" (metalica), disfonia, estridor inspiratorio EN REPOSO, tiraje supraesternal leve. Sin fiebre alta ni toxicidad. Padres muy asustados.',
'Cual es el tratamiento?',
'{"A":"Antibioticos IV","B":"CRUP (laringotraqueitis viral) moderado: DEXAMETASONA 0.6 mg/kg VO/IM/IV dosis unica (max 10 mg) + ADRENALINA NEBULIZADA (racemica o L-adrenalina) + O2 humidificado si SpO2 menor a 92% + observacion 3-4h post-adrenalina","C":"Nebulizacion solo con SF","D":"Intubacion inmediata"}'::jsonb,
'B',
'La respuesta correcta es B. Crup viral (laringotraqueitis, mas frecuente parainfluenza) con ESTRIDOR EN REPOSO = MODERADO (escala Westley). Tratamiento: 1) DEXAMETASONA 0.6 mg/kg VO/IM/IV dosis unica (equivalente prednisolona 1-2 mg/kg), reduce inflamacion subglotica, MEJORA en 2-6 horas, 2) ADRENALINA NEBULIZADA (L-adrenalina 5 mL 1:1000 sin diluir o racemica), efecto rapido pero temporal, OBSERVAR mayor o igual 3-4h por REBOTE, 3) O2 humidificado, 4) Ambiente calmado (llanto empeora). CASOS LEVES (sin estridor reposo): solo dexametasona. SEVEROS con obstruccion critica: adrenalina + dexa + preparar via aerea. NO ANTIBIOTICOS (viral). NO racemica no es superior a L-adrenalina.',
ARRAY['IMSS-244-10']::text[], TRUE)
ON CONFLICT DO NOTHING;

-- ---------- MCQ 5: Vacunas esquema mexicano ----------
INSERT INTO mcq_bank (specialty, difficulty, topic, vignette, question, options_json, correct_answer, explanation_correct, guidelines_ids, is_active)
VALUES ('Pediatria', 'basic', 'Vacunas',
'Nino de 2 meses acude a control sano por primera vez. Sin vacunas previas (madre extranjera). Peso y talla adecuados. Sano.',
'Cuales vacunas del esquema mexicano corresponden a los 2 MESES?',
'{"A":"Solo BCG y hepatitis B","B":"HEXAVALENTE (DPaT + Hib + hepatitis B + polio inactivada IPV) + ROTAVIRUS + NEUMOCOCICA CONJUGADA + polio si esquema atrasado","C":"Solo triple viral","D":"Fiebre amarilla"}'::jsonb,
'B',
'La respuesta correcta es B. Esquema Nacional de Vacunacion Mexico a los 2 MESES: 1) HEXAVALENTE ACELULAR (DPaT + Haemophilus influenzae B + Hepatitis B + Polio IPV), 2) ROTAVIRUS (oral, primera dosis debe ser antes de 15 semanas), 3) NEUMOCOCO CONJUGADA (PCV13). AL NACER: BCG (unica dosis) + Hepatitis B (primera dosis). A los 4, 6 meses: refuerzos hexavalente + rotavirus + neumococo. A los 12 meses: TRIPLE VIRAL (SRP, sarampion-rubeola-parotiditis) + neumococo refuerzo. Influenza anual desde 6 meses. Contraindicaciones ABSOLUTAS: anafilaxia previa a componente. Fiebre alta = POSPONER. Enfermedad leve NO es contraindicacion.',
ARRAY['IMSS-244-10']::text[], TRUE)
ON CONFLICT DO NOTHING;

-- ---------- MCQ 6: Desnutricion aguda severa ----------
INSERT INTO mcq_bank (specialty, difficulty, topic, vignette, question, options_json, correct_answer, explanation_correct, guidelines_ids, is_active)
VALUES ('Pediatria', 'advanced', 'Desnutricion severa',
'Nino 3 anos con emaciacion severa. Peso 8 kg (P/T z-score -3.5), edema bipodal, cabello quebradizo, apatia. Diarrea y neumonia asociadas.',
'Cual es el manejo INICIAL nutricional?',
'{"A":"Alimentacion normal inmediata alta en calorias","B":"MANEJO EN 10 PASOS OMS: 1) Tratar/prevenir HIPOGLUCEMIA (dextrosa VO/IV) + 2) HIPOTERMIA (calentamiento) + 3) DESHIDRATACION (ReSoMal, NO sales OMS regulares por Na alto) + 4) ELECTROLITOS (K, Mg, no Fe inicial) + 5) INFECCION (antibiotico empirico) + 6-7) Micronutrientes + 8) INICIAR alimentacion cauta con F-75 (baja proteina/energia por RIESGO REALIMENTACION) + luego F-100 + 10) Estimulacion","C":"NPT","D":"Solo hidratacion"}'::jsonb,
'B',
'La respuesta correcta es B. Desnutricion aguda severa (kwashiorkor mixto por edema): PROTOCOLO OMS 10 PASOS. Fase ESTABILIZACION 1-7 dias: prevenir/tratar HIPOGLUCEMIA (bolus dextrosa + alimentacion cada 2-3h), HIPOTERMIA (calentar, cubrir), DESHIDRATACION con ReSoMal (Rehidratacion Solucion Malnutricion, MENOS sodio que SRO regular por retencion Na), ELECTROLITOS (K, Mg altos; NO HIERRO fase inicial por riesgo infeccion), TRATAR INFECCION (ATB empirico amoxicilina/ampicilina + gentamicina), MICRONUTRIENTES (vitamina A, folato, zinc), INICIAR alimentacion CAUTA con F-75 (75 kcal/100mL, baja proteina para evitar SINDROME REALIMENTACION mortal). Fase REHABILITACION: transicion a F-100, ganancia rapida peso, luego alimentos habituales. Estimulacion emocional y seguimiento.',
ARRAY['IMSS-244-10']::text[], TRUE)
ON CONFLICT DO NOTHING;

-- ---------- MCQ 7: Convulsion febril ----------
INSERT INTO mcq_bank (specialty, difficulty, topic, vignette, question, options_json, correct_answer, explanation_correct, guidelines_ids, is_active)
VALUES ('Pediatria', 'intermediate', 'Convulsion febril',
'Nino 18 meses con fiebre 39.5C por otitis media, presenta convulsion tonico-clonica generalizada de 3 minutos. Recupera consciencia progresivamente. Sin foco neurologico. Sin antecedentes. Exploracion post-crisis normal.',
'Cual es la conducta apropiada?',
'{"A":"EEG y RM cerebral urgentes","B":"CONVULSION FEBRIL SIMPLE (menor a 15 min, generalizada, unica en 24h, edad 6m-5a, sin foco neurologico). Manejo: tratar CAUSA de fiebre (otitis con amoxicilina) + antipireticos (paracetamol) + educacion a padres + NO EEG, NO RM, NO PUNCION LUMBAR de rutina, NO anticonvulsivantes crónicos","C":"Fenobarbital cronico","D":"Ingresar UCI"}'::jsonb,
'B',
'La respuesta correcta es B. Convulsion FEBRIL SIMPLE criterios: edad 6 meses-5 anos, con fiebre, GENERALIZADA, menor a 15 minutos, UNICA en 24h, sin foco neurologico, sin antecedentes epilepsia. PRONOSTICO BUENO (no aumenta riesgo epilepsia significativamente, recurrencia 30%). Manejo: 1) TRATAR CAUSA DE FIEBRE, 2) Antipireticos (paracetamol/ibuprofeno), 3) EDUCACION padres (registros de duracion, posicion lateral), 4) NO estudios rutinarios (EEG, imagen, PL solo si menor a 12m o meningismo). Convulsion FEBRIL COMPLEJA: focal, mayor a 15 min, recurrente 24h, mas riesgo. Considerar PL si menor a 12m o antibiotico previo. Estatus epileptico: diazepam rectal 0.5 mg/kg o midazolam intranasal.',
ARRAY['IMSS-244-10']::text[], TRUE)
ON CONFLICT DO NOTHING;

-- ---------- MCQ 8: Otitis media aguda ----------
INSERT INTO mcq_bank (specialty, difficulty, topic, vignette, question, options_json, correct_answer, explanation_correct, guidelines_ids, is_active)
VALUES ('Pediatria', 'basic', 'OMA',
'Nino 3 anos con otalgia derecha, fiebre 38.5, otoscopia: membrana timpanica ABOMBADA, ERITEMATOSA, OPACA, sin perforacion. Sin secrecion. Primer episodio.',
'Cual es el tratamiento?',
'{"A":"Observacion 48-72h sin antibiotico","B":"OMA CONFIRMADA (abombamiento + eritema) en nino mayor a 2 anos SIN toxicidad: puede optarse por OBSERVACION 48-72h (analgesia sola) O AMOXICILINA 80-90 mg/kg/dia VO cada 12h por 10 dias como primera linea (segun preferencia familia, gravedad). Si falla: amoxicilina-clavulanico","C":"Ceftriaxona IM","D":"Miringotomia"}'::jsonb,
'B',
'La respuesta correcta es B. OMA (abombamiento MT + inflamacion + sintomas) en NINO MAYOR A 2 ANOS sin toxicidad, unilateral, sin otorrea = ESTRATEGIA DE OBSERVACION 48-72h con analgesia (paracetamol/ibuprofeno) es opcion valida (60-80% resuelve solo). Si NO mejora en 48-72h o EMPEORA: iniciar antibiotico. TRATAMIENTO INMEDIATO indicado si: MENOR a 2 anos, BILATERAL, OTORREA, toxicidad, fiebre mayor a 39, dolor severo. PRIMERA LINEA: AMOXICILINA 80-90 mg/kg/dia dividido cada 8-12h por 10 dias (5-7 dias si mayor a 6 anos). SEGUNDA LINEA (falla 48-72h, uso ATB reciente, OMA recurrente): AMOXICILINA-CLAVULANICO. Alergia penicilina: cefdinir/cefpodoxima o azitromicina. Descongestivos y antihistaminicos SIN evidencia.',
ARRAY['IMSS-063-08']::text[], TRUE)
ON CONFLICT DO NOTHING;

-- ---------- MCQ 9: Anemia ferropenica lactante ----------
INSERT INTO mcq_bank (specialty, difficulty, topic, vignette, question, options_json, correct_answer, explanation_correct, guidelines_ids, is_active)
VALUES ('Pediatria', 'intermediate', 'Anemia',
'Lactante 12 meses con palidez, irritabilidad, alimentacion basada en LECHE DE VACA ENTERA desde los 6 meses (mayor a 500 mL/dia), poca variedad alimentaria. Hb 8.5 g/dL, VCM 68 fL (bajo), RDW alto, ferritina 5 ng/mL (baja).',
'Cual es el diagnostico + tratamiento?',
'{"A":"Talasemia, transfusion","B":"ANEMIA FERROPENICA por consumo excesivo de leche de vaca. Tratamiento: HIERRO ELEMENTAL 3-6 mg/kg/dia VO (sulfato ferroso) por 3-6 meses + LIMITAR leche de vaca a menor a 500 mL/dia + alimentos ricos en hierro (carne, hemo mejor absorcion) + Vitamina C (mejora absorcion) + reevaluar Hb en 4 semanas (debe subir mayor o igual 1 g/dL)","C":"Vitamina B12","D":"Acido folico"}'::jsonb,
'B',
'La respuesta correcta es B. ANEMIA FERROPENICA (Hb baja + VCM bajo + RDW alto + FERRITINA BAJA) es la anemia mas comun en lactantes. Causa clasica: exceso de LECHE DE VACA (baja en hierro, causa perdidas GI microscopicas, satura y desplaza alimentos ricos en hierro). Tratamiento: 1) HIERRO ELEMENTAL 3-6 mg/kg/dia VO (sulfato ferroso, mejor con jugo citricos, entre comidas), 2) LIMITAR leche de vaca a menor a 500 mL/dia (idealmente introducir despues del ano), 3) DIETA rica en hierro (carne roja, higado, legumbres, cereales fortificados), 4) VITAMINA C (mejora absorcion), 5) Reevaluar Hb 4 semanas (debe subir mayor o igual 1 g/dL), 6) Continuar hierro 2-3 meses despues de normalizar Hb (repletar depositos). PROFILAXIS: hierro 1 mg/kg desde 4-6 meses en lactancia exclusiva o pretermino.',
ARRAY['IMSS-063-08']::text[], TRUE)
ON CONFLICT DO NOTHING;

-- ---------- MCQ 10: Displasia de cadera ----------
INSERT INTO mcq_bank (specialty, difficulty, topic, vignette, question, options_json, correct_answer, explanation_correct, guidelines_ids, is_active)
VALUES ('Pediatria', 'intermediate', 'DDC',
'Recien nacido femenino con antecedente presentacion podalica, hermana con displasia cadera. Exploracion: maniobra de ORTOLANI positiva (clic al abducir cadera), pliegues gluteos asimetricos.',
'Cual es el estudio + tratamiento?',
'{"A":"Rx pelvis (menor a 4-6 meses es INFERIOR al USG por osificacion incompleta)","B":"ULTRASONIDO DE CADERA (metodo de eleccion menor a 4-6 meses, mas sensible sin radiacion) + ARNES DE PAVLIK (mantener flexion y abduccion, 3-6 meses) con revisiones semanales. Efectividad mayor a 90% si menor a 6 meses","C":"Yeso pelvipedio inmediato","D":"Cirugia inmediata"}'::jsonb,
'B',
'La respuesta correcta es B. DDC (displasia desarrollo cadera) factores riesgo: SEXO FEMENINO, PRESENTACION PODALICA, ANTECEDENTE FAMILIAR, primogenito, oligohidramnios. Diagnostico clinico: ORTOLANI (reduccion cadera luxada al abducir), BARLOW (luxa cadera reducible), asimetria pliegues, limitacion abduccion, signo Galeazzi (rodillas asimetricas). ESTUDIO: USG DE CADERA es de eleccion en menores 4-6 meses (mejor que Rx por cartilago no osificado). Rx pelvis util despues 4-6 meses. TAMIZAJE UNIVERSAL clinico al nacer y controles. TRATAMIENTO menor a 6 meses: ARNES DE PAVLIK (flexion 90-100 y abduccion, 23h/dia por 3-6 meses, 90% exito). 6m-18m: reduccion cerrada + yeso pelvipedio. Mayor a 18m: reduccion quirurgica. Sin tratamiento: cojera, artrosis precoz.',
ARRAY['IMSS-063-08']::text[], TRUE)
ON CONFLICT DO NOTHING;

COMMIT;

SELECT COUNT(*) AS total_pediatria FROM mcq_bank WHERE specialty = 'Pediatria';
