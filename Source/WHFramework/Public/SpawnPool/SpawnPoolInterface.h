// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SpawnPoolInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USpawnPoolInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class WHFRAMEWORK_API ISpawnPoolInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnSpawn();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnDespawn();

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	int32 GetLimit() const;
};
