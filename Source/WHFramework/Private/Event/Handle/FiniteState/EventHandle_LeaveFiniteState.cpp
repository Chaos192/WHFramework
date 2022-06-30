// Fill out your copyright notice in the Description page of Project Settings.


#include "Event/Handle/FiniteState/EventHandle_LeaveFiniteState.h"

UEventHandle_LeaveFiniteState::UEventHandle_LeaveFiniteState()
{
	EventType = EEventType::Multicast;
}

void UEventHandle_LeaveFiniteState::OnDespawn_Implementation()
{
	FiniteState = nullptr;
	FSMComponent = nullptr;
}

void UEventHandle_LeaveFiniteState::Fill_Implementation(const TArray<FParameter>& InParams)
{
	if(InParams.IsValidIndex(0))
	{
		FiniteState = InParams[0].GetObjectValue<UFiniteStateBase>();
	}
	if(InParams.IsValidIndex(0))
	{
		FSMComponent = InParams[0].GetObjectValue<UFSMComponent>();
	}
}
