// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Ability/AbilityModuleTypes.h"
#include "GameFramework/Actor.h"
#include "AbilityItemBase.generated.h"

class AAbilityCharacterBase;

/**
 * 物品基类
 */
UCLASS()
class WHFRAMEWORK_API AAbilityItemBase : public AWHActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAbilityItemBase();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FAbilityItem Item;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AAbilityCharacterBase* OwnerCharacter;

protected:
	virtual int32 GetLimit_Implementation() const override { return 1000; }

	virtual void OnSpawn_Implementation(const TArray<FParameter>& InParams) override;

	virtual void OnDespawn_Implementation(bool bRecovery) override;

public:
	UFUNCTION(BlueprintNativeEvent)
	void Initialize(AAbilityCharacterBase* InOwnerCharacter, const FAbilityItem& InItem = FAbilityItem::Empty);

public:
	template<class T >
	T& GetItemData() const
	{
		return Item.GetData<T>();
	}

	UAbilityItemDataBase& GetItemData() const
	{
		return Item.GetData();
	}

	FAbilityItem& GetItem() { return Item; }

	template<class T>
	T* GetOwnerCharacter() const
	{
		return Cast<T>(OwnerCharacter);
	}

	UFUNCTION(BlueprintPure)
	AAbilityCharacterBase* GetOwnerCharacter() const { return OwnerCharacter; }
};
