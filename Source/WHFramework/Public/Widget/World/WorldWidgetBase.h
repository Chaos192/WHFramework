// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "Blueprint/UserWidget.h"
#include "Common/CommonTypes.h"
#include "Components/WidgetComponent.h"
#include "Input/InputModuleTypes.h"
#include "ObjectPool/ObjectPoolInterface.h"
#include "Parameter/ParameterModuleTypes.h"
#include "Widget/WidgetModuleTypes.h"
#include "Widget/Interfaces/PanelWidgetInterface.h"

#include "WorldWidgetBase.generated.h"

class UCanvasPanelSlot;
/**
 * 
 */
UCLASS(BlueprintType, meta = (DisableNativeTick))
class WHFRAMEWORK_API UWorldWidgetBase : public UUserWidget, public IPanelWidgetInterface, public IObjectPoolInterface
{
	friend class UWidgetModule;
	
	GENERATED_BODY()

public:
	UWorldWidgetBase(const FObjectInitializer& ObjectInitializer);
	
protected:
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
	virtual FReply NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
	virtual FReply NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
	virtual FReply NativeOnTouchGesture(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
	
	virtual FReply NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
	
	virtual FReply NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
	
	virtual FReply NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;

protected:
	UPROPERTY(EditDefaultsOnly)
	bool bWidgetTickAble;

public:
	virtual bool IsTickAble_Implementation() const override { return bWidgetTickAble; }

	virtual void OnTick_Implementation(float DeltaSeconds) override;

public:
	virtual int32 GetLimit_Implementation() const override { return 0; }

	virtual void OnSpawn_Implementation(UObject* InOwner, const TArray<FParameter>& InParams) override;

	virtual void OnDespawn_Implementation(bool bRecovery) override;

public:
	UFUNCTION(BlueprintImplementableEvent, DisplayName = "OnCreate")
	void K2_OnCreate(UObject* InOwner, const TArray<FParameter>& InParams);
	UFUNCTION()
	virtual void OnCreate(UObject* InOwner, FWorldWidgetMapping InMapping, const TArray<FParameter>& InParams);
	
	UFUNCTION(BlueprintImplementableEvent, DisplayName = "OnRefresh")
	void K2_OnRefresh();
	UFUNCTION()
	virtual void OnRefresh();
	
	UFUNCTION(BlueprintImplementableEvent, DisplayName = "OnDestroy")
	void K2_OnDestroy(bool bRecovery = false);
	UFUNCTION()
	virtual void OnDestroy(bool bRecovery = false);

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Refresh();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Destroy(bool bRecovery = false);

protected:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void RefreshLocation(UWidget* InWidget, FWorldWidgetMapping InMapping);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void RefreshVisibility();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void RefreshLocationAndVisibility();

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void BindWidgetPoint(UWidget* InWidget, FWorldWidgetMapping InMapping);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void UnBindWidgetPoint(UWidget* InWidget);
	
	UFUNCTION(BlueprintPure, BlueprintNativeEvent)
	bool GetWidgetMapping(UWidget* InWidget, FWorldWidgetMapping& OutMapping);
	
	UFUNCTION(BlueprintPure, BlueprintNativeEvent)
	bool IsWidgetVisible(bool bRefresh = false);

protected:
	UPROPERTY(EditDefaultsOnly)
	FName WidgetName;

	UPROPERTY(EditDefaultsOnly)
	EWidgetSpace WidgetSpace;

	UPROPERTY(EditDefaultsOnly, meta = (EditConditionHides, EditCondition = "WidgetSpace == EWidgetSpace::Screen"))
	int32 WidgetZOrder;

	UPROPERTY(EditDefaultsOnly, meta = (EditConditionHides, EditCondition = "WidgetSpace == EWidgetSpace::Screen"))
	FAnchors WidgetAnchors;

	UPROPERTY(EditDefaultsOnly)
	bool bWidgetPenetrable;

	UPROPERTY(EditDefaultsOnly)
	bool bWidgetAutoSize;

	UPROPERTY(EditDefaultsOnly, meta = (EditConditionHides, EditCondition = "WidgetSpace == EWidgetSpace::World && bWidgetAutoSize == false"))
	FVector2D WidgetDrawSize;
	
	UPROPERTY(EditDefaultsOnly, meta = (EditConditionHides, EditCondition = "WidgetSpace == EWidgetSpace::Screen && bWidgetAutoSize == false"))
	FMargin WidgetOffsets;

	UPROPERTY(EditDefaultsOnly)
	FVector2D WidgetAlignment;

	UPROPERTY(EditDefaultsOnly)
	EWidgetRefreshType WidgetRefreshType;

	UPROPERTY(EditDefaultsOnly, meta = (EditConditionHides, EditCondition = "WidgetRefreshType == EWidgetRefreshType::Timer"))
	float WidgetRefreshTime;
	
	UPROPERTY(EditDefaultsOnly)
	EWorldWidgetVisibility WidgetVisibility;
	
	UPROPERTY(EditDefaultsOnly, meta = (EditConditionHides, EditCondition = "WidgetVisibility == EWorldWidgetVisibility::DistanceOnly || WidgetVisibility == EWorldWidgetVisibility::RenderAndDistance || WidgetVisibility == EWorldWidgetVisibility::ScreenAndDistance"))
	float WidgetShowDistance;

	UPROPERTY(EditDefaultsOnly)
	TArray<FParameter> WidgetParams;

	UPROPERTY(EditDefaultsOnly)
	EInputMode WidgetInputMode;

	UPROPERTY(Transient)
	UObject* OwnerObject;
	
	UPROPERTY(Transient)
	int32 WidgetIndex;
	
	UPROPERTY(Transient)
	class UWorldWidgetComponent* WidgetComponent;

	UPROPERTY(Transient)
	TMap<UWidget*, FWorldWidgetMapping> BindWidgetMap;

public:
	UPROPERTY(BlueprintAssignable, meta = (DisplayName = "OnDestroyed"))
	FK2_OnWidgetDestroyed K2_OnDestroyed;

	FOnWidgetDestroyed OnDestroyed;

private:
	FTimerHandle RefreshTimerHandle;
	
public:
	UFUNCTION(BlueprintPure)
	virtual FName GetWidgetName() const override { return WidgetName; }
	
	UFUNCTION(BlueprintPure)
	virtual int32 GetWidgetZOrder() const override { return WidgetZOrder; }

	UFUNCTION(BlueprintPure)
	virtual FAnchors GetWidgetAnchors() const override { return WidgetAnchors; }
	
	UFUNCTION(BlueprintPure)
	virtual bool IsWidgetPenetrable() const override { return bWidgetPenetrable; }

	UFUNCTION(BlueprintPure)
	virtual bool IsWidgetAutoSize() const override { return bWidgetAutoSize; }

	UFUNCTION(BlueprintPure)
	virtual FVector2D GetWidgetDrawSize() const override { return WidgetDrawSize; }

	UFUNCTION(BlueprintPure)
	virtual FMargin GetWidgetOffsets() const override { return WidgetOffsets; }

	UFUNCTION(BlueprintPure)
	virtual FVector2D GetWidgetAlignment() const override { return WidgetAlignment; }

	UFUNCTION(BlueprintPure)
	virtual EWidgetRefreshType GetWidgetRefreshType() const override { return WidgetRefreshType; }
	
	UFUNCTION(BlueprintPure)
	TArray<FParameter> GetWidgetParams() const { return WidgetParams; }

	UFUNCTION(BlueprintPure)
	virtual EInputMode GetWidgetInputMode() const override { return WidgetInputMode; }

	template<class T>
	T* GetOwnerObject() const
	{
		return Cast<T>(OwnerObject);
	}

	virtual UObject* GetOwnerObject() const override { return OwnerObject; }

	UFUNCTION(BlueprintPure, meta = (DeterminesOutputType = "InClass"))
	virtual UObject* GetOwnerObject(TSubclassOf<UObject> InClass) const { return GetDeterminesOutputObject(OwnerObject, InClass); }

	UFUNCTION(BlueprintPure)
	virtual int32 GetWidgetIndex() const { return WidgetIndex; }

	UFUNCTION(BlueprintCallable)
	virtual void SetWidgetIndex(int32 InIndex) { WidgetIndex = InIndex; }

	UFUNCTION(BlueprintPure)
	UWorldWidgetComponent* GetWidgetComponent() const { return WidgetComponent; }

	UFUNCTION(BlueprintPure)
	EWidgetSpace GetWidgetSpace() const;
	
	UFUNCTION(BlueprintPure)
	UPanelWidget* GetRootPanelWidget() const;

	UFUNCTION(BlueprintPure)
	TArray<UWidget*> GetAllPoolWidgets() const;
};
