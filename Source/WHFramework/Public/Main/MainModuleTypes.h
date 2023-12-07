// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/UObjectGlobals.h"
#include "Main/MainModuleStatics.h"
#include "Debug/DebugModuleTypes.h"

#include "MainModuleTypes.generated.h"

UENUM(BlueprintType)
enum class EModuleState : uint8
{
	None,
	Running,
	Paused,
	Terminated
};

#define GENERATED_MAIN_MODULE(ModuleClass) \
protected: \
	static ModuleClass* Instance; \
	static ModuleClass* InstanceEditor; \
public: \
	static ModuleClass& Get(bool bInEditor = false, bool bForce = false); \
	static ModuleClass* GetPtr(bool bInEditor = false, bool bForce = false);

#define GENERATED_MODULE(ModuleClass) \
protected: \
	static ModuleClass* Instance; \
	static ModuleClass* InstanceEditor; \
public: \
	static ModuleClass& Get(bool bInEditor = false); \
	static ModuleClass* GetPtr(bool bInEditor = false); \

#define TERMINATION_MAIN_MODULE(ModuleClass) \
	if(ModuleClass::Instance == this) \
	{ \
		ModuleClass::Instance = nullptr; \
	} \
	if(ModuleClass::InstanceEditor == this) \
	{ \
		ModuleClass::InstanceEditor = nullptr; \
	} \

#define TERMINATION_MODULE(ModuleClass) \
	if(ModuleClass::Instance == this) \
	{ \
		ModuleClass::Instance = nullptr; \
	} \
	if(ModuleClass::InstanceEditor == this) \
	{ \
		ModuleClass::InstanceEditor = nullptr; \
	} \

#define IMPLEMENTATION_MAIN_MODULE(ModuleClass) \
ModuleClass* ModuleClass::Instance = nullptr; \
ModuleClass* ModuleClass::InstanceEditor = nullptr; \
ModuleClass& ModuleClass::Get(bool bInEditor, bool bForce) \
{ \
	return ModuleClass::GetPtr(bInEditor, bForce) ? *ModuleClass::GetPtr(bInEditor, bForce) : *NewObject<ModuleClass>(); \
} \
ModuleClass* ModuleClass::GetPtr(bool bInEditor, bool bForce) \
{ \
	if(!bInEditor) \
	{ \
		if(!Instance || bForce) \
		{ \
			Instance = UCommonStatics::GetObjectInExistedWorld<ModuleClass>([](const UWorld* World) { \
													return UGameplayStatics::GetActorOfClass(World, ModuleClass::StaticClass()); \
												}, false); \
		} \
		return Instance; \
	} \
	else \
	{ \
		if(!InstanceEditor || bForce) \
		{ \
			InstanceEditor = UCommonStatics::GetObjectInExistedWorld<ModuleClass>([](const UWorld* World) { \
                             						return UGameplayStatics::GetActorOfClass(World, ModuleClass::StaticClass()); \
                             					}, true); \
		} \
		return InstanceEditor; \
	} \
}

#define IMPLEMENTATION_MODULE(ModuleClass) \
ModuleClass* ModuleClass::Instance = nullptr; \
ModuleClass* ModuleClass::InstanceEditor = nullptr; \
ModuleClass& ModuleClass::Get(bool bInEditor) \
{ \
	return ModuleClass::GetPtr(bInEditor) ? *ModuleClass::GetPtr(bInEditor) : *NewObject<ModuleClass>(); \
} \
ModuleClass* ModuleClass::GetPtr(bool bInEditor) \
{ \
	if(!bInEditor) \
	{ \
		if(!Instance) \
		{ \
			Instance = Cast<ModuleClass>(UMainModuleStatics::GetModuleByClass(ModuleClass::StaticClass(), false)); \
		} \
		return Instance; \
	} \
	else \
	{ \
		if(!InstanceEditor) \
		{ \
			InstanceEditor = Cast<ModuleClass>(UMainModuleStatics::GetModuleByClass(ModuleClass::StaticClass(), true)); \
		} \
		return InstanceEditor; \
	} \
}

USTRUCT(BlueprintType)
struct WHFRAMEWORK_API FModuleListItemStates
{
	GENERATED_BODY()

public:
	FORCEINLINE FModuleListItemStates()
	{
		
	}
};
