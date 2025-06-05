// Copyright Lim Young.


#include "AnimNotify/AnimNotify_ApplySkeletalMeshDestruction.h"

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
