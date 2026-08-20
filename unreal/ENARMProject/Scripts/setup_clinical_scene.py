# =============================================================
#  setup_clinical_scene.py - Script Python para UE5 Editor
#  Crea la escena clinica con MetaHuman + UPatientAIComponent
#
#  USO (dentro de UE5 Editor):
#    Tools > Execute Python Script > seleccionar este archivo
#  O desde consola:
#    "C:\Program Files\Epic Games\UE_5.8\Engine\Binaries\Win64\UnrealEditor-Cmd.exe"
#      "C:\...\ENARMProject.uproject" -run=pythonscript -script="setup_clinical_scene.py"
# =============================================================
import unreal

def log(msg):
    unreal.log("[ENARM-Setup] " + msg)

def create_clinical_scene():
    """Crea la escena ClinicalScene con MetaHuman + componentes ENARM."""
    log("Creando escena clinica...")

    # 1. Crear nivel
    asset_tools = unreal.AssetToolsHelpers.get_asset_tools()
    factory = unreal.WorldFactory()
    world_path = "/Game/ClinicalScene/ClinicalScene"
    world = asset_tools.create_asset("ClinicalScene", "/Game/ClinicalScene",
                                     unreal.World, factory)
    if world is None:
        log("ERROR: No se pudo crear el nivel ClinicalScene")
        return False
    log("Nivel creado: " + world_path)

    # 2. Crear GameMode (ya existe en C++: AClinicalSceneGameMode)
    #    Se asigna via DefaultGame.ini

    # 3. Crear el Actor del paciente virtual
    editor_subsystem = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
    patient_actor = editor_subsystem.spawn_actor_from_class(
        unreal.MetaHuman, unreal.Vector(0, 0, 0), unreal.Rotator(0, 0, 0))
    if patient_actor is None:
        log("WARN: MetaHuman no disponible, creando Actor basico")
        patient_actor = editor_subsystem.spawn_actor_from_class(
            unreal.Character, unreal.Vector(0, 0, 0), unreal.Rotator(0, 0, 0))
    patient_actor.set_actor_label("VirtualPatient")
    log("Paciente virtual creado: " + patient_actor.get_actor_label())

    # 4. Agregar UPatientAIComponent al paciente
    patient_actor.add_component_by_class(
        unreal.PatientAIComponent, False, unreal.Vector(0, 0, 0), unreal.Rotator(0, 0, 0))
    log("UPatientAIComponent agregado al paciente")

    # 5. Crear camara de consultorio
    camera = editor_subsystem.spawn_actor_from_class(
        unreal.CameraActor, unreal.Vector(-300, 0, 160), unreal.Rotator(0, 0, 0))
    camera.set_actor_label("ConsultorioCamera")
    log("Camara creada")

    # 6. Guardar nivel
    unreal.EditorLevelLibrary.save_current_level()
    log("Escena guardada en /Game/ClinicalScene/ClinicalScene")
    return True

if __name__ == "__main__":
    create_clinical_scene()
