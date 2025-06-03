// Copyright Lim Young.

#pragma once

#include "CoreMinimal.h"
#include "SkeletalMergingLibrary.h"
#include "SkeletalMeshDestructionConfig.generated.h"

UENUM(BlueprintType)
enum class ESkeletalMeshCombineType : uint8
{
	LeaderPose,
	MeshMerge,
};

UENUM(BlueprintType)
enum class ESKMDegradationMode : uint8
{
	Sequence,
	Random
};

USTRUCT(BlueprintType)
struct SKELETALMESHDESTRUCTION_API FSKMDegradationConfig
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Degradation")
	TObjectPtr<USkeletalMesh> OriginalMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Degradation")
	TArray<TObjectPtr<USkeletalMesh>> DegradationMeshes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Degradation")
	ESKMDegradationMode DegradationMode = ESKMDegradationMode::Sequence;

	bool IsValid() const
	{
		return OriginalMesh != nullptr;
	}
};

UENUM(BlueprintType)
enum class ESKMDismembermentMode : uint8
{
	Sequence,
	Random
};

USTRUCT(BlueprintType)
struct SKELETALMESHDESTRUCTION_API FSKMDismembermentConfig
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dismemberment")
	TArray<TObjectPtr<UStaticMesh>> DismembermentMeshes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dismemberment")
	ESKMDismembermentMode DismembermentMode = ESKMDismembermentMode::Sequence;
};

/**
 * 
 */
USTRUCT(BlueprintType)
struct SKELETALMESHDESTRUCTION_API FSkeletalMeshDestructionConfig
{
	GENERATED_BODY()

private:
	FSkeletalMeshMergeParams MergeParams;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletalMeshDestruction")
	FName SkeletalMeshCombineName = FName("CharacterMesh0");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletalMeshDestruction")
	bool bUseDismemberment = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletalMeshDestruction|Dismemberment",
		meta = (EditCondition = "bUseDismemberment", EditConditionHides))
	TMap<FName, FSKMDismembermentConfig> DismembermentConfigs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletalMeshDestruction")
	bool bUseDegradation = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletalMeshDestruction|Degradation",
		meta = (EditCondition = "bUseDegradation", EditConditionHides))
	ESkeletalMeshCombineType SkeletonCombineType = ESkeletalMeshCombineType::LeaderPose;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletalMeshDestruction|Degradation",
		meta = (EditCondition = "bUseDegradation", EditConditionHides))
	TObjectPtr<USkeleton> Skeleton = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletalMeshDestruction|Degradation",
		meta = (EditCondition = "bUseDegradation", EditConditionHides))
	TObjectPtr<USkeletalMesh> LeaderMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletalMeshDestruction|Degradation",
		meta = (EditCondition = "bUseDegradation", EditConditionHides))
	TMap<FName, FSKMDegradationConfig> DegradationConfigs;

	bool IsValid() const
	{
		if (bUseDegradation)
		{
			return Skeleton != nullptr && LeaderMesh != nullptr;
		}
		return true;
	}
};
