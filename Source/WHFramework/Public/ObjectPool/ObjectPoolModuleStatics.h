// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "ObjectPoolModule.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ObjectPoolModuleStatics.generated.h"

/**
 * 
 */
UCLASS()
class WHFRAMEWORK_API UObjectPoolModuleStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	template<class T>
	static T* SpawnObject(const TArray<FParameter>* InParams = nullptr, TSubclassOf<UObject> InType = T::StaticClass())
	{
		return UObjectPoolModule::Get().SpawnObject<T>(InParams, InType);
	}

	template<class T>
	static T* SpawnObject(const TArray<FParameter>& InParams, TSubclassOf<UObject> InType = T::StaticClass())
	{
		return UObjectPoolModule::Get().SpawnObject<T>(InParams, InType);
	}

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "InWorldContext", DeterminesOutputType = "InType", AutoCreateRefTerm = "InParams"), Category = "ObjectPoolModuleStatics")
	static UObject* SpawnObject(TSubclassOf<UObject> InType, const TArray<FParameter>& InParams);

	UFUNCTION(BlueprintCallable, Category = "ObjectPoolModuleStatics")
	static void DespawnObject(UObject* InObject, bool bRecovery = true);

	template<class T>
	static void DespawnObjects(TArray<T*> InObjects, bool bRecovery = true)
	{
		for(auto Iter : InObjects)
		{
			DespawnObject(Iter, bRecovery);
		}
	}

	UFUNCTION(BlueprintCallable, Category = "ObjectPoolModuleStatics")
	static void DespawnObjects(TArray<UObject*> InObjects, bool bRecovery = true);

	template<class T>
	static void ClearObject(TSubclassOf<UObject> InType = T::StaticClass())
	{
		UObjectPoolModule::Get().ClearObject<T>(InType);
	}

	UFUNCTION(BlueprintCallable, Category = "ObjectPoolModuleStatics")
	static void ClearObject(TSubclassOf<UObject> InType);

	UFUNCTION(BlueprintCallable, Category = "ObjectPoolModuleStatics")
	static void ClearAllObject();
};
