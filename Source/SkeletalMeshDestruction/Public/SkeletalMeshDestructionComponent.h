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
	// Sets default values for this component's properties
	USkeletalMeshDestructionComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

public:
	UFUNCTION(BlueprintCallable, Category="SkeletalMeshDestruction")
	void InitializeSkeletalMesh();

protected:
	void CombineSkeletalMesh();

private:
	UPROPERTY()
	TObjectPtr<USkeletalMeshComponent> DrivenSkeletalMeshComponent;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SkeletalMeshDestruction")
	FSkeletalMeshDestructionConfig SkeletalMeshDestructionConfig;
};
