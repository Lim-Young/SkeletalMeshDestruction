// Copyright Lim Young.


#include "SkeletalMeshDestructionSubsystem.h"


void USkeletalMeshDestructionSubsystem::PostInitialize()
{
	const USkeletalMeshDestructionSettings* DestructionSettings = GetDefault<USkeletalMeshDestructionSettings>();

	PhysicalSimulationAvoidanceSpeedRange = DestructionSettings->PhysicalSimulationAvoidanceForceRange;
	MaxPhysicalSimulationAvoidanceTime = DestructionSettings->MaxPhysicalSimulationAvoidanceTime;
	PhysicalSimulationAvoidanceCollisionProfileName = DestructionSettings->
	                                                  PhysicalSimulationAvoidanceCollisionProfileName.Name;
}

ADismembermentLimbActor* USkeletalMeshDestructionSubsystem::GetDismembermentLimbActor(const FTransform& Transform) const
{
	ADismembermentLimbActor* LimbActor = GetWorld()->SpawnActor<ADismembermentLimbActor>(
		ADismembermentLimbActor::StaticClass(), Transform);
	return LimbActor;
}

bool USkeletalMeshDestructionSubsystem::CalculatePhysicalSimulationAvoidanceTime(
	const float Force, float& OutAvoidanceTime) const
{
	if (Force <= PhysicalSimulationAvoidanceSpeedRange.X)
	{
		OutAvoidanceTime = MaxPhysicalSimulationAvoidanceTime;
		return true;
	}

	if (Force >= PhysicalSimulationAvoidanceSpeedRange.Y)
	{
		OutAvoidanceTime = MinPhysicalSimulationAvoidanceTime;
		return false;
	}

	const float Alpha = FMath::Clamp((Force - PhysicalSimulationAvoidanceSpeedRange.X) /
	                                 (PhysicalSimulationAvoidanceSpeedRange.Y - PhysicalSimulationAvoidanceSpeedRange.
		                                 X),
	                                 0, 1);
	OutAvoidanceTime = FMath::Lerp(MinPhysicalSimulationAvoidanceTime,
	                               MaxPhysicalSimulationAvoidanceTime, Alpha);
	return true;
}

FName USkeletalMeshDestructionSubsystem::GetPhysicalSimulationAvoidanceCollisionProfile() const
{
	return PhysicalSimulationAvoidanceCollisionProfileName;
}
