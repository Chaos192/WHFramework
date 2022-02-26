// Fill out your copyright notice in the Description page of Project Settings.


#include "Main/MainModule.h"

#include "Audio/AudioModule.h"
#include "Asset/AssetModule.h"
#include "Base/ModuleBase.h"
#include "Character/CharacterModule.h"
#include "Debug/DebugModule.h"
#include "Event/EventModule.h"
#include "Event/EventModuleBPLibrary.h"
#include "Event/Handle/Main/EventHandle_ModuleInitialized.h"
#include "Input/InputModule.h"
#include "LatentAction/LatentActionModule.h"
#include "Media/MediaModule.h"
#include "Network/NetworkModule.h"
#include "SpawnPool/SpawnPoolModule.h"
#include "Parameter/ParameterModule.h"
#include "ObjectPool/ObjectPoolModule.h"
#include "WebRequest/WebRequestModule.h"
#include "Net/UnrealNetwork.h"
#include "Procedure/ProcedureModule.h"
#include "SaveGame/SaveGameModule.h"
#include "Scene/SceneModule.h"
#include "Widget/WidgetModule.h"

AMainModule* AMainModule::Current = nullptr;

// ParamSets default values
AMainModule::AMainModule()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(FName("RootComponent"));

	ModuleClasses = TArray<TSubclassOf<AModuleBase>>();
	ModuleClasses.Add(AAssetModule::StaticClass());
	ModuleClasses.Add(AAudioModule::StaticClass());
	ModuleClasses.Add(ACharacterModule::StaticClass());
	ModuleClasses.Add(ADebugModule::StaticClass());
	ModuleClasses.Add(AEventModule::StaticClass());
	ModuleClasses.Add(AInputModule::StaticClass());
	ModuleClasses.Add(ALatentActionModule::StaticClass());
	ModuleClasses.Add(AMediaModule::StaticClass());
	ModuleClasses.Add(ANetworkModule::StaticClass());
	ModuleClasses.Add(AObjectPoolModule::StaticClass());
	ModuleClasses.Add(AParameterModule::StaticClass());
	ModuleClasses.Add(AProcedureModule::StaticClass());
	ModuleClasses.Add(ASaveGameModule::StaticClass());
	ModuleClasses.Add(ASceneModule::StaticClass());
	ModuleClasses.Add(ASpawnPoolModule::StaticClass());
	ModuleClasses.Add(AWebRequestModule::StaticClass());
	ModuleClasses.Add(AWidgetModule::StaticClass());
	
	ModuleRefs = TArray<TScriptInterface<IModule>>();

	ModuleMap = TMap<FName, TScriptInterface<IModule>>();
}

// Called when the game starts or when spawned
void AMainModule::BeginPlay()
{
	Super::BeginPlay();

}

void AMainModule::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	TerminationModules();
}

// Called every frame
void AMainModule::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RefreshModules(DeltaTime);
}

AMainModule* AMainModule::Get()
{
	if(!Current || !Current->IsValidLowLevel())
	{
		TArray<AActor*> Actors;
		UGameplayStatics::GetAllActorsOfClass(GWorld, AMainModule::StaticClass(), Actors);
		if(Actors.IsValidIndex(0))
		{
			Current = Cast<AMainModule>(Actors[0]);
		}
	}
	return Current;
}

