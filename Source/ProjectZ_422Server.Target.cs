// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class ProjectZ_422ServerTarget : TargetRules
{
	public ProjectZ_422ServerTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Server;

		ExtraModuleNames.AddRange( new string[] { "ProjectZ_422", "ProjectZLoadingScreen" } );
	}

    //public override bool GetSupportedPlatforms(ref List<UnrealTargetPlatform> OutPlatforms)
    //{
    //    // It is valid for only server platforms
    //    return UnrealBuildTool.UnrealBuildTool.GetAllServerPlatforms(ref OutPlatforms, false);
    //}


}
