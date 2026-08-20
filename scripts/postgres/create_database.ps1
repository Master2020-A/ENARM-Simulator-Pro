<#
.SYNOPSIS
    Crea base de datos enarm_db con usuario dedicado y ejecuta migraciones.
#>

[CmdletBinding()]
param(
    [string]$DbName = "enarm_db",
    [string]$DbUser = "enarm_user",
    [string]$DbPassword = "enarm_dev_2026",
    [string]$SuperuserPassword = "",   # Vacio = pedira interactivamente
    [string]$PostgresVersion = "17"
)

$ErrorActionPreference = "Stop"

function Write-Section {
    param([string]$Title)
    Write-Host ""
    Write-Host "-------------------------------------------------------" -ForegroundColor Cyan
    Write-Host "  $Title" -ForegroundColor Cyan
    Write-Host "-------------------------------------------------------" -ForegroundColor Cyan
}

# ─── Detectar psql ───
$pgBin = $null
foreach ($p in @(
    "C:\Program Files\PostgreSQL\$PostgresVersion\bin",
    "C:\Program Files (x86)\PostgreSQL\$PostgresVersion\bin"
)) {
    if (Test-Path "$p\psql.exe") { $pgBin = $p; break }
}

if (-not $pgBin) {
    Write-Host "ERROR: psql no encontrado. Ejecuta setup_postgres.ps1 primero" -ForegroundColor Red
    exit 1
}

$psql = "$pgBin\psql.exe"

# ─── Password superuser ───
if (-not $SuperuserPassword) {
    Write-Host ""
    Write-Host "Necesito el password del usuario 'postgres' (superuser)"
    Write-Host "Este password se configuro durante instalacion de PostgreSQL"
    $sec = Read-Host "Password de postgres" -AsSecureString
    $SuperuserPassword = [System.Runtime.InteropServices.Marshal]::PtrToStringAuto(
        [System.Runtime.InteropServices.Marshal]::SecureStringToBSTR($sec))
}

$env:PGPASSWORD = $SuperuserPassword

# ─── Verificar conexion ───
Write-Section "Verificando conexion como postgres"

$testConn = & $psql -U postgres -d postgres -c "SELECT version();" 2>&1
if ($LASTEXITCODE -ne 0) {
    Write-Host "ERROR: No pude conectar como postgres" -ForegroundColor Red
    Write-Host $testConn -ForegroundColor Red
    exit 1
}
Write-Host "OK: Conexion establecida" -ForegroundColor Green

# ─── Crear usuario ENARM ───
Write-Section "Creando usuario '$DbUser'"

$createUserSql = @"
DO
`$do`$
BEGIN
    IF NOT EXISTS (SELECT FROM pg_catalog.pg_roles WHERE rolname = '$DbUser') THEN
        CREATE ROLE $DbUser LOGIN PASSWORD '$DbPassword' CREATEDB;
        RAISE NOTICE 'Usuario $DbUser creado';
    ELSE
        ALTER ROLE $DbUser WITH PASSWORD '$DbPassword';
        RAISE NOTICE 'Usuario $DbUser ya existia - password actualizado';
    END IF;
END
`$do`$;
"@

$createUserSql | & $psql -U postgres -d postgres
if ($LASTEXITCODE -ne 0) {
    Write-Host "ERROR creando usuario" -ForegroundColor Red
    exit 1
}

# ─── Crear base de datos ───
Write-Section "Creando base '$DbName'"

$dbExists = & $psql -U postgres -d postgres -tAc "SELECT 1 FROM pg_database WHERE datname='$DbName';"
if ($dbExists -eq "1") {
    Write-Host "Base '$DbName' ya existe" -ForegroundColor Yellow
    $recreate = Read-Host "Recrearla? (DESTRUYE DATOS) [s/N]"
    if ($recreate -eq "s" -or $recreate -eq "S") {
        & $psql -U postgres -d postgres -c "DROP DATABASE $DbName;"
        & $psql -U postgres -d postgres -c "CREATE DATABASE $DbName OWNER $DbUser ENCODING 'UTF8';"
        Write-Host "Base recreada" -ForegroundColor Green
    }
} else {
    & $psql -U postgres -d postgres -c "CREATE DATABASE $DbName OWNER $DbUser ENCODING 'UTF8';"
    if ($LASTEXITCODE -eq 0) {
        Write-Host "Base '$DbName' creada" -ForegroundColor Green
    }
}

