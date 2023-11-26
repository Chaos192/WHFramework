// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Event/Handle/EventHandleBase.h"
#include "Parameter/ParameterModuleTypes.h"

#include "EventHandle_ExitGame.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class WHFRAMEWORK_API UEventHandle_ExitGame : public UEventHandleBase
{
	GENERATED_BODY()

public:
	UEventHandle_ExitGame();
	
public:
	virtual void OnDespawn_Implementation(bool bRecovery) override;

public:
	virtual void Fill_Implementation(const TArray<FParameter>& InParams) override;

public:
	UPROPERTY(BlueprintReadOnly)
	bool bIsSimulating;
};
