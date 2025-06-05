// Copyright Lim Young.


#include "SkeletalMeshDestructionConfigDataAssetTypeActions.h"

#include "SkeletalMeshDestructionConfig.h"


UClass* FSkeletalMeshDestructionConfigDataAssetTypeActions::GetSupportedClass() const
{
	return USkeletalMeshDestructionConfigData::StaticClass();
}

FText FSkeletalMeshDestructionConfigDataAssetTypeActions::GetName() const
{
	return INVTEXT("Skeletal Mesh Destruction Config");
}

FColor FSkeletalMeshDestructionConfigDataAssetTypeActions::GetTypeColor() const
{
	return FColor(197, 23, 46);
}

uint32 FSkeletalMeshDestructionConfigDataAssetTypeActions::GetCategories()
{
	return FAssetToolsModule::GetModule().Get().RegisterAdvancedAssetCategory(
		FName(TEXT("SkeletalMeshDestruction")), INVTEXT("Skeletal Mesh Destruction"));
}
