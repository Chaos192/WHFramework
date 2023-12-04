// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Base/ProcedureAsset.h"
#include "Base/ProcedureBase.h"
#include "Main/Base/ModuleBase.h"

#include "ProcedureModule.generated.h"

class UEventHandle_SwitchProcedure;
class UProcedureBase;

/**
 * 
 */
UCLASS()
class WHFRAMEWORK_API UProcedureModule : public UModuleBase
{
	GENERATED_BODY()
		
	GENERATED_MODULE(UProcedureModule)

	friend class FProcedureModuleDetailsPanel;

public:
	// ParamSets default values for this actor's properties
	UProcedureModule();

	~UProcedureModule();

	//////////////////////////////////////////////////////////////////////////
	/// Module
public:
	#if WITH_EDITOR
	virtual void OnGenerate() override;

	virtual void OnDestroy() override;
	#endif

	virtual void OnInitialize() override;

	virtual void OnPreparatory(EPhase InPhase) override;

	virtual void OnRefresh(float DeltaSeconds) override;

	virtual void OnPause() override;

	virtual void OnUnPause() override;

	virtual void OnTermination(EPhase InPhase) override;

	//////////////////////////////////////////////////////////////////////////
	/// ProcedureModule
protected:
	/// 自动切换初始流程 
	UPROPERTY(EditAnywhere, Category = "ProcedureModule")
	bool bAutoSwitchFirst;

protected:
	UFUNCTION(BlueprintCallable)
	void OnSwitchProcedure(UObject* InSender, UEventHandle_SwitchProcedure* InEventHandle);

public:
	UFUNCTION(BlueprintCallable)
	void SwitchProcedure(UProcedureBase* InProcedure);

	UFUNCTION(BlueprintCallable)
	void SwitchProcedureByIndex(int32 InIndex);

	template<class T>
	void SwitchProcedureByClass() { SwitchProcedureByClass(T::StaticClass()); }

	UFUNCTION(BlueprintCallable)
	void SwitchProcedureByClass(TSubclassOf<UProcedureBase> InClass);

	UFUNCTION(BlueprintCallable)
	void SwitchFirstProcedure();

	UFUNCTION(BlueprintCallable)
	void SwitchLastProcedure();

	UFUNCTION(BlueprintCallable)
	void SwitchNextProcedure();

	UFUNCTION(BlueprintCallable)
	void GuideCurrentProcedure();

	//////////////////////////////////////////////////////////////////////////
	/// Procedure Stats
protected:
	/// 默认流程资产 
	UPROPERTY(EditAnywhere, Category = "ProcedureModule|Procedure Stats")
	UProcedureAsset* DefaultAsset;
	/// 当前流程资产 
	UPROPERTY(VisibleAnywhere, Category = "ProcedureModule|Procedure Stats")
	UProcedureAsset* CurrentAsset;
	/// 当前流程 
	UPROPERTY(VisibleAnywhere, Transient, Category = "ProcedureModule|Procedure Stats")
	UProcedureBase* CurrentProcedure;

public:
	/**
	* 获取默认资产
	*/
	UFUNCTION(BlueprintPure)
	UProcedureAsset* GetDefaultAsset() const { return DefaultAsset; }
	/**
	* 获取当前资产
	*/
	UFUNCTION(BlueprintPure)
	UProcedureAsset* GetCurrentAsset() const { return CurrentAsset; }
	/**
	* 设置当前资产
	*/
	UFUNCTION(BlueprintCallable)
	void SetCurrentAsset(UProcedureAsset* InProcedureAsset, bool bInAutoSwitchFirst = false);

	UFUNCTION(BlueprintPure)
	bool HasProcedureByIndex(int32 InIndex) const;

	template<class T>
	T* GetProcedureByIndex(int32 InIndex) const
	{
		if(HasProcedureByIndex(InIndex))
		{
			return Cast<T>(CurrentAsset->Procedures[InIndex]);
		}
		return nullptr;
	}
	
	UFUNCTION(BlueprintPure, meta = (DeterminesOutputType = "InIndex"))
	UProcedureBase* GetProcedureByIndex(int32 InIndex, TSubclassOf<UProcedureBase> InClass) const;
	
	template<class T>
	bool HasProcedureByClass(TSubclassOf<UProcedureBase> InClass = T::StaticClass()) const
	{
		if(!InClass) return false;

		return CurrentAsset->ProcedureMap.Contains(InClass);
	}
	
	UFUNCTION(BlueprintPure)
	bool HasProcedureByClass(TSubclassOf<UProcedureBase> InClass) const;

	template<class T>
	T* GetProcedureByClass(TSubclassOf<UProcedureBase> InClass = T::StaticClass()) const
	{
		if(!InClass) return nullptr;

		if(HasProcedureByClass<T>(InClass))
		{
			return Cast<T>(CurrentAsset->ProcedureMap[InClass]);
		}
		return nullptr;
	}
	
	UFUNCTION(BlueprintPure, meta = (DeterminesOutputType = "InClass"))
	UProcedureBase* GetProcedureByClass(TSubclassOf<UProcedureBase> InClass) const;
		
	UFUNCTION(BlueprintPure)
	bool IsCurrentProcedure(UProcedureBase* InProcedure) const;

	UFUNCTION(BlueprintPure)
	bool IsCurrentProcedureIndex(int32 InIndex) const;

	template<class T>
	bool IsCurrentProcedureClass() const
	{
		return IsCurrentProcedureClass(T::StaticClass());
	}

	UFUNCTION(BlueprintPure)
	bool IsCurrentProcedureClass(TSubclassOf<UProcedureBase> InClass) const
	{
		if(!InClass) return false;

		return CurrentProcedure && CurrentProcedure->GetClass() == InClass;
	}

public:
	/**
	* 获取初始流程
	*/
	UFUNCTION(BlueprintPure)
	UProcedureBase* GetFirstProcedure() const { return CurrentAsset ? CurrentAsset->FirstProcedure : nullptr; }
	/**
	* 获取当前流程
	*/
	template<class T>
	T* GetCurrentProcedure() const
	{
		return Cast<T>(CurrentProcedure);
	}
	UFUNCTION(BlueprintPure, meta = (DeterminesOutputType = "InClass"))
	UProcedureBase* GetCurrentProcedure(TSubclassOf<UProcedureBase> InClass = nullptr) const { return GetDeterminesOutputObject(CurrentProcedure, InClass); }
	/**
	* 获取流程列表
	*/
	UFUNCTION(BlueprintPure)
	TArray<UProcedureBase*> GetProcedures() const { return CurrentAsset ? CurrentAsset->Procedures : TArray<UProcedureBase*>(); }
	/**
	* 获取流程Map
	*/
	UFUNCTION(BlueprintPure)
	TMap<TSubclassOf<UProcedureBase>, UProcedureBase*> GetProcedureMap() const { return CurrentAsset ? CurrentAsset->ProcedureMap : TMap<TSubclassOf<UProcedureBase>, UProcedureBase*>(); }
};
