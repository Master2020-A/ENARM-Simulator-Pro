# ═══════════════════════════════════════════════════════════════
#   ENARM SIMULATOR - Epica 2 Parte 2c: Fix completo
#   INSTRUCCION: guarda este archivo como:
#   C:\Users\Master\Documents\ENARM-IA\ENARMSimulator\e2p2c.ps1
#   y ejecuta:  & "C:\Users\Master\Documents\ENARM-IA\ENARMSimulator\e2p2c.ps1"
# ═══════════════════════════════════════════════════════════════

$root    = "C:\Users\Master\Documents\ENARM-IA\ENARMSimulator"
$build   = "$root\build"

Write-Host ""
Write-Host "=======================================================" -ForegroundColor Cyan
Write-Host "  ENARM - Epica 2 Parte 2c" -ForegroundColor Cyan
Write-Host "=======================================================" -ForegroundColor Cyan
Write-Host ""

# ---------------------------------------------------------------
# UTIL: escritura segura UTF-8 sin BOM
# ---------------------------------------------------------------
function Write-Utf8 {
    param([string]$Path, [string]$Content)
    $dir = Split-Path $Path -Parent
    if (-not (Test-Path $dir)) {
        New-Item -ItemType Directory -Path $dir -Force | Out-Null
    }
    [System.IO.File]::WriteAllText(
        $Path, $Content,
        [System.Text.UTF8Encoding]::new($false))
    $kb = [math]::Round((Get-Item $Path).Length / 1KB, 1)
    Write-Host "   OK  $(Split-Path $Path -Leaf)  ($kb KB)" -ForegroundColor Green
}

# ---------------------------------------------------------------
# PASO 0: Localizar CMakeLists real de ENARM.GUI
# ---------------------------------------------------------------
Write-Host "[0/6] Localizando CMakeLists.txt de ENARM.GUI..." -ForegroundColor Yellow

$cmakeGuiCandidates = @(
    "$root\engine\ENARM.GUI\CMakeLists.txt",
    "$root\src\ENARM.GUI\CMakeLists.txt",
    "$root\ENARM.GUI\CMakeLists.txt"
)
$cmakeGui = $null
foreach ($c in $cmakeGuiCandidates) {
    if (Test-Path $c) { $cmakeGui = $c; break }
}

if ($cmakeGui) {
    Write-Host "   Encontrado: $cmakeGui" -ForegroundColor Green
} else {
    # Busqueda recursiva
    $found = Get-ChildItem -Path $root -Recurse -Filter "CMakeLists.txt" |
             Where-Object { (Get-Content $_.FullName -Raw) -match "ENARM\.GUI" } |
             Select-Object -First 1
    if ($found) {
        $cmakeGui = $found.FullName
        Write-Host "   Encontrado (busqueda): $cmakeGui" -ForegroundColor Green
    } else {
        Write-Host "   ERROR: No se encontro CMakeLists.txt de ENARM.GUI" -ForegroundColor Red
    }
}

# Derivar rutas de src e include del modulo GUI
if ($cmakeGui) {
    $guiModuleRoot = Split-Path $cmakeGui -Parent
    $guiSrc        = "$guiModuleRoot\src"
    $guiInc        = "$guiModuleRoot\include\ENARM\GUI"
    Write-Host "   guiModuleRoot = $guiModuleRoot" -ForegroundColor DarkGray
} else {
    Write-Host "   Abortando: sin CMakeLists no podemos continuar." -ForegroundColor Red
    exit 1
}

# Localizar GuidelineRepository.cpp
$dataRepoCppCandidates = @(
    "$root\engine\ENARM.Data\src\Repositories\GuidelineRepository.cpp",
    "$root\engine\ENARM.Data\src\GuidelineRepository.cpp",
    "$root\src\ENARM.Data\src\Repositories\GuidelineRepository.cpp",
    "$root\src\ENARM.Data\src\GuidelineRepository.cpp"
)
$dataRepoCpp = $null
foreach ($c in $dataRepoCppCandidates) {
    if (Test-Path $c) { $dataRepoCpp = $c; break }
}
if (-not $dataRepoCpp) {
    $found2 = Get-ChildItem -Path $root -Recurse -Filter "GuidelineRepository.cpp" |
              Select-Object -First 1
    if ($found2) { $dataRepoCpp = $found2.FullName }
}

