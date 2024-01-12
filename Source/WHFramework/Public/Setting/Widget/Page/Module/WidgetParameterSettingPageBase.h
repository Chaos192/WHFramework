// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Setting/Widget/Page/WidgetSettingPageBase.h"

#include "WidgetParameterSettingPageBase.generated.h"

class UWidgetKeySettingItemBase;
/**
 * 
 */
UCLASS()
class WHFRAMEWORK_API UWidgetParameterSettingPageBase : public UWidgetSettingPageBase
{
	GENERATED_BODY()
	
public:
	UWidgetParameterSettingPageBase(const FObjectInitializer& ObjectInitializer);
	
public:
	virtual void OnInitialize(UObject* InOwner, const TArray<FParameter>& InParams) override;

	virtual void OnCreate(UObject* InOwner, const TArray<FParameter>& InParams) override;

	virtual void OnOpen(const TArray<FParameter>& InParams, bool bInstant) override;

	virtual void OnApply() override;

	virtual void OnReset(bool bForce) override;

	virtual void OnValuesChange(UWidgetSettingItemBase* InSettingItem, const TArray<FParameter>& InValues) override;

	virtual void OnClose(bool bInstant) override;

public:
	virtual bool CanApply_Implementation() const override;

	virtual bool CanReset_Implementation() const override;

protected:
	virtual FSaveData* GetDefaultSaveData() const override;
};
