﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbilityVitalityStateBase.h"
#include "UObject/Object.h"
#include "AbilityVitalityState_Death.generated.h"

class IAbilityVitalityInterface;
/**
 * 
 */
UCLASS()
class WHFRAMEWORK_API UAbilityVitalityState_Death : public UAbilityVitalityStateBase
{
	GENERATED_BODY()

	friend class AAbilityVitalityBase;

public:
	UAbilityVitalityState_Death();

public:
	virtual void OnInitialize(UFSMComponent* InFSM, int32 InStateIndex) override;

	virtual bool OnEnterValidate(UFiniteStateBase* InLastState, const TArray<FParameter>& InParams) override;

	virtual void OnEnter(UFiniteStateBase* InLastState, const TArray<FParameter>& InParams) override;
	
	virtual void OnRefresh(float DeltaSeconds) override;

	virtual void OnLeave(UFiniteStateBase* InNextState) override;

	virtual void OnTermination() override;

public:
	UFUNCTION(BlueprintCallable)
	virtual void DeathStart();

	UFUNCTION(BlueprintCallable)
	virtual void DeathEnd();

protected:
	IAbilityVitalityInterface* Killer;
};
