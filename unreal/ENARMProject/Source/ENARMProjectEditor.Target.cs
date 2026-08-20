// ENARMProjectEditor.Target.cs
using UnrealBuildTool;
using System.Collections.Generic;

public class ENARMProjectEditorTarget : TargetRules
{
    public ENARMProjectEditorTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Editor;
        DefaultBuildSettings = BuildSettingsVersion.V7;
        IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_8;
        ExtraModuleNames.Add("ENARMProject");
    }
}
