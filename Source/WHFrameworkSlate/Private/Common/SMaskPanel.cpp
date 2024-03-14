// Fill out your copyright notice in the Description page of Project Settings.

#include "Common/SMaskPanel.h"

#include "SlateOptMacros.h"
#include "WHFrameworkSlateStyle.h"
#include "Common/SBasePanel.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SMaskPanel::Construct(const FArguments& InArgs)
{
	PanelWidget = InArgs._PanelWidget;
	
	ChildSlot
	[
		SNew(SOverlay)

		+SOverlay::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[
			SNew(SBorder)
			.BorderImage(FWHFrameworkSlateStyle::Get().GetBrush("Icons.White"))
			.BorderBackgroundColor(InArgs._MaskColor)
			[
				SNew(SButton)
				.ButtonStyle(&FWHFrameworkSlateStyle::Get().GetWidgetStyle<FButtonStyle>("Button.NoBorder"))
				.OnClicked(this, &SMaskPanel::OnMaskButtonClicked)
			]
		]

		+SOverlay::Slot()
		.HAlign(InArgs._HAlign)
		.VAlign(InArgs._VAlign)
		[
			InArgs._Content.Widget->AsShared()
		]
	];
}

FReply SMaskPanel::OnMaskButtonClicked()
{
	if(PanelWidget)
	{
		PanelWidget->Close();
	}
	else
	{
		SetVisibility(EVisibility::Collapsed);
	}
	return FReply::Handled();
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
