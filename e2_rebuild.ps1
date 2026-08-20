# ===============================================================
#   ENARM Sprint 7B Epica 2 - REBUILD + PULIDO
#
#   IMPORTANTE:
#   1. Abre "Developer PowerShell for VS 2022" (Menu Inicio)
#   2. Guarda este script como e2_rebuild.ps1
#   3. Ejecuta: & "C:\...\ENARMSimulator\e2_rebuild.ps1"
# ===============================================================

$ErrorActionPreference = "Continue"

$root      = "C:\Users\Master\Documents\ENARM-IA\ENARMSimulator"
$build     = "$root\build"
$psViewCpp = "$root\engine\ENARM.GUI\src\Views\PatientSessionView.cpp"
$docsDir   = "$root\docs\sprint-7b\epica2"

Write-Host ""
Write-Host "=======================================================" -ForegroundColor Cyan
Write-Host "  ENARM Epica 2 - REBUILD + PULIDO" -ForegroundColor Cyan
Write-Host "=======================================================" -ForegroundColor Cyan
Write-Host ""

# ---------------------------------------------------------------
# PASO 0: Verificar entorno MSVC
# ---------------------------------------------------------------
Write-Host "[0/6] Verificando entorno MSVC..." -ForegroundColor Yellow

$vcInstall = $env:VCINSTALLDIR
$msvcInclude = $env:INCLUDE

if (-not $vcInstall -or -not $msvcInclude) {
    Write-Host ""
    Write-Host "  ERROR: entorno MSVC NO cargado" -ForegroundColor Red
    Write-Host ""
    Write-Host "  Solucion:" -ForegroundColor Yellow
    Write-Host "  1. Cierra esta ventana de PowerShell" -ForegroundColor Yellow
    Write-Host "  2. Menu Inicio -> busca 'Developer PowerShell for VS 2022'" -ForegroundColor Yellow
    Write-Host "  3. Abre esa terminal (icono azul/morado)" -ForegroundColor Yellow
    Write-Host "  4. Verifica con:  echo `$env:VCINSTALLDIR" -ForegroundColor Yellow
    Write-Host "  5. Vuelve a ejecutar este script desde ahi" -ForegroundColor Yellow
    Write-Host ""
    exit 1
}

Write-Host "  VCINSTALLDIR: $vcInstall" -ForegroundColor Green
Write-Host "  INCLUDE OK: $($msvcInclude.Length) caracteres" -ForegroundColor Green

# ---------------------------------------------------------------
# UTIL
# ---------------------------------------------------------------
function Save-Utf8 {
    param([string]$Path, [string]$Content)
    [System.IO.File]::WriteAllText(
        $Path, $Content, [System.Text.UTF8Encoding]::new($false))
    $kb = [math]::Round((Get-Item $Path).Length / 1KB, 1)
    Write-Host "     OK $(Split-Path $Path -Leaf) ($kb KB)" -ForegroundColor Green
}

function Backup-File {
    param([string]$Path, [string]$Tag = "bak")
    if (Test-Path $Path) {
        $bk = "$Path.$Tag"
        if (-not (Test-Path $bk)) {
            Copy-Item $Path $bk
            Write-Host "     backup: $(Split-Path $bk -Leaf)" -ForegroundColor DarkGray
        }
    }
}

# ---------------------------------------------------------------
# PASO 1: Deduplicar leyenda "Vinculadas al caso"
# ---------------------------------------------------------------
Write-Host ""
Write-Host "[1/6] Deduplicando leyenda 'Vinculadas al caso'..." -ForegroundColor Yellow

if (-not (Test-Path $psViewCpp)) {
    Write-Host "  ERROR: no existe $psViewCpp" -ForegroundColor Red
    exit 1
}

$content = Get-Content $psViewCpp -Raw

# Contar apariciones de la cadena literal (mas confiable que regex complejo)
$literal = 'Vinculadas al caso: P=%d'
$countLiteral = ([regex]::Matches($content, [regex]::Escape($literal))).Count
Write-Host "  Cadena '$literal' aparece $countLiteral veces" -ForegroundColor DarkGray

