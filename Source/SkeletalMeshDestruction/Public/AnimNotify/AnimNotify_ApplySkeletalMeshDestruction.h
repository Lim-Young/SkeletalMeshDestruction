// Copyright Lim Young.

#pragma once

#include "CoreMinimal.h"
#include "SkeletalMeshDestructionComponent.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_ApplySkeletalMeshDestruction.generated.h"

/**
 * 
 */
UCLASS(DisplayName = "Apply SKM Destruction")
class SKELETALMESHDESTRUCTION_API UAnimNotify_ApplySkeletalMeshDestruction : public UAnimNotify
{
	GENERATED_BODY()

	UAnimNotify_ApplySkeletalMeshDestruction();

protected:
	UPROPERTY(EditAnywhere, Category = "Skeletal Mesh Destruction|DLOD")
	bool bApplyDLOD = false;

	UPROPERTY(EditAnywhere, Category = "Skeletal Mesh Destruction|DLOD",
		meta = (EditCondition = "bApplyDLOD", EditConditionHides, ForceInlineRow))
	TMap<FName, uint8> ApplyDLODConfigs;

	UPROPERTY(EditAnywhere, Category = "Skeletal Mesh Destruction|Dismemberment")
	bool bApplyDismemberment = false;

	UPROPERTY(EditAnywhere, Category = "Skeletal Mesh Destruction|Dismemberment",
		meta = (EditCondition = "bApplyDismemberment", EditConditionHides, ForceInlineRow))
	TMap<FName, FDismembermentConfig> ApplyDismembermentConfigs;

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	                    const FAnimNotifyEventReference& EventReference) override;

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(FDataValidationContext& Context) const override;
	virtual FString GetNotifyName_Implementation() const override;
#endif
};
