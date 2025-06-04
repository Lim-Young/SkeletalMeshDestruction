// Copyright Lim Young.

#pragma once

#include "CoreMinimal.h"
#include "DismembermentLimbActor.h"
#include "Subsystems/WorldSubsystem.h"
#include "SkeletalMeshDestructionSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class SKELETALMESHDESTRUCTION_API USkeletalMeshDestructionSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	ADismembermentLimbActor* GetDismembermentLimbActor(const FTransform& Transform) const;
};
