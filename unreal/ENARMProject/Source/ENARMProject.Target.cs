// ENARMProject.Target.cs
using UnrealBuildTool;
using System.Collections.Generic;

public class ENARMProjectTarget : TargetRules
{
    public ENARMProjectTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Game;
        DefaultBuildSettings = BuildSettingsVersion.V7;
        IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_8;
        ExtraModuleNames.Add("ENARMProject");
    }
}
