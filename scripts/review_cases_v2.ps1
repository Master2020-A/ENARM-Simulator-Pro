# ═══════════════════════════════════════════════════════════════
# review_cases_v2.ps1 - Review hibrido con timeout adaptativo
# ═══════════════════════════════════════════════════════════════
param(
    [int]$MaxCases = 10,
    [string]$Model = "qwen2.5:14b",       # default rapido
    [int]$TimeoutSec = 180,                # 3 min por caso default
    [switch]$AutoPublish,
    [switch]$DryRun,
    [switch]$OnlyRejected                 # re-review de casos rechazados
)

$env:PGPASSWORD = "postgres"

Write-Host "`n=========================================================" -ForegroundColor Cyan
Write-Host "  ENARM Case Reviewer v2 (hybrid)" -ForegroundColor Cyan
Write-Host "=========================================================" -ForegroundColor Cyan
Write-Host "  Modelo:        $Model" -ForegroundColor White
Write-Host "  Max casos:     $MaxCases" -ForegroundColor White
Write-Host "  Timeout:       ${TimeoutSec}s por caso" -ForegroundColor White
Write-Host "  Auto-publish:  $AutoPublish" -ForegroundColor White
Write-Host ""

# Query de casos pendientes
$whereClause = if ($OnlyRejected) {
    "reviewed = TRUE AND is_published = FALSE AND reviewer_notes LIKE '%reject%'"
} else {
    "generated_by IS NOT NULL AND reviewed = FALSE"
}

$query = "SELECT id, title, specialty, difficulty FROM clinical_cases WHERE $whereClause ORDER BY created_at ASC LIMIT $MaxCases;"

$pendingCases = & psql -U postgres -h localhost -d enarm_db -tAc $query 2>&1

if (-not $pendingCases -or $pendingCases -notmatch "-") {
    Write-Host "  No hay casos pendientes" -ForegroundColor Green
    exit 0
}

$cases = $pendingCases | Where-Object { $_ -match "^[a-f0-9]{8}-" } | ForEach-Object {
    $parts = $_ -split "\|"
    [PSCustomObject]@{
        id = $parts[0]
        title = $parts[1]
        specialty = $parts[2]
        difficulty = $parts[3]
    }
}

Write-Host "  Casos a revisar: $($cases.Count)" -ForegroundColor Yellow
Write-Host ""

$stats = @{approved=0; needsReview=0; rejected=0; failed=0}
$counter = 0
$totalStart = Get-Date