# Localizar GuidelineRepository.h
$dataRepoHCandidates = @(
    "$root\engine\ENARM.Data\include\ENARM\Data\GuidelineRepository.h",
    "$root\src\ENARM.Data\include\ENARM\Data\GuidelineRepository.h"
)
$dataRepoH = $null
foreach ($c in $dataRepoHCandidates) {
    if (Test-Path $c) { $dataRepoH = $c; break }
}
if (-not $dataRepoH) {
    $found3 = Get-ChildItem -Path $root -Recurse -Filter "GuidelineRepository.h" |
              Select-Object -First 1
    if ($found3) { $dataRepoH = $found3.FullName }
}

Write-Host "   GuidelineRepository.h   = $dataRepoH" -ForegroundColor DarkGray
Write-Host "   GuidelineRepository.cpp = $dataRepoCpp" -ForegroundColor DarkGray

# ---------------------------------------------------------------
# PASO 1: Patch GuidelineRepository.h
#   Agrega struct LinkedGuideline y declaracion FindLinkedGuidelines
#   solo si no existen ya
# ---------------------------------------------------------------
Write-Host ""
Write-Host "[1/6] Patch GuidelineRepository.h..." -ForegroundColor Yellow

if (-not $dataRepoH) {
    Write-Host "   ERROR: No se encontro GuidelineRepository.h" -ForegroundColor Red
} else {
    $hContent = Get-Content $dataRepoH -Raw

    $needsStruct = $hContent -notmatch "struct LinkedGuideline"
    $needsMethod = $hContent -notmatch "FindLinkedGuidelines"

    if (-not $needsStruct -and -not $needsMethod) {
        Write-Host "   Ya tiene LinkedGuideline y FindLinkedGuidelines, sin cambios." -ForegroundColor DarkGreen
    } else {
        # Bloque a inyectar antes de "class GuidelineRepository"
        $structBlock = @'

// ---- Sprint 7B Epica 2 ----------------------------------------
struct LinkedGuideline {
    Domain::ClinicalGuideline guideline;
    int8_t      relevance = 1;   // 1=PRIMARIA 2=SECUNDARIA 3=TERCIARIA
    std::string rationale;

    const char* RelevanceLabel() const noexcept {
        switch (relevance) {
            case 1:  return "PRIMARIA";
            case 2:  return "SECUNDARIA";
            default: return "TERCIARIA";
        }
    }
    bool IsPrimary()   const noexcept { return relevance == 1; }
    bool IsSecondary() const noexcept { return relevance == 2; }
    bool IsTertiary()  const noexcept { return relevance == 3; }
};
// ---------------------------------------------------------------

'@
        $methodDecl = @'

    // Sprint 7B: vinculaciones explicitas caso<->GPC
    Common::Result<std::vector<LinkedGuideline>>
        FindLinkedGuidelines(const std::string& case_id);

'@
        if ($needsStruct) {
            $hContent = $hContent -replace "(class GuidelineRepository)", "$structBlock`$1"
        }
        if ($needsMethod) {
            # Insertar antes del cierre de clase (ultima llave de cierre antes de namespace)
            $hContent = $hContent -replace "(private:)", "`$1$methodDecl"
        }

        # Asegurar include de cstdint
        if ($hContent -notmatch "#include\s*<cstdint>") {
            $hContent = $hContent -replace "(#pragma once)", "`$1`n#include <cstdint>"
        }

        [System.IO.File]::WriteAllText(
            $dataRepoH, $hContent,
            [System.Text.UTF8Encoding]::new($false))
        Write-Host "   Patch aplicado: $dataRepoH" -ForegroundColor Green
    }
}

# ---------------------------------------------------------------
# PASO 2: Patch GuidelineRepository.cpp
#   Agrega implementacion de FindLinkedGuidelines al final
# ---------------------------------------------------------------
Write-Host ""
Write-Host "[2/6] Patch GuidelineRepository.cpp..." -ForegroundColor Yellow

