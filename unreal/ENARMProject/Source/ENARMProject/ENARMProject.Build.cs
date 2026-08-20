// ENARMProject.Build.cs - Modulo principal del proyecto
using UnrealBuildTool;

public class ENARMProject : ModuleRules
{
    public ENARMProject(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] {
            "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput",
            "UMG", "Slate", "SlateCore"
        });

        PrivateDependencyModuleNames.AddRange(new string[] {
            "ENARMCoreBridge"
        });
    }
}
