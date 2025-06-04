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
	bool CheckBoneDLODLevelValid(const FName& BoneName, uint8 DLODLevel);

	void InitializeDLODLevel();
	void CombineSkeletalMesh();

private:
	UPROPERTY()
	TObjectPtr<USkeletalMeshComponent> DrivenSkeletalMeshComponent;

	TMap<FName, uint8> CurrentDLODLevels;

	UPROPERTY(Transient)
	TObjectPtr<USkeletalMeshDestructionSubsystem> SkeletalMeshDestructionSubsystem;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SkeletalMeshDestruction")
	FSkeletalMeshDestructionConfig SkeletalMeshDestructionConfig;

public:
	UFUNCTION(BlueprintCallable, Category="SkeletalMeshDestruction|DLOD", meta = (AutoCreateRefTerm = "BoneName"))
	bool SetDLODByBoneName(const FName& BoneName, const uint8 DLODLevel);

	UFUNCTION(BlueprintCallable, Category="SkeletalMeshDestruction|DLOD", meta = (AutoCreateRefTerm = "BoneName"))
	bool DegradeSkeletalMesh(const FName& BoneName);

	UFUNCTION(BlueprintCallable, Category="SkeletalMeshDestruction|Dismemberment",
		meta = (AutoCreateRefTerm = "BoneName, ImpactDirection, AngularImpulseDegrees"))
	bool ApplyDismemberment(const FName& BoneName, const FVector& ImpactDirection, const float ImpactForce,
	                        const FVector& AngularImpulseDegrees, const ELimbSpace LimbSpace = ELimbSpace::BoneSpace,
	                        const bool bUseWorldScale = true);
};
