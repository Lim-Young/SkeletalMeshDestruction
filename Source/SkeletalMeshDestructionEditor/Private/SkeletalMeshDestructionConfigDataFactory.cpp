// Copyright Lim Young.


#include "SkeletalMeshDestructionConfigDataFactory.h"

#include "SkeletalMeshDestructionConfig.h"

USkeletalMeshDestructionConfigDataFactory::USkeletalMeshDestructionConfigDataFactory()
{
	SupportedClass = USkeletalMeshDestructionConfigData::StaticClass();
	bCreateNew = true;
}

UObject* USkeletalMeshDestructionConfigDataFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName,
	EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	return NewObject<USkeletalMeshDestructionConfigData>(InParent, InClass, InName, Flags, Context);
}
