-- ================================================================
-- 009_new_cases_part2.sql
-- Casos 007, 008, 009, 010
-- ================================================================

BEGIN;

-- ================================================================
-- CASO 007: TCE severo por accidente automovilistico
-- ================================================================
INSERT INTO clinical_cases (
    title, slug, chief_complaint, specialty, sub_specialty,
    difficulty, setting, patient_json, history_present_illness,
    symptoms_json, expected_diagnosis_json, expected_studies_json,
    expected_treatments_json, expected_followup, learning_objectives_json,
    pubmed_pmids, guidelines_ids, teaching_points, author_name, is_published
) VALUES (
    'Trauma craneoencefalico severo por accidente automovilistico en varon de 32 anos',
    'tce-severo-accidente-007',
    'Perdida de conciencia post trauma craneal, Glasgow 7',
    'EmergencyMedicine',
    'Trauma',
    'advanced',
    'emergency',
    '{"full_name": "Carlos Mendez Aguilar", "age": 32, "sex": "M", "occupation": "Chofer", "weight_kg": 78, "height_cm": 175, "allergies": ["ninguna conocida"], "medications": ["ninguna"], "medical_history": ["Sano previo al trauma"], "mechanism": "Colision frontal a 80 km/h sin cinturon de seguridad, expulsion parcial del vehiculo"}'::jsonb,
    'Masculino de 32 anos ingresado por servicio de urgencias tras colision automovilistica hace 45 minutos. Fue chofer sin cinturon, con expulsion parcial del vehiculo. Llega en ambulancia con collarin cervical, inmovilizado en tabla rigida. Presenta perdida de conciencia desde el trauma, Glasgow inicial 7 (O2 V2 M3). Pupila derecha midriatica 5mm hiporreactiva, izquierda 3mm normorreactiva. Sangrado activo en cuero cabelludo region parietal derecha. Hematoma periorbitario bilateral (ojos de mapache). Otorragia derecha. Signos vitales: PA 90/60, FC 55, FR 8 irregular, SpO2 88% con mascarilla reservorio.',
    '["Glasgow 7 (severo)", "anisocoria (midriasis derecha)", "signos de fractura de base de craneo", "otorragia", "ojos de mapache", "hipoventilacion", "bradicardia con hipotension"]'::jsonb,
    '["TCE severo (Glasgow <=8)", "Sospecha de hematoma epidural o subdural derecho", "Fractura de base de craneo", "Hipertension intracraneal con triada de Cushing incipiente"]'::jsonb,
    '["ABCDE con proteccion cervical", "TAC de craneo simple URGENTE", "TAC cervical completo", "Radiografia toraco-abdominal en camilla", "FAST ecografia abdominal", "Biometria hematica", "Quimica sanguinea", "Tiempos de coagulacion", "Grupo sanguineo y pruebas cruzadas", "Gasometria arterial", "Alcoholemia y toxicos"]'::jsonb,
    '["Intubacion orotraqueal con secuencia rapida (Glasgow <8)", "Ventilacion mecanica con normoventilacion (PaCO2 35-40)", "Elevacion de cabecera 30 grados en posicion neutra", "Manitol 1 g/kg IV en bolo o solucion salina hipertonica 3%", "Meta PAM >=80 mmHg", "Meta PPC 60-70 mmHg", "Interconsulta URGENTE a neurocirugia", "Craniectomia descompresiva si hematoma expansivo", "Sedoanalgesia con propofol y fentanilo", "Profilaxis anticonvulsivante con levetiracetam"]'::jsonb,
    'UCI con monitoreo de PIC. Control TAC en 6-12h. Vigilar aparicion de lesiones tardias. Rehabilitacion multidisciplinaria temprana. Pronostico depende de Glasgow inicial y lesiones asociadas.',
    '["Aplicar ABCDE en trauma con proteccion cervical", "Reconocer signos de HIC: anisocoria + bradicardia + hipertension (triada Cushing)", "Indicaciones de intubacion en TCE (Glasgow <=8)", "Manejo osmotico (manitol vs salina hipertonica)", "Signos clinicos de fractura de base de craneo"]'::jsonb,
    ARRAY['27932556', '28959891']::text[],
    ARRAY[]::text[],
    'Glasgow: 13-15 leve, 9-12 moderado, 3-8 severo. Todo Glasgow <=8 requiere intubacion. Signos de Cushing (HTA + bradicardia + respiracion irregular) indican HIC. Manitol contraindicado si hipotension. Evitar hiperventilacion agresiva (isquemia). Objetivo PPC 60-70 (PPC = PAM - PIC).',
    'ENARM Content Team',
    TRUE
)
ON CONFLICT (slug) DO NOTHING;


