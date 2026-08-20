# Sprint 7B - Epica 2: Vinculacion explicita caso <-> GPC

## Estado: ✅ COMPLETADA

## Objetivo
Permitir que cada caso clinico tenga vinculadas explicitamente las GPCs
mas relevantes, con un nivel de relevancia (PRIMARIA/SECUNDARIA/TERCIARIA)
y una razon de vinculacion.

## Cambios implementados

### Base de datos
- **Migracion**: `scripts/postgres/026_case_guidelines.sql`
- **Tabla nueva**: `case_guidelines` (case_id, guideline_id, relevance, rationale)
- **Datos**: 33 vinculaciones para 10 casos activos
  - 14 primarias (r=1)
  - 12 secundarias (r=2)
  - 7 terciarias (r=3)

### Backend C++
- **Nuevo struct**: `Data::GuidelineRepository::LinkedGuidelineRow`
- **Nuevo metodo**: `GuidelineRepository::FindLinkedGuidelines(case_id)`
- **Modificado**: `AppContext::LoadSessionGuidelines()`
  - Intento 1: buscar vinculos explicitos en `case_guidelines`
  - Intento 2 (fallback): buscar por especialidad como antes
- **Nuevos campos en AppContext**:
  - `sessionGuidelineRelevance : unordered_map<string,int>`
  - `sessionGuidelineRationale : unordered_map<string,string>`

### UI (PatientSessionView)
- Tab "Guias":
  - Header muestra "Vinculadas al caso: P=N S=N T=N"
  - Cada GPC con badge coloreado antes del titulo:
    - PRIMARIA   -> verde  (RGB 38,184,94)
    - SECUNDARIA -> naranja (RGB 224,148,20)
    - TERCIARIA  -> gris   (RGB 107,117,140)
  - Hover en badge -> tooltip con el rationale

## Casos con vinculaciones

| Caso                      | GPCs | Primarias | Secundarias | Terciarias |
|---------------------------|:----:|:---------:|:-----------:|:----------:|
| IAM STEMI                 |  4   |    2      |     1       |     1      |
| Shock septico ITU         |  4   |    3      |     1       |     0      |
| LRA por AINEs             |  5   |    1      |     3       |     1      |
| Neumonia en EPOC          |  3   |    2      |     1       |     0      |
| ACV isquemico             |  3   |    2      |     0       |     1      |
| CAD debut                 |  3   |    1      |     1       |     1      |
| Preeclampsia              |  3   |    1      |     2       |     0      |
| Apendicitis pediatrica    |  3   |    1      |     1       |     1      |
| TCE severo                |  2   |    1      |     1       |     0      |
| Bronquiolitis VSR         |  3   |    0      |     1       |     2      |

## Como probar

1. Abrir ENARM.exe
2. Seleccionar cualquier caso (ej. IAM STEMI)
3. Ir al tab "Guias"
4. Verificar:
   - Header con contador P/S/T
   - Solo aparecen las GPCs vinculadas (4 para STEMI, no 9)
   - Cada card tiene su badge coloreado
   - Hover sobre badge muestra tooltip con rationale

## Screenshots

- `before.png` - Tab Guias mostrando 9 GPCs de Cardiology sin distincion
- `after.png`  - Tab Guias con 4 GPCs vinculadas y badges coloreados

## Archivos modificados

- `engine/ENARM.Data/include/ENARM/Data/Repositories/GuidelineRepository.h`
- `engine/ENARM.Data/src/Repositories/GuidelineRepository.cpp`
- `engine/ENARM.GUI/include/ENARM/GUI/AppContext.h`
- `engine/ENARM.GUI/src/Views/PatientSessionView.cpp`
- `scripts/postgres/026_case_guidelines.sql` (nuevo)

## Proxima epica

**Epica 3**: Filtros de relevancia + ordenamiento configurable