// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "Main/Base/ModuleNetworkComponentBase.h"

#include "MediaModuleNetworkComponent.generated.h"

class AMediaPlayerBase;
/**
* 
*/
UCLASS()
class WHFRAMEWORK_API UMediaModuleNetworkComponent : public UModuleNetworkComponentBase
{
	GENERATED_BODY()

public:
	UMediaModuleNetworkComponent();
	
public:
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void ServerPlayMovieMulticast(AMediaPlayerBase* InMediaPlayer, const FName& InName);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void ServerStopMovieMulticast(AMediaPlayerBase* InMediaPlayer, bool bSkip);
};