-- ================================================================
-- CASO 008: Cetoacidosis diabetica en adolescente
-- ================================================================
INSERT INTO clinical_cases (
    title, slug, chief_complaint, specialty, sub_specialty,
    difficulty, setting, patient_json, history_present_illness,
    symptoms_json, expected_diagnosis_json, expected_studies_json,
    expected_treatments_json, expected_followup, learning_objectives_json,
    pubmed_pmids, guidelines_ids, teaching_points, author_name, is_published
) VALUES (
    'Cetoacidosis diabetica como debut en adolescente de 15 anos',
    'cad-debut-adolescente-008',
    'Poliuria, polidipsia, perdida de peso y dolor abdominal de 3 semanas',
    'PediatricEndocrinology',
    'Diabetes',
    'advanced',
    'emergency',
    '{"full_name": "Sofia Torres Delgado", "age": 15, "sex": "F", "weight_kg": 52, "height_cm": 165, "weight_loss_kg": 8, "allergies": ["ninguna"], "medications": ["ninguna"], "medical_history": ["Sin antecedentes previos", "Menarca a los 12 anos, ciclos regulares"], "family_history": ["Abuela materna con DM2"], "school_grade": "3ro secundaria"}'::jsonb,
    'Adolescente de 15 anos previamente sana, llevada a urgencias por sus padres. Refiere cuadro de 3 semanas de evolucion con poliuria y polidipsia progresivas (bebe mas de 4 litros al dia y se levanta 3-4 veces a orinar por la noche). Perdida de peso de 8 kg no intencional. En los ultimos 2 dias inicio con nauseas, vomitos, dolor abdominal difuso y somnolencia progresiva. Al examen: paciente somnolienta pero orientada, respiracion profunda y rapida (Kussmaul), aliento a frutas (cetonico), signos de deshidratacion severa (mucosas secas, ojos hundidos, llenado capilar 4 seg), taquicardica.',
    '["poliuria", "polidipsia", "perdida de peso 8 kg en 3 semanas", "vomitos", "dolor abdominal", "respiracion de Kussmaul", "aliento cetonico", "deshidratacion severa", "alteracion del estado de alerta"]'::jsonb,
    '["Diabetes mellitus tipo 1 (debut)", "Cetoacidosis diabetica moderada-severa", "Deshidratacion severa"]'::jsonb,
    '["Glucosa capilar y venosa STAT", "Gasometria arterial con anion gap", "Cetonas en sangre (beta-hidroxibutirato) y orina", "Electrolitos sericos (Na, K, Cl, P, Mg)", "Biometria hematica", "Quimica sanguinea (BUN, creatinina)", "Examen general de orina", "Hemoglobina glucosilada A1c", "Anticuerpos anti-GAD y anti-islote", "Peptido C basal", "Cultivos si sospecha de infeccion desencadenante"]'::jsonb,
    '["Solucion salina 0.9% 20 ml/kg en 30-60 min, luego calcular hidratacion 48h", "Insulina regular en infusion 0.05-0.1 UI/kg/hora IV (NO bolo en pediatricos)", "Reponer potasio si K <5.5 (10-40 mEq/L en sueros)", "Cambiar a suero glucosado 5% cuando glucosa <250 mg/dL", "Bicarbonato solo si pH <6.9 (controversial en pediatria)", "Monitoreo neurologico cada 1h (riesgo de edema cerebral)", "Transicion a insulina SC cuando resolucion de acidosis"]'::jsonb,
    'Hospitalizacion en UCI pediatrica primeras 24-48h. Educacion diabetologica intensiva a paciente y familia antes del alta. Inicio de esquema basal-bolo con analogos de insulina. Referencia a endocrinologia pediatrica. Consulta nutriologia y psicologia. Automonitoreo capilar 4-6 veces al dia.',
    '["Criterios diagnosticos de CAD: glucosa >250, pH <7.3 o HCO3 <15, cetonemia/cetonuria", "Reposicion cuidadosa de liquidos en pediatricos (riesgo edema cerebral)", "Insulinoterapia sin bolo IV inicial en ninos", "Manejo de potasio (bajara con insulina)", "Identificar desencadenantes (infeccion, mala adherencia)", "Diferenciar DM1 vs DM2 en adolescentes"]'::jsonb,
    ARRAY['30948445', '32006810']::text[],
    ARRAY['IMSS-032-08', 'IMSS-104-08']::text[],
    'CAD pediatrica tiene mayor riesgo de EDEMA CEREBRAL (mortalidad 20-25%). Reposicion de liquidos calculada para 48h, no rapida. Signos alarma edema cerebral: cefalea, cambio conductual, incontinencia, HTA, bradicardia. Tratar con manitol 0.5-1 g/kg o salina hipertonica 3%. Anion gap = Na - (Cl + HCO3), normal 8-12. Recordar que la kalemia baja con insulina (introducirla a suero cuando K entra al rango normal).',
    'ENARM Content Team',
    TRUE
)
ON CONFLICT (slug) DO NOTHING;


