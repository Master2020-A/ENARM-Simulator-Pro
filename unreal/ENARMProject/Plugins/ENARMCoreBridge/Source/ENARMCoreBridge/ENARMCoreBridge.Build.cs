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

        // ─── ENARM Core (librerias estaticas C++ puras) ───
        string EnarmInclude = Path.GetFullPath(Path.Combine(ModuleDirectory, "ThirdParty/ENARM/include"));
        string EnarmLib     = Path.GetFullPath(Path.Combine(ModuleDirectory, "ThirdParty/ENARM/lib"));

        PublicIncludePaths.Add(EnarmInclude);

        // ─── vcpkg (cpr, nlohmann-json, pqxx, spdlog, fmt) ───
        string VcpkgRoot = Path.GetFullPath(Path.Combine(ModuleDirectory, "../../../../../../vcpkg_installed/x64-windows"));
        string VcpkgInclude = Path.Combine(VcpkgRoot, "include");
        string VcpkgLib     = Path.Combine(VcpkgRoot, "lib");

        if (Directory.Exists(VcpkgInclude))
        {
            PublicIncludePaths.Add(VcpkgInclude);
        }

        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            // Modulos ENARM puros
            PublicAdditionalLibraries.Add(Path.Combine(EnarmLib, "ENARM.Common.lib"));
            PublicAdditionalLibraries.Add(Path.Combine(EnarmLib, "ENARM.Domain.lib"));
            PublicAdditionalLibraries.Add(Path.Combine(EnarmLib, "ENARM.Physiology.lib"));
            PublicAdditionalLibraries.Add(Path.Combine(EnarmLib, "ENARM.Pharmacology.lib"));
            PublicAdditionalLibraries.Add(Path.Combine(EnarmLib, "ENARM.Simulation.lib"));
            PublicAdditionalLibraries.Add(Path.Combine(EnarmLib, "ENARM.AI.lib"));
            PublicAdditionalLibraries.Add(Path.Combine(EnarmLib, "ENARM.Data.lib"));

            // vcpkg libs
            if (Directory.Exists(VcpkgLib))
            {
                PublicAdditionalLibraries.Add(Path.Combine(VcpkgLib, "cpr.lib"));
                PublicAdditionalLibraries.Add(Path.Combine(VcpkgLib, "pqxx.lib"));
                PublicAdditionalLibraries.Add(Path.Combine(VcpkgLib, "libpq.lib"));
                PublicAdditionalLibraries.Add(Path.Combine(VcpkgLib, "spdlog.lib"));
                PublicAdditionalLibraries.Add(Path.Combine(VcpkgLib, "fmt.lib"));
                PublicAdditionalLibraries.Add(Path.Combine(VcpkgLib, "libcurl.lib"));
                PublicAdditionalLibraries.Add(Path.Combine(VcpkgLib, "z.lib"));
                PublicAdditionalLibraries.Add(Path.Combine(VcpkgLib, "libssl.lib"));
                PublicAdditionalLibraries.Add(Path.Combine(VcpkgLib, "libcrypto.lib"));
            }
        }
    }
}
