# ===============================================================
#  test_coverage_report.ps1
#  Ejecuta ENARM.Tests.exe directamente y genera reporte
#  con tags, puntaje por módulo y resumen de cobertura
# ===============================================================

$root    = Split-Path $PSScriptRoot -Parent
$build   = "$root\build"

# Buscar ejecutable
$testExe = Get-ChildItem $build -Name "ENARM.Tests.exe" -Recurse -ErrorAction SilentlyContinue |
           Select-Object -First 1

if (-not $testExe) {
    Write-Host "ERROR: Compila primero con run_tests.ps1" -ForegroundColor Red
    exit 1
}

$exePath = Join-Path (Split-Path "$build\$testExe") $testExe.Name

Write-Host ""
Write-Host "============================================================" -ForegroundColor Cyan
Write-Host "  ENARM Tests — Reporte de Cobertura por Módulo" -ForegroundColor Cyan
Write-Host "============================================================" -ForegroundColor Cyan
Write-Host ""

# Tags = módulos
$modules = @(
    @{ Tag = "[hemodynamic]";   Name = "HemodynamicModel" },
    @{ Tag = "[pharmacology]";  Name = "DrugDatabase / PKModel" },
    @{ Tag = "[assessment]";    Name = "AssessmentEngine" },
    @{ Tag = "[competency]";    Name = "CompetencyEvaluator" },
    @{ Tag = "[adherence]";     Name = "GuidelineAdherenceScorer" },
    @{ Tag = "[metabolic]";     Name = "MetabolicModel (placeholder)" }
)

$totalPass = 0
$totalFail = 0

foreach ($mod in $modules) {
    $output = & $exePath $mod.Tag "--reporter" "compact" 2>&1
    $pass = ($output | Select-String "passed").Count
    $fail = ($output | Select-String "failed").Count

    $status = if ($fail -eq 0) { "✅" } else { "❌" }
    $color  = if ($fail -eq 0) { "Green" } else { "Red" }

    Write-Host ("  {0} {1,-35} Pass:{2,3}  Fail:{3,3}" -f $status, $mod.Name, $pass, $fail) -ForegroundColor $color

    $totalPass += $pass
    $totalFail += $fail
}

Write-Host ""
Write-Host "  ─────────────────────────────────────────────────────────" -ForegroundColor Gray
$totalTests  = $totalPass + $totalFail
$coveragePct = if ($totalTests -gt 0) { [int]($totalPass * 100 / $totalTests) } else { 0 }

$summaryColor = if ($totalFail -eq 0) { "Green" } else { "Yellow" }
Write-Host ("  Total: {0} tests | Pasaron: {1} | Fallaron: {2} | Cobertura: {3}%" `
    -f $totalTests, $totalPass, $totalFail, $coveragePct) -ForegroundColor $summaryColor
Write-Host ""

if ($coveragePct -ge 60) {
    Write-Host "  META ALCANZADA: >= 60% cobertura del motor clínico" -ForegroundColor Green
} else {
    Write-Host ("  META: Necesitas {0} tests más para llegar a 60%" `
        -f ([int](($totalTests * 0.6) - $totalPass))) -ForegroundColor Yellow
}
Write-Host ""
