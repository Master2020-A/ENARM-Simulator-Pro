-- ================================================================
-- 025_gpcs_fix_final.sql
-- Sprint 7A.4.7: Agregar HSA + enriquecer 11 GPCs originales
-- ================================================================

BEGIN;

-- ========== FIX: IMSS-372-10 Hemorragia Subaracnoidea (referencia rota) ==========
INSERT INTO clinical_guidelines (id, title, organization, year, url, specialty, summary, key_recommendations, tags)
VALUES ('IMSS-372-10',
'Diagnostico y Tratamiento de la Hemorragia Subaracnoidea Espontanea',
'IMSS', 2018, 'http://www.cenetec.salud.gob.mx',
'Neurologia',
'HSA por rotura de aneurisma es emergencia neuroquirurgica. Cefalea explosiva "la peor de mi vida". TAC craneo urgente (95% sensibilidad primeras 6h). Nimodipino previene vasoespasmo (principal causa morbimortalidad tardia).',
'[
  {"level":"A","text":"Cefalea EXPLOSIVA (thunderclap) + vomito + rigidez de nuca = sospecha HSA hasta demostrar contrario"},
  {"level":"A","text":"TAC craneo SIN contraste urgente (sensibilidad 95% primeras 6h)"},
  {"level":"A","text":"Si TAC negativo con alta sospecha: PUNCION LUMBAR (xantocromia)"},
  {"level":"A","text":"AngioTAC o panangiografia para localizar aneurisma"},
  {"level":"A","text":"Tratamiento del aneurisma en <24-72h: CLIPAJE quirurgico o EMBOLIZACION endovascular (coils)"},
  {"level":"A","text":"NIMODIPINO 60mg VO cada 4h por 21 dias (previene vasoespasmo, principal causa morbimortalidad)"},
  {"level":"A","text":"Control TA meta sistolica <160 mmHg"},
  {"level":"B","text":"Escala de Hunt-Hess y Fisher para pronostico"}
]'::jsonb,
ARRAY['HSA','hemorragia subaracnoidea','aneurisma','nimodipino','vasoespasmo','neurologia','urgencias']::text[])
ON CONFLICT (id) DO UPDATE SET
  specialty = EXCLUDED.specialty, summary = EXCLUDED.summary,
  key_recommendations = EXCLUDED.key_recommendations, tags = EXCLUDED.tags;

-- ================================================================
-- ENRIQUECER 11 GPCs ORIGINALES (agregar specialty, key_recommendations, tags)
-- ================================================================

-- IMSS-234-09 IAM CEST (original)
UPDATE clinical_guidelines SET
    specialty = 'Cardiologia',
    key_recommendations = '[
        {"level":"A","text":"Reperfusion oportuna: ICP primaria en <90 min o fibrinolisis <30 min"},
        {"level":"A","text":"Doble antiagregacion (AAS + inhibidor P2Y12) inmediata"},
        {"level":"A","text":"Anticoagulacion parenteral (HNF, enoxaparina o bivalirudina)"},
        {"level":"B","text":"Estatinas alta intensidad al ingreso, betabloqueadores si no contraindicados"}
    ]'::jsonb,
    tags = ARRAY['IAM','STEMI','reperfusion','cardiologia']::text[]
WHERE id = 'IMSS-234-09';

-- IMSS-162-09 SCA SEST (original)
UPDATE clinical_guidelines SET
    specialty = 'Cardiologia',
    key_recommendations = '[
        {"level":"A","text":"Estratificar riesgo TIMI/GRACE para estrategia invasiva vs conservadora"},
        {"level":"A","text":"Doble antiagregacion + anticoagulacion"},
        {"level":"A","text":"NO usar fibrinoliticos en SCASEST"}
    ]'::jsonb,
    tags = ARRAY['SCA','SCASEST','angina inestable','cardiologia']::text[]
WHERE id = 'IMSS-162-09';

-- IMSS-014-08 HTA
UPDATE clinical_guidelines SET
    specialty = 'Cardiologia',
    key_recommendations = '[
        {"level":"A","text":"Diagnostico: >=2 mediciones >=140/90 en 2 visitas separadas"},
        {"level":"A","text":"Meta general <140/90; <130/80 en DM, ERC, ECV establecida"},
        {"level":"A","text":"Primera linea: IECA/ARA-II, calcio antagonistas, diureticos tiazidicos"},
        {"level":"A","text":"Cambios estilo vida: dieta DASH, ejercicio, restriccion sodio <2g/dia"}
    ]'::jsonb,
    tags = ARRAY['HTA','hipertension','IECA','cardiologia','primer nivel']::text[]
