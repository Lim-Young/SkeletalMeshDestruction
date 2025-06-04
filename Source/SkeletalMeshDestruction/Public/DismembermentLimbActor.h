// Copyright Lim Young.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DismembermentLimbActor.generated.h"

UCLASS()
class SKELETALMESHDESTRUCTION_API ADismembermentLimbActor : public AActor
{
	GENERATED_BODY()

public:
	ADismembermentLimbActor();

protected:
	float LimbLifeTime = 5.0f;
	FTimerHandle ReInitializeTimerHandle;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Limb")
	TObjectPtr<UStaticMeshComponent> LimbMesh;

	void ReInitializeLimb(const FTransform& NewTransform, UStaticMesh* NewMesh, FName NewCollisionProfileName);

	void AddImpulse(const FVector& ImpulseDirection, float ImpactForce, const FVector& AngularImpulseDegrees) const;

protected:
	void StopLimbPhysicsSimulation() const;
};
