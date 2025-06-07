// Copyright Lim Young.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "SkeletalMeshDestructionSettings.generated.h"

/**
 * 
 */
UCLASS(Config = SkeletalMeshDestructionSettings, DefaultConfig,
	AutoExpandCategories=("Skeletal Mesh Destruction|Dismemberment|PhysicalSimulationAvoidance"),
	meta = (DisplayName = "Skeletal Mesh Destruction Settings"))
class SKELETALMESHDESTRUCTION_API USkeletalMeshDestructionSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	virtual FName GetContainerName() const override
	{
		return TEXT("Project");
	}

	virtual FName GetCategoryName() const override
	{
		return TEXT("Skeletal Mesh Destruction");
	}

	virtual FName GetSectionName() const override
	{
		return TEXT("Skeletal Mesh Destruction Settings");
	}

#if WITH_EDITOR
	virtual FText GetSectionText() const override
	{
		return FText::FromString("Skeletal Mesh Destruction Settings");
	}

	virtual FText GetSectionDescription() const override
	{
		return FText::FromString("Settings for Skeletal Mesh Destruction plugin.");
	}
#endif

	UPROPERTY(Config, EditAnywhere, DisplayName = "Avoidance Speed Range",
		Category = "Skeletal Mesh Destruction|Dismemberment|PhysicalSimulationAvoidance",
		meta = (ClampMin = 0))
	FVector2D PhysicalSimulationAvoidanceForceRange = FVector2D(20.0f, 2000.0f);

	UPROPERTY(Config, EditAnywhere, DisplayName = "Max Avoidance Time",
		Category = "Skeletal Mesh Destruction|Dismemberment|PhysicalSimulationAvoidance",
		meta = (ClampMin = 0))
	float MaxPhysicalSimulationAvoidanceTime = 2.0f;

	UPROPERTY(Config, EditAnywhere, DisplayName = "Avoidance Collision Profile",
		Category = "Skeletal Mesh Destruction|Dismemberment|PhysicalSimulationAvoidance")
	FCollisionProfileName PhysicalSimulationAvoidanceCollisionProfileName = FCollisionProfileName(
		TEXT("PhysicalSimulationAvoidance"));
};
