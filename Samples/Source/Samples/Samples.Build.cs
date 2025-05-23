// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Samples : ModuleRules
{
	public Samples(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] {
            "Core",
            "CoreUObject",
            "Engine",
            // GAS
            "GameplayTags",
            "GameplayTasks",
            "GameplayAbilities",
            // Game Features
            "ModularGameplay",
            "GameFeatures",
            "ModularGameplayActors",
			//Input
            "InputCore",
            "EnhancedInput",
			// CommonUser
			"CommonUser",
			// CommonGame
			"CommonGame",
			// CommonUI
			"CommonUI",
			// UMG
			"UMG",
			// UIExtension
			"UIExtension",
			// Slate
			"Slate",
			"SlateCore",
        });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
