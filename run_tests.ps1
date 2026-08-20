# ═══════════════════════════════════════════════════════════════
#   run_tests.ps1 — Compilar y ejecutar tests ENARM
#   Uso:
#     .\run_tests.ps1                    # Todos los tests
#     .\run_tests.ps1 -Tag "[unit]"      # Solo unitarios
#     .\run_tests.ps1 -Tag "[hemodynamic]"
#     .\run_tests.ps1 -Tag "~[integration]"  # Excluir integración
# ═══════════════════════════════════════════════════════════════

param(
    [string]$Tag       = "",
    [string]$BuildType = "Debug",
    [switch]$NoBuild   = $false
)

$root      = "C:\Users\Master\Documents\ENARM-IA\ENARMSimulator"
$buildDir  = "$root\build"
$testExe   = "$buildDir\engine\ENARM.Tests\$BuildType\ENARM.Tests.exe"

Write-Host ""
Write-Host "════════════════════════════════════════════" -ForegroundColor Cyan
Write-Host "  ENARM - Suite de Tests" -ForegroundColor Cyan
Write-Host "════════════════════════════════════════════" -ForegroundColor Cyan

# ── Build ────────────────────────────────────────────────────────
if (-not $NoBuild) {
    Write-Host ""
    Write-Host "🔨 Compilando ENARM.Tests..." -ForegroundColor Yellow

    $buildResult = cmake --build $buildDir `
                         --config $BuildType `
                         --target ENARM.Tests `
                         --parallel 8 2>&1

    if ($LASTEXITCODE -ne 0) {
        Write-Host ""
        Write-Host "❌ ERROR DE COMPILACIÓN:" -ForegroundColor Red
        $buildResult | Where-Object { $_ -match "error" } |
            ForEach-Object { Write-Host "   $_" -ForegroundColor Red }
        Write-Host ""
        Write-Host "💡 Errores comunes:" -ForegroundColor Yellow
        Write-Host "   - Include path incorrecto → ajustar en test_*.cpp" -ForegroundColor White
        Write-Host "   - Método no existe → verificar API real del header" -ForegroundColor White
        Write-Host "   - Namespace incorrecto → checar usando IntelliSense" -ForegroundColor White
        exit 1
    }

    Write-Host "  ✅ Compilación exitosa" -ForegroundColor Green
}

# ── Ejecutar tests ───────────────────────────────────────────────
if (-not (Test-Path $testExe)) {
    # Buscar en otras ubicaciones
    $testExe = Get-ChildItem $buildDir -Recurse -Filter "ENARM.Tests.exe" |
               Select-Object -First 1 -ExpandProperty FullName
}

if (-not $testExe -or -not (Test-Path $testExe)) {
    Write-Host "❌ No se encontró ENARM.Tests.exe en $buildDir" -ForegroundColor Red
    exit 1
}

Write-Host ""
Write-Host "🧪 Ejecutando tests..." -ForegroundColor Yellow
if ($Tag) { Write-Host "   Filtro: $Tag" -ForegroundColor Cyan }
Write-Host ""

$testArgs = @("--reporter", "console", "--success", "--colour-mode", "ansi")
if ($Tag) { $testArgs += $Tag }

& $testExe @testArgs

$exitCode = $LASTEXITCODE

Write-Host ""
if ($exitCode -eq 0) {
    Write-Host "════════════════════════════════════════════" -ForegroundColor Green
    Write-Host "  ✅ TODOS LOS TESTS PASARON" -ForegroundColor Green
    Write-Host "════════════════════════════════════════════" -ForegroundColor Green
} else {
    Write-Host "════════════════════════════════════════════" -ForegroundColor Red
    Write-Host "  ❌ ALGUNOS TESTS FALLARON (código: $exitCode)" -ForegroundColor Red
    Write-Host "════════════════════════════════════════════" -ForegroundColor Red
    Write-Host ""
    Write-Host "📋 Para ver solo los fallos:" -ForegroundColor Yellow
    Write-Host "   .\run_tests.ps1 -NoBuild" -ForegroundColor White
    Write-Host ""
    Write-Host "📋 Para ver tests de un módulo:" -ForegroundColor Yellow
    Write-Host "   .\run_tests.ps1 -Tag '[hemodynamic]' -NoBuild" -ForegroundColor White
}

Write-Host ""
exit $exitCode