#if WITH_EDITOR
void AMainModule::GenerateModules_Implementation()
{
	TArray<AActor*> ChildActors;
	GetAttachedActors(ChildActors);
	for(auto Iter : ChildActors)
	{
		if(Iter->GetClass()->ImplementsInterface(UModule::StaticClass()) && !ModuleRefs.Contains(Iter))
		{
			ModuleRefs.Add(Iter);
		}
	}

	for(int32 i = 0; i < ModuleClasses.Num(); i++)
	{
		if(!ModuleClasses[i]) continue;

		bool IsNeedSpawn = true;
		for(auto Iter : ModuleRefs)
		{
			if(Iter.GetObject() && Iter.GetObject()->IsA(ModuleClasses[i]))
			{
				Iter->Execute_OnGenerate(Iter.GetObject());
				IsNeedSpawn = false;
				break;
			}
		}
		if(!IsNeedSpawn) continue;
		
		FActorSpawnParameters ActorSpawnParameters = FActorSpawnParameters();
		ActorSpawnParameters.Owner = this;
		ActorSpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		if(AModuleBase* Module = GetWorld()->SpawnActor<AModuleBase>(ModuleClasses[i], ActorSpawnParameters))
		{
			Module->SetActorLabel(ModuleClasses[i]->GetDefaultObject<AModuleBase>()->Execute_GetModuleName(ModuleClasses[i]->GetDefaultObject<AModuleBase>()).ToString());
			Module->AttachToActor(this, FAttachmentTransformRules::SnapToTargetIncludingScale);
			Module->Execute_OnGenerate(Module);
			ModuleRefs.Add(Module);
		}
	}

	TArray<TScriptInterface<IModule>> RemoveList;

	for(auto Iter : ModuleRefs)
	{
		if(!Iter.GetObject() || !ModuleClasses.Contains(Iter.GetObject()->GetClass()))
		{
			RemoveList.Add(Iter);
			if(AActor* Actor = Cast<AActor>(Iter.GetObject()))
			{
				Actor->Destroy();
			}
		}
	}

	for(auto Iter : RemoveList)
	{
		ModuleRefs.Remove(Iter);
	}

	Modify();
}

void AMainModule::DestroyModules_Implementation()
{
	for(int32 i = 0; i < ModuleRefs.Num(); i++)
	{
		if(ModuleRefs[i] && ModuleRefs[i].GetObject()->IsValidLowLevel())
		{
			ModuleRefs[i]->Execute_OnDestroy(ModuleRefs[i].GetObject());
			if(AActor* Actor = Cast<AActor>(ModuleRefs[i].GetObject()))
			{
				Actor->Destroy();
			}
		}
	}
	ModuleRefs.Empty();
	Modify();
}
#endif

void AMainModule::InitializeModules_Implementation()
{
	for(int32 i = 0; i < ModuleRefs.Num(); i++)
	{
		if(ModuleRefs[i] && ModuleRefs[i].GetObject()->IsValidLowLevel())
		{
			ModuleRefs[i]->Execute_OnInitialize(ModuleRefs[i].GetObject());
			ModuleMap.Add(ModuleRefs[i]->Execute_GetModuleName(ModuleRefs[i].GetObject()), ModuleRefs[i]);
		}
	}
	UEventModuleBPLibrary::BroadcastEvent(UEventHandle_ModuleInitialized::StaticClass(), EEventNetType::Multicast, this, TArray<FParameter>());
}

void AMainModule::RefreshModules_Implementation(float DeltaSeconds)
{
	for(int32 i = 0; i < ModuleRefs.Num(); i++)
	{
		if(ModuleRefs[i] && ModuleRefs[i].GetObject()->IsValidLowLevel() && ModuleRefs[i]->Execute_GetModuleState(ModuleRefs[i].GetObject()) == EModuleState::Executing)
		{
			ModuleRefs[i]->Execute_OnRefresh(ModuleRefs[i].GetObject(), DeltaSeconds);
		}
	}
}

void AMainModule::PauseModules_Implementation()
{
	for(int32 i = 0; i < ModuleRefs.Num(); i++)
	{
		if(ModuleRefs[i] && ModuleRefs[i].GetObject()->IsValidLowLevel())
		{
			ModuleRefs[i]->Execute_Pause(ModuleRefs[i].GetObject());
		}
	}
}

void AMainModule::UnPauseModules_Implementation()
{
	for(int32 i = 0; i < ModuleRefs.Num(); i++)
	{
		if(ModuleRefs[i] && ModuleRefs[i].GetObject()->IsValidLowLevel())
		{
			ModuleRefs[i]->Execute_UnPause(ModuleRefs[i].GetObject());
		}
	}
}

void AMainModule::TerminationModules_Implementation()
{
	for(int32 i = 0; i < ModuleRefs.Num(); i++)
	{
		if(ModuleRefs[i] && ModuleRefs[i].GetObject()->IsValidLowLevel())
		{
			ModuleRefs[i]->Execute_OnTermination(ModuleRefs[i].GetObject());
		}
	}
	ModuleMap.Empty();
}

void AMainModule::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMainModule, ModuleRefs);
}
