<#
.SYNOPSIS
    Instala PostgreSQL 17 como servicio Windows para ENARM.
.DESCRIPTION
    - Descarga e instala PostgreSQL 17 via winget
    - Configura servicio Windows automatico
    - Crea usuario postgres con password conocido
.NOTES
    Requiere PowerShell como Administrador
#>

[CmdletBinding()]
param(
    [string]$PostgresVersion = "17",
    [string]$SuperuserPassword = "postgres_enarm_dev"
)

$ErrorActionPreference = "Stop"

function Write-Section {
    param([string]$Title)
    Write-Host ""
    Write-Host "===============================================================" -ForegroundColor Cyan
    Write-Host "  $Title" -ForegroundColor Cyan
    Write-Host "===============================================================" -ForegroundColor Cyan
}

# ─── Verificar admin ───
$isAdmin = ([Security.Principal.WindowsPrincipal] `
    [Security.Principal.WindowsIdentity]::GetCurrent()).IsInRole(
    [Security.Principal.WindowsBuiltInRole]::Administrator)

if (-not $isAdmin) {
    Write-Host "ERROR: Este script requiere PowerShell como Administrador" -ForegroundColor Red
    exit 1
}

Write-Section "ENARM - Instalando PostgreSQL $PostgresVersion"

# ─── Verificar si ya esta instalado ───
$existingService = Get-Service -Name "postgresql-x64-$PostgresVersion" -ErrorAction SilentlyContinue
if ($existingService) {
    Write-Host "PostgreSQL $PostgresVersion ya esta instalado como servicio" -ForegroundColor Green
    Write-Host "  Estado: $($existingService.Status)" -ForegroundColor White

    if ($existingService.Status -ne "Running") {
        Write-Host "  Iniciando servicio..." -ForegroundColor Yellow
        Start-Service -Name "postgresql-x64-$PostgresVersion"
    }
} else {
    Write-Section "Instalando via winget"

    # Instalar
    Write-Host "Descargando PostgreSQL (~500 MB)..." -ForegroundColor Yellow
    winget install --id PostgreSQL.PostgreSQL.$PostgresVersion `
        --accept-package-agreements --accept-source-agreements `
        --silent --disable-interactivity

    Write-Host "Esperando servicio Windows..." -ForegroundColor Yellow
    Start-Sleep -Seconds 10

    # Verificar
    $svc = Get-Service -Name "postgresql-x64-$PostgresVersion" -ErrorAction SilentlyContinue
    if (-not $svc) {
        Write-Host "ERROR: Servicio PostgreSQL no encontrado tras instalacion" -ForegroundColor Red
        Write-Host "Instala manualmente desde: https://www.postgresql.org/download/windows/" -ForegroundColor Yellow
        exit 1
    }
}

# ─── Detectar ruta de instalacion ───
Write-Section "Detectando ruta de instalacion"

$possiblePaths = @(
    "C:\Program Files\PostgreSQL\$PostgresVersion\bin",
    "C:\Program Files (x86)\PostgreSQL\$PostgresVersion\bin"
)

$pgBin = $null
foreach ($p in $possiblePaths) {
    if (Test-Path "$p\psql.exe") {
        $pgBin = $p
        break
    }
}

if (-not $pgBin) {
    Write-Host "ERROR: No se encuentra psql.exe" -ForegroundColor Red
    exit 1
}

Write-Host "  psql en: $pgBin" -ForegroundColor Green

# ─── Agregar al PATH ───
$currentPath = [Environment]::GetEnvironmentVariable("Path", "Machine")
if ($currentPath -notlike "*$pgBin*") {
    Write-Host "Agregando PostgreSQL al PATH del sistema..." -ForegroundColor Yellow
    [Environment]::SetEnvironmentVariable("Path", "$currentPath;$pgBin", "Machine")
    $env:Path += ";$pgBin"
}

# ─── Verificar version ───
Write-Section "Verificando"

$version = & "$pgBin\psql.exe" --version 2>&1
Write-Host "  $version" -ForegroundColor Green

$service = Get-Service -Name "postgresql-x64-$PostgresVersion"
Write-Host "  Servicio: $($service.Status)" -ForegroundColor Green
Write-Host "  Nombre: $($service.Name)" -ForegroundColor Green

Write-Host ""
Write-Host "===============================================================" -ForegroundColor Green
Write-Host "  PostgreSQL instalado correctamente" -ForegroundColor Green
Write-Host "===============================================================" -ForegroundColor Green
Write-Host ""
Write-Host "IMPORTANTE:" -ForegroundColor Yellow
Write-Host "  Si winget te pidio password durante instalacion, usalo."
Write-Host "  Si no, el password default suele ser: postgres"
Write-Host ""
Write-Host "Siguiente:" -ForegroundColor Cyan
Write-Host "  .\create_database.ps1"
Write-Host ""