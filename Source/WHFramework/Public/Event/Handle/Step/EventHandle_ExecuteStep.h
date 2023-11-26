// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Event/Handle/EventHandleBase.h"

#include "EventHandle_ExecuteStep.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class WHFRAMEWORK_API UEventHandle_ExecuteStep : public UEventHandleBase
{
	GENERATED_BODY()

public:
	UEventHandle_ExecuteStep();

public:
	virtual void OnDespawn_Implementation(bool bRecovery) override;

	virtual void Fill_Implementation(const TArray<FParameter>& InParams) override;

public:
	UPROPERTY(BlueprintReadOnly)
	class UStepBase* Step;
};