-- ================================================================
-- CASO 009: Lesion renal aguda por AINEs
-- ================================================================
INSERT INTO clinical_cases (
    title, slug, chief_complaint, specialty, sub_specialty,
    difficulty, setting, patient_json, history_present_illness,
    symptoms_json, expected_diagnosis_json, expected_studies_json,
    expected_treatments_json, expected_followup, learning_objectives_json,
    pubmed_pmids, guidelines_ids, teaching_points, author_name, is_published
) VALUES (
    'Lesion renal aguda por AINEs en adulto mayor con HTA y DM2',
    'lra-aines-adulto-mayor-009',
    'Disminucion del volumen urinario y edema de miembros inferiores de 5 dias',
    'Nephrology',
    'Lesion renal aguda',
    'intermediate',
    'ward',
    '{"full_name": "Guillermo Salinas Ochoa", "age": 71, "sex": "M", "occupation": "Jubilado", "weight_kg": 78, "height_cm": 170, "allergies": ["ninguna"], "medications": ["Metformina 850mg BID", "Enalapril 10mg BID", "Hidroclorotiazida 25mg/dia", "Ibuprofeno 600mg cada 8h desde hace 10 dias por lumbalgia", "Omeprazol 20mg/dia"], "medical_history": ["HTA 15 anos", "DM2 8 anos con Hb A1c 7.2%", "Osteoartritis de columna lumbar", "TFG basal estimada 68 mL/min hace 6 meses"]}'::jsonb,
    'Masculino de 71 anos con HTA y DM2, quien acude por cuadro de 5 dias de evolucion caracterizado por disminucion progresiva del volumen urinario (refiere orinar 3 veces al dia con volumen reducido), edema de miembros inferiores hasta rodillas, fatiga, nauseas y anorexia. Hace 10 dias inicio ibuprofeno 600mg cada 8h por lumbalgia intensa, ademas de continuar con su tratamiento habitual (metformina, enalapril, hidroclorotiazida). Refiere disminucion de ingesta hidrica en la ultima semana por sensacion de saciedad. Al examen: PA 155/92, FC 82, FR 20, edema 2+ maleolar bilateral, ruidos cardiacos ritmicos sin soplos, campos pulmonares con estertores basales bilaterales.',
    '["oliguria", "edema periferico", "fatiga", "nauseas", "anorexia", "hipertension", "sobrecarga hidrica pulmonar leve"]'::jsonb,
    '["Lesion renal aguda KDIGO 2-3 (probable pre-renal o renal por AINEs)", "Nefritis intersticial aguda inducida por AINEs", "Empeoramiento por combinacion IECA + diuretico + AINE (triple whammy)"]'::jsonb,
    '["Creatinina serica y comparar con basal", "Urea y BUN", "Electrolitos (Na, K, Cl, HCO3, Ca, P)", "Biometria hematica", "Examen general de orina con sedimento", "Excrecion fraccional de sodio (FENa)", "Ultrasonido renal y de vias urinarias", "Gasometria venosa", "Osmolaridad urinaria", "Proteinuria en 24h o indice proteina/creatinina"]'::jsonb,
    '["SUSPENDER ibuprofeno de inmediato", "SUSPENDER enalapril e hidroclorotiazida temporalmente", "SUSPENDER metformina (riesgo acidosis lactica)", "Restaurar volemia con cristaloides IV (evaluar sobrecarga)", "Manejo de hipercalemia si K >5.5 (calcio, insulina + glucosa, resinas, dialisis)", "Analgesico alternativo (paracetamol)", "Furosemida si sobrecarga hidrica con adecuada perfusion", "Interconsulta a nefrologia", "Considerar biopsia renal si no mejora en 7-14 dias"]'::jsonb,
    'Vigilancia diaria de funcion renal y diuresis. Recuperacion esperada en 7-14 dias tras retiro de AINE. Reintroducir enalapril cuando creatinina basal recuperada. NUNCA reiniciar AINEs. Educacion sobre nefrotoxicidad de OTC. Control con nefrologia en 4 semanas. Considerar switch de metformina si TFG <30.',
    '["Reconocer triple whammy nefrotoxico: IECA + diuretico + AINE", "Clasificacion KDIGO de LRA (creatinina + diuresis)", "Diferenciar pre-renal vs renal (FENa <1% vs >2%)", "Nefritis intersticial aguda: eosinofiluria, cilindros leucocitarios", "Manejo urgente de hipercalemia", "Ajuste de medicamentos segun TFG"]'::jsonb,
    ARRAY['31537691', '32603795']::text[],
    ARRAY['IMSS-020-08']::text[],
    'Criterios KDIGO 2012 para LRA: aumento creatinina >=0.3 mg/dL en 48h, o aumento >=1.5x basal en 7 dias, o diuresis <0.5 mL/kg/h por 6+h. AINEs causan vasoconstriccion de arteriola aferente por inhibicion prostaglandinas (importante en adultos mayores, diabeticos, hipertensos, deshidratacion). El "triple whammy" (IECA + diuretico + AINE) multiplica riesgo x8. Metformina se suspende con creatinina >1.5 hombres o >1.4 mujeres.',
    'ENARM Content Team',
    TRUE
)
ON CONFLICT (slug) DO NOTHING;


