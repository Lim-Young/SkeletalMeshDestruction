// Copyright Lim Young.

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "SkeletalMeshDestructionConfigDataFactory.generated.h"

/**
 * 
 */
UCLASS()
class SKELETALMESHDESTRUCTIONEDITOR_API USkeletalMeshDestructionConfigDataFactory : public UFactory
{
	GENERATED_BODY()

public:
	USkeletalMeshDestructionConfigDataFactory();

	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags,
									  UObject* Context, FFeedbackContext* Warn) override;
};
