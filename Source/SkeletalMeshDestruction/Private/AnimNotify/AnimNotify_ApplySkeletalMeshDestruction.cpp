// Copyright Lim Young.


#include "AnimNotify/AnimNotify_ApplySkeletalMeshDestruction.h"

#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

UAnimNotify_ApplySkeletalMeshDestruction::UAnimNotify_ApplySkeletalMeshDestruction()
{
#if WITH_EDITORONLY_DATA
	NotifyColor = FColor(197, 23, 46);
#endif
}

void UAnimNotify_ApplySkeletalMeshDestruction::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                                      const FAnimNotifyEventReference& EventReference)
{
	const AActor* Owner = MeshComp->GetOwner();
	if (!IsValid(Owner))
	{
		return;
	}

	USkeletalMeshDestructionComponent* DestructionComponent = Owner->FindComponentByClass<
		USkeletalMeshDestructionComponent>();
	if (!IsValid(DestructionComponent))
	{
		return;
	}

	if (bApplyDLOD && ApplyDLODConfigs.Num() > 0 && DestructionComponent->IsUseDLOD())
	{
		for (auto ApplyDLODConfig : ApplyDLODConfigs)
		{
			DestructionComponent->SetDLODByBoneName(ApplyDLODConfig.Key, ApplyDLODConfig.Value);
		}
	}

	if (bApplyDismemberment && ApplyDismembermentConfigs.Num() && DestructionComponent->IsUseDismemberment())
	{
		for (auto ApplyDismembermentConfig : ApplyDismembermentConfigs)
		{
			DestructionComponent->ApplyDismemberment(ApplyDismembermentConfig.Key, ApplyDismembermentConfig.Value);
		}
	}
}

#if WITH_EDITOR
EDataValidationResult UAnimNotify_ApplySkeletalMeshDestruction::IsDataValid(FDataValidationContext& Context) const
{
	EDataValidationResult Result = Super::IsDataValid(Context);

	if (bApplyDLOD && ApplyDLODConfigs.Num() == 0)
	{
		Context.AddError(
			NSLOCTEXT("AnimNotify_ApplySkeletalMeshDestruction", "DLODConfigsEmpty",
			          "DLODConfigs is empty. Please add at least one DLOD config."));
		Result = EDataValidationResult::Invalid;
	}

	if (bApplyDismemberment && ApplyDismembermentConfigs.Num() == 0)
	{
		Context.AddError(
			NSLOCTEXT("AnimNotify_ApplySkeletalMeshDestruction", "DismembermentConfigsEmpty",
			          "DismembermentConfigs is empty. Please add at least one Dismemberment config."));
		Result = EDataValidationResult::Invalid;
	}

	return Result;
}

FString UAnimNotify_ApplySkeletalMeshDestruction::GetNotifyName_Implementation() const
{
	if (bApplyDismemberment && bApplyDLOD)
	{
		return FString::Printf(TEXT("Apply DLOD and Dismemberment"));
	}
	if (bApplyDismemberment)
	{
		return FString::Printf(TEXT("Apply Dismemberment"));
	}
	if (bApplyDLOD)
	{
		return FString::Printf(TEXT("Apply DLOD"));
	}

	return FString::Printf(TEXT("Apply Nothing"));
}
#endif