WHERE id = 'IMSS-014-08';

-- IMSS-718-14 NAC
UPDATE clinical_guidelines SET
    specialty = 'Neumologia',
    key_recommendations = '[
        {"level":"A","text":"Estratificar gravedad con CURB-65 (0-1 ambulatorio, 2 hospital, >=3 UCI)"},
        {"level":"A","text":"Hospitalario: beta-lactamico (ceftriaxona) + macrolido (azitromicina)"},
        {"level":"A","text":"Ambulatorio previamente sano: amoxicilina o azitromicina"},
        {"level":"B","text":"Rx torax confirma, hemocultivos si hospitalizado, antigeno urinario neumococo/Legionella si grave"}
    ]'::jsonb,
    tags = ARRAY['NAC','neumonia','CURB-65','neumologia','infectologia']::text[]
WHERE id = 'IMSS-718-14';

-- IMSS-104-08 DM2
UPDATE clinical_guidelines SET
    specialty = 'Endocrinologia',
    key_recommendations = '[
        {"level":"A","text":"Diagnostico: HbA1c >=6.5% O glucosa ayuno >=126 O CTOG 2h >=200 O sintomas + glucosa >=200"},
        {"level":"A","text":"Metformina 1a linea (contraindicada TFG <30)"},
        {"level":"A","text":"SGLT2i con beneficio CV/renal (empagliflozina, dapagliflozina)"},
        {"level":"A","text":"GLP-1RA (semaglutida) con beneficio CV y perdida de peso"},
        {"level":"A","text":"Meta HbA1c <7% (individualizar)"}
    ]'::jsonb,
    tags = ARRAY['DM2','diabetes','metformina','SGLT2i','endocrinologia']::text[]
WHERE id = 'IMSS-104-08';

-- IMSS-020-08 (era ERC en original, ahora colisiona con preeclampsia)
UPDATE clinical_guidelines SET
    specialty = 'Nefrologia',
    key_recommendations = '[
        {"level":"A","text":"Tamizaje anual en poblacion de riesgo (DM, HTA, edad >60, familia con ERC)"},
        {"level":"A","text":"Diagnostico: TFG <60 mL/min/1.73m2 por >=3 meses O albuminuria >=30 mg/g"},
        {"level":"A","text":"Estadificacion KDIGO por TFG (G1-G5) y albuminuria (A1-A3)"},
        {"level":"A","text":"IECA/ARA-II si proteinuria, meta TA <130/80"}
    ]'::jsonb,
    tags = ARRAY['ERC','IRC','KDIGO','nefroproteccion','nefrologia']::text[]
WHERE id = 'IMSS-020-08';

-- IMSS-085-08 Sepsis (original)
UPDATE clinical_guidelines SET
    specialty = 'Infectologia',
    key_recommendations = '[
        {"level":"A","text":"Bundle 1a hora: lactato + hemocultivos + antibiotico + cristaloides + vasopresor"},
        {"level":"A","text":"Cristaloides 30 mL/kg si hipotension o lactato >4"},
        {"level":"A","text":"Norepinefrina 1a linea, meta PAM >=65"},
        {"level":"A","text":"Control foco infeccioso <6-12h"}
    ]'::jsonb,
    tags = ARRAY['sepsis','shock septico','bundle','infectologia','urgencias']::text[]
WHERE id = 'IMSS-085-08';

-- IMSS-320-10 Asma (original - actualiza con datos ya definidos)
UPDATE clinical_guidelines SET
    specialty = 'Neumologia',
    key_recommendations = '[
        {"level":"A","text":"Crisis: SABA + O2 (SpO2 >94%) + corticoide sistemico en 1a hora"},
        {"level":"A","text":"Corticoide inhalado a largo plazo es piedra angular"},
        {"level":"A","text":"NUNCA usar SABA como monoterapia (aumenta mortalidad)"},
        {"level":"B","text":"Sulfato de magnesio 2g IV si crisis severa refractaria"}
    ]'::jsonb,
    tags = ARRAY['asma','crisis asmatica','SABA','neumologia']::text[]
WHERE id = 'IMSS-320-10';

