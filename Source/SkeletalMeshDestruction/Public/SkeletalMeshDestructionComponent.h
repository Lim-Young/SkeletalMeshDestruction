// Copyright Lim Young.

#pragma once

#include "CoreMinimal.h"
#include "SkeletalMeshDestructionConfig.h"
#include "SkeletalMeshDestructionSubsystem.h"
#include "Components/ActorComponent.h"
#include "SkeletalMeshDestructionComponent.generated.h"

UENUM(BlueprintType)
enum class ELimbSpace : uint8
{
	WorldSpace,
	BoneSpace
};

USTRUCT(BlueprintType)
struct FDismembermentConfig
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dismemberment")
	FVector ImpactDirection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dismemberment")
	float ImpactForce;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dismemberment")
	FVector AngularImpulseDegrees;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dismemberment")
	ELimbSpace LimbSpace = ELimbSpace::BoneSpace;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dismemberment")
	bool bUseWorldScale = true;
};

UCLASS(ClassGroup=(SkeletalMeshDestruction), meta=(BlueprintSpawnableComponent))
class SKELETALMESHDESTRUCTION_API USkeletalMeshDestructionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USkeletalMeshDestructionComponent();
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category="SkeletalMeshDestruction")
	void InitializeSkeletalMesh();

protected:
	bool CheckShouldUseDegradationSystem() const;
	bool CheckBoneDLODConfigValid(const FName& BoneName) const;
	bool CheckBoneDLODLevelValid(const FName& BoneName, uint8 DLODLevel) const;

	void InitializeDLODLevel();
	void CombineSkeletalMesh();

private:
	UPROPERTY()
	TObjectPtr<USkeletalMeshComponent> DrivenSkeletalMeshComponent;

	TMap<FName, uint8> CurrentDLODLevels;

	UPROPERTY(Transient)
	TObjectPtr<USkeletalMeshDestructionSubsystem> SkeletalMeshDestructionSubsystem;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletalMeshDestruction")
	FName DrivenSkeletalMeshComponentName = FName("CharacterMesh0");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SkeletalMeshDestruction",
		DisplayName = "SKM Destruction Config Data")
	TObjectPtr<USkeletalMeshDestructionConfigData> SkeletalMeshDestructionConfigData;

public:
	UFUNCTION(BlueprintCallable, Category="SkeletalMeshDestruction|DLOD", meta = (AutoCreateRefTerm = "BoneName"))
	bool SetDLODByBoneName(const FName& BoneName, const uint8 DLODLevel);

	UFUNCTION(BlueprintCallable, Category="SkeletalMeshDestruction|DLOD", meta = (AutoCreateRefTerm = "BoneName"))
	bool DegradeSkeletalMesh(const FName& BoneName);

	UFUNCTION(BlueprintCallable, Category="SkeletalMeshDestruction|Dismemberment",
		meta = (AutoCreateRefTerm = "BoneName, DismembermentConfig"))
	bool ApplyDismemberment(const FName& BoneName, const FDismembermentConfig& DismembermentConfig);

public:
	UFUNCTION(BlueprintPure, Category="SkeletalMeshDestruction|DLOD")
	bool IsUseDLOD() const;

	UFUNCTION(BlueprintPure, Category="SkeletalMeshDestruction|Dismemberment")
	bool IsUseDismemberment() const;
};
