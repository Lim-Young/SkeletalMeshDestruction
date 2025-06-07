// Copyright Lim Young.

#pragma once

#include "CoreMinimal.h"
#include "DismembermentLimbActor.h"
#include "SkeletalMeshDestructionSettings.h"
#include "Subsystems/WorldSubsystem.h"
#include "SkeletalMeshDestructionSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class SKELETALMESHDESTRUCTION_API USkeletalMeshDestructionSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

	FVector2D PhysicalSimulationAvoidanceSpeedRange;

	float MaxPhysicalSimulationAvoidanceTime;

	FName PhysicalSimulationAvoidanceCollisionProfileName;

	const float MinPhysicalSimulationAvoidanceTime = 0.0166666666666667f; // 1/60 seconds

protected:
	UPROPERTY()
	TObjectPtr<USkeletalMeshDestructionSettings> SkeletalMeshDestructionSettings;

public:
	virtual void PostInitialize() override;
	ADismembermentLimbActor* GetDismembermentLimbActor(const FTransform& Transform) const;

	bool CalculatePhysicalSimulationAvoidanceTime(float Force, float& OutAvoidanceTime) const;

	FName GetPhysicalSimulationAvoidanceCollisionProfile() const;
};
