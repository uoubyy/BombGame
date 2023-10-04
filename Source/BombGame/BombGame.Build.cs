// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class BombGame : ModuleRules
{
	public BombGame(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "Niagara", "EnhancedInput","UMG", "ApplicationCore", "SlateCore" });

        OptimizeCode = CodeOptimization.Never;
    }
}