if (-not $dataRepoCpp) {
    Write-Host "   ERROR: No se encontro GuidelineRepository.cpp" -ForegroundColor Red
} else {
    $cppContent = Get-Content $dataRepoCpp -Raw

    if ($cppContent -match "FindLinkedGuidelines") {
        Write-Host "   Ya tiene FindLinkedGuidelines, sin cambios." -ForegroundColor DarkGreen
    } else {
        $implBlock = @'

// ---------------------------------------------------------------
// FindLinkedGuidelines  -- Sprint 7B Epica 2
// ---------------------------------------------------------------
Common::Result<std::vector<LinkedGuideline>>
GuidelineRepository::FindLinkedGuidelines(const std::string& case_id)
{
    if (case_id.empty())
        return Common::Result<std::vector<LinkedGuideline>>::Failure(
            "case_id vacio");

    try {
        auto conn = m_db.GetConnection();
        pqxx::work txn(*conn);

        const std::string SQL =
            "SELECT cg.relevance, cg.rationale, "
            "       gl.id, gl.title, gl.specialty, gl.category, "
            "       gl.institution, gl.version, gl.year, gl.summary, gl.url "
            "FROM case_guidelines cg "
            "INNER JOIN clinical_guidelines gl ON gl.id = cg.guideline_id "
            "WHERE cg.case_id = $1 "
            "ORDER BY cg.relevance ASC, gl.id ASC";

        auto result = txn.exec_params(SQL, case_id);
        txn.commit();

        std::vector<LinkedGuideline> linked;
        linked.reserve(result.size());

        for (const auto& row : result) {
            LinkedGuideline lg;
            lg.relevance  = static_cast<int8_t>(row["relevance"].as<int>());
            lg.rationale  = row["rationale"].c_str();

            Domain::ClinicalGuideline& g = lg.guideline;
            g.id          = row["id"].c_str();
            g.title       = row["title"].c_str();
            g.specialty   = row["specialty"].c_str();
            g.category    = row["category"].is_null()    ? "" : row["category"].c_str();
            g.institution = row["institution"].is_null() ? "" : row["institution"].c_str();
            g.version     = row["version"].is_null()     ? "" : row["version"].c_str();
            g.year        = row["year"].is_null()         ? 0  : row["year"].as<int>();
            g.summary     = row["summary"].is_null()     ? "" : row["summary"].c_str();
            g.url         = row["url"].is_null()         ? "" : row["url"].c_str();

            linked.push_back(std::move(lg));
        }

        SPDLOG_INFO("FindLinkedGuidelines: caso={} -> {} GPCs", case_id, linked.size());
        return Common::Result<std::vector<LinkedGuideline>>::Success(std::move(linked));
    }
    catch (const pqxx::sql_error& e) {
        SPDLOG_ERROR("FindLinkedGuidelines SQL: {}", e.what());
        return Common::Result<std::vector<LinkedGuideline>>::Failure(e.what());
    }
    catch (const std::exception& e) {
        SPDLOG_ERROR("FindLinkedGuidelines: {}", e.what());
        return Common::Result<std::vector<LinkedGuideline>>::Failure(e.what());
    }
}

'@
        # Quitar ultimo cierre de namespace si existe, reinsertar con el metodo adentro
        # Estrategia mas segura: simplemente append antes del ultimo "} // namespace"
        if ($cppContent -match "}\s*//\s*namespace") {
            $cppContent = $cppContent -replace
                "(}\s*//\s*namespace[^\n]*)\s*$",
                ("$implBlock`n`$1")
        } else {
            $cppContent = $cppContent + $implBlock
        }

        [System.IO.File]::WriteAllText(
            $dataRepoCpp, $cppContent,
            [System.Text.UTF8Encoding]::new($false))
        Write-Host "   Patch aplicado: $dataRepoCpp" -ForegroundColor Green
    }
}

# ---------------------------------------------------------------
# PASO 3: Crear PatientSessionView_Guidelines.cpp
# ---------------------------------------------------------------
Write-Host ""
Write-Host "[3/6] Creando PatientSessionView_Guidelines.cpp..." -ForegroundColor Yellow

