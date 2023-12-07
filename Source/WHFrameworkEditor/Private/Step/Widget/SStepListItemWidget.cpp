// Fill out your copyright notice in the Description page of Project Settings.


#include "Step/Widget/SStepListItemWidget.h"

#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SStepListItemWidget::Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& InOwnerTableView)
{
	Item = InArgs._Item;
	STableRow::Construct
	(
		STableRow::FArguments()
		.Style(FAppStyle::Get(), "SceneOutliner.TableViewRow")
		.ShowWires(false)
		.Content()
		[
			SNew(STextBlock)
			.Text_Lambda([this](){ return FText::FromString(FString::Printf(TEXT("%d.%s"), Item->Step->StepIndex + 1, *Item->Step->StepDisplayName.ToString())); })
		], 
		InOwnerTableView
	);
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
