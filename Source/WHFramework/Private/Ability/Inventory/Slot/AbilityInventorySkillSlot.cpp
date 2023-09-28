// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "Ability/Inventory/Slot/AbilityInventorySkillSlot.h"

#include "Ability/Inventory/AbilityInventoryBase.h"
#include "Ability/Inventory/Slot/AbilityInventorySlot.h"
#include "Ability/Item/Skill/AbilitySkillDataBase.h"

UAbilityInventorySkillSlot::UAbilityInventorySkillSlot()
{
}

void UAbilityInventorySkillSlot::OnInitialize(UAbilityInventoryBase* InInventory, EAbilityItemType InLimitType, ESlotSplitType InSplitType, int32 InSlotIndex)
{
	Super::OnInitialize(InInventory, InLimitType, InSplitType, InSlotIndex);
}

void UAbilityInventorySkillSlot::OnItemPreChange(FAbilityItem& InNewItem)
{
	Super::OnItemPreChange(InNewItem);
	if(Item.IsValid() && Item.GetData<UAbilitySkillDataBase>().SkillMode == ESkillMode::Passive)
	{
		CancelItem(true);
	}
}

void UAbilityInventorySkillSlot::OnItemChanged(FAbilityItem& InOldItem)
{
	Super::OnItemChanged(InOldItem);
	if(Item.IsValid() && Item.GetData<UAbilitySkillDataBase>().SkillMode == ESkillMode::Passive)
	{
		ActiveItem(true);
	}
}

bool UAbilityInventorySkillSlot::ActiveItem(bool bPassive /*= false*/)
{
	return Super::ActiveItem(bPassive);
}

void UAbilityInventorySkillSlot::CancelItem(bool bPassive /*= false*/)
{
	return Super::CancelItem(bPassive);
}
