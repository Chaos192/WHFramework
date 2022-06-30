// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "FSMModuleTypes.h"
#include "Main/Base/ModuleBase.h"

#include "FSMModule.generated.h"

/**
 * 
 */
UCLASS()
class WHFRAMEWORK_API AFSMModule : public AModuleBase
{
	GENERATED_BODY()

public:
	// ParamSets default values for this actor's properties
	AFSMModule();

	//////////////////////////////////////////////////////////////////////////
	/// Module
public:
	#if WITH_EDITOR
	virtual void OnGenerate_Implementation() override;

	virtual void OnDestroy_Implementation() override;
	#endif

	virtual void OnInitialize_Implementation() override;

	virtual void OnPreparatory_Implementation() override;

	virtual void OnRefresh_Implementation(float DeltaSeconds) override;

	virtual void OnPause_Implementation() override;

	virtual void OnUnPause_Implementation() override;

	virtual void OnTermination_Implementation() override;
};
