// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Rogue : ModuleRules
{
    public Rogue(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore",
                                                            "EnhancedInput", "UMG", "AIModule", "NavigationSystem",
                                                            "Niagara" });
    }
}
