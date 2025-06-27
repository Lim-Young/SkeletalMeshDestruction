// Copyright Lim Young.


#include "SkeletalMeshDestructionComponent.h"

#include "DismembermentLimbActor.h"
#include "SkeletalMeshDestructionSubsystem.h"
#include "SkeletalMergingLibrary.h"
#include "PhysicsEngine/PhysicsAsset.h"


USkeletalMeshDestructionComponent::USkeletalMeshDestructionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USkeletalMeshDestructionComponent::BeginPlay()
{
	Super::BeginPlay();

	SkeletalMeshDestructionSubsystem = GetWorld()->GetSubsystem<USkeletalMeshDestructionSubsystem>();
	check(SkeletalMeshDestructionSubsystem);

	if (bAutoInitialize)
	{
		InitializeSkeletalMesh();
	}
}

void USkeletalMeshDestructionComponent::InitializeSkeletalMesh()
{
	if (!IsValid(SkeletalMeshDestructionConfigData))
	{
		return;
	}

	if (SkeletalMeshDestructionConfigData->SkeletalMeshDestructionConfig.IsValid())
	{
		TArray<USkeletalMeshComponent*> SkeletalMeshComponents;
		GetOwner()->GetComponents(USkeletalMeshComponent::StaticClass(), SkeletalMeshComponents);
		for (USkeletalMeshComponent* SkeletalMeshComponent : SkeletalMeshComponents)
		{
			if (SkeletalMeshComponent->GetName() == DrivenSkeletalMeshComponentName)
			{
				DrivenSkeletalMeshComponent = SkeletalMeshComponent;
				break;
			}
		}

		if (!CheckShouldUseDegradationSystem())
		{
			return;
		}

		InitializeDLODLevel();
		CombineSkeletalMesh();
	}
}

bool USkeletalMeshDestructionComponent::CheckShouldUseDegradationSystem() const
{
	if (!IsValid(DrivenSkeletalMeshComponent))
	{
		return false;
	}

	if (!SkeletalMeshDestructionConfigData->SkeletalMeshDestructionConfig.IsValid())
	{
		return false;
	}

	if (!SkeletalMeshDestructionConfigData->SkeletalMeshDestructionConfig.bUseDLOD)
	{
		return false;
	}
	return true;
}

bool USkeletalMeshDestructionComponent::CheckBoneDLODConfigValid(const FName& BoneName) const
{
	if (!SkeletalMeshDestructionConfigData->SkeletalMeshDestructionConfig.DegradationConfigs.Contains(BoneName))
	{
		return false;
	}

	if (!SkeletalMeshDestructionConfigData->SkeletalMeshDestructionConfig.DegradationConfigs[BoneName].Valid())
	{
		return false;
	}

	return true;
}

bool USkeletalMeshDestructionComponent::CheckBoneDLODLevelValid(const FName& BoneName, const uint8 DLODLevel) const
{
	if (!CurrentDLODLevels.Contains(BoneName))
	{
		return false;
	}

	// DLOD level must be within the valid range
	if (!SkeletalMeshDestructionConfigData->SkeletalMeshDestructionConfig.DegradationConfigs[BoneName].DLODMeshes.
		IsValidIndex(DLODLevel))
	{
		return false;
	}
	return true;
}

void USkeletalMeshDestructionComponent::InitializeDLODLevel()
{
	for (auto DegradationConfig : SkeletalMeshDestructionConfigData->SkeletalMeshDestructionConfig.DegradationConfigs)
	{
		CurrentDLODLevels.FindOrAdd(DegradationConfig.Key, 0);
	}
}

