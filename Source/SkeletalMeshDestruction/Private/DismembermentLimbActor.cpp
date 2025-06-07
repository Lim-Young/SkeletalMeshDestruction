// Copyright Lim Young.


#include "DismembermentLimbActor.h"

#include "SkeletalMeshDestructionSubsystem.h"


ADismembermentLimbActor::ADismembermentLimbActor()
{
	PrimaryActorTick.bCanEverTick = false;

	LimbMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LimbMesh"));
	RootComponent = LimbMesh;

	LimbMesh->SetSimulatePhysics(true);
}

void ADismembermentLimbActor::BeginPlay()
{
	SkeletalMeshDestructionSubsystem = GetWorld()->GetSubsystem<USkeletalMeshDestructionSubsystem>();
	check(SkeletalMeshDestructionSubsystem);
}

void ADismembermentLimbActor::ReInitializeLimb(const FTransform& NewTransform, UStaticMesh* NewMesh,
                                               const FName NewCollisionProfileName)
{
	if (LimbMesh)
	{
		LimbMesh->SetStaticMesh(NewMesh);
		CurrentCollisionProfileName = NewCollisionProfileName;

		LimbMesh->SetSimulatePhysics(false);
		LimbMesh->SetWorldTransform(NewTransform);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("LimbMesh is not valid!"));
	}
}

void ADismembermentLimbActor::AddImpulse(const FVector& ImpulseDirection, const float ImpactForce,
                                         const FVector& AngularImpulseDegrees)
{
	if (LimbMesh)
	{
		const FVector Impulse = ImpulseDirection.GetSafeNormal() * ImpactForce;

		StartLimbPhysicsSimulation(ImpactForce);

		LimbMesh->AddImpulse(Impulse, NAME_None, true);
		LimbMesh->AddAngularImpulseInDegrees(AngularImpulseDegrees, NAME_None, true);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("LimbMesh is not valid!"));
	}
}

void ADismembermentLimbActor::StartLimbPhysicsSimulation(const float ImpactForce)
{
	if (LimbMesh)
	{
		// Physical Simulation Avoidance
		float AvoidanceTime = 0.0f;
		DisableLimbCollision();
		if (SkeletalMeshDestructionSubsystem->CalculatePhysicalSimulationAvoidanceTime(ImpactForce, AvoidanceTime))
		{
			GetWorld()->GetTimerManager().SetTimer(PhysicalSimulationAvoidanceTimerHandle,
			                                       this, &ThisClass::EnableLimbCollision, AvoidanceTime, false);
		}
		else
		{
			PhysicalSimulationAvoidanceTimerHandle = GetWorld()->GetTimerManager().SetTimerForNextTick(
				this, &ThisClass::EnableLimbCollision);
		}

		LimbMesh->SetSimulatePhysics(true);

		// Life Time Control
		GetWorld()->GetTimerManager().SetTimer(
			ReInitializeTimerHandle, this, &ThisClass::StopLimbPhysicsSimulation, LimbLifeTime + AvoidanceTime,
			false);
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

void ADismembermentLimbActor::EnableLimbCollision() const
{
	if (LimbMesh)
	{
		LimbMesh->SetCollisionProfileName(CurrentCollisionProfileName);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("LimbMesh is not valid!"));
	}
}

void ADismembermentLimbActor::DisableLimbCollision() const
{
	if (LimbMesh)
	{
		LimbMesh->SetCollisionProfileName(
			SkeletalMeshDestructionSubsystem->GetPhysicalSimulationAvoidanceCollisionProfile());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("LimbMesh is not valid!"));
	}
}
