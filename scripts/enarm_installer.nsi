; ENARM Simulator Pro - Instalador NSIS
; Genera instalador.exe con ENARM.exe, CLI, DLLs y docs

!define APP_NAME "ENARM Simulator Pro"
!define APP_VERSION "0.1.0"
!define APP_PUBLISHER "ENARM"
!define APP_EXE "ENARM.exe"
!define BUILD_DIR "C:\Users\Master\Documents\ENARM-IA\ENARMSimulator\build-vs\bin\Release"

Name "${APP_NAME}"
OutFile "C:\Users\Master\Documents\ENARM-IA\ENARMSimulator\build-vs\ENARM-Simulator-Pro-${APP_VERSION}-setup.exe"
InstallDir "$PROGRAMFILES64\ENARM-Simulator-Pro"
RequestExecutionLevel admin

Page directory
Page instfiles
UninstPage uninstConfirm
UninstPage instfiles

Section "ENARM Simulator Pro"
    SetOutPath "$INSTDIR"
    
    ; Binarios principales
    File "${BUILD_DIR}\ENARM.exe"
    File "${BUILD_DIR}\ENARM.CLI.exe"
    
    ; DLLs de dependencias (vcpkg)
    File /r "${BUILD_DIR}\*.dll"
    
    ; Atajo en el menu de inicio
    CreateDirectory "$SMPROGRAMS\ENARM Simulator Pro"
    CreateShortcut "$SMPROGRAMS\ENARM Simulator Pro\ENARM Simulator Pro.lnk" "$INSTDIR\${APP_EXE}"
    CreateShortcut "$SMPROGRAMS\ENARM Simulator Pro\CLI.lnk" "$INSTDIR\ENARM.CLI.exe"
    
    ; Desinstalador
    WriteUninstaller "$INSTDIR\uninstall.exe"
SectionEnd

Section "Uninstall"
    Delete "$INSTDIR\uninstall.exe"
    Delete "$INSTDIR\${APP_EXE}"
    Delete "$INSTDIR\ENARM.CLI.exe"
    Delete "$INSTDIR\*.dll"
    RMDir "$INSTDIR"
    Delete "$SMPROGRAMS\ENARM Simulator Pro\ENARM Simulator Pro.lnk"
    Delete "$SMPROGRAMS\ENARM Simulator Pro\CLI.lnk"
    RMDir "$SMPROGRAMS\ENARM Simulator Pro"
SectionEnd
