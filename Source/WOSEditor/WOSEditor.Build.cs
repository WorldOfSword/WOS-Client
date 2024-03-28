using UnrealBuildTool;
using System.Collections.Generic;

public class WOSEditor : ModuleRules
{
    public WOSEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UnrealEd", "WOS", "ComponentVisualizers" });

        PrivateDependencyModuleNames.AddRange(new string[] { });
        PublicIncludePaths.AddRange(new string[]
        {
            "WOS/",
        });
        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}