foreach ($case in $cases) {
    $counter++
    Write-Host "`n---------------------------------------------------------" -ForegroundColor DarkCyan
    Write-Host "  [$counter/$($cases.Count)] $($case.specialty) - $($case.difficulty)" -ForegroundColor Cyan
    Write-Host "  Titulo: $($case.title)" -ForegroundColor White
    Write-Host "  ID:     $($case.id)" -ForegroundColor Gray

    # Traer JSON compacto del caso
    $q = "SELECT jsonb_build_object('title',title,'specialty',specialty,'difficulty',difficulty,'chief',chief_complaint,'dx',expected_diagnosis_json,'tx',expected_treatments_json) FROM clinical_cases WHERE id = '$($case.id)';"
    $caseJson = & psql -U postgres -h localhost -d enarm_db -tAc $q 2>&1

    if ($caseJson.Length -gt 3000) {
        $caseJson = $caseJson.Substring(0, 3000) + "...(truncado)"
    }

    $auditPrompt = @"
Eres auditor medico del ENARM Mexico. Audita este caso clinico generado por IA.

CASO:
$caseJson

Responde SOLO JSON valido:
{
  "clinically_valid": true|false,
  "specialty_matches": true|false,
  "difficulty_matches": true|false,
  "issues": ["issue1"],
  "recommendation": "approve|review|reject",
  "confidence": 0.85
}

- approve: caso correcto medicamente y en su especialidad
- review: necesita ajustes menores
- reject: errores medicos graves o especialidad incorrecta
"@

    Write-Host "  Consultando $Model (timeout ${TimeoutSec}s)..." -ForegroundColor Yellow

    $reqBody = @{
        model = $Model
        prompt = $auditPrompt
        stream = $false
        format = "json"
        options = @{
            temperature = 0.2
            num_ctx = 4096
        }
    } | ConvertTo-Json -Depth 10

    $startTime = Get-Date
    $audit = $null

    try {
        $response = Invoke-RestMethod `
            -Uri "http://localhost:11434/api/generate" `
            -Method POST `
            -ContentType "application/json" `
            -Body $reqBody `
            -TimeoutSec $TimeoutSec

        try {
            $audit = $response.response | ConvertFrom-Json
        } catch {
            Write-Host "  JSON invalido de $Model" -ForegroundColor Red
            $stats.failed++
            continue
        }
    } catch {
        Write-Host "  TIMEOUT o error: $($_.Exception.Message.Substring(0, [Math]::Min(80, $_.Exception.Message.Length)))" -ForegroundColor Red
        $stats.failed++
        continue
    }

    $duration = ((Get-Date) - $startTime).TotalSeconds

    $recColor = switch ($audit.recommendation) {
        "approve" { "Green" }
        "review"  { "Yellow" }
        "reject"  { "Red" }
        default   { "Gray" }
    }

    Write-Host "  -> $($audit.recommendation.ToUpper()) (conf=$($audit.confidence)) en $([math]::Round($duration,1))s" -ForegroundColor $recColor

    if ($audit.issues -and $audit.issues.Count -gt 0) {
        $audit.issues | Select-Object -First 2 | ForEach-Object {
            Write-Host "    ! $_" -ForegroundColor Yellow
        }
    }

    if (-not $audit.specialty_matches) {
        Write-Host "    ! Specialty mismatch" -ForegroundColor Red
    }

    switch ($audit.recommendation) {
        "approve" { $stats.approved++ }
        "review"  { $stats.needsReview++ }
        "reject"  { $stats.rejected++ }
    }

    if (-not $DryRun) {
        $auditJson = $audit | ConvertTo-Json -Depth 10 -Compress
        $auditEscaped = $auditJson -replace "'", "''"
        $shouldPublish = $AutoPublish -and `
                        ($audit.recommendation -eq "approve") -and `
                        $audit.specialty_matches -and `
                        $audit.clinically_valid

        $updateSql = @"
UPDATE clinical_cases
SET reviewed = TRUE,
    reviewer_notes = '$auditEscaped',
    is_published = $(if($shouldPublish){"TRUE"}else{"FALSE"}),
    reviewed_by = '$Model'
WHERE id = '$($case.id)';
"@
        $tmp = "$env:TEMP\rev_$($case.id).sql"
        [System.IO.File]::WriteAllText($tmp, $updateSql, [System.Text.UTF8Encoding]::new($false))
        & psql -U postgres -h localhost -d enarm_db -f $tmp 2>&1 | Out-Null
        Remove-Item $tmp -ErrorAction SilentlyContinue
    }

    Start-Sleep -Milliseconds 500
}

$totalMin = ((Get-Date) - $totalStart).TotalMinutes

Write-Host "`n=========================================================" -ForegroundColor Cyan
Write-Host "  REVIEW COMPLETADO en $([math]::Round($totalMin,1)) minutos" -ForegroundColor Cyan
Write-Host "=========================================================" -ForegroundColor Cyan
Write-Host "  Approve:  $($stats.approved)" -ForegroundColor Green
Write-Host "  Review:   $($stats.needsReview)" -ForegroundColor Yellow
Write-Host "  Reject:   $($stats.rejected)" -ForegroundColor Red
Write-Host "  Failed:   $($stats.failed)" -ForegroundColor DarkGray

Write-Host "`nEstado BD:" -ForegroundColor Yellow
& psql -U postgres -h localhost -d enarm_db -c @"
SELECT
    COUNT(*) as total,
    COUNT(*) FILTER (WHERE reviewed = TRUE) as reviewed,
    COUNT(*) FILTER (WHERE is_published = TRUE) as published,
    COUNT(*) FILTER (WHERE generated_by IS NOT NULL) as ai_generated
FROM clinical_cases;
"@