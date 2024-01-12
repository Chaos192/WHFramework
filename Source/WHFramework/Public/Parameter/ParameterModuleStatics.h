// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "ParameterModuleTypes.h"
#include "Common/CommonTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ParameterModuleStatics.generated.h"

class UParameterModule;
class ATargetPoint;

/**
 * 
 */
UCLASS()
class WHFRAMEWORK_API UParameterModuleStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	//////////////////////////////////////////////////////////////////////////
	/// Global Parameter
	UFUNCTION(BlueprintPure, Category = "ParameterModuleStatics")
	static bool HasGlobalParameter(FName InName, bool bEnsured = true);

	UFUNCTION(BlueprintCallable, Category = "ParameterModuleStatics")
	static void SetGlobalParameter(FName InName, FParameter InParameter);

	UFUNCTION(BlueprintPure, Category = "ParameterModuleStatics")
	static FParameter GetGlobalParameter(FName InName, bool bEnsured = true);

	UFUNCTION(BlueprintPure, Category = "ParameterModuleStatics")
	static TArray<FParameter> GetGlobalParameters(FName InName, bool bEnsured = true);
	
	UFUNCTION(BlueprintCallable, Category = "ParameterModuleStatics")
	static void RemoveGlobalParameter(FName InName);
	
	UFUNCTION(BlueprintCallable, Category = "ParameterModuleStatics")
	static void RemoveGlobalParameters(FName InName);

	UFUNCTION(BlueprintCallable, Category = "ParameterModuleStatics")
	static void ClearAllGlobalParameter();

	//////////////////////////////////////////////////////////////////////////
	/// Parameter
public:
	UFUNCTION(BlueprintPure, Category = "ParameterModuleStatics")
	static FParameter MakeIntegerParameter(int32 InValue);

	UFUNCTION(BlueprintPure, Category = "ParameterModuleStatics")
	static FParameter MakeFloatParameter(float InValue);

	UFUNCTION(BlueprintPure, Category = "ParameterModuleStatics")
	static FParameter MakeStringParameter(const FString& InValue);
	
	UFUNCTION(BlueprintPure, Category = "ParameterModuleStatics")
	static FParameter MakeNameParameter(const FName InValue);

	UFUNCTION(BlueprintPure, Category = "ParameterModuleStatics")
	static FParameter MakeTextParameter(const FText& InValue);

	UFUNCTION(BlueprintPure, Category = "ParameterModuleStatics")
	static FParameter MakeBooleanParameter(bool InValue);

	UFUNCTION(BlueprintPure, Category = "ParameterModuleStatics")
	static FParameter MakeVectorParameter(const FVector& InValue);

	UFUNCTION(BlueprintPure, Category = "ParameterModuleStatics")
	static FParameter MakeRotatorParameter(const FRotator& InValue);

	UFUNCTION(BlueprintPure, Category = "ParameterModuleStatics")
	static FParameter MakeColorParameter(FColor InValue);

	UFUNCTION(BlueprintPure, meta = (AutoCreateRefTerm = "InValue"), Category = "ParameterModuleStatics")
	static FParameter MakeKeyParameter(const FKey& InValue);

	UFUNCTION(BlueprintPure, meta = (AutoCreateRefTerm = "InValue"), Category = "ParameterModuleStatics")
	static FParameter MakeTagParameter(const FGameplayTag& InValue);

	UFUNCTION(BlueprintPure, Category = "ParameterModuleStatics")
	static FParameter MakeTagsParameter(const FGameplayTagContainer& InValue);

	UFUNCTION(BlueprintPure, Category = "ParameterModuleStatics")
	static FParameter MakeClassParameter(UClass* InValue);

	UFUNCTION(BlueprintPure, Category = "ParameterModuleStatics")
	static FParameter MakeClassPtrParameter(const TSoftClassPtr<UObject>& InValue);

	UFUNCTION(BlueprintPure, Category = "ParameterModuleStatics")
	static FParameter MakeObjectParameter(UObject* InValue);

	UFUNCTION(BlueprintPure, Category = "ParameterModuleStatics")
	static FParameter MakeObjectPtrParameter(const TSoftObjectPtr<UObject>& InValue);

