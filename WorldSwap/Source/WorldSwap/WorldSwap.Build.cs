// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class WorldSwap : ModuleRules
{
	public WorldSwap(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"WorldSwap",
			"WorldSwap/Variant_Platforming",
			"WorldSwap/Variant_Platforming/Animation",
			"WorldSwap/Variant_Combat",
			"WorldSwap/Variant_Combat/AI",
			"WorldSwap/Variant_Combat/Animation",
			"WorldSwap/Variant_Combat/Gameplay",
			"WorldSwap/Variant_Combat/Interfaces",
			"WorldSwap/Variant_Combat/UI",
			"WorldSwap/Variant_SideScrolling",
			"WorldSwap/Variant_SideScrolling/AI",
			"WorldSwap/Variant_SideScrolling/Gameplay",
			"WorldSwap/Variant_SideScrolling/Interfaces",
			"WorldSwap/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
