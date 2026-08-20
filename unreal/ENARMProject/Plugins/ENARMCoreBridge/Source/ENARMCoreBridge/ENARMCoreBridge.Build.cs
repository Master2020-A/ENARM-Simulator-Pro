// ENARMCoreBridge.Build.cs
using UnrealBuildTool;
using System.IO;

public class ENARMCoreBridge : ModuleRules
{
    public ENARMCoreBridge(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] {
            "Core", "CoreUObject", "Engine"
        });

        // ─── ENARM Core (librerias estaticas C++ puras, sin vcpkg) ───
        string EnarmInclude = Path.GetFullPath(Path.Combine(ModuleDirectory, "ThirdParty/ENARM/include"));
        string EnarmLib     = Path.GetFullPath(Path.Combine(ModuleDirectory, "ThirdParty/ENARM/lib"));

        PublicIncludePaths.Add(EnarmInclude);

        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            // Solo modulos sin dependencias externas (vcpkg)
            PublicAdditionalLibraries.Add(Path.Combine(EnarmLib, "ENARM.Physiology.lib"));
        }
    }
}
