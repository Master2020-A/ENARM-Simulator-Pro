# GUARDAR EN: C:\Users\Master\Documents\ENARM-IA\ENARMSimulator\scripts\generate_case.ps1

param(
    [Parameter(Mandatory=$true)]
    [string]$Specialty,

    [ValidateSet("basico","intermedio","avanzado","basic","intermediate","advanced")]
    [string]$Difficulty = "intermedio",

    [string]$Topic = "",

    [string]$Model = "qwen2.5-coder:7b",

    [switch]$DryRun,     # No inserta en BD
    [switch]$VerboseOutput
)

# Normalizar difficulty al formato en_db
$DifficultyNormalized = switch ($Difficulty) {
    "basico" { "basic" }
    "intermedio" { "intermediate" }
    "avanzado" { "advanced" }
    default { $Difficulty }
}

Write-Host "`n============================================================" -ForegroundColor Cyan
Write-Host "  ENARM Case Generator" -ForegroundColor Cyan
Write-Host "============================================================" -ForegroundColor Cyan
Write-Host "  Especialidad:  $Specialty" -ForegroundColor White
Write-Host "  Dificultad:    $DifficultyNormalized" -ForegroundColor White
Write-Host "  Modelo:        $Model" -ForegroundColor White
Write-Host "  Topico:        $(if($Topic){$Topic}else{'auto'})" -ForegroundColor White
Write-Host ""

# ═══════════════════════════════════════════════════════════════
# SYSTEM PROMPT
# ═══════════════════════════════════════════════════════════════
$systemPrompt = @"
Eres un medico especialista mexicano experto en el ENARM (Examen Nacional para Aspirantes a Residencias Medicas).

Debes generar UN caso clinico realista para estudiantes de medicina siguiendo el formato JSON EXACTO que se te indica.

REGLAS CRITICAS:
1. TODA la informacion clinica debe ser medicamente correcta y consistente
2. Los signos vitales deben ser coherentes con el diagnostico
3. Usa nombres mexicanos (Juan Perez Garcia, Maria Lopez Hernandez, etc.)
4. Edades y demografia realistas para Mexico
5. Guias clinicas: IMSS/CENETEC, Surviving Sepsis, GOLD, AHA/ACC
6. Escribe SIN acentos ni caracteres especiales
7. La respuesta debe ser SOLO el JSON valido, sin markdown ni explicaciones

FORMATO JSON REQUERIDO (respeta EXACTAMENTE estos nombres de campos):
{
  "title": "Titulo descriptivo del caso (max 500 chars)",
  "slug": "titulo-en-minusculas-con-guiones",
  "chief_complaint": "Motivo de consulta en palabras del paciente",
  "specialty": "$Specialty",
  "difficulty": "$DifficultyNormalized",
  "setting": "emergency|outpatient|inpatient|icu",
  "patient_json": {
    "name": "Nombre Apellido Apellido",
    "age": 45,
    "age_unit": "years",
    "sex": "M|F",
    "weight_kg": 70,
    "height_cm": 170,
    "occupation": "Ocupacion",
    "vitals_on_arrival": {
      "hr": 80, "sbp": 120, "dbp": 80,
      "rr": 16, "spo2": 98, "temp_c": 36.8
    },
    "past_medical_history": ["antecedente 1", "antecedente 2"],
    "allergies": "ninguna conocida",
    "current_medications": ["med 1"],
    "family_history": "relevante o negado",
    "social_history": "tabaquismo, alcohol, drogas"
  },
  "history_present_illness": "Descripcion detallada del padecimiento actual (mininmo 200 palabras, formato narrativo medico)",
  "symptoms_json": [
    {"name": "sintoma1", "duration": "3 dias", "severity": "moderada"},
    {"name": "sintoma2", "duration": "1 semana", "severity": "leve"}
  ],
  "expected_diagnosis_json": {
    "primary": "Diagnostico exacto principal",
    "icd10": "Codigo CIE-10 (ej: I21.0)",
    "differential": ["Dx diferencial 1", "Dx diferencial 2", "Dx diferencial 3"],
    "justification": "Por que este diagnostico basado en anamnesis, exploracion y estudios"
  },
  "expected_studies_json": [
    {"name": "ECG 12 derivaciones", "priority": "urgent", "expected_finding": "elevacion ST V1-V4"},
    {"name": "Troponinas seriadas", "priority": "urgent", "expected_finding": "elevadas"},
    {"name": "Radiografia de torax", "priority": "routine", "expected_finding": "sin datos de congestion"}
  ],
  "expected_treatments_json": [
    {"treatment": "Aspirina 300mg VO", "priority": "immediate", "evidence": "Class I - AHA/ACC 2022"},
    {"treatment": "Clopidogrel 600mg carga VO", "priority": "immediate", "evidence": "Class I"},
    {"treatment": "Enoxaparina 1mg/kg SC c/12h", "priority": "urgent", "evidence": "Class I"},
    {"treatment": "Angioplastia primaria en <90 min", "priority": "urgent", "evidence": "Class I"}
  ],
  "expected_followup": "Descripcion del seguimiento esperado",
  "learning_objectives_json": [
    "Identificar criterios diagnosticos de X",
    "Aplicar protocolo Y en primera hora",
    "Reconocer complicaciones de Z"
  ],
  "teaching_points": "Puntos clave para el estudiante ENARM"
}

