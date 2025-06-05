// Copyright Lim Young.


#include "SkeletalMeshDestructionEditor.h"

#define LOCTEXT_NAMESPACE "FSkeletalMeshDestructionEditorModule"

void FSkeletalMeshDestructionEditorModule::StartupModule()
{
	SkeletalMeshDestructionConfigDataAssetTypeActions = MakeShared<
		FSkeletalMeshDestructionConfigDataAssetTypeActions>();
	FAssetToolsModule::GetModule().Get().RegisterAssetTypeActions(
		SkeletalMeshDestructionConfigDataAssetTypeActions.ToSharedRef());
}

void FSkeletalMeshDestructionEditorModule::ShutdownModule()
{
	if (!FModuleManager::Get().IsModuleLoaded("AssetTools"))
	{
		return;
	}

	FAssetToolsModule::GetModule().Get().UnregisterAssetTypeActions(
		SkeletalMeshDestructionConfigDataAssetTypeActions.ToSharedRef());
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FSkeletalMeshDestructionEditorModule, SkeletalMeshDestructionEditor)