# Encontrar PatientSessionView.cpp para saber el include correcto
$psViewCppFound = Get-ChildItem -Path $root -Recurse -Filter "PatientSessionView.cpp" |
                  Select-Object -First 1
$psViewInclude  = "ENARM/GUI/Views/PatientSessionView.h"
if ($psViewCppFound) {
    $psViewDir = Split-Path $psViewCppFound.FullName -Parent
    Write-Host "   PatientSessionView.cpp en: $psViewDir" -ForegroundColor DarkGray
}

$glCppPath = if ($psViewCppFound) {
    Join-Path (Split-Path $psViewCppFound.FullName -Parent) "PatientSessionView_Guidelines.cpp"
} else {
    "$guiSrc\Views\PatientSessionView_Guidelines.cpp"
}

Write-Host "   Destino: $glCppPath" -ForegroundColor DarkGray

$glCppContent = @'
// ---------------------------------------------------------------
// PatientSessionView_Guidelines.cpp
// Sprint 7B Epica 2: LoadSessionGuidelines + RenderTabGuias
//
// Implementa SOLO los metodos de guias de PatientSessionView.
// El resto de tabs sigue en PatientSessionView.cpp
// ---------------------------------------------------------------
#include "ENARM/GUI/Views/PatientSessionView.h"
#include <imgui.h>
#include <spdlog/spdlog.h>
#include <fmt/format.h>

