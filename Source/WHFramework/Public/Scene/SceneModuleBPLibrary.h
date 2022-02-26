// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SceneModuleTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "SceneModuleBPLibrary.generated.h"

/**
 * 
 */
UCLASS()
class WHFRAMEWORK_API USceneModuleBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	//////////////////////////////////////////////////////////////////////////
	/// Level
	UFUNCTION(BlueprintCallable, Category = "SceneModuleBPLibrary")
	static void AsyncLoadLevel(FName InLevelPath, const FOnAsyncLoadLevelFinished& InOnAsyncLoadLevelFinished, float InFinishDelayTime = 1.f, bool bCreateLoadingWidget = true);

	UFUNCTION(BlueprintCallable, Category = "SceneModuleBPLibrary")
	static void AsyncUnloadLevel(FName InLevelPath, const FOnAsyncUnloadLevelFinished& InOnAsyncUnloadLevelFinished, float InFinishDelayTime = 1.f, bool bCreateLoadingWidget = true);

	UFUNCTION(BlueprintPure, Category = "SceneModuleBPLibrary")
	static float GetAsyncLoadLevelProgress(FName InLevelPath);

	UFUNCTION(BlueprintPure, Category = "SceneModuleBPLibrary")
	static float GetAsyncUnloadLevelProgress(FName InLevelPath);

	//////////////////////////////////////////////////////////////////////////
	/// Scene Object
	UFUNCTION(BlueprintPure, Category = "SceneModuleBPLibrary")
	static bool HasSceneObject(FName InName, bool bEnsured = true);

	UFUNCTION(BlueprintPure, Category = "SceneModuleBPLibrary")
	static TScriptInterface<class ISceneObject> GetSceneObject(FName InName, bool bEnsured = true);

	UFUNCTION(BlueprintCallable, Category = "SceneModuleBPLibrary")
	static void SetSceneObject(FName InName, TScriptInterface<class ISceneObject> InObject);

	//////////////////////////////////////////////////////////////////////////
	/// Target Point
	UFUNCTION(BlueprintPure, Category = "SceneModuleBPLibrary")
	static bool HasTargetPoint(FName InName, bool bEnsured = true);

	UFUNCTION(BlueprintPure, Category = "SceneModuleBPLibrary")
	static class ATargetPoint* GetTargetPoint(FName InName, bool bEnsured = true);

	UFUNCTION(BlueprintCallable, Category = "SceneModuleBPLibrary")
	static void SetTargetPoint(FName InName, class ATargetPoint* InPoint);

	//////////////////////////////////////////////////////////////////////////
	/// Scene Point
	UFUNCTION(BlueprintPure, Category = "SceneModuleBPLibrary")
	static bool HasScenePoint(FName InName, bool bEnsured = true);

	UFUNCTION(BlueprintPure, Category = "SceneModuleBPLibrary")
	static USceneComponent* GetScenePoint(FName InName, bool bEnsured = true);

	UFUNCTION(BlueprintCallable, Category = "SceneModuleBPLibrary")
	static void SetScenePoint(FName InName, USceneComponent* InSceneComp);

	//////////////////////////////////////////////////////////////////////////
	/// Scene Point
	UFUNCTION(BlueprintPure, Category = "SceneModuleBPLibrary")
	static bool HasPhysicsVolume(FName InName, bool bEnsured = true);

	UFUNCTION(BlueprintPure, Category = "SceneModuleBPLibrary")
	static class APhysicsVolumeBase* GetPhysicsVolume(FName InName, bool bEnsured = true);

	UFUNCTION(BlueprintCallable, Category = "SceneModuleBPLibrary")
	static void SetPhysicsVolume(FName InName, class APhysicsVolumeBase* InPhysicsVolume);
};
