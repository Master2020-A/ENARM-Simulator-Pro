<#
.SYNOPSIS
    Setup completo PostgreSQL + base ENARM (wrapper).
#>

$ErrorActionPreference = "Stop"
$here = Split-Path $MyInvocation.MyCommand.Path

Write-Host ""
Write-Host "===============================================================" -ForegroundColor Cyan
Write-Host "  ENARM - Setup completo PostgreSQL" -ForegroundColor Cyan
Write-Host "===============================================================" -ForegroundColor Cyan
Write-Host ""

Write-Host "Paso 1: Instalar PostgreSQL 17 (requiere ADMIN)" -ForegroundColor Yellow
& "$here\setup_postgres.ps1"

Write-Host ""
Write-Host "Paso 2: Crear base ENARM y ejecutar migraciones" -ForegroundColor Yellow
& "$here\create_database.ps1"

Write-Host ""
Write-Host "===============================================================" -ForegroundColor Green
Write-Host "  Setup completo" -ForegroundColor Green
Write-Host "===============================================================" -ForegroundColor Green