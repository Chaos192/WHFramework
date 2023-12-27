// Copyright Epic Games, Inc. All Rights Reserved.

#include "Widget/Common/CommonButton.h"
#include "CommonTextBlock.h"

UCommonButton::UCommonButton(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	Txt_Title = nullptr;
	
	bClicked = false;

	bStandalone = false;

	static ConstructorHelpers::FClassFinder<UCommonButtonStyle> StyleClassFinder(TEXT("/Script/Engine.Blueprint'/WHFramework/Widget/Blueprints/Common/Style/CBS_Default.CBS_Default_C'"));
	if(StyleClassFinder.Succeeded())
	{
		Style = StyleClassFinder.Class;
	}
}

void UCommonButton::OnSpawn_Implementation(UObject* InOwner, const TArray<FParameter>& InParams)
{
	
}

void UCommonButton::OnDespawn_Implementation(bool bRecovery)
{
	bClicked = false;

	SetTitle(FText::GetEmpty());

	if(GetSelected())
	{
		SetSelectedInternal(false, false, false);
	}

	RemoveFromParent();
}

void UCommonButton::NativePreConstruct()
{
	Super::NativePreConstruct();

	SetTitle(Title);
}

void UCommonButton::NativeOnCurrentTextStyleChanged()
{
	Super::NativeOnCurrentTextStyleChanged();

	if(Txt_Title)
	{
		Txt_Title->SetStyle(GetCurrentTextStyleClass());
	}
}

void UCommonButton::NativeOnClicked()
{
	Super::NativeOnClicked();
}

void UCommonButton::NativeOnSelected(bool bBroadcast)
{
	bClicked = bBroadcast;

	Super::NativeOnSelected(bBroadcast);
}

void UCommonButton::NativeOnDeselected(bool bBroadcast)
{
	bClicked = bBroadcast;

	Super::NativeOnDeselected(bBroadcast);
}

void UCommonButton::SetTitle(const FText InTitle)
{
	Title = InTitle;
	if(Txt_Title)
	{
		Txt_Title->SetText(Title);
		Txt_Title->SetVisibility(Title.IsEmpty() ? ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);
	}
}

void UCommonButton::SetMinWidth(int32 InValue)
{
	MinWidth = InValue;
}

void UCommonButton::SetMinHeight(int32 InValue)
{
	MinHeight = InValue;
}
