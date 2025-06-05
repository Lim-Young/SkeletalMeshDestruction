// Copyright Lim Young.

#pragma once

#include "CoreMinimal.h"
#include "SkeletalMeshDestructionConfigDataAssetTypeActions.h"
#include "Modules/ModuleManager.h"

class FSkeletalMeshDestructionEditorModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	TSharedPtr<FSkeletalMeshDestructionConfigDataAssetTypeActions> SkeletalMeshDestructionConfigDataAssetTypeActions;
};
