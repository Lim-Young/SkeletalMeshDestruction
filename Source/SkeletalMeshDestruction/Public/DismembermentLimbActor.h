// Copyright Lim Young.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DismembermentLimbActor.generated.h"

class USkeletalMeshDestructionSubsystem;

UCLASS()
class SKELETALMESHDESTRUCTION_API ADismembermentLimbActor : public AActor
{
	GENERATED_BODY()

public:
	ADismembermentLimbActor();

protected:
	virtual void BeginPlay() override;

	float LimbLifeTime = 5.0f;
	FTimerHandle ReInitializeTimerHandle;
	FTimerHandle PhysicalSimulationAvoidanceTimerHandle;

	UPROPERTY(Transient)
	TObjectPtr<USkeletalMeshDestructionSubsystem> SkeletalMeshDestructionSubsystem;

	FName CurrentCollisionProfileName;

public:
	UPROPERTY(BlueprintReadOnly, Category = "Limb")
	TObjectPtr<UStaticMeshComponent> LimbMesh;

	void ReInitializeLimb(const FTransform& NewTransform, UStaticMesh* NewMesh, FName NewCollisionProfileName);

	void AddImpulse(const FVector& ImpulseDirection, float ImpactForce, const FVector& AngularImpulseDegrees);

protected:
	void StartLimbPhysicsSimulation(float ImpactForce);
	void StopLimbPhysicsSimulation() const;
	void EnableLimbCollision() const;
	void DisableLimbCollision() const;
};