void USkeletalMeshDestructionComponent::CombineSkeletalMesh()
{
	switch (SkeletalMeshDestructionConfigData->SkeletalMeshDestructionConfig.SkeletonCombineType)
	{
	case ESkeletalMeshCombineType::LeaderPose:
		{
			DrivenSkeletalMeshComponent->SetSkeletalMeshAsset(
				SkeletalMeshDestructionConfigData->SkeletalMeshDestructionConfig.LeaderMesh);

			for (auto DegradationConfig : SkeletalMeshDestructionConfigData->SkeletalMeshDestructionConfig.
			                                                                 DegradationConfigs)
			{
				if (DegradationConfig.Key == NAME_None)
				{
					continue;
				}

				if (!DegradationConfig.Value.Valid())
				{
					continue;
				}

				// Create a new SkeletalMeshComponent for each degradation config
				USkeletalMeshComponent* DegradationComponent = NewObject<USkeletalMeshComponent>(
					GetOwner(), USkeletalMeshComponent::StaticClass(),
					FName(DrivenSkeletalMeshComponentName.ToString() + TEXT("_") + DegradationConfig.Key.ToString()));

				if (!IsValid(DegradationComponent))
				{
					continue;
				}

				const uint8 DLODLevel = CurrentDLODLevels[DegradationConfig.Key];
				DegradationComponent->SetSkeletalMeshAsset(DegradationConfig.Value.DLODMeshes[DLODLevel]);
				DegradationComponent->AttachToComponent(DrivenSkeletalMeshComponent,
				                                        FAttachmentTransformRules::SnapToTargetIncludingScale);
				DegradationComponent->SetLeaderPoseComponent(DrivenSkeletalMeshComponent);
			}
			break;
		}
	case ESkeletalMeshCombineType::MeshMerge:
		{
			FSkeletalMeshMergeParams MergeParams;
			MergeParams.bNeedsCpuAccess = true;
			MergeParams.Skeleton = SkeletalMeshDestructionConfigData->SkeletalMeshDestructionConfig.Skeleton;
			MergeParams.MeshesToMerge.Add(SkeletalMeshDestructionConfigData->SkeletalMeshDestructionConfig.LeaderMesh);

			for (auto DegradationConfig : SkeletalMeshDestructionConfigData->SkeletalMeshDestructionConfig.
			                                                                 DegradationConfigs)
			{
				if (DegradationConfig.Key == NAME_None)
				{
					continue;
				}

				if (!DegradationConfig.Value.Valid())
				{
					continue;
				}

				const uint8 DLODLevel = CurrentDLODLevels[DegradationConfig.Key];
				MergeParams.MeshesToMerge.Add(DegradationConfig.Value.DLODMeshes[DLODLevel]);
			}

			const TObjectPtr<USkeletalMesh> CombinedSkeletalMesh = USkeletalMergingLibrary::MergeMeshes(MergeParams);
			DrivenSkeletalMeshComponent->SetSkeletalMeshAsset(CombinedSkeletalMesh);

			if (IsValid(SkeletalMeshDestructionConfigData->SkeletalMeshDestructionConfig.PhysicsAsset))
			{
				DrivenSkeletalMeshComponent->SetPhysicsAsset(
					SkeletalMeshDestructionConfigData->SkeletalMeshDestructionConfig.PhysicsAsset);
			}
			break;
		}
	default:
		checkNoEntry();
		break;
	}
}

bool USkeletalMeshDestructionComponent::SetDLODByBoneName(const FName& BoneName, const uint8 DLODLevel)
{
	if (!CheckShouldUseDegradationSystem())
	{
		return false;
	}

	if (!CheckBoneDLODConfigValid(BoneName))
	{
		return false;
	}

	if (!CheckBoneDLODLevelValid(BoneName, DLODLevel))
	{
		return false;
	}

	CurrentDLODLevels[BoneName] = DLODLevel;
	CombineSkeletalMesh();

	return true;
}

