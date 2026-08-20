# ENARM Simulator Pro

Simulador clínico profesional para preparación del **ENARM** (Examen Nacional de Aspirantes a Residencias Médicas).

Combina un motor fisiológico/farmacológico determinista en C++20 con un paciente virtual conversacional impulsado por LLMs locales (Ollama), evaluación por competencias con rúbricas, y persistencia en PostgreSQL 18.

## Características

### Motor clínico (C++20 determinista)
- **Hemodinámica**: Modelo Windkessel 4 elementos (PA, FC, gasto cardíaco, presión de pulso)
- **Respiratorio**: Compliance pulmonar dinámica (FR, SpO₂, PaO₂, PaCO₂, EtCO₂)
- **Metabólico**: Balance ácido-base, electrolitos, lactato, glucosa
- **ECG sintético**: 12 derivaciones, ritmos (sinusal, taquicardia, FA, STEMI anterior/inferior, bloqueo AV, FV)
- **Farmacología ICU**: 8 categorías, bolos + infusiones, PK 2 compartimentos
- **Progresión de enfermedad**: Fases Estable → Compensado → Descompensando → Crítico
- **Códigos de emergencia**: Código Azul, STEMI, Stroke, Trauma, Sepsis con cronómetros

### Paciente virtual (Ollama)
- Chat conversacional con persona configurable (edad, sexo, síntomas, historia)
- **Revelación progresiva**: detección de intención (12 categorías), el paciente no revela todo de golpe
- Multi-modelo: phi3.5 (paciente rápido), meditron:70b (razonamiento), qwen2-math (dosis)

### Evaluación
- **AssessmentEngine** con 7 rúbricas (IAM, Neumonía, Preeclampsia, Apendicitis, TCE, CAD, LRA, Bronquiolitis)
- **GuidelineAdherenceScorer**: adherencia a GPCs en tiempo real
- **MCQ Engine**: práctica libre, modo débil, retroalimentación por opción
- **Learning adaptativo**: SpacedRepetition (FSRS), WeaknessDetector, AdaptiveCaseSelector

### Datos
- **89+ casos clínicos** generados en 21+ especialidades
- **PostgreSQL 16** con pgvector para búsqueda semántica
- **RAGService**: embeddings + búsqueda de GPCs por similitud

### Interfaces
- **GUI**: Dear ImGui + DX11 (dashboard, casos, guías, sesión clínica, scoring)
- **CLI**: Interactivo de 1000+ líneas
- **UE5**: Proyecto `unreal/ENARMProject` con plugin `ENARMCoreBridge` que expone el motor clínico a Blueprints

## Requisitos

- Windows 10/11 x64
- Visual Studio 2022/2026 (MSVC, C++20)
- CMake 3.28+ / Ninja
- vcpkg
- PostgreSQL 16+
- Ollama (con phi3.5:latest, qwen2.5-coder:7b)
- UE 5.8 (opcional, para la UI 3D)

## Build

```bash
# Configurar
cmake -S . -B build -G "Visual Studio 18 2026" -A x64

# Compilar
cmake --build build --config Release

# Tests
.\scripts\run_tests.ps1
```

## Run

```bash
# GUI
.\build\bin\Release\ENARM.exe

# CLI
.\build\bin\Release\ENARM.CLI.exe
```

## Tests

- **43 tests** con Catch2 (100% pasan)
- Coverage por módulo: fisiológico, farmacología, assessment, metabolismo, ECG

## Roadmap

| Sprint | Estado | Descripción |
|--------|--------|-------------|
| S1 | ✅ | Infraestructura (CMake, vcpkg, CI) |
| S2 | ✅ | Motor fisiológico nivel 1 |
| S3 | ✅ | Base de datos + pgvector |
| S4 | ✅ | IA core: Ollama + router |
| S5 | ✅ | Paciente virtual conversacional |
| S7 | ✅ | Evaluación por competencias |
| S8 | ✅ | MCQ Engine |
| S9 | ✅ | UE5 + Bridge (fase 1) |
| S13 | ✅ | Contenido: 89 casos, 21 especialidades |
| S14 | ✅ | Learning adaptativo |
| S16 | ✅ | Códigos de emergencia |
| S18 | ✅ | Sala multi-usuario (esqueleto) |
| S19 | ✅ | Analytics embebidos |
| S21 | ✅ | Instalador NSIS/ZIP |
| S10-12 | 🚧 | MetaHumans, entornos 3D, equipos médicos |
| S15 | 🚧 | Simulador quirúrgico/anestesia |
| S17 | 🚧 | PubMed + FAISS |

## Licencia

MIT