DIFICULTAD SEGUN NIVEL:
- basic: Presentacion clasica del padecimiento, examen fisico claro, dx obvio
- intermediate: Presentacion tipica con 1-2 elementos atipicos, dx diferencial importante
- advanced: Presentacion compleja, comorbilidades, requiere razonamiento clinico avanzado

Genera el caso AHORA. Solo JSON valido, nada mas.
"@

$userPrompt = if ($Topic) {
    "Genera un caso clinico de $Specialty nivel $DifficultyNormalized sobre: $Topic"
} else {
    "Genera un caso clinico de $Specialty nivel $DifficultyNormalized. Elige un topico comun del ENARM para esta especialidad."
}

# ═══════════════════════════════════════════════════════════════
# LLAMADA A OLLAMA
# ═══════════════════════════════════════════════════════════════
Write-Host "[1/3] Generando caso con Ollama ($Model)..." -ForegroundColor Yellow

$requestBody = @{
    model = $Model
    prompt = "$systemPrompt`n`n$userPrompt"
    stream = $false
    format = "json"
    options = @{
        temperature = 0.7
        num_ctx = 8192
    }
} | ConvertTo-Json -Depth 10

$startTime = Get-Date
try {
    $response = Invoke-RestMethod `
        -Uri "http://localhost:11434/api/generate" `
        -Method POST `
        -ContentType "application/json" `
        -Body $requestBody `
        -TimeoutSec 300
} catch {
    Write-Host "  ERROR: $_" -ForegroundColor Red
    exit 1
}
$duration = ((Get-Date) - $startTime).TotalSeconds

Write-Host "  OK - Generado en $([math]::Round($duration,1))s" -ForegroundColor Green
Write-Host "  Tokens: $($response.eval_count) eval / $($response.prompt_eval_count) prompt" -ForegroundColor Gray

# ═══════════════════════════════════════════════════════════════
# VALIDACION JSON
# ═══════════════════════════════════════════════════════════════
Write-Host "`n[2/3] Validando JSON..." -ForegroundColor Yellow

$rawJson = $response.response
if ($VerboseOutput) {
    Write-Host "  JSON raw ($($rawJson.Length) chars):" -ForegroundColor Gray
    Write-Host $rawJson.Substring(0, [Math]::Min(500, $rawJson.Length)) -ForegroundColor DarkGray
    Write-Host "..." -ForegroundColor DarkGray
}

try {
    $caseData = $rawJson | ConvertFrom-Json
} catch {
    Write-Host "  ERROR - JSON invalido: $_" -ForegroundColor Red
    Write-Host "`n  Raw output:" -ForegroundColor Yellow
    Write-Host $rawJson.Substring(0, [Math]::Min(1000, $rawJson.Length)) -ForegroundColor DarkGray
    exit 1
}

# Verificar campos obligatorios
$requiredFields = @("title", "slug", "chief_complaint", "specialty", "difficulty",
                    "setting", "patient_json", "expected_diagnosis_json")
$missing = @()
foreach ($field in $requiredFields) {
    if (-not $caseData.PSObject.Properties[$field] -or -not $caseData.$field) {
        $missing += $field
    }
}

if ($missing.Count -gt 0) {
    Write-Host "  ERROR - Campos faltantes:" -ForegroundColor Red
    $missing | ForEach-Object { Write-Host "    - $_" -ForegroundColor Red }
    exit 1
}

Write-Host "  OK - JSON valido con todos los campos requeridos" -ForegroundColor Green
Write-Host "  Titulo: $($caseData.title)" -ForegroundColor White
Write-Host "  Slug:   $($caseData.slug)" -ForegroundColor White

# ═══════════════════════════════════════════════════════════════
# GUARDAR JSON EN DISCO
# ═══════════════════════════════════════════════════════════════
$outputDir = "C:\Users\Master\Documents\ENARM-IA\ENARMSimulator\Content\cases\generated"
New-Item -ItemType Directory -Force -Path $outputDir | Out-Null

$timestamp = Get-Date -Format "yyyyMMdd_HHmmss"
$jsonPath = "$outputDir\case_${Specialty}_${DifficultyNormalized}_${timestamp}.json"

$prettyJson = $caseData | ConvertTo-Json -Depth 20
[System.IO.File]::WriteAllText($jsonPath, $prettyJson, [System.Text.UTF8Encoding]::new($false))
Write-Host "  Guardado: $jsonPath" -ForegroundColor Gray

# ═══════════════════════════════════════════════════════════════
# INSERTAR EN BD (si no es dry-run)
# ═══════════════════════════════════════════════════════════════
if ($DryRun) {
    Write-Host "`n[3/3] DRY-RUN activo - NO se inserta en BD" -ForegroundColor Yellow
    Write-Host "`n============================================================" -ForegroundColor Green
    Write-Host "  Caso generado (dry-run)" -ForegroundColor Green
    Write-Host "============================================================" -ForegroundColor Green
    exit 0
}

Write-Host "`n[3/3] Insertando en enarm_db..." -ForegroundColor Yellow

# Escapar strings para SQL
function Escape-SqlString($s) {
    if ($null -eq $s) { return "NULL" }
    return "'" + ($s -replace "'", "''") + "'"
}

function ToJsonSql($obj) {
    if ($null -eq $obj) { return "'{}'::jsonb" }
    $json = $obj | ConvertTo-Json -Depth 20 -Compress
    return "'" + ($json -replace "'", "''") + "'::jsonb"
}

$title              = Escape-SqlString $caseData.title
$slug               = Escape-SqlString $caseData.slug
$chief              = Escape-SqlString $caseData.chief_complaint
$spec               = Escape-SqlString $caseData.specialty
$diff               = Escape-SqlString $caseData.difficulty
$setting            = Escape-SqlString $caseData.setting
$patientJson        = ToJsonSql $caseData.patient_json
$hpi                = Escape-SqlString $caseData.history_present_illness
$symptomsJson       = ToJsonSql $caseData.symptoms_json
$dxJson             = ToJsonSql $caseData.expected_diagnosis_json
$studiesJson        = ToJsonSql $caseData.expected_studies_json
$treatmentsJson     = ToJsonSql $caseData.expected_treatments_json
$followup           = Escape-SqlString $caseData.expected_followup
$learningJson       = ToJsonSql $caseData.learning_objectives_json
$teachingPoints     = Escape-SqlString $caseData.teaching_points
$generatedBy        = Escape-SqlString "ollama:$Model"
$genPrompt          = Escape-SqlString $userPrompt

$insertSql = @"
INSERT INTO clinical_cases (
    title, slug, chief_complaint, specialty, difficulty, setting,
    patient_json, history_present_illness, symptoms_json,
    expected_diagnosis_json, expected_studies_json, expected_treatments_json,
    expected_followup, learning_objectives_json, teaching_points,
    generated_by, generation_prompt, reviewed, is_published
) VALUES (
    $title, $slug, $chief, $spec, $diff, $setting,
    $patientJson, $hpi, $symptomsJson,
    $dxJson, $studiesJson, $treatmentsJson,
    $followup, $learningJson, $teachingPoints,
    $generatedBy, $genPrompt, FALSE, FALSE
) RETURNING id;
"@

$tmpSqlFile = "$env:TEMP\case_insert_$timestamp.sql"
[System.IO.File]::WriteAllText($tmpSqlFile, $insertSql, [System.Text.UTF8Encoding]::new($false))

$env:PGPASSWORD = "postgres"
$insertResult = & psql -U postgres -h localhost -d enarm_db -f $tmpSqlFile 2>&1
Remove-Item $tmpSqlFile -Force -ErrorAction SilentlyContinue

if ($LASTEXITCODE -eq 0) {
    Write-Host "  OK - Insertado en BD" -ForegroundColor Green
    $insertResult | Where-Object { $_ -match "^\s*[a-f0-9]{8}" } | ForEach-Object {
        Write-Host "  ID: $($_.Trim())" -ForegroundColor White
    }
} else {
    Write-Host "  ERROR - Fallo insercion:" -ForegroundColor Red
    $insertResult | ForEach-Object { Write-Host "    $_" -ForegroundColor Red }
    exit 1
}

Write-Host "`n============================================================" -ForegroundColor Green
Write-Host "  Caso generado, validado e insertado en enarm_db" -ForegroundColor Green
Write-Host "============================================================" -ForegroundColor Green
Write-Host "  Titulo:  $($caseData.title)"
Write-Host "  JSON:    $jsonPath"
Write-Host "  Tiempo:  $([math]::Round($duration,1))s"
Write-Host ""
Write-Host "  Verificar en BD:" -ForegroundColor Gray
Write-Host "  psql -U postgres -d enarm_db -c `"SELECT title FROM clinical_cases ORDER BY created_at DESC LIMIT 1;`"" -ForegroundColor Gray