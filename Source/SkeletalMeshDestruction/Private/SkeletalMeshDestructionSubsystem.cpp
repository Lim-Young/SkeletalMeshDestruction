// Copyright Lim Young.


#include "SkeletalMeshDestructionSubsystem.h"


ADismembermentLimbActor* USkeletalMeshDestructionSubsystem::GetDismembermentLimbActor(const FTransform& Transform) const
{
	ADismembermentLimbActor* LimbActor = GetWorld()->SpawnActor<ADismembermentLimbActor>(ADismembermentLimbActor::StaticClass(), Transform);
	return LimbActor;
}
