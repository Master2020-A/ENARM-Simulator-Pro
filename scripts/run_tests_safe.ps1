# ===============================================================
#  run_tests_safe.ps1 â€” Ejecuta tests protegiendo los .cpp
#  Uso: & ".\scripts\run_tests_safe.ps1"
# ===============================================================
param(
    [string]$Filter    = "",
    [switch]$Verbose   = $false,
    [switch]$RebuildAll = $false
)

$root      = Split-Path $PSScriptRoot -Parent
$build     = "$root\build"
$testsDir  = "$root\engine\ENARM.Tests\src"
$snapDir   = "$root\engine\ENARM.Tests\.snapshot"

Write-Host "`n============================================================" -ForegroundColor Cyan
Write-Host "  ENARM Tests Runner (SAFE MODE)" -ForegroundColor Cyan
Write-Host "============================================================" -ForegroundColor Cyan

# â”€â”€ PASO 0: Snapshot de tests â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€
Write-Host "`n[0] Snapshot de tests..." -ForegroundColor Yellow
New-Item -ItemType Directory -Force -Path $snapDir | Out-Null

$currentTests = Get-ChildItem $testsDir -Filter "*.cpp"
foreach ($cpp in $currentTests) {
    Copy-Item $cpp.FullName "$snapDir\$($cpp.Name)" -Force
}
Write-Host "    Snapshot: $($currentTests.Count) archivos" -ForegroundColor Green

# â”€â”€ PASO 1: CMake Configure â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€
Write-Host "`n[1] Configurando CMake..." -ForegroundColor Yellow

if ($RebuildAll -and (Test-Path $build)) {
    Remove-Item $build -Recurse -Force
}
New-Item -ItemType Directory -Force -Path $build | Out-Null

& cmake -S $root -B $build -DCMAKE_TOOLCHAIN_FILE="$env:VCPKG_ROOT\scripts\buildsystems\vcpkg.cmake" 2>&1 | Out-Null
if ($LASTEXITCODE -ne 0) {
    Write-Host "    ERROR" -ForegroundColor Red
    exit 1
}
Write-Host "    OK" -ForegroundColor Green

# â”€â”€ PASO 2: Verificar integridad â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€
Write-Host "`n[2] Verificando integridad..." -ForegroundColor Yellow
$modified = @()
foreach ($cpp in Get-ChildItem $testsDir -Filter "*.cpp") {
    $snap = "$snapDir\$($cpp.Name)"
    if (Test-Path $snap) {
        $h1 = (Get-FileHash $cpp.FullName -Algorithm MD5).Hash
        $h2 = (Get-FileHash $snap -Algorithm MD5).Hash
        if ($h1 -ne $h2) {
            $modified += $cpp.Name
            Copy-Item $snap $cpp.FullName -Force
        }
    }
}

if ($modified.Count -gt 0) {
    Write-Host "    RESTAURADOS $($modified.Count) archivos:" -ForegroundColor Red
    $modified | ForEach-Object { Write-Host "      - $_" -ForegroundColor Red }
} else {
    Write-Host "    OK - Sin modificaciones" -ForegroundColor Green
}

# â”€â”€ PASO 3: Compilar â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€
Write-Host "`n[3] Compilando ENARM.Tests..." -ForegroundColor Yellow
& cmake --build $build --config Release --target ENARM.Tests --parallel 4 2>&1 |
    Where-Object { $_ -match "error C|fatal error|error LNK|->.*\.exe" } |
    ForEach-Object { Write-Host "    $_" }

if ($LASTEXITCODE -ne 0) {
    Write-Host "    ERROR" -ForegroundColor Red
    exit 1
}
Write-Host "    OK" -ForegroundColor Green

# â”€â”€ PASO 4: Ejecutar â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€
Write-Host "`n[4] Ejecutando tests..." -ForegroundColor Yellow

$ctestArgs = @("--test-dir", $build, "-C", "Release", "--output-on-failure")
if ($Verbose) { $ctestArgs += "-V" }
if ($Filter)  { $ctestArgs += "-R"; $ctestArgs += $Filter }

& ctest @ctestArgs
$exitCode = $LASTEXITCODE

Write-Host "`n============================================================" -ForegroundColor $(if($exitCode -eq 0){"Green"}else{"Red"})
if ($exitCode -eq 0) {
    Write-Host "  TODOS LOS TESTS PASARON" -ForegroundColor Green
} else {
    Write-Host "  FALLARON tests (exit $exitCode)" -ForegroundColor Red
}
Write-Host "============================================================" -ForegroundColor Cyan

exit $exitCode