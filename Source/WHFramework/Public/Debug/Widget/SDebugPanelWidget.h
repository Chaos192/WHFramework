// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widget/Screen/Slate/SSlateWidgetBase.h"

/**
 * 
 */
class WHFRAMEWORK_API SDebugPanelWidget : public SSlateWidgetBase
{
public:
	SDebugPanelWidget();
	
	SLATE_BEGIN_ARGS(SDebugPanelWidget) {}
	
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

public:
	virtual void OnCreate(UObject* InOwner, const TArray<FParameter>& InParams) override;

	virtual void OnOpen(const TArray<FParameter>& InParams, bool bInstant) override;

	virtual void OnClose(bool bInstant) override;
};
