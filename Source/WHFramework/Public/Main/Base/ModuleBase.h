// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "Common/Base/WHObject.h"
#include "Main/MainModuleTypes.h"
#include "SaveGame/Base/SaveDataInterface.h"
#include "ModuleBase.generated.h"

class AMainModule;
class USaveGameBase;
class UModuleNetworkComponentBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FModuleStateChanged, EModuleState, InModuleState);

UCLASS()
class WHFRAMEWORK_API UModuleBase : public UWHObject, public ISaveDataInterface
{
	GENERATED_BODY()

	friend class AWHPlayerController;
	friend class AMainModule;
	
public:	
	// ParamSets default values for this actor's properties
	UModuleBase();

	//////////////////////////////////////////////////////////////////////////
	/// Module
public:
#if WITH_EDITOR
	/**
	* 当构建
	*/
	virtual void OnGenerate();
	/**
	* 当销毁
	*/
	virtual void OnDestroy();
#endif
	/**
	* 当初始化
	*/
	UFUNCTION(BlueprintImplementableEvent, DisplayName = "OnInitialize")
	void K2_OnInitialize();
	UFUNCTION()
	virtual void OnInitialize();
	/**
	* 当准备
	*/
	UFUNCTION(BlueprintImplementableEvent, DisplayName = "OnPreparatory")
	void K2_OnPreparatory(EPhase InPhase);
	UFUNCTION()
	virtual void OnPreparatory(EPhase InPhase);
	/**
	* 当重置
	*/
	UFUNCTION(BlueprintImplementableEvent, DisplayName = "OnReset")
	void K2_OnReset();
	UFUNCTION()
	virtual void OnReset();
	/**
	* 当暂停
	*/
	UFUNCTION(BlueprintImplementableEvent, DisplayName = "OnPause")
	void K2_OnPause();
	UFUNCTION()
	virtual void OnPause();
	/**
	* 当恢复
	*/
	UFUNCTION(BlueprintImplementableEvent, DisplayName = "OnUnPause")
	void K2_OnUnPause();
	UFUNCTION()
	virtual void OnUnPause();
	/**
	* 当刷新
	*/
	UFUNCTION(BlueprintImplementableEvent, DisplayName = "OnRefresh")
	void K2_OnRefresh(float DeltaSeconds);
	UFUNCTION()
	virtual void OnRefresh(float DeltaSeconds);
	/**
	* 当暂停
	*/
	UFUNCTION(BlueprintImplementableEvent, DisplayName = "OnTermination")
	void K2_OnTermination(EPhase InPhase);
	UFUNCTION()
	virtual void OnTermination(EPhase InPhase);
	/**
	* 当状态改变
	*/
	UFUNCTION(BlueprintImplementableEvent, DisplayName = "OnStateChanged")
	void K2_OnStateChanged(EModuleState InModuleState);
	UFUNCTION()
	virtual void OnStateChanged(EModuleState InModuleState);

protected:
	virtual void LoadData(FSaveData* InSaveData, EPhase InPhase) override;

	virtual void UnloadData(EPhase InPhase) override;

	virtual FSaveData* ToData() override;

public:
	/**
	* 加载
	*/
	UFUNCTION(BlueprintNativeEvent)
	void Load();
	/**
	* 保存
	*/
	UFUNCTION(BlueprintNativeEvent)
	void Save();
	/**
	* 运行
	*/
	UFUNCTION(BlueprintNativeEvent)
	void Run();
	/**
	* 重置
	*/
	UFUNCTION(BlueprintNativeEvent)
	void Reset();
	/**
	* 暂停
	*/
	UFUNCTION(BlueprintNativeEvent)
	void Pause();
	/**
	* 恢复
	*/
	UFUNCTION(BlueprintNativeEvent)
	void UnPause();
	/**
	* 结束
	*/
	UFUNCTION(BlueprintNativeEvent)
	void Termination();

protected:
	/// 模块名称
	UPROPERTY(EditDefaultsOnly)
	FName ModuleName;
	/// 模块显示名称
	UPROPERTY(EditAnywhere)
	FText ModuleDisplayName;
	/// 模块描述
	UPROPERTY(EditAnywhere, meta = (MultiLine = "true"))
	FText ModuleDescription;
	/// 模块状态
	UPROPERTY(VisibleAnywhere, Replicated)
	EModuleState ModuleState;
	/// 模块索引
	UPROPERTY(VisibleAnywhere)
	int32 ModuleIndex;
	/// 自动运行
	UPROPERTY(EditAnywhere)
	bool bModuleAutoRun;
	/// 自动保存
	UPROPERTY(EditAnywhere)
	bool bModuleAutoSave;
	/// 模块存档
	UPROPERTY(EditAnywhere, meta = (EditConditionHides, EditCondition = "bModuleAutoSave == true"))
	TSubclassOf<USaveGameBase> ModuleSaveGame;
	/// 模块网络组件
	UPROPERTY(EditAnywhere)
	TSubclassOf<UModuleNetworkComponentBase> ModuleNetworkComponent;

public:
	UPROPERTY(BlueprintAssignable)
	FModuleStateChanged OnModuleStateChanged;

public:
	/**
	* 是否自动运行
	*/
	UFUNCTION(BlueprintPure)
	bool IsAutoRunModule() const;
	/**
	* 获取模块名称
	*/
	UFUNCTION(BlueprintPure)
	FName GetModuleName() const;
	/**
	* 获取模块显示名称
	*/
	UFUNCTION(BlueprintPure)
	FText GetModuleDisplayName() const;
	/**
	* 获取模块描述
	*/
	UFUNCTION(BlueprintPure)
	FText GetModuleDescription() const;
	/**
	* 获取模块状态
	*/
	UFUNCTION(BlueprintPure)
	EModuleState GetModuleState() const;
	/**
	* 获取模块索引
	*/
	UFUNCTION(BlueprintPure)
	int32 GetModuleIndex() const;
	/**
	* 获取模块拥有者
	*/
	UFUNCTION(BlueprintPure)
	AMainModule* GetModuleOwner() const;
	/**
	* 获取模块存档
	*/
	template<class T>
	static T* GetModuleSaveGame()
	{
		return Cast<T>(GetModuleSaveGame());
	}
	UFUNCTION(BlueprintPure)
	USaveGameBase* GetModuleSaveGame() const;
	/**
	* 获取模块网络组件
	*/
	template<class T>
	T* GetModuleNetworkComponent()
	{
		return Cast<T>(GetModuleNetworkComponent());
	}
	UFUNCTION(BlueprintPure)
	UModuleNetworkComponentBase* GetModuleNetworkComponent() const;

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//////////////////////////////////////////////////////////////////////////
	/// ModuleListItem
public:
	UPROPERTY()
	FModuleListItemStates ModuleListItemStates;
public:
	/**
	* 构建流程列表项
	*/
	virtual void GenerateListItem(TSharedPtr<struct FModuleListItem> OutModuleListItem);
	/**
	* 更新流程列表项
	*/
	virtual void UpdateListItem(TSharedPtr<struct FModuleListItem> OutModuleListItem);

#if WITH_EDITOR
	virtual bool CanEditChange(const FProperty* InProperty) const override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};

/**
 * 流程列表项
 */ 
struct FModuleListItem : public TSharedFromThis<FModuleListItem>
{
public:
	FModuleListItem()
	{
		Module = nullptr;
	}

	UModuleBase* Module;

public:
	FModuleListItemStates& GetStates() const
	{
		return Module->ModuleListItemStates;
	}

	int32 GetModuleIndex() const
	{
		return Module->GetModuleIndex();
	}
};