bool USkeletalMeshDestructionComponent::DegradeSkeletalMesh(const FName& BoneName)
{
	if (!CheckShouldUseDegradationSystem())
	{
		return false;
	}

	if (!CheckBoneDLODConfigValid(BoneName))
	{
		return false;
	}

	switch (SkeletalMeshDestructionConfigData->SkeletalMeshDestructionConfig.DegradationConfigs[BoneName].
		DegradationMode)
	{
	case ESKMDegradationMode::Sequence:
		CurrentDLODLevels[BoneName]++;
		if (CurrentDLODLevels[BoneName] >= SkeletalMeshDestructionConfigData->SkeletalMeshDestructionConfig.
		                                                                      DegradationConfigs[BoneName].DLODMeshes.
			Num())
		{
			// Clamp the DLOD level to the maximum
			CurrentDLODLevels[BoneName]--;
			return false;
		}
	case ESKMDegradationMode::Random:
		CurrentDLODLevels[BoneName] = FMath::RandRange(
			0, SkeletalMeshDestructionConfigData->SkeletalMeshDestructionConfig.DegradationConfigs[BoneName].DLODMeshes.
			Num() - 1);
		break;
	default:
		checkNoEntry();
		return false;
	}

	if (!CheckBoneDLODLevelValid(BoneName, CurrentDLODLevels[BoneName]))
	{
		return false;
	}

	CombineSkeletalMesh();
	return true;
}

bool USkeletalMeshDestructionComponent::ApplyDismemberment(const FName& BoneName,
                                                           const FDismembermentConfig& DismembermentConfig)
{
	if (!IsValid(DrivenSkeletalMeshComponent))
	{
		return false;
	}

	if (!IsValid(DrivenSkeletalMeshComponent->GetSkeletalMeshAsset()))
	{
		return false;
	}

	if (!SkeletalMeshDestructionConfigData->SkeletalMeshDestructionConfig.bUseDismemberment)
	{
		return false;
	}

	if (!SkeletalMeshDestructionConfigData->SkeletalMeshDestructionConfig.DismembermentConfigs.Contains(BoneName))
	{
		return false;
	}

	if (!SkeletalMeshDestructionConfigData->SkeletalMeshDestructionConfig.DismembermentConfigs[BoneName].Valid())
	{
		return false;
	}

	FTransform BoneTransform = DrivenSkeletalMeshComponent->GetBoneTransform(BoneName);
	DrivenSkeletalMeshComponent->HideBoneByName(BoneName, PBO_Term);

	ADismembermentLimbActor* LimbActor = SkeletalMeshDestructionSubsystem->GetDismembermentLimbActor(BoneTransform);
	FVector FinalImpactDirection;
	switch (DismembermentConfig.LimbSpace)
	{
	case ELimbSpace::WorldSpace:
		FinalImpactDirection = DismembermentConfig.ImpactDirection;
		break;
	case ELimbSpace::BoneSpace:
		FinalImpactDirection = BoneTransform.TransformVector(DismembermentConfig.ImpactDirection);
		break;
	default:
		checkNoEntry();
		break;
	}

	if (DismembermentConfig.bUseWorldScale)
	{
		BoneTransform.SetScale3D(DrivenSkeletalMeshComponent->GetComponentScale());
	}

	LimbActor->ReInitializeLimb(BoneTransform,
	                            SkeletalMeshDestructionConfigData->SkeletalMeshDestructionConfig.DismembermentConfigs[
		                            BoneName].
	                            GetRandomDismembermentMesh(),
	                            SkeletalMeshDestructionConfigData->SkeletalMeshDestructionConfig.
	                                                               DismembermentLimbCollisionProfile.Name);
	LimbActor->AddImpulse(FinalImpactDirection, DismembermentConfig.ImpactForce,
	                      DismembermentConfig.AngularImpulseDegrees);
	return true;
}

bool USkeletalMeshDestructionComponent::IsUseDLOD() const
{
	return SkeletalMeshDestructionConfigData->SkeletalMeshDestructionConfig.bUseDLOD;
}

bool USkeletalMeshDestructionComponent::IsUseDismemberment() const
{
	return SkeletalMeshDestructionConfigData->SkeletalMeshDestructionConfig.bUseDismemberment;
}
