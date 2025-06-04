// Copyright Lim Young.

#pragma once

#include "CoreMinimal.h"
#include "SkeletalMeshDestructionConfig.h"
#include "Components/ActorComponent.h"
#include "SkeletalMeshDestructionComponent.generated.h"


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
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SkeletalMeshDestruction")
	FSkeletalMeshDestructionConfig SkeletalMeshDestructionConfig;

public:
	UFUNCTION(BlueprintCallable, Category="SkeletalMeshDestruction|DLOD", meta = (AutoCreateRefTerm = "BoneName"))
	bool SetDLODByBoneName(const FName& BoneName, const uint8 DLODLevel);

	UFUNCTION(BlueprintCallable, Category="SkeletalMeshDestruction|DLOD", meta = (AutoCreateRefTerm = "BoneName"))
	bool DegradeSkeletalMesh(const FName& BoneName);
};
