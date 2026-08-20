# ═══════════════════════════════════════════════════════════════
#   diagnose_api.ps1
#   Extrae la API pública de los módulos para ajustar los tests
#   Ejecutar si los tests dan error de compilación
# ═══════════════════════════════════════════════════════════════

$root = "C:\Users\Master\Documents\ENARM-IA\ENARMSimulator"

function Show-PublicApi {
    param([string]$Module, [string]$HeaderPattern)

    Write-Host ""
    Write-Host "── $Module ──────────────────────────────────" -ForegroundColor Cyan

    $headers = Get-ChildItem "$root\engine\$Module" `
               -Recurse -Include $HeaderPattern

    foreach ($h in $headers) {
        Write-Host "  📄 $($h.Name)" -ForegroundColor Yellow
        # Extraer líneas con métodos públicos, structs y enums
        Get-Content $h.FullName |
            Where-Object { $_ -match "(class |struct |enum |public:|void |bool |float |int |auto |Result)" } |
            Where-Object { $_ -notmatch "^//" } |
            Select-Object -First 30 |
            ForEach-Object { Write-Host "    $_" -ForegroundColor Gray }
        Write-Host ""
    }
}

Show-PublicApi "ENARM.Physiology"   "HemodynamicModel.h"
Show-PublicApi "ENARM.Physiology"   "RespiratoryModel.h"
Show-PublicApi "ENARM.Pharmacology" "DrugDatabase.h"
Show-PublicApi "ENARM.Pharmacology" "DrugAdministration.h"
Show-PublicApi "ENARM.Simulation"   "PatientSimulator.h"
Show-PublicApi "ENARM.Simulation"   "ClinicalScenarios.h"
Show-PublicApi "ENARM.Assessment"   "AssessmentEngine.h"
Show-PublicApi "ENARM.Assessment"   "GuidelineAdherenceScorer.h"
Show-PublicApi "ENARM.Data"         "DatabaseManager.h"