# ─── Instalar extensiones (como superuser) ───
Write-Section "Instalando extensiones"

# uuid-ossp: para UUIDs nativos
& $psql -U postgres -d $DbName -c "CREATE EXTENSION IF NOT EXISTS ""uuid-ossp"";"
Write-Host "  uuid-ossp: OK" -ForegroundColor Green

# pgcrypto: para hashes (passwords)
& $psql -U postgres -d $DbName -c "CREATE EXTENSION IF NOT EXISTS pgcrypto;"
Write-Host "  pgcrypto: OK" -ForegroundColor Green

# pgvector: para embeddings (busqueda semantica)
$vectorResult = & $psql -U postgres -d $DbName -c "CREATE EXTENSION IF NOT EXISTS vector;" 2>&1
if ($LASTEXITCODE -eq 0) {
    Write-Host "  vector (pgvector): OK" -ForegroundColor Green
} else {
    Write-Host "  vector (pgvector): NO instalado - se usara sin busqueda semantica" -ForegroundColor Yellow
    Write-Host "    Instalar despues desde: https://github.com/pgvector/pgvector" -ForegroundColor Gray
}

# ─── Ejecutar migraciones SQL ───
Write-Section "Ejecutando migraciones"

$scriptDir = Split-Path $MyInvocation.MyCommand.Path
$schemaDir = Join-Path (Split-Path (Split-Path $scriptDir)) "engine\ENARM.Data\schema"

if (-not (Test-Path $schemaDir)) {
    Write-Host "ADVERTENCIA: No encontre carpeta schema en $schemaDir" -ForegroundColor Yellow
    Write-Host "Ejecuta este script desde ENARMSimulator\scripts\postgres\" -ForegroundColor Yellow
} else {
    $migrations = Get-ChildItem "$schemaDir\*.sql" | Sort-Object Name
    foreach ($mig in $migrations) {
        Write-Host "  Aplicando: $($mig.Name)" -ForegroundColor Yellow
        & $psql -U postgres -d $DbName -f $mig.FullName 2>&1 | Out-Null
        if ($LASTEXITCODE -eq 0) {
            Write-Host "    OK" -ForegroundColor Green
        } else {
            Write-Host "    ERROR" -ForegroundColor Red
        }
    }
}

# ─── Otorgar permisos al usuario ENARM ───
Write-Section "Otorgando permisos"

& $psql -U postgres -d $DbName -c "GRANT ALL PRIVILEGES ON ALL TABLES IN SCHEMA public TO $DbUser;"
& $psql -U postgres -d $DbName -c "GRANT ALL PRIVILEGES ON ALL SEQUENCES IN SCHEMA public TO $DbUser;"
& $psql -U postgres -d $DbName -c "GRANT ALL PRIVILEGES ON ALL FUNCTIONS IN SCHEMA public TO $DbUser;"

# ─── Verificacion final ───
Write-Section "Verificacion"

$env:PGPASSWORD = $DbPassword
$tables = & $psql -U $DbUser -d $DbName -tAc "SELECT COUNT(*) FROM information_schema.tables WHERE table_schema='public';"
Write-Host "  Tablas creadas: $tables" -ForegroundColor Green

$version = & $psql -U $DbUser -d $DbName -tAc "SELECT version();"
Write-Host "  PostgreSQL: $version" -ForegroundColor Green

Write-Host ""
Write-Host "===============================================================" -ForegroundColor Green
Write-Host "  Base de datos ENARM lista" -ForegroundColor Green
Write-Host "===============================================================" -ForegroundColor Green
Write-Host ""
Write-Host "Datos de conexion:" -ForegroundColor Cyan
Write-Host "  Host:     localhost"
Write-Host "  Port:     5432"
Write-Host "  Database: $DbName"
Write-Host "  User:     $DbUser"
Write-Host "  Password: $DbPassword"
Write-Host ""
Write-Host "Connection string:"
Write-Host "  postgresql://${DbUser}:${DbPassword}@localhost:5432/${DbName}" -ForegroundColor White
Write-Host ""

# Limpiar password del env
$env:PGPASSWORD = $null