public:
	//////////////////////////////////////////////////////////////////////////
	UFUNCTION(BlueprintPure, Category = "ParameterModuleStatics")
	static EParameterType GetParameterType(UPARAM(ref) const FParameter& InParameter) { return InParameter.GetParameterType(); }

	UFUNCTION(BlueprintCallable, Category = "ParameterModuleStatics")
	static void SetParameterType(UPARAM(ref) FParameter& InParameter, EParameterType InParameterType) { InParameter.SetParameterType(InParameterType); }

	//////////////////////////////////////////////////////////////////////////
	UFUNCTION(BlueprintPure, Category = "ParameterModuleStatics")
	static int32 GetIntegerValue(UPARAM(ref) const FParameter& InParameter) { return InParameter.GetIntegerValue(); }

	UFUNCTION(BlueprintCallable, Category = "ParameterModuleStatics")
	static void SetIntegerValue(UPARAM(ref) FParameter& InParameter, int32 InValue) { InParameter.SetIntegerValue(InValue); }

	//////////////////////////////////////////////////////////////////////////
	UFUNCTION(BlueprintPure, Category = "ParameterModuleStatics")
	static float GetFloatValue(UPARAM(ref) const FParameter& InParameter) { return InParameter.GetFloatValue(); }

	UFUNCTION(BlueprintCallable, Category = "ParameterModuleStatics")
	static void SetFloatValue(UPARAM(ref) FParameter& InParameter, float InValue) { InParameter.SetFloatValue(InValue); }

	//////////////////////////////////////////////////////////////////////////
	UFUNCTION(BlueprintPure, Category = "ParameterModuleStatics")
	static FString GetStringValue(UPARAM(ref) const FParameter& InParameter) { return InParameter.GetStringValue(); }

	UFUNCTION(BlueprintCallable, Category = "ParameterModuleStatics")
	static void SetStringValue(UPARAM(ref) FParameter& InParameter, const FString& InValue) { InParameter.SetStringValue(InValue); }

	//////////////////////////////////////////////////////////////////////////
	UFUNCTION(BlueprintPure, Category = "ParameterModuleStatics")
	static FName GetNameValue(UPARAM(ref) const FParameter& InParameter) { return InParameter.GetNameValue(); }

	UFUNCTION(BlueprintCallable, Category = "ParameterModuleStatics")
	static void SetNameValue(UPARAM(ref) FParameter& InParameter, const FName InValue) { InParameter.SetNameValue(InValue); }

	//////////////////////////////////////////////////////////////////////////
	UFUNCTION(BlueprintPure, Category = "ParameterModuleStatics")
	static FText GetTextValue(UPARAM(ref) const FParameter& InParameter) { return InParameter.GetTextValue(); }

	UFUNCTION(BlueprintCallable, Category = "ParameterModuleStatics")
	static void SetTextValue(UPARAM(ref) FParameter& InParameter, const FText InValue) { InParameter.SetTextValue(InValue); }

	//////////////////////////////////////////////////////////////////////////
	UFUNCTION(BlueprintPure, Category = "ParameterModuleStatics")
	static bool GetBooleanValue(UPARAM(ref) const FParameter& InParameter) { return InParameter.GetBooleanValue(); }

	UFUNCTION(BlueprintCallable, Category = "ParameterModuleStatics")
	static void SetBooleanValue(UPARAM(ref) FParameter& InParameter, bool InValue) { InParameter.SetBooleanValue(InValue); }

	//////////////////////////////////////////////////////////////////////////
	UFUNCTION(BlueprintPure, Category = "ParameterModuleStatics")
	static FVector GetVectorValue(UPARAM(ref) const FParameter& InParameter) { return InParameter.GetVectorValue(); }

	UFUNCTION(BlueprintCallable, Category = "ParameterModuleStatics")
	static void SetVectorValue(UPARAM(ref) FParameter& InParameter, const FVector& InValue) { InParameter.SetVectorValue(InValue); }

	//////////////////////////////////////////////////////////////////////////
	UFUNCTION(BlueprintPure, Category = "ParameterModuleStatics")
	static FRotator GetRotatorValue(UPARAM(ref) const FParameter& InParameter) { return InParameter.GetRotatorValue(); }

	UFUNCTION(BlueprintCallable, Category = "ParameterModuleStatics")
	static void SetRotatorValue(UPARAM(ref) FParameter& InParameter, const FRotator& InValue) { InParameter.SetRotatorValue(InValue); }

	//////////////////////////////////////////////////////////////////////////
	UFUNCTION(BlueprintPure, Category = "ParameterModuleStatics")
	static FColor GetColorValue(UPARAM(ref) const FParameter& InParameter) { return InParameter.GetColorValue(); }

	UFUNCTION(BlueprintCallable, Category = "ParameterModuleStatics")
	static void SetColorValue(UPARAM(ref) FParameter& InParameter, const FColor& InValue) { InParameter.SetColorValue(InValue); }

	//////////////////////////////////////////////////////////////////////////
	UFUNCTION(BlueprintPure, Category = "ParameterModuleStatics")
	static FKey GetKeyValue(UPARAM(ref) const FParameter& InParameter) { return InParameter.GetKeyValue(); }

	UFUNCTION(BlueprintCallable, meta = (AutoCreateRefTerm = "InValue"), Category = "ParameterModuleStatics")
	static void SetKeyValue(UPARAM(ref) FParameter& InParameter, const FKey& InValue) { InParameter.SetKeyValue(InValue); }

	//////////////////////////////////////////////////////////////////////////
	UFUNCTION(BlueprintPure, Category = "ParameterModuleStatics")
	static FGameplayTag GetTagValue(UPARAM(ref) const FParameter& InParameter) { return InParameter.GetTagValue(); }

	UFUNCTION(BlueprintCallable, meta = (AutoCreateRefTerm = "InValue"), Category = "ParameterModuleStatics")
	static void SetTagValue(UPARAM(ref) FParameter& InParameter, const FGameplayTag& InValue) { InParameter.SetTagValue(InValue); }

	//////////////////////////////////////////////////////////////////////////
	UFUNCTION(BlueprintPure, Category = "ParameterModuleStatics")
	static FGameplayTagContainer GetTagsValue(UPARAM(ref) const FParameter& InParameter) { return InParameter.GetTagsValue(); }

	UFUNCTION(BlueprintCallable, Category = "ParameterModuleStatics")
	static void SetTagsValue(UPARAM(ref) FParameter& InParameter, const FGameplayTagContainer& InValue) { InParameter.SetTagsValue(InValue); }

	//////////////////////////////////////////////////////////////////////////
	UFUNCTION(BlueprintPure, Category = "ParameterModuleStatics")
	static UClass* GetClassValue(UPARAM(ref) const FParameter& InParameter) { return InParameter.GetClassValue(); }

	UFUNCTION(BlueprintCallable, Category = "ParameterModuleStatics")
	static void SetClassValue(UPARAM(ref) FParameter& InParameter, UClass* InValue) { InParameter.SetClassValue(InValue); }

	//////////////////////////////////////////////////////////////////////////
	UFUNCTION(BlueprintPure, Category = "ParameterModuleStatics")
	static TSoftClassPtr<UObject> GetClassPtrValue(UPARAM(ref) const FParameter& InParameter) { return InParameter.GetClassPtrValue(); }

	UFUNCTION(BlueprintCallable, Category = "ParameterModuleStatics")
	static void SetClassPtrValue(UPARAM(ref) FParameter& InParameter, const TSoftClassPtr<UObject>& InValue) { InParameter.SetClassPtrValue(InValue); }

	//////////////////////////////////////////////////////////////////////////
	UFUNCTION(BlueprintPure, meta = (DeterminesOutputType = "InClass"), Category = "ParameterModuleStatics")
	static UObject* GetObjectValue(UPARAM(ref) const FParameter& InParameter, TSubclassOf<UObject> InClass = nullptr) { return GetDeterminesOutputObject(InParameter.GetObjectValue(), InClass); }

	UFUNCTION(BlueprintCallable, Category = "ParameterModuleStatics")
	static void SetObjectValue(UPARAM(ref) FParameter& InParameter, UObject* InValue) { InParameter.SetObjectValue(InValue); }

	//////////////////////////////////////////////////////////////////////////
	UFUNCTION(BlueprintPure, Category = "ParameterModuleStatics")
	static TSoftObjectPtr<UObject> GetObjectPtrValue(UPARAM(ref) const FParameter& InParameter) { return InParameter.GetObjectPtrValue(); }

	UFUNCTION(BlueprintCallable, Category = "ParameterModuleStatics")
	static void SetObjectPtrValue(UPARAM(ref) FParameter& InParameter, const TSoftObjectPtr<UObject>& InValue) { InParameter.SetObjectPtrValue(InValue); }

	//////////////////////////////////////////////////////////////////////////
	/// Parameters
