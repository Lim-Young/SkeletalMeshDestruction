// Copyright Lim Young.


#include "SkeletalMeshDestructionComponent.h"


// Sets default values for this component's properties
USkeletalMeshDestructionComponent::USkeletalMeshDestructionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void USkeletalMeshDestructionComponent::BeginPlay()
{
	Super::BeginPlay();

	SkeletalMeshDestructionSubsystem = GetWorld()->GetSubsystem<USkeletalMeshDestructionSubsystem>();
	check(SkeletalMeshDestructionSubsystem);
}

void USkeletalMeshDestructionComponent::InitializeSkeletalMesh()
{
	if (SkeletalMeshDestructionConfig.IsValid())
	{
		TArray<USkeletalMeshComponent*> SkeletalMeshComponents;
		GetOwner()->GetComponents(USkeletalMeshComponent::StaticClass(), SkeletalMeshComponents);
		for (USkeletalMeshComponent* SkeletalMeshComponent : SkeletalMeshComponents)
		{
			if (SkeletalMeshComponent->GetName() == SkeletalMeshDestructionConfig.SkeletalMeshCombineName)
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

	if (!SkeletalMeshDestructionConfig.IsValid())
	{
		return false;
	}

	if (!SkeletalMeshDestructionConfig.bUseDegradation)
	{
		return false;
	}
	return true;
}

bool USkeletalMeshDestructionComponent::CheckBoneDLODConfigValid(const FName& BoneName) const
{
	if (!SkeletalMeshDestructionConfig.DegradationConfigs.Contains(BoneName))
	{
		return false;
	}

	if (!SkeletalMeshDestructionConfig.DegradationConfigs[BoneName].Valid())
	{
		return false;
	}

	return true;
}

bool USkeletalMeshDestructionComponent::CheckBoneDLODLevelValid(const FName& BoneName, const uint8 DLODLevel)
{
	if (!CurrentDLODLevels.Contains(BoneName))
	{
		return false;
	}

	// DLOD level must be within the valid range
	if (!SkeletalMeshDestructionConfig.DegradationConfigs[BoneName].DLODMeshes.IsValidIndex(DLODLevel))
	{
		return false;
	}
	return true;
}

void USkeletalMeshDestructionComponent::InitializeDLODLevel()
{
	for (auto DegradationConfig : SkeletalMeshDestructionConfig.DegradationConfigs)
	{
		CurrentDLODLevels.FindOrAdd(DegradationConfig.Key, 0);
	}
}

void USkeletalMeshDestructionComponent::CombineSkeletalMesh()
{
	// if (!IsValid(DrivenSkeletalMeshComponent))
	// {
	// 	return;
	// }
	//
	// if (!SkeletalMeshDestructionConfig.IsValid())
	// {
	// 	return;
	// }
	//
	// if (!SkeletalMeshDestructionConfig.bUseDegradation)
	// {
	// 	return;
	// }

	switch (SkeletalMeshDestructionConfig.SkeletonCombineType)
	{
	case ESkeletalMeshCombineType::LeaderPose:
		{
			DrivenSkeletalMeshComponent->SetSkeletalMeshAsset(SkeletalMeshDestructionConfig.LeaderMesh);

			for (auto DegradationConfig : SkeletalMeshDestructionConfig.DegradationConfigs)
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
					FName(SkeletalMeshDestructionConfig.SkeletalMeshCombineName.ToString() + TEXT("_") +
						DegradationConfig.Key.ToString()));

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
			MergeParams.Skeleton = SkeletalMeshDestructionConfig.Skeleton;
			MergeParams.MeshesToMerge.Add(SkeletalMeshDestructionConfig.LeaderMesh);

			for (auto DegradationConfig : SkeletalMeshDestructionConfig.DegradationConfigs)
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

	switch (SkeletalMeshDestructionConfig.DegradationConfigs[BoneName].DegradationMode)
	{
	case ESKMDegradationMode::Sequence:
		CurrentDLODLevels[BoneName]++;
		if (CurrentDLODLevels[BoneName] >= SkeletalMeshDestructionConfig.DegradationConfigs[BoneName].DLODMeshes.Num())
		{
			// Clamp the DLOD level to the maximum
			CurrentDLODLevels[BoneName]--;
			return false;
		}
	case ESKMDegradationMode::Random:
		CurrentDLODLevels[BoneName] = FMath::RandRange(
			0, SkeletalMeshDestructionConfig.DegradationConfigs[BoneName].DLODMeshes.Num() - 1);
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
