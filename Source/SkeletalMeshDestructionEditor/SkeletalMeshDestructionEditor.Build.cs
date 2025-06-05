using UnrealBuildTool;

public class SkeletalMeshDestructionEditor : ModuleRules
{
	public SkeletalMeshDestructionEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
			}
		);

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"SkeletalMeshDestruction",
				"UnrealEd",
				"AssetTools"
			}
		);
	}
}