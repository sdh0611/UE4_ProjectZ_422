// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class ProjectZ_422 : ModuleRules
{
	public ProjectZ_422(ReadOnlyTargetRules Target) : base(Target)
	{
        bEnableExceptions = true;

        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UMG",
            "NavigationSystem", "AIModule", "GameplayTasks", "MoviePlayer",
        "OnlineSubsystem", "OnlineSubsystemUtils", "OnlineSubsystemSteam",
            "GameLiftServerSDK", "GameLiftClientSDK" });

		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore", "ProjectZLoadingScreen" ,
            "Networking", "Http", "Json", "JsonUtilities", "Sockets"});

        //DynamicallyLoadedModuleNames.Add("OnlineSubsystemSteam");

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
