# ===============================================================
#  run_tests.ps1 — Compila y ejecuta todos los tests de ENARM
#  Uso: & ".\scripts\run_tests.ps1"
#  Opciones:
#    -Filter "[hemodynamic]"   — Solo tests con ese tag
#    -Verbose                  — Output detallado
#    -RebuildAll               — Limpia build antes de compilar
# ===============================================================
param(
    [string]$Filter    = "",
    [switch]$Verbose   = $false,
    [switch]$RebuildAll = $false
)

$root     = Split-Path $PSScriptRoot -Parent
$build    = "$root\build"
$testExe  = "$build\Release\ENARM.Tests.exe"

Write-Host ""
Write-Host "============================================================" -ForegroundColor Cyan
Write-Host "  ENARM Tests Runner" -ForegroundColor Cyan
Write-Host "============================================================" -ForegroundColor Cyan
Write-Host "  Build: $build" -ForegroundColor Gray
Write-Host "  Test:  $testExe" -ForegroundColor Gray
Write-Host ""

# Limpiar si se pide
if ($RebuildAll -and (Test-Path $build)) {
    Write-Host "[0] Limpiando build anterior..." -ForegroundColor Yellow
    Remove-Item $build -Recurse -Force
    Write-Host "    OK" -ForegroundColor Green
}

# Crear build dir si no existe
New-Item -ItemType Directory -Force -Path $build | Out-Null

# Configurar con CMake
Write-Host "[1] Configurando CMake..." -ForegroundColor Yellow
$configArgs = @(
    "-S", $root,
    "-B", $build,
    "-DCMAKE_BUILD_TYPE=Release",
    "-DCMAKE_TOOLCHAIN_FILE=$env:VCPKG_ROOT\scripts\buildsystems\vcpkg.cmake"
)

$configResult = & cmake @configArgs 2>&1
if ($LASTEXITCODE -ne 0) {
    Write-Host "    ERROR en configuración:" -ForegroundColor Red
    $configResult | Write-Host
    exit 1
}
Write-Host "    OK" -ForegroundColor Green

# Compilar solo ENARM.Tests
Write-Host "[2] Compilando ENARM.Tests..." -ForegroundColor Yellow
$buildResult = & cmake --build $build --config Release --target ENARM.Tests 2>&1
if ($LASTEXITCODE -ne 0) {
    Write-Host "    ERROR en compilación:" -ForegroundColor Red
    $buildResult | Write-Host
    exit 1
}
Write-Host "    OK" -ForegroundColor Green

# Verificar ejecutable
if (-not (Test-Path $testExe)) {
    # Buscar en otras ubicaciones
    $testExe = Get-ChildItem $build -Name "ENARM.Tests.exe" -Recurse | Select-Object -First 1
    if (-not $testExe) {
        Write-Host "    ERROR: ENARM.Tests.exe no encontrado" -ForegroundColor Red
        exit 1
    }
    $testExe = "$build\$testExe"
}

# Ejecutar tests con ctest
Write-Host "[3] Ejecutando tests..." -ForegroundColor Yellow
Write-Host ""

$ctestArgs = @(
    "--test-dir", $build,
    "-C", "Release",
    "--output-on-failure"
)

if ($Verbose) {
    $ctestArgs += "-V"
}

if ($Filter -ne "") {
    $ctestArgs += "-R"
    $ctestArgs += $Filter
}

& ctest @ctestArgs
$exitCode = $LASTEXITCODE

Write-Host ""
if ($exitCode -eq 0) {
    Write-Host "============================================================" -ForegroundColor Green
    Write-Host "  TODOS LOS TESTS PASARON" -ForegroundColor Green
    Write-Host "============================================================" -ForegroundColor Green
} else {
    Write-Host "============================================================" -ForegroundColor Red
    Write-Host "  ALGUNOS TESTS FALLARON (exit code: $exitCode)" -ForegroundColor Red
    Write-Host "  Revisa los errores arriba" -ForegroundColor Red
    Write-Host "============================================================" -ForegroundColor Red
}

Write-Host ""
Write-Host "  Comandos útiles:" -ForegroundColor Yellow
Write-Host '  .\scripts\run_tests.ps1 -Filter "[hemodynamic]"' -ForegroundColor White
Write-Host '  .\scripts\run_tests.ps1 -Filter "[pharmacology]"' -ForegroundColor White
Write-Host '  .\scripts\run_tests.ps1 -Filter "[assessment]"' -ForegroundColor White
Write-Host '  .\scripts\run_tests.ps1 -Verbose' -ForegroundColor White
Write-Host '  .\scripts\run_tests.ps1 -RebuildAll' -ForegroundColor White
Write-Host ""

exit $exitCode