namespace ENARM::GUI {

// ---------------------------------------------------------------
// LoadSessionGuidelines
//   Intento 1: vinculos explicitos en case_guidelines
//   Intento 2: fallback por especialidad
//   Idempotente: solo carga una vez por caso activo
// ---------------------------------------------------------------
void PatientSessionView::LoadSessionGuidelines()
{
    if (m_guidelinesLoaded) return;
    if (!m_currentCase)     return;

    const std::string caseId    = m_currentCase->id;
    const std::string specialty = m_currentCase->specialty;

    // Intento 1: vinculos explicitos
    auto res = m_glRepo.FindLinkedGuidelines(caseId);
    if (res.IsSuccess() && !res.Value().empty()) {
        m_linkedGuidelines = std::move(res.Value());
        m_guidelinesLoaded = true;
        SPDLOG_INFO("[PSV] Guias: {} GPCs via vinculo explicito",
                    m_linkedGuidelines.size());
        return;
    }

    // Intento 2: fallback por especialidad
    SPDLOG_WARN("[PSV] Sin vinculos para caso {}. Fallback: {}", caseId, specialty);
    auto fb = m_glRepo.FindBySpecialty(specialty);
    if (fb.IsSuccess() && !fb.Value().empty()) {
        m_linkedGuidelines.clear();
        m_linkedGuidelines.reserve(fb.Value().size());
        for (auto& gl : fb.Value()) {
            Data::LinkedGuideline lg;
            lg.guideline = std::move(gl);
            lg.relevance = 1;
            lg.rationale = "Fallback por especialidad";
            m_linkedGuidelines.push_back(std::move(lg));
        }
        m_guidelinesLoaded = true;
        SPDLOG_INFO("[PSV] Guias: {} GPCs via fallback", m_linkedGuidelines.size());
        return;
    }

    SPDLOG_WARN("[PSV] Sin GPCs para caso {}", caseId);
    m_guidelinesLoaded = true;
}

// ---------------------------------------------------------------
// RenderTabGuias
// ---------------------------------------------------------------
void PatientSessionView::RenderTabGuias()
{
    LoadSessionGuidelines();

    if (m_linkedGuidelines.empty()) {
        ImGui::Spacing();
        ImGui::TextDisabled("No hay Guias de Practica Clinica vinculadas a este caso.");
        return;
    }

    // Colores por relevancia
    auto BadgeBg = [](int8_t rel) -> ImVec4 {
        switch (rel) {
            case 1:  return ImVec4(0.15f, 0.72f, 0.37f, 1.00f); // verde
            case 2:  return ImVec4(0.88f, 0.58f, 0.08f, 1.00f); // naranja
            default: return ImVec4(0.42f, 0.46f, 0.55f, 1.00f); // gris
        }
    };
    auto BorderCol = [](int8_t rel) -> ImVec4 {
        switch (rel) {
            case 1:  return ImVec4(0.15f, 0.72f, 0.37f, 0.50f);
            case 2:  return ImVec4(0.88f, 0.58f, 0.08f, 0.50f);
            default: return ImVec4(0.42f, 0.46f, 0.55f, 0.40f);
        }
    };

    // Contar por relevancia
    int nP = 0, nS = 0, nT = 0;
    for (const auto& lg : m_linkedGuidelines) {
        if      (lg.IsPrimary())   ++nP;
        else if (lg.IsSecondary()) ++nS;
        else                       ++nT;
    }

    // Header
    ImGui::Spacing();
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.75f, 0.85f, 1.00f, 1.0f));
    ImGui::Text("GPCs relevantes: %zu   (P:%d  S:%d  T:%d)",
                m_linkedGuidelines.size(), nP, nS, nT);
    ImGui::PopStyleColor();
    ImGui::Separator();
    ImGui::Spacing();

    // Lista de cards
    const float cardH  = 92.0f;
    const float padX   = 12.0f;
    const float padY   = 9.0f;
    const float btnW   = 108.0f;

    for (size_t i = 0; i < m_linkedGuidelines.size(); ++i) {
        const auto& lg  = m_linkedGuidelines[i];
        const ImVec4 bg = BadgeBg(lg.relevance);
        const ImVec4 bd = BorderCol(lg.relevance);

        ImGui::PushID(static_cast<int>(i));

        float      cardW = ImGui::GetContentRegionAvail().x;
        ImVec2     cMin  = ImGui::GetCursorScreenPos();
        ImVec2     cMax  = { cMin.x + cardW, cMin.y + cardH };
        ImDrawList* dl   = ImGui::GetWindowDrawList();

        // Sombra
        dl->AddRectFilled(
            { cMin.x + 2, cMin.y + 3 },
            { cMax.x + 2, cMax.y + 3 },
            IM_COL32(0, 0, 0, 55), 8.0f);

        // Fondo
        dl->AddRectFilled(cMin, cMax, IM_COL32(27, 32, 46, 215), 8.0f);

        // Borde coloreado
        dl->AddRect(cMin, cMax,
            IM_COL32((int)(bd.x*255),(int)(bd.y*255),(int)(bd.z*255),(int)(bd.w*255)),
            8.0f, 0, 1.8f);

        // Barra lateral izquierda
        dl->AddRectFilled(
            { cMin.x,     cMin.y + 5 },
            { cMin.x + 4, cMax.y - 5 },
            IM_COL32((int)(bg.x*255),(int)(bg.y*255),(int)(bg.z*255), 210),
            2.0f);

        // Reservar espacio
        ImGui::Dummy({ cardW, cardH });

        // --- Fila 1: badge + id + boton ---
        float row1Y = cMin.y + padY;
        ImGui::SetCursorScreenPos({ cMin.x + padX + 5, row1Y });

        ImGui::PushStyleColor(ImGuiCol_Button,        bg);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, bg);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive,  bg);
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1,1,1,1));
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 6.0f, 2.0f });
        ImGui::SmallButton(lg.RelevanceLabel());
        ImGui::PopStyleVar(2);
        ImGui::PopStyleColor(4);

        ImGui::SameLine(0, 8);
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.55f, 0.70f, 0.95f, 1.0f));
        ImGui::Text("%s", lg.guideline.id.c_str());
        ImGui::PopStyleColor();

        // Boton "Detalle" a la derecha
        ImGui::SetCursorScreenPos({ cMax.x - btnW - padX, row1Y });
        ImGui::PushStyleColor(ImGuiCol_Button,
            ImVec4(0.18f, 0.32f, 0.62f, 0.85f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
            ImVec4(0.25f, 0.45f, 0.80f, 1.00f));
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f);
        if (ImGui::SmallButton("Ver detalle")) {
            if (m_onGuidelineSelected)
                m_onGuidelineSelected(lg.guideline.id);
        }
        ImGui::PopStyleVar();
        ImGui::PopStyleColor(2);

        // --- Fila 2: titulo ---
        ImGui::SetCursorScreenPos({ cMin.x + padX + 5, cMin.y + padY + 26 });
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.94f, 0.96f, 1.00f, 1.0f));
        // Truncar si es muy largo
        std::string title = lg.guideline.title;
        const float maxW  = cardW - btnW - padX * 3;
        while (!title.empty() &&
               ImGui::CalcTextSize(title.c_str()).x > maxW)
            title.resize(title.size() - 1);
        if (title.size() < lg.guideline.title.size()) title += "...";
        ImGui::TextUnformatted(title.c_str());
        ImGui::PopStyleColor();

        // --- Fila 3: rationale + meta ---
        ImGui::SetCursorScreenPos({ cMin.x + padX + 5, cMin.y + padY + 52 });
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.52f, 0.57f, 0.64f, 1.0f));
        ImGui::TextUnformatted(lg.rationale.c_str());
        ImGui::PopStyleColor();

        std::string meta = fmt::format("{} . {}",
            lg.guideline.institution.empty() ? "IMSS/CENETEC" : lg.guideline.institution,
            lg.guideline.year > 0 ? std::to_string(lg.guideline.year) : "s/f");
        float metaW = ImGui::CalcTextSize(meta.c_str()).x;
        ImGui::SetCursorScreenPos({ cMax.x - metaW - padX, cMin.y + padY + 52 });
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.38f, 0.42f, 0.50f, 1.0f));
        ImGui::TextUnformatted(meta.c_str());
        ImGui::PopStyleColor();

        ImGui::PopID();
        ImGui::Spacing();
        ImGui::Spacing();
    }
}

} // namespace ENARM::GUI
'@

