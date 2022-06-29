// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "SaveGame/Parameter/ParameterSaveGame.h"

#include "Camera/CameraModuleBPLibrary.h"
#include "Global/GlobalBPLibrary.h"
#include "Kismet/GameplayStatics.h"

UParameterSaveGame::UParameterSaveGame()
{
	SaveName = FName("Parameter");
}

void UParameterSaveGame::OnCreate_Implementation(int32 InSaveIndex)
{
	Super::OnCreate_Implementation(InSaveIndex);
}

void UParameterSaveGame::OnLoad_Implementation()
{
	Super::OnLoad_Implementation();
}

void UParameterSaveGame::OnUnload_Implementation()
{
	Super::OnLoad_Implementation();
}

void UParameterSaveGame::OnRefresh_Implementation()
{
	Super::OnRefresh_Implementation();
	
}