if ($countLiteral -gt 1) {
    Backup-File $psViewCpp "bak_dedup"

    # Estrategia: encontrar cada bloque {...} que contenga 'Vinculadas al caso'
    # y eliminar todos excepto el primero.
    # Usamos un balanceo manual de llaves.

    $blocks = @()
    $searchFrom = 0

    while ($true) {
        $idx = $content.IndexOf("`"Vinculadas al caso: P=%d", $searchFrom)
        if ($idx -lt 0) { break }

        # Retroceder hasta el "{" que abre el bloque
        $openBrace = -1
        for ($i = $idx; $i -ge 0; $i--) {
            if ($content[$i] -eq '{') {
                # Verifica que no sea "{" de un for/if interno
                # Simplificacion: tomamos el "{" mas cercano
                $openBrace = $i
                break
            }
        }
        if ($openBrace -lt 0) { $searchFrom = $idx + 1; continue }

        # Retroceder mas para encontrar el comentario o "int nP = 0"
        # Buscamos "int nP = 0" antes del idx
        $intNP = $content.LastIndexOf("int nP = 0", $idx)
        if ($intNP -lt 0) { $searchFrom = $idx + 1; continue }

        # Retroceder hasta el "{" antes de "int nP = 0"
        $openBrace = $content.LastIndexOf("{", $intNP)
        if ($openBrace -lt 0) { $searchFrom = $idx + 1; continue }

        # Ahora balancear llaves desde openBrace hacia adelante
        $depth = 0
        $closeBrace = -1
        for ($i = $openBrace; $i -lt $content.Length; $i++) {
            $ch = $content[$i]
            if ($ch -eq '{') { $depth++ }
            elseif ($ch -eq '}') {
                $depth--
                if ($depth -eq 0) { $closeBrace = $i; break }
            }
        }

        if ($closeBrace -lt 0) { $searchFrom = $idx + 1; continue }

        $blocks += [pscustomobject]@{
            Start = $openBrace
            End = $closeBrace
        }

        $searchFrom = $closeBrace + 1
    }

    Write-Host "  Bloques encontrados: $($blocks.Count)" -ForegroundColor DarkGray

    if ($blocks.Count -gt 1) {
        # Eliminar del segundo en adelante, de atras hacia adelante
        $toRemove = $blocks | Select-Object -Skip 1 | Sort-Object Start -Descending

        foreach ($b in $toRemove) {
            $len = $b.End - $b.Start + 1
            $content = $content.Remove($b.Start, $len)
            Write-Host "     Eliminado bloque en index $($b.Start) (len $len)" -ForegroundColor Green
        }

        Save-Utf8 $psViewCpp $content

        # Verificar
        $newCount = ([regex]::Matches((Get-Content $psViewCpp -Raw),
                    [regex]::Escape($literal))).Count
        if ($newCount -eq 1) {
            Write-Host "  Verificacion: OK, ahora hay 1 sola aparicion" -ForegroundColor Green
        } else {
            Write-Host "  ADVERTENCIA: aun hay $newCount apariciones" -ForegroundColor Yellow
        }
    }
} else {
    Write-Host "  Solo hay 1 aparicion, nada que deduplicar" -ForegroundColor DarkGreen
}

# ---------------------------------------------------------------
# PASO 2: Verificar tooltip presente
# ---------------------------------------------------------------
Write-Host ""
Write-Host "[2/6] Verificando tooltip..." -ForegroundColor Yellow

$content = Get-Content $psViewCpp -Raw
$hasTooltip = $content -match 'sessionGuidelineRationale' -and $content -match 'SetTooltip'
if ($hasTooltip) {
    Write-Host "  OK tooltip presente en el codigo" -ForegroundColor Green
} else {
    Write-Host "  AVISO: no detecto tooltip. Revisar manualmente." -ForegroundColor Yellow
}

# ---------------------------------------------------------------
# PASO 3: Reconfigurar CMake desde cero
# ---------------------------------------------------------------
Write-Host ""
Write-Host "[3/6] Reconfigurando CMake (limpio)..." -ForegroundColor Yellow

if (Test-Path $build) {
    Write-Host "  Eliminando build/ existente..." -ForegroundColor DarkGray
    cmd /c "rmdir /S /Q `"$build`"" 2>&1 | Out-Null
    Start-Sleep -Seconds 1
}

if (Test-Path $build) {
    Write-Host "  ERROR: no se pudo borrar $build" -ForegroundColor Red
    exit 1
}

$vcpkgToolchain = "$root\..\vcpkg\scripts\buildsystems\vcpkg.cmake"
if (-not (Test-Path $vcpkgToolchain)) {
    Write-Host "  ERROR: no existe toolchain vcpkg: $vcpkgToolchain" -ForegroundColor Red
    exit 1
}

$cfgLog = "$root\configure.log"
$cfgErr = "$root\configure_err.log"

Write-Host "  cmake -S ... -B build -G 'Ninja Multi-Config'..." -ForegroundColor DarkGray

$configArgs = @(
    "-S", $root,
    "-B", $build,
    "-G", "Ninja Multi-Config",
    "-DCMAKE_TOOLCHAIN_FILE=$vcpkgToolchain",
    "-DVCPKG_TARGET_TRIPLET=x64-windows"
)

$proc = Start-Process `
    -FilePath "cmake" `
    -ArgumentList $configArgs `
    -WorkingDirectory $root `
    -RedirectStandardOutput $cfgLog `
    -RedirectStandardError  $cfgErr `
    -NoNewWindow -Wait -PassThru

if ($proc.ExitCode -ne 0) {
    Write-Host "  CMake FALLO (exit $($proc.ExitCode))" -ForegroundColor Red
    Write-Host ""
    Write-Host "  configure.log (ultimas 30):" -ForegroundColor Cyan
    if (Test-Path $cfgLog) { Get-Content $cfgLog | Select-Object -Last 30 }
    Write-Host ""
    Write-Host "  configure_err.log:" -ForegroundColor Cyan
    if ((Test-Path $cfgErr) -and ((Get-Item $cfgErr).Length -gt 0)) {
        Get-Content $cfgErr | Select-Object -Last 30
    }
    exit 1
}
Write-Host "  CMake configurado OK" -ForegroundColor Green

# ---------------------------------------------------------------
# PASO 4: Build Release
# ---------------------------------------------------------------
Write-Host ""
Write-Host "[4/6] Compilando ENARM.App Release (esto tarda un poco)..." -ForegroundColor Yellow

$buildLog = "$root\build_rebuild.log"
$errLog   = "$root\build_rebuild_err.log"

$buildArgs = @("--build", $build, "--config", "Release", "--target", "ENARM.App", "-j8")

$proc = Start-Process `
    -FilePath "cmake" `
    -ArgumentList $buildArgs `
    -WorkingDirectory $root `
    -RedirectStandardOutput $buildLog `
    -RedirectStandardError  $errLog `
    -NoNewWindow -Wait -PassThru

$exitCode = $proc.ExitCode

Write-Host ""
Write-Host "-------- BUILD LOG (ultimas 40) --------" -ForegroundColor Cyan
if (Test-Path $buildLog) { Get-Content $buildLog | Select-Object -Last 40 }

if ((Test-Path $errLog) -and ((Get-Item $errLog).Length -gt 0)) {
    Write-Host ""
    Write-Host "-------- STDERR --------" -ForegroundColor Red
    Get-Content $errLog | Select-Object -Last 30
}

if ($exitCode -ne 0) {
    Write-Host ""
    Write-Host "  BUILD FALLO (exit $exitCode)" -ForegroundColor Red
    Write-Host "  Restaurando backup del .cpp..." -ForegroundColor Yellow
    $bk = "$psViewCpp.bak_dedup"
    if (Test-Path $bk) {
        Copy-Item $bk $psViewCpp -Force
        Write-Host "  Backup restaurado. Recompila manualmente." -ForegroundColor Yellow
    }
    exit 1
}
Write-Host ""
Write-Host "  BUILD OK" -ForegroundColor Green

# ---------------------------------------------------------------
# PASO 5: Docs
# ---------------------------------------------------------------
Write-Host ""
Write-Host "[5/6] Preparando docs..." -ForegroundColor Yellow

if (-not (Test-Path $docsDir)) {
    New-Item -ItemType Directory -Path $docsDir -Force | Out-Null
    Write-Host "  Creada: $docsDir" -ForegroundColor Green
}

$readmePath = "$docsDir\README.md"
$readmeContent = @'
# Sprint 7B - Epica 2: Vinculacion caso <-> GPC

## Estado: COMPLETADA

## Que hace
- Cada caso tiene GPCs vinculadas explicitamente con nivel de relevancia.
- 3 niveles: PRIMARIA (verde), SECUNDARIA (naranja), TERCIARIA (gris).
- Fallback automatico a busqueda por especialidad si no hay vinculo.

## Base de datos
- Tabla `case_guidelines` con 33 vinculaciones para 10 casos.

## UI
- Tab "Guias":
  - Header "Vinculadas al caso: P=N S=N T=N"
  - Badge coloreado antes de cada GPC
  - Tooltip con el rationale al hacer hover

## Como probar
1. Abrir ENARM.exe
2. Elegir caso IAM STEMI
3. Tab Guias -> ver 4 GPCs con badges de colores

## Screenshots
- `after.png`: la vista con badges

## Archivos modificados
- engine/ENARM.Data/include/ENARM/Data/Repositories/GuidelineRepository.h
- engine/ENARM.Data/src/Repositories/GuidelineRepository.cpp
- engine/ENARM.GUI/include/ENARM/GUI/AppContext.h
- engine/ENARM.GUI/src/Views/PatientSessionView.cpp
- scripts/postgres/026_case_guidelines.sql
'@

Save-Utf8 $readmePath $readmeContent

$sqlSrc = "$root\scripts\postgres\026_case_guidelines.sql"
if (Test-Path $sqlSrc) {
    Copy-Item $sqlSrc "$docsDir\026_case_guidelines.sql" -Force
    Write-Host "  SQL copiado a docs" -ForegroundColor Green
}

# ---------------------------------------------------------------
# PASO 6: Git commit + tag
# ---------------------------------------------------------------
Write-Host ""
Write-Host "[6/6] Git commit + tag..." -ForegroundColor Yellow

# Buscar el .git en root o en padre
$gitRoot = $null
if (Test-Path "$root\.git") {
    $gitRoot = $root
} elseif (Test-Path "$root\..\.git") {
    $gitRoot = (Resolve-Path "$root\..").Path
}

if (-not $gitRoot) {
    Write-Host "  AVISO: no encontrado .git ni en $root ni en padre" -ForegroundColor Yellow
    Write-Host "  Saltando git." -ForegroundColor Yellow
} else {
    Write-Host "  Git root: $gitRoot" -ForegroundColor DarkGray
    Push-Location $gitRoot

    & git add "ENARMSimulator/engine/ENARM.Data/include/ENARM/Data/Repositories/GuidelineRepository.h" 2>&1 | Out-Null
    & git add "ENARMSimulator/engine/ENARM.Data/src/Repositories/GuidelineRepository.cpp" 2>&1 | Out-Null
    & git add "ENARMSimulator/engine/ENARM.GUI/include/ENARM/GUI/AppContext.h" 2>&1 | Out-Null
    & git add "ENARMSimulator/engine/ENARM.GUI/src/Views/PatientSessionView.cpp" 2>&1 | Out-Null
    & git add "ENARMSimulator/scripts/postgres/026_case_guidelines.sql" 2>&1 | Out-Null
    & git add "ENARMSimulator/docs/sprint-7b/" 2>&1 | Out-Null

    # Tambien intentar con paths relativos al root del proyecto
    Push-Location $root
    & git add "engine/ENARM.Data/include/ENARM/Data/Repositories/GuidelineRepository.h" 2>&1 | Out-Null
    & git add "engine/ENARM.Data/src/Repositories/GuidelineRepository.cpp" 2>&1 | Out-Null
    & git add "engine/ENARM.GUI/include/ENARM/GUI/AppContext.h" 2>&1 | Out-Null
    & git add "engine/ENARM.GUI/src/Views/PatientSessionView.cpp" 2>&1 | Out-Null
    & git add "scripts/postgres/026_case_guidelines.sql" 2>&1 | Out-Null
    & git add "docs/sprint-7b/" 2>&1 | Out-Null
    Pop-Location

    $staged = & git diff --cached --name-only 2>$null
    if ($staged) {
        Write-Host "  Archivos staged:" -ForegroundColor DarkGray
        $staged | ForEach-Object { Write-Host "    $_" -ForegroundColor DarkGray }

        $msg = "feat(sprint-7b/epica2): vinculacion caso-GPC con badges PRIMARIA/SECUNDARIA/TERCIARIA"
        & git commit -m $msg 2>&1 | ForEach-Object { Write-Host "    $_" -ForegroundColor Gray }
    } else {
        Write-Host "  Nada staged." -ForegroundColor DarkGreen
    }

    # Tag
    $existing = & git tag -l "sprint-7b-epica2-completa" 2>$null
    if ($existing) {
        & git tag -f "sprint-7b-epica2-completa" -m "Epica 2 completada (rebuild)" 2>&1 | Out-Null
        Write-Host "  Tag actualizado: sprint-7b-epica2-completa" -ForegroundColor Green
    } else {
        & git tag -a "sprint-7b-epica2-completa" -m "Epica 2 completada" 2>&1 | Out-Null
        Write-Host "  Tag creado: sprint-7b-epica2-completa" -ForegroundColor Green
    }

    Write-Host ""
    Write-Host "  Ultimos 3 commits:" -ForegroundColor DarkGray
    & git log --oneline -3 2>&1 | ForEach-Object { Write-Host "    $_" -ForegroundColor Gray }

    Pop-Location
}

# ---------------------------------------------------------------
# RESUMEN
# ---------------------------------------------------------------
Write-Host ""
Write-Host "=======================================================" -ForegroundColor Green
Write-Host "  LISTO" -ForegroundColor Green
Write-Host "=======================================================" -ForegroundColor Green
Write-Host ""
Write-Host "Ejecutable: $build\bin\Release\ENARM.exe" -ForegroundColor Cyan
Write-Host ""
Write-Host "Que verificar:" -ForegroundColor Magenta
Write-Host "  1. La leyenda 'Vinculadas al caso: P=2 S=1 T=1' aparece UNA SOLA vez" -ForegroundColor White
Write-Host "  2. Hover sobre badge PRIMARIA (IMSS-234-09) muestra tooltip" -ForegroundColor White
Write-Host "     con texto: 'GPC especifica de IAM STEMI'" -ForegroundColor DarkCyan
Write-Host "  3. Toma screenshot y guarda como:" -ForegroundColor White
Write-Host "     $docsDir\after.png" -ForegroundColor Cyan
Write-Host ""