-- ================================================================
-- CASO 010: Bronquiolitis aguda en lactante
-- ================================================================
INSERT INTO clinical_cases (
    title, slug, chief_complaint, specialty, sub_specialty,
    difficulty, setting, patient_json, history_present_illness,
    symptoms_json, expected_diagnosis_json, expected_studies_json,
    expected_treatments_json, expected_followup, learning_objectives_json,
    pubmed_pmids, guidelines_ids, teaching_points, author_name, is_published
) VALUES (
    'Bronquiolitis aguda por VSR en lactante de 6 meses con dificultad respiratoria',
    'bronquiolitis-vsr-lactante-010',
    'Tos, dificultad para respirar y rechazo al alimento de 3 dias',
    'Pediatrics',
    'Neumologia pediatrica',
    'intermediate',
    'emergency',
    '{"full_name": "Emilio Ruiz Cortes", "age_months": 6, "sex": "M", "weight_kg": 7.8, "height_cm": 68, "gestational_age_at_birth": 39, "birth_weight_kg": 3.2, "allergies": ["ninguna"], "medications": ["ninguna"], "medical_history": ["Recien nacido a termino sin complicaciones", "Alimentacion con lactancia materna exclusiva hasta el mes 4, luego formula", "Vacunas completas al mes 6", "Sin hospitalizaciones previas"], "epidemiology": "Hermano de 3 anos con cuadro respiratorio leve la semana pasada, guarderia desde los 4 meses"}'::jsonb,
    'Lactante masculino de 6 meses, previamente sano, llevado por sus padres a urgencias por cuadro de 3 dias de evolucion. Inicio con rinorrea hialina, estornudos y tos seca. En las ultimas 24h progreso a tos productiva, dificultad respiratoria con tiraje intercostal, sibilancias audibles a distancia, rechazo al alimento (solo ha tomado 60% de sus tomas habituales) y fiebre de hasta 38.5C. Refieren disminucion de panales mojados en las ultimas 12h (3 vs 6 habituales). Contacto epidemiologico: hermano mayor con cuadro respiratorio leve la semana previa. Al examen: FR 62 rpm, FC 165 lpm, SpO2 90% ambiente, T 38.2C, aleteo nasal, tiraje sub e intercostal, sibilancias espiratorias difusas bilaterales, estertores crepitantes bibasales.',
    '["rinorrea", "tos productiva", "sibilancias", "dificultad respiratoria con tiraje", "aleteo nasal", "taquipnea", "hipoxemia", "rechazo alimentario", "signos de deshidratacion leve"]'::jsonb,
    '["Bronquiolitis aguda por VSR (probable)", "Insuficiencia respiratoria aguda moderada", "Deshidratacion leve"]'::jsonb,
    '["Oximetria de pulso", "Panel viral respiratorio (VSR, influenza, parainfluenza, metapneumovirus)", "Radiografia de torax (solo si duda diagnostica o mala evolucion)", "Gasometria capilar si dificultad respiratoria severa", "Biometria hematica y PCR (no rutinaria, solo si sospecha bacteriemia)", "NO se recomienda de rutina cultivos, panel completo o TAC"]'::jsonb,
    '["Oxigeno suplementario para SpO2 >=92%", "Hidratacion con tomas fraccionadas o SNG si intolerancia", "Solucion salina hipertonica al 3% nebulizada (opcional, controversial)", "Aspiracion gentil de secreciones nasales", "Posicion semifowler", "NO se recomiendan de rutina: broncodilatadores, corticoides, antibioticos", "Lavados nasales con solucion salina", "Vigilancia estrecha de dificultad respiratoria"]'::jsonb,
    'Hospitalizacion si: SpO2 <90% persistente, dificultad respiratoria severa, apneas, deshidratacion, edad <3 meses. La mayoria se resuelve en 7-10 dias. Signos de alarma para padres: aumento del trabajo respiratorio, cianosis, apneas, letargo, incapacidad de alimentarse. Prevencion: lavado de manos, aislamiento respiratorio, palivizumab en prematuros de alto riesgo.',
    '["Bronquiolitis: primer episodio de sibilancias en menor de 2 anos", "Etiologia predominante VSR (70%)", "Diagnostico CLINICO, no de laboratorio", "Manejo principalmente de soporte (oxigeno + hidratacion)", "Evitar broncodilatadores y corticoides (evidencia no muestra beneficio)", "Criterios de hospitalizacion", "Rol del palivizumab en prevencion"]'::jsonb,
    ARRAY['31577892', '32406196']::text[],
    ARRAY[]::text[],
    'La bronquiolitis es la infeccion respiratoria baja mas comun en <2 anos, causada 70% por VSR. El manejo es de SOPORTE: oxigeno para mantener SpO2 >=90-92%, hidratacion, aspiracion gentil de secreciones. Las guias actuales (AAP 2014) NO recomiendan uso rutinario de: broncodilatadores, corticoides sistemicos, antibioticos, fisioterapia respiratoria. El palivizumab (anticuerpo monoclonal anti-VSR) esta indicado para prevencion en prematuros <29 SDG, displasia broncopulmonar, cardiopatia congenita hemodinamicamente significativa.',
    'ENARM Content Team',
    TRUE
)
ON CONFLICT (slug) DO NOTHING;

COMMIT;

-- Verificacion
SELECT id, title, specialty, difficulty
FROM clinical_cases
WHERE slug LIKE '%-007%' OR slug LIKE '%-008%' OR slug LIKE '%-009%' OR slug LIKE '%-010%'
ORDER BY slug;

SELECT COUNT(*) AS total_casos FROM clinical_cases;
SELECT specialty, COUNT(*) FROM clinical_cases GROUP BY specialty ORDER BY specialty;