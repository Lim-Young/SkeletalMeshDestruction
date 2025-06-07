// Copyright Lim Young.

#pragma once

#include "CoreMinimal.h"
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

	/**
	 * Degradation Level of Detail (LOD) meshes.
	 * DLOD0 is the original mesh, DLOD1 is the first degradation mesh, and so on.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Degradation")
	TArray<TObjectPtr<USkeletalMesh>> DLODMeshes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Degradation")
	ESKMDegradationMode DegradationMode = ESKMDegradationMode::Sequence;

	bool Valid() const
	{
		if (DLODMeshes.Num() <= 0)
		{
			return false;
		}

		bool bValid = true;
		for (auto Mesh : DLODMeshes)
		{
			if (!IsValid(Mesh))
			{
				bValid = false;
				break;
			}
		}

		return bValid;
	}
};

USTRUCT(BlueprintType)
struct SKELETALMESHDESTRUCTION_API FSKMDismembermentConfig
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dismemberment")
	TArray<TObjectPtr<UStaticMesh>> DismembermentMeshes;

	bool Valid() const
	{
		if (DismembermentMeshes.Num() <= 0)
		{
			return false;
		}

		bool bValid = true;
		for (auto Mesh : DismembermentMeshes)
		{
			if (!IsValid(Mesh))
			{
				bValid = false;
				break;
			}
		}

		return bValid;
	}

	UStaticMesh* GetRandomDismembermentMesh() const
	{
		if (DismembermentMeshes.Num() <= 0)
		{
			return nullptr;
		}

		const int32 RandomIndex = FMath::RandRange(0, DismembermentMeshes.Num() - 1);
		return DismembermentMeshes[RandomIndex];
	}
};

/**
 * 
 */
USTRUCT(BlueprintType)
struct SKELETALMESHDESTRUCTION_API FSkeletalMeshDestructionConfig
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletalMeshDestruction")
	bool bUseDismemberment = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletalMeshDestruction|Dismemberment",
		meta = (EditCondition = "bUseDismemberment", EditConditionHides))
	TMap<FName, FSKMDismembermentConfig> DismembermentConfigs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletalMeshDestruction|Dismemberment",
		meta = (EditCondition = "bUseDismemberment", EditConditionHides))
	FCollisionProfileName DismembermentLimbCollisionProfile = FCollisionProfileName(TEXT("DismembermentLimb"));

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletalMeshDestruction")
	bool bUseDLOD = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletalMeshDestruction|Degradation",
		meta = (EditCondition = "bUseDLOD", EditConditionHides))
	ESkeletalMeshCombineType SkeletonCombineType = ESkeletalMeshCombineType::MeshMerge;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletalMeshDestruction|Degradation",
		meta = (EditCondition = "bUseDLOD", EditConditionHides))
	TObjectPtr<USkeleton> Skeleton = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletalMeshDestruction|Degradation",
		meta = (EditCondition = "bUseDLOD", EditConditionHides))
	TObjectPtr<USkeletalMesh> LeaderMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletalMeshDestruction|Degradation",
		meta = (EditCondition = "bUseDLOD", EditConditionHides))
	TMap<FName, FSKMDegradationConfig> DegradationConfigs;

	bool IsValid() const
	{
		if (bUseDLOD)
		{
			return Skeleton != nullptr && LeaderMesh != nullptr;
		}
		return true;
	}
};

UCLASS(BlueprintType)
class SKELETALMESHDESTRUCTION_API USkeletalMeshDestructionConfigData : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletalMeshDestruction")
	FSkeletalMeshDestructionConfig SkeletalMeshDestructionConfig;
};