-- IMSS-037-08 EPOC
UPDATE clinical_guidelines SET
    specialty = 'Neumologia',
    key_recommendations = '[
        {"level":"A","text":"Diagnostico: espirometria post-broncodilatador FEV1/CVF <0.7"},
        {"level":"A","text":"Clasificacion GOLD A/B/C/D por sintomas + exacerbaciones"},
        {"level":"A","text":"Exacerbacion Anthonisen tipo 1 (disnea + tos + purulencia): ATB + corticoide + broncodilatador"},
        {"level":"A","text":"O2 controlado meta SpO2 88-92% (evitar hiperoxia)"},
        {"level":"A","text":"Cesacion tabaquica y rehabilitacion pulmonar"}
    ]'::jsonb,
    tags = ARRAY['EPOC','exacerbacion','GOLD','Anthonisen','neumologia']::text[]
WHERE id = 'IMSS-037-08';

-- IMSS-058-08 EVC (era EVC isquemico en original, ahora colisiona con preeclampsia usada)
UPDATE clinical_guidelines SET
    specialty = 'Neurologia',
    key_recommendations = '[
        {"level":"A","text":"Alteplase IV en ventana <=4.5h desde inicio sintomas"},
        {"level":"A","text":"Trombectomia mecanica hasta 6-24h en oclusion grande"},
        {"level":"A","text":"TAC craneo urgente sin contraste"},
        {"level":"A","text":"NIHSS estratifica gravedad"}
    ]'::jsonb,
    tags = ARRAY['ACV','EVC','alteplase','trombolisis','neurologia']::text[]
WHERE id = 'IMSS-058-08';

-- IMSS-706-14 Preeclampsia
UPDATE clinical_guidelines SET
    specialty = 'Gineco-Obstetricia',
    key_recommendations = '[
        {"level":"A","text":"Sulfato de magnesio (impregnacion 4-6g IV + mantto 1-2 g/h) previene eclampsia"},
        {"level":"A","text":"Antihipertensivo IV en severa (hidralazina, labetalol) meta TAS 140-155"},
        {"level":"A","text":"Betametasona 12mg IM cada 24h x2 dosis (maduracion pulmonar <34 SDG)"},
        {"level":"A","text":"Interrupcion embarazo tras estabilizacion en severa/HELLP"}
    ]'::jsonb,
    tags = ARRAY['preeclampsia','eclampsia','HELLP','sulfato magnesio','ginecologia']::text[]
WHERE id = 'IMSS-706-14';

-- IMSS-032-08 CAD adultos (ya era CAD en original)
UPDATE clinical_guidelines SET
    specialty = 'Endocrinologia',
    key_recommendations = '[
        {"level":"A","text":"Hidratacion PRIMERO: SF 0.9% 1-1.5 L en 1a hora en adultos"},
        {"level":"A","text":"Insulina EN INFUSION 0.1 UI/kg/h (en adultos SI bolo inicial 0.1 UI/kg)"},
        {"level":"A","text":"Potasio (KCl 20-40 mEq/L) cuando K <5.2 con diuresis"},
        {"level":"A","text":"Dextrosa cuando glucosa <200-250"},
        {"level":"C","text":"Bicarbonato SOLO si pH <6.9"}
    ]'::jsonb,
    tags = ARRAY['CAD','cetoacidosis','DM1','DM2','endocrinologia']::text[]
WHERE id = 'IMSS-032-08';

COMMIT;

-- ================================================================
-- VERIFICACION FINAL
-- ================================================================
SELECT 'TOTAL GPCs:' AS metrica, COUNT(*)::text AS valor FROM clinical_guidelines
UNION ALL
SELECT 'GPCs SIN specialty:', COUNT(*)::text FROM clinical_guidelines WHERE specialty IS NULL
UNION ALL
SELECT 'GPCs SIN key_recommendations:', COUNT(*)::text FROM clinical_guidelines WHERE key_recommendations IS NULL
UNION ALL
SELECT 'Referencias rotas:', COUNT(DISTINCT r.gpc_id)::text
FROM (
    SELECT UNNEST(guidelines_ids) AS gpc_id FROM mcq_bank
    UNION
    SELECT UNNEST(guidelines_ids) FROM clinical_cases
) r
LEFT JOIN clinical_guidelines g ON g.id = r.gpc_id
WHERE g.id IS NULL;
