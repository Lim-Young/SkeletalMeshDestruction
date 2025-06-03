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


// Called when the game starts
void USkeletalMeshDestructionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}


// Called every frame
void USkeletalMeshDestructionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                      FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
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

		CombineSkeletalMesh();
	}
}

void USkeletalMeshDestructionComponent::CombineSkeletalMesh()
{
	if (!IsValid(DrivenSkeletalMeshComponent))
	{
		return;
	}

	if (!SkeletalMeshDestructionConfig.IsValid())
	{
		return;
	}

	if (!SkeletalMeshDestructionConfig.bUseDegradation)
	{
		return;
	}

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

				if (!DegradationConfig.Value.IsValid())
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

				DegradationComponent->SetSkeletalMeshAsset(DegradationConfig.Value.OriginalMesh);
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

				if (!DegradationConfig.Value.IsValid())
				{
					continue;
				}

				MergeParams.MeshesToMerge.Add(DegradationConfig.Value.OriginalMesh);
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