Write-Utf8 $glCppPath $glCppContent

# ---------------------------------------------------------------
# PASO 4: Registrar en CMakeLists.txt de ENARM.GUI
# ---------------------------------------------------------------
Write-Host ""
Write-Host "[4/6] Registrando en CMakeLists.txt de ENARM.GUI..." -ForegroundColor Yellow

$cmakeContent = Get-Content $cmakeGui -Raw
$glCppRel     = Split-Path $glCppPath -Leaf
$glCppParent  = Split-Path (Split-Path $glCppPath -Parent) -Leaf

# Construir ruta relativa al CMakeLists
$glCppRelPath = if ($glCppParent -eq "Views") {
    "src/Views/PatientSessionView_Guidelines.cpp"
} else {
    "src/PatientSessionView_Guidelines.cpp"
}

if ($cmakeContent -match "PatientSessionView_Guidelines") {
    Write-Host "   Ya registrado en CMakeLists.txt" -ForegroundColor DarkGreen
} else {
    # Insertar despues de PatientSessionView.cpp
    $patternFound = $false

    foreach ($pattern in @(
        "src/Views/PatientSessionView\.cpp",
        "src/PatientSessionView\.cpp"
    )) {
        if ($cmakeContent -match $pattern) {
            $cmakeContent = $cmakeContent -replace
                "($pattern)",
                "`$1`n        $glCppRelPath"
            $patternFound = $true
            break
        }
    }

    if (-not $patternFound) {
        Write-Host "   AVISO: no encontre PatientSessionView.cpp en CMakeLists" -ForegroundColor Yellow
        Write-Host "   Agrega manualmente: $glCppRelPath" -ForegroundColor Yellow
    } else {
        [System.IO.File]::WriteAllText(
            $cmakeGui, $cmakeContent,
            [System.Text.UTF8Encoding]::new($false))
        Write-Host "   OK  CMakeLists.txt actualizado" -ForegroundColor Green
    }
}

# ---------------------------------------------------------------
# PASO 5: Verificacion pre-build
# ---------------------------------------------------------------
Write-Host ""
Write-Host "[5/6] Verificacion pre-build..." -ForegroundColor Yellow
Write-Host ""

$allOk = $true

$checks = @(
    @{
        Path     = $dataRepoH
        Label    = "GuidelineRepository.h"
        Patterns = @("struct LinkedGuideline","FindLinkedGuidelines","RelevanceLabel")
    }
    @{
        Path     = $dataRepoCpp
        Label    = "GuidelineRepository.cpp"
        Patterns = @("FindLinkedGuidelines","case_guidelines","exec_params")
    }
    @{
        Path     = $glCppPath
        Label    = "PatientSessionView_Guidelines.cpp"
        Patterns = @("LoadSessionGuidelines","RenderTabGuias","Fallback por especialidad")
    }
    @{
        Path     = $cmakeGui
        Label    = "CMakeLists.txt ENARM.GUI"
        Patterns = @("PatientSessionView_Guidelines")
    }
)