public:
	UFUNCTION(BlueprintPure, Category = "ParameterModuleStatics")
	static bool HasParameter(UPARAM(ref) const FParameters& InParameters, FName InName, bool bEnsured = true) { return InParameters.HasParameter(InName, bEnsured); }

	UFUNCTION(BlueprintCallable, Category = "ParameterModuleStatics")
	static void SetParameter(UPARAM(ref) FParameters& InParameters, FName InName, FParameter InParameter){ InParameters.SetParameter(InName, InParameter); }

	UFUNCTION(BlueprintPure, Category = "ParameterModuleStatics")
	static FParameter GetParameter(UPARAM(ref) const FParameters& InParameters, FName InName, bool bEnsured = true){ return InParameters.GetParameter(InName, bEnsured); }

	UFUNCTION(BlueprintPure, Category = "ParameterModuleStatics")
	static TArray<FParameter> GetParameters(UPARAM(ref) const FParameters& InParameters, FName InName, bool bEnsured = true){ return InParameters.GetParameters(InName, bEnsured); }

	UFUNCTION(BlueprintCallable, Category = "ParameterModuleStatics")
	static void RemoveParameter(UPARAM(ref) FParameters& InParameters, FName InName){ InParameters.RemoveParameter(InName); }

	UFUNCTION(BlueprintCallable, Category = "ParameterModuleStatics")
	static void RemoveParameters(UPARAM(ref) FParameters& InParameters, FName InName){ InParameters.RemoveParameters(InName); }

	UFUNCTION(BlueprintCallable, Category = "ParameterModuleStatics")
	static void ClearAllParameter(UPARAM(ref) FParameters& InParameters) { InParameters.ClearAllParameter(); }
};

