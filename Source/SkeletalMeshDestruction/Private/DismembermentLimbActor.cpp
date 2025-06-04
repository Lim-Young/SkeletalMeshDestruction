// Copyright Lim Young.


#include "DismembermentLimbActor.h"


ADismembermentLimbActor::ADismembermentLimbActor()
{
	PrimaryActorTick.bCanEverTick = false;

	LimbMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LimbMesh"));
	RootComponent = LimbMesh;

	LimbMesh->SetSimulatePhysics(true);
}

void ADismembermentLimbActor::ReInitializeLimb(const FTransform& NewTransform, UStaticMesh* NewMesh,
                                               const FName NewCollisionProfileName)
{
	if (LimbMesh)
	{
		LimbMesh->SetStaticMesh(NewMesh);
		LimbMesh->SetCollisionProfileName(NewCollisionProfileName);

		LimbMesh->SetSimulatePhysics(false);
		LimbMesh->SetWorldTransform(NewTransform);
		LimbMesh->SetSimulatePhysics(true);

		GetWorld()->GetTimerManager().SetTimer(
			ReInitializeTimerHandle, this, &ThisClass::StopLimbPhysicsSimulation, LimbLifeTime, false);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("LimbMesh is not valid!"));
	}
}

void ADismembermentLimbActor::AddImpulse(const FVector& ImpulseDirection, const float ImpactForce,
                                         const FVector& AngularImpulseDegrees) const
{
	if (LimbMesh)
	{
		const FVector Impulse = ImpulseDirection.GetSafeNormal() * ImpactForce;
		LimbMesh->AddImpulse(Impulse, NAME_None, true);
		LimbMesh->AddAngularImpulseInDegrees(AngularImpulseDegrees, NAME_None, true);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("LimbMesh is not valid!"));
	}
}

void ADismembermentLimbActor::StopLimbPhysicsSimulation() const
{
	if (LimbMesh)
	{
		LimbMesh->SetSimulatePhysics(false);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("LimbMesh is not valid!"));
	}
}