foreach ($chk in $checks) {
    $label = $chk.Label
    Write-Host "  $label" -ForegroundColor White
    if (-not $chk.Path -or -not (Test-Path $chk.Path)) {
        Write-Host "    x ARCHIVO NO ENCONTRADO" -ForegroundColor Red
        $allOk = $false
        continue
    }
    $txt = Get-Content $chk.Path -Raw
    foreach ($pat in $chk.Patterns) {
        $ok  = $txt -match [regex]::Escape($pat)
        $sym = if ($ok) { "OK" } else { "FALTA" }
        $col = if ($ok) { "Green" } else { "Red" }
        if (-not $ok) { $allOk = $false }
        Write-Host "    $sym  $pat" -ForegroundColor $col
    }
    Write-Host ""
}

if (-not $allOk) {
    Write-Host "  Hay elementos faltantes. Revisa antes de continuar." -ForegroundColor Red
    Write-Host ""
}

# ---------------------------------------------------------------
# PASO 6: Build con target correcto
# ---------------------------------------------------------------
Write-Host "[6/6] Compilando ENARM.App..." -ForegroundColor Yellow
Write-Host ""

$buildLog = "$root\build_e2p2c.log"
$errLog   = "$root\build_e2p2c_err.log"

# IMPORTANTE: no usar $args (reservada en PS), usar $buildArgs
$buildArgs = @(
    "--build", $build,
    "--config", "Release",
    "--target", "ENARM.App",
    "-j8"
)

Write-Host "   cmake $($buildArgs -join ' ')" -ForegroundColor DarkGray

$proc = Start-Process `
    -FilePath "cmake" `
    -ArgumentList $buildArgs `
    -WorkingDirectory $root `
    -RedirectStandardOutput $buildLog `
    -RedirectStandardError  $errLog `
    -NoNewWindow -Wait -PassThru

$exitCode = $proc.ExitCode

Write-Host ""
Write-Host "-------- BUILD LOG (ultimas 60 lineas) --------" -ForegroundColor Cyan
if (Test-Path $buildLog) {
    Get-Content $buildLog | Select-Object -Last 60
} else {
    Write-Host "Sin build log." -ForegroundColor Yellow
}

Write-Host ""
Write-Host "-------- STDERR (ultimas 40 lineas) -----------" -ForegroundColor Cyan
if ((Test-Path $errLog) -and ((Get-Item $errLog).Length -gt 0)) {
    Get-Content $errLog | Select-Object -Last 40
} else {
    Write-Host "Sin stderr." -ForegroundColor Green
}

Write-Host ""
if ($exitCode -eq 0) {
    Write-Host "=======================================================" -ForegroundColor Green
    Write-Host "  BUILD OK - Epica 2 Parte 2 completada" -ForegroundColor Green
    Write-Host "=======================================================" -ForegroundColor Green
    Write-Host ""
    Write-Host "Prueba:" -ForegroundColor Magenta
    Write-Host "  1. Abre ENARM.exe"
    Write-Host "  2. Elige caso IAM STEMI"
    Write-Host "  3. Tab Guias"
    Write-Host "  4. Deben verse 4 cards con badges:"
    Write-Host "     PRIMARIA   (verde)  - IMSS-234-09, IMSS-357-13"
    Write-Host "     SECUNDARIA (naranja)- IMSS-014-08"
    Write-Host "     TERCIARIA  (gris)   - IMSS-076-08"
    Write-Host ""
} else {
    Write-Host "=======================================================" -ForegroundColor Red
    Write-Host "  BUILD FALLO (exit $exitCode)" -ForegroundColor Red
    Write-Host "=======================================================" -ForegroundColor Red
    Write-Host ""
    Write-Host "Pega el bloque BUILD LOG + STDERR completo." -ForegroundColor Yellow
    Write-Host ""
}