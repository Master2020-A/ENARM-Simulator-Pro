<#
.SYNOPSIS
    Aplica migraciones SQL a enarm_db.
    v3: no aborta en NOTICE de PostgreSQL.
#>

[CmdletBinding()]
param(
    [string]$DbName = "enarm_db",
    [string]$DbUser = "postgres",
    [string]$DbPassword = "manager"
)

# NO $ErrorActionPreference = "Stop" - deja que psql maneje sus NOTICEs

# Detectar psql
$pgBin = $env:PG_BIN
if (-not $pgBin -or -not (Test-Path "$pgBin\psql.exe")) {
    foreach ($v in @("18","17","16","15","14","13")) {
        foreach ($b in @("C:\Program Files\PostgreSQL","C:\Program Files (x86)\PostgreSQL")) {
            if (Test-Path "$b\$v\bin\psql.exe") { $pgBin = "$b\$v\bin"; break }
        }
        if ($pgBin) { break }
    }
}
if (-not $pgBin) { Write-Host "ERROR: psql no encontrado" -ForegroundColor Red; exit 1 }

$psql = "$pgBin\psql.exe"
$env:PGPASSWORD = $DbPassword

Write-Host ""
Write-Host "===============================================================" -ForegroundColor Cyan
Write-Host "  Migraciones ENARM en $DbName" -ForegroundColor Cyan
Write-Host "  psql: $pgBin" -ForegroundColor Cyan
Write-Host "===============================================================" -ForegroundColor Cyan
Write-Host ""

# Verificar conexion
Write-Host "Conexion..." -NoNewline
$test = & $psql -U $DbUser -d $DbName -tAc "SELECT 1;" 2>&1
if ($LASTEXITCODE -ne 0) {
    Write-Host " ERROR" -ForegroundColor Red
    Write-Host $test
    exit 1
}
Write-Host " OK" -ForegroundColor Green

# Instalar extensiones (con archivo temporal, redirigiendo stderr)
Write-Host ""
Write-Host "Extensiones..." -NoNewline

$extSql = @"
CREATE EXTENSION IF NOT EXISTS `"uuid-ossp`";
CREATE EXTENSION IF NOT EXISTS pgcrypto;
CREATE EXTENSION IF NOT EXISTS pg_trgm;
CREATE EXTENSION IF NOT EXISTS vector;
"@

$tmpExt = [System.IO.Path]::GetTempFileName() + ".sql"
[System.IO.File]::WriteAllText($tmpExt, $extSql, [System.Text.UTF8Encoding]::new($false))
# 2>$null suprime stderr (donde van los NOTICE)
$null = & $psql -U $DbUser -d $DbName -f $tmpExt 2>$null
Remove-Item $tmpExt -EA SilentlyContinue
Write-Host " OK" -ForegroundColor Green

# Listar extensiones instaladas
Write-Host ""
Write-Host "Extensiones activas:" -ForegroundColor White
$exts = & $psql -U $DbUser -d $DbName -tAc "SELECT extname FROM pg_extension ORDER BY 1;" 2>$null
$exts -split "`n" | ForEach-Object {
    $line = $_.Trim()
    if ($line) { Write-Host "  * $line" -ForegroundColor Cyan }
}

# Aplicar migraciones
Write-Host ""
Write-Host "Aplicando SQL:" -ForegroundColor Yellow

$scriptDir = Split-Path $MyInvocation.MyCommand.Path
$schemaDir = Join-Path (Split-Path (Split-Path $scriptDir)) "engine\ENARM.Data\schema"

if (-not (Test-Path $schemaDir)) {
    Write-Host "ERROR: No encontre $schemaDir" -ForegroundColor Red
    exit 1
}

$migrations = Get-ChildItem "$schemaDir\*.sql" | Sort-Object Name
$totalOk = 0
$totalErr = 0
foreach ($mig in $migrations) {
    Write-Host "  $($mig.Name)..." -NoNewline

    # Ejecutar con stderr redirigido - solo se detiene en errores reales
    $stdout = & $psql -U $DbUser -d $DbName -v ON_ERROR_STOP=1 -f $mig.FullName 2>&1
    $exitCode = $LASTEXITCODE

    if ($exitCode -eq 0) {
        Write-Host " OK" -ForegroundColor Green
        $totalOk++
    } else {
        Write-Host " ERROR (exit=$exitCode)" -ForegroundColor Red
        # Filtrar solo lineas de ERROR (no NOTICE ni HINT)
        $stdout | Where-Object { $_ -match "^ERROR|^psql.*ERROR" } | ForEach-Object {
            Write-Host "    $_" -ForegroundColor Red
        }
        $totalErr++
    }
}

# Verificacion final
Write-Host ""
Write-Host "===============================================================" -ForegroundColor Green
Write-Host "  Verificacion" -ForegroundColor Green
Write-Host "===============================================================" -ForegroundColor Green

$tables = & $psql -U $DbUser -d $DbName -tAc "SELECT COUNT(*) FROM information_schema.tables WHERE table_schema='public';" 2>$null
Write-Host "  Tablas creadas: $($tables.Trim())" -ForegroundColor White
Write-Host "  Migraciones OK: $totalOk / $($migrations.Count)" -ForegroundColor White
if ($totalErr -gt 0) { Write-Host "  Errores: $totalErr" -ForegroundColor Yellow }

Write-Host ""
Write-Host "Lista de tablas:" -ForegroundColor White
& $psql -U $DbUser -d $DbName -c "\dt" 2>$null

$env:PGPASSWORD = $null

Write-Host ""
Write-Host "Connection: postgresql://${DbUser}:${DbPassword}@localhost:5432/${DbName}" -ForegroundColor Cyan
Write-Host ""