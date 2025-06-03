// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class prototype : ModuleRules
{
    public prototype(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "ProceduralMeshComponent" });

        PublicIncludePaths.AddRange(new string[] {
            //includeéûÇÃÉpÉXÇÃè»ó™
            "prototype",
            "prototype/Interface",
            });

        PrivateDependencyModuleNames.AddRange(new string[] { });
    }
}
