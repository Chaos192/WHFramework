// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "InputModuleTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "InputModuleBPLibrary.generated.h"

/**
 * 
 */
UCLASS()
class WHFRAMEWORK_API UInputModuleBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	//////////////////////////////////////////////////////////////////////////
	// InputShortcuts
public:
	UFUNCTION(BlueprintPure, Category = "InputModuleBPLibrary")
	static FInputKeyShortcut GetKeyShortcutByName(const FName InName);

	//////////////////////////////////////////////////////////////////////////
	// InputStates
public:
	UFUNCTION(BlueprintPure, Category = "InputModuleBPLibrary")
	static int32 GetTouchPressedCount();

	//////////////////////////////////////////////////////////////////////////
	// InputMode
public:
	UFUNCTION(BlueprintPure, Category = "InputModuleBPLibrary")
	static EInputMode GetGlobalInputMode();

	UFUNCTION(BlueprintCallable, Category = "InputModuleBPLibrary")
	static void SetGlobalInputMode(EInputMode InInputMode);

	UFUNCTION(BlueprintCallable, Category = "InputModuleBPLibrary")
	static void UpdateGlobalInputMode();
};