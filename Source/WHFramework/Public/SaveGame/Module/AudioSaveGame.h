// Copyright 2021 Sam Carey. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Audio/AudioModuleTypes.h"
#include "SaveGame/Base/SaveGameBase.h"
#include "AudioSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class WHFRAMEWORK_API UAudioSaveGame : public USaveGameBase
{
	GENERATED_BODY()
	
public:
	UAudioSaveGame();

public:
	virtual void OnCreate_Implementation(int32 InIndex) override;
	
	virtual void OnLoad_Implementation(EPhase InPhase) override;
	
	virtual void OnUnload_Implementation(EPhase InPhase) override;

	virtual void OnRefresh_Implementation() override;
	
protected:
	UPROPERTY()
	FAudioModuleSaveData SaveData;

public:
	virtual FSaveData* GetSaveData() override { return &SaveData; }

	virtual void SetSaveData(FSaveData* InSaveData) override { SaveData = InSaveData->CastRef<FAudioModuleSaveData>(); }
};
