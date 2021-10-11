// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameFramework/GameModeBase.h"
#include "WHGameMode.generated.h"

/**
 * 游戏模式基类
 */
UCLASS()
class WHFRAMEWORK_API AWHGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AWHGameMode();

protected:
	virtual void BeginPlay() override;
};
