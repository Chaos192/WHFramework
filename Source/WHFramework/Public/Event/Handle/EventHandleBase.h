// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "Event/EventModuleTypes.h"
#include "Common/Base/WHObject.h"
#include "Parameter/ParameterModuleTypes.h"

#include "EventHandleBase.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, EditInlineNew)
class WHFRAMEWORK_API UEventHandleBase : public UWHObject
{
	GENERATED_BODY()
	
public:
	UEventHandleBase();

public:
	virtual int32 GetLimit_Implementation() const override { return -1; }

	virtual void OnSpawn_Implementation(const TArray<FParameter>& InParams) override;
	
	virtual void OnDespawn_Implementation(bool bRecovery) override;

public:
	UFUNCTION(BlueprintNativeEvent)
	bool Filter(UObject* InOwner, const FName InFuncName) const;

	UFUNCTION(BlueprintNativeEvent)
	void Fill(const TArray<FParameter>& InParams);

	UFUNCTION(BlueprintNativeEvent)
	TArray<FParameter> Pack() const;    

public:
	UPROPERTY(BlueprintReadOnly)
	EEventType EventType;
};
