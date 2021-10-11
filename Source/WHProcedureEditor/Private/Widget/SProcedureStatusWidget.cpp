// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/SProcedureStatusWidget.h"
#include "SlateOptMacros.h"
#include "Widget/SProcedureListWidget.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SProcedureStatusWidget::Construct(const FArguments& InArgs)
{
	ListWidget = InArgs._ListWidget;
	
	if(!ListWidget) return;
	
	ChildSlot
	[
		SNew(SBorder)
		.Padding(1.f)
		.BorderImage(FCoreStyle::Get().GetBrush("ToolPanel.GroupBorder"))
		[
			SNew(SVerticalBox)

			+ SVerticalBox::Slot()
			.VAlign(VAlign_Fill)
			.HAlign(HAlign_Fill)
			.FillHeight(1)
			[
				SNew(SHorizontalBox)
				
				+ SHorizontalBox::Slot()
				.VAlign(VAlign_Fill)
				.HAlign(HAlign_Left)
				.AutoWidth()
				[
					SNew(STextBlock)
					.Text(FText::FromString(TEXT("List:  ")))
					.ColorAndOpacity(FSlateColor(FLinearColor::Yellow))
				]
				
				+ SHorizontalBox::Slot()
				.VAlign(VAlign_Fill)
				.HAlign(HAlign_Left)
				.AutoWidth()
				[
					SNew(STextBlock)
					.Text_Lambda([this](){ return FText::FromString(FString::Printf(TEXT("Total Num: %d"), ListWidget->GetTotalProcedureNum())); } )
				]
				
				+ SHorizontalBox::Slot()
				.VAlign(VAlign_Fill)
				.HAlign(HAlign_Left)
				.AutoWidth()
				[
					SNew(STextBlock)
					.Text_Lambda([this](){ return FText::FromString(FString::Printf(TEXT("  Selected Num: %d"), ListWidget->GetSelectedProcedureNum())); } )
				]
			]
		]
	];
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION
