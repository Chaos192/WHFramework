// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "ProcedureModuleTypes.h"
#include "Main/Base/ModuleBase.h"

#include "ProcedureModule.generated.h"

class UProcedureBase;
class URootProcedureBase;

/**
 * 
 */

UCLASS()
class WHFRAMEWORK_API AProcedureModule : public AModuleBase
{
	GENERATED_BODY()

public:	
	// ParamSets default values for this actor's properties
	AProcedureModule();

	//////////////////////////////////////////////////////////////////////////
	/// Module
public:
#if WITH_EDITOR
	virtual void OnGenerate_Implementation() override;

	virtual void OnDestroy_Implementation() override;
#endif

	virtual void OnInitialize_Implementation() override;

	virtual void OnPreparatory_Implementation() override;

	virtual void OnRefresh_Implementation(float DeltaSeconds) override;

	virtual void OnPause_Implementation() override;

	virtual void OnUnPause_Implementation() override;

	//////////////////////////////////////////////////////////////////////////
	/// Procedure
public:
	UFUNCTION(BlueprintCallable)
	void StartProcedure(int32 InRootProcedureIndex = -1, bool bSkipProcedures = false);

	UFUNCTION(BlueprintCallable)
	void EndProcedure(bool bRestoreProcedures = false);

	UFUNCTION(BlueprintCallable)
	void RestoreProcedure(UProcedureBase* InProcedure);

	UFUNCTION(BlueprintCallable)
	void EnterProcedure(UProcedureBase* InProcedure);

	UFUNCTION(BlueprintCallable)
	void GuideProcedure(UProcedureBase* InProcedure);

	UFUNCTION(BlueprintCallable)
	void ExecuteProcedure(UProcedureBase* InProcedure);

	UFUNCTION(BlueprintCallable)
	void CompleteProcedure(UProcedureBase* InProcedure, EProcedureExecuteResult InProcedureExecuteResult = EProcedureExecuteResult::Succeed);

	UFUNCTION(BlueprintCallable)
	void LeaveProcedure(UProcedureBase* InProcedure);

	//////////////////////////////////////////////////////////////////////////
	/// Editor
public:
#if WITH_EDITOR
	UFUNCTION(CallInEditor, Category = "ProcedureModule")
	void OpenProcedureEditor();

	UFUNCTION(CallInEditor, Category = "ProcedureModule")
	void ClearAllProcedure();

	void GenerateListItem(TArray<TSharedPtr<struct FProcedureListItem>>& OutProcedureListItems);

	void UpdateListItem(TArray<TSharedPtr<struct FProcedureListItem>>& OutProcedureListItems);
#endif

	//////////////////////////////////////////////////////////////////////////
	/// Root Procedure
protected:
	/// 初始根流程索引
	UPROPERTY(EditAnywhere, Category = "ProcedureModule|Root Procedure")
	int32 FirstRootProcedureIndex;
	/// 当前根流程索引
	UPROPERTY(VisibleAnywhere, Category = "ProcedureModule|Root Procedure")
	int32 CurrentRootProcedureIndex;
	/// 根流程
	UPROPERTY(VisibleAnywhere, Category = "ProcedureModule|Root Procedure")
	TArray<URootProcedureBase*> RootProcedures;
	
public:
	UFUNCTION(BlueprintPure)
	int32 GetCurrentRootProcedureIndex() const { return CurrentRootProcedureIndex; }
	/**
	* 获取当前根流程
	*/
	UFUNCTION(BlueprintPure)
	URootProcedureBase* GetCurrentRootProcedure() const
	{
		if(RootProcedures.IsValidIndex(CurrentRootProcedureIndex))
		{
			return RootProcedures[CurrentRootProcedureIndex];
		}
		return nullptr;
	}

	UFUNCTION(BlueprintPure)
	TArray<URootProcedureBase*>& GetRootProcedures() { return RootProcedures; }

	//////////////////////////////////////////////////////////////////////////
	/// Current Procedure
protected:
	/// 当前流程 
	UPROPERTY(VisibleAnywhere, Transient, Category = "ProcedureModule|Current Procedure")
	UProcedureBase* CurrentProcedure;
public:
	/**
	* 获取当前流程
	*/
	UFUNCTION(BlueprintPure)
	UProcedureBase* GetCurrentProcedure() const { return CurrentProcedure; }
	
	//////////////////////////////////////////////////////////////////////////
	/// Global Options
protected:
	/// 流程执行方式
	UPROPERTY(EditAnywhere, Category = "ProcedureModule|Global Options")
	EProcedureExecuteType GlobalProcedureExecuteType;
	/// 流程完成方式
	UPROPERTY(EditAnywhere, Category = "ProcedureModule|Global Options")
	EProcedureCompleteType GlobalProcedureCompleteType;
	/// 流程离开方式
	UPROPERTY(EditAnywhere, Category = "ProcedureModule|Global Options")
	EProcedureLeaveType GlobalProcedureLeaveType;
	
public:
	UFUNCTION(BlueprintPure)
	EProcedureExecuteType GetGlobalProcedureExecuteType() const { return GlobalProcedureExecuteType; }

	UFUNCTION(BlueprintCallable)
	void SetGlobalProcedureExecuteType(EProcedureExecuteType InGlobalProcedureExecuteType) { this->GlobalProcedureExecuteType = InGlobalProcedureExecuteType; }

	UFUNCTION(BlueprintPure)
	EProcedureCompleteType GetGlobalProcedureCompleteType() const { return GlobalProcedureCompleteType; }

	UFUNCTION(BlueprintCallable)
	void SetGlobalProcedureCompleteType(EProcedureCompleteType InGlobalProcedureCompleteType) { this->GlobalProcedureCompleteType = InGlobalProcedureCompleteType; }

	UFUNCTION(BlueprintPure)
	EProcedureLeaveType GetGlobalProcedureLeaveType() const { return GlobalProcedureLeaveType; }

	UFUNCTION(BlueprintCallable)
	void SetGlobalProcedureLeaveType(EProcedureLeaveType InGlobalProcedureLeaveType) { this->GlobalProcedureLeaveType = InGlobalProcedureLeaveType; }
};
