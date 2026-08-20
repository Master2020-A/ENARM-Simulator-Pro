# Write-SqlFile.ps1
# Escribe archivos SQL SIN BOM (compatible con psql)
#
# Uso:
#   . .\scripts\Write-SqlFile.ps1
#   Write-SqlFile -Path "path\to\file.sql" -Content @"
#   BEGIN;
#   ...
#   COMMIT;
#   "@

function Write-SqlFile {
    param(
        [Parameter(Mandatory=$true)][string]$Path,
        [Parameter(Mandatory=$true)][string]$Content
    )

    # Asegurar directorio existe
    $dir = Split-Path $Path -Parent
    if ($dir -and -not (Test-Path $dir)) {
        New-Item -ItemType Directory -Force -Path $dir | Out-Null
    }

    # UTF-8 SIN BOM
    [System.IO.File]::WriteAllText(
        $Path,
        $Content,
        [System.Text.UTF8Encoding]::new($false))

    # Verificar
    $bytes = [System.IO.File]::ReadAllBytes($Path)
    if ($bytes.Length -ge 3 -and $bytes[0] -eq 0xEF -and $bytes[1] -eq 0xBB -and $bytes[2] -eq 0xBF) {
        Write-Host "  WARN: $Path aun tiene BOM" -ForegroundColor Yellow
    } else {
        Write-Host "  OK: $Path (sin BOM, $([math]::Round($bytes.Length/1024, 1)) KB)" -ForegroundColor Green
    }
}

function Invoke-SqlFile {
    param(
        [Parameter(Mandatory=$true)][string]$Path,
        [string]$Database = "enarm_db",
        [string]$User = "postgres",
        [string]$Password = "manager"
    )

    $env:PGPASSWORD = $Password
    Write-Host "  Ejecutando: $Path" -ForegroundColor Cyan
    & psql -U $User -d $Database -f $Path
}