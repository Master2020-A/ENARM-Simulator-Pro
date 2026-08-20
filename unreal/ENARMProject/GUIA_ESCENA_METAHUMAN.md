# Guía: Crear la escena clínica con MetaHuman en UE 5.8

## Requisitos previos
- UE 5.8 instalado (ya verificado)
- Proyecto `unreal/ENARMProject/ENARMProject.uproject` (ya compilado)
- Plugin MetaHuman (instalar desde Epic Games Launcher si no está)

## Paso 1: Abrir el proyecto
1. Doble clic en `unreal/ENARMProject/ENARMProject.uproject`
2. Si pregunta "Missing modules", acepta recompilar (ya compilado)
3. Espera a que el editor cargue (~1-2 min)

## Paso 2: Crear la escena (opción A - automática)
1. En el editor: `Tools > Execute Python Script`
2. Selecciona `unreal/ENARMProject/Scripts/setup_clinical_scene.py`
3. Debería crear: nivel `ClinicalScene`, actor `VirtualPatient` con `UPatientAIComponent`, cámara

## Paso 2b: Crear la escena (opción B - manual)
1. `File > New Level > Empty Level`
2. Guardar como `/Game/ClinicalScene/ClinicalScene`
3. Desde Content Browser: `Add > Add Feature or Content Pack > MetaHumans`
4. Arrastra un MetaHuman al nivel (o crea uno en MetaHuman Creator)
5. Selecciona el MetaHuman y `Add Component > PatientAIComponent`
6. Añade una cámara (`Place Actors > Camera`)

## Paso 3: Asignar GameMode
1. `Project Settings > Maps & Modes`
2. `Default GameMode` = `ClinicalSceneGameMode` (del plugin ENARMCoreBridge)
3. `Game Default Map` = `ClinicalScene`

## Paso 4: Probar
1. Pulsa `Play` (F8)
2. El GameMode debería encontrar el `UPatientAIComponent` y loguear:
   `[ENARM] Paciente virtual encontrado`
3. En el HUD (si lo creas en Blueprint) puedes llamar:
   - `GetMAP()`, `GetHeartRate()`, `GetSystolicBP()`
   - `ApplyScenario("sepsis")`
   - `GiveBolus("norepinephrine", 0.1)`

## Paso 5: Conectar el chat (Sprint 10)
El `AskPatient()` ya está expuesto. Para que el MetaHuman responda:
1. Crea un Blueprint `BP_VirtualPatient` derivado de `Character`
2. Añade `UPatientAIComponent`
3. En `Event Tick` llama `TickSimulation(DeltaSeconds)`
4. Para el chat: llama `AskPatient("Como se siente?")` y conecta la respuesta a un widget de texto
5. (Opcional) Integra TTS (Piper) para que el MetaHuman hable

## Troubleshooting
- **"Missing ENARMCoreBridge module"**: recompila con Build.bat (ver README)
- **MetaHuman no aparece**: instala el plugin desde Epic Games Launcher
- **No encuentra el componente**: verifica que el actor tenga `PatientAIComponent` agregado
