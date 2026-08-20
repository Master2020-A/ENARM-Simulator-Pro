# ENARM Simulator Pro - Testing Guide

## Estado actual
- **78 tests activos** (100% pass rate)
- **Framework**: Catch2 v3.15 + CTest
- **Tiempo total**: ~2.3 segundos

## Ejecución

### Modo SAFE (recomendado)
```powershell
.\scripts\run_tests_safe.ps1

Este script:

    Hace snapshot MD5 de los .cpp
    Ejecuta CMake configure
    Verifica que ningún .cpp fue modificado (restaura si es necesario)
    Compila y ejecuta

Modo normal

PowerShell

.\scripts\run_tests.ps1
.\scripts\run_tests.ps1 -Filter "[hemodynamic]"
.\scripts\run_tests.ps1 -Verbose
.\scripts\run_tests.ps1 -RebuildAll

Filtros por tag disponibles
TagTestsMódulo
[hemodynamic]19HemodynamicModel
[pharmacology]14DrugDatabase
[assessment]14AssessmentEngine
[competency]12CompetencyEvaluator
[adherence]5GuidelineAdherenceScorer
[metabolic]6Contratos MetabolicModel
[ecg]4ECGSynthesizer
[progression]4DiseaseProgressionEngine
[shock]4Cruzado
[sepsis]4Cruzado
[stemi]2Cruzado
[iam]3Cruzado
Tipos de tests
API real (43 tests)

    Prueban métodos públicos reales del motor
    Requieren linking con las libs
    Ejemplo: HemodynamicModel::GetMAP(), DrugDatabase::Instance()

Lógica pura / contratos (35 tests)

    Validan fórmulas médicas y matemáticas
    Sin dependencias de headers del proyecto
    Ejemplos: MAP = (PAS + 2×PAD)/3, KDIGO, CURB-65, Glasgow

Tests pendientes de integrar

    test_respiratory.cpp (~8 tests) — A.3
    test_hemodynamic_extended.cpp (~6 tests) — futuro
    test_assessment_engine.cpp (~10 tests) — futuro
    test_database_manager.cpp (~15 tests) — requiere PostgreSQL corriendo
    test_patient_simulator.cpp (~10 tests) — requiere ENARM.Simulation

Meta de cobertura
FaseTests objetivoCobertura estimada
✅ Fase 0 (actual)78~15%
🎯 A.3 + A.486~17%
🎯 Fase I completa120~25%
🎯 Fase II completa180~45%
🎯 Fase VI Sprint 20300+~90%