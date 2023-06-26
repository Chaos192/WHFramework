// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/WHPlayerController.h"

#include "Net/UnrealNetwork.h"
#include "Audio/AudioModuleNetworkComponent.h"
#include "Camera/CameraModule.h"
#include "Camera/CameraModuleNetworkComponent.h"
#include "Camera/Base/CameraPawnBase.h"
#include "Character/CharacterModuleNetworkComponent.h"
#include "Components/WidgetInteractionComponent.h"
#include "Debug/DebugModuleTypes.h"
#include "Event/EventModuleNetworkComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Gameplay/WHPlayerInterface.h"
#include "Global/GlobalBPLibrary.h"
#include "Input/InputModule.h"
#include "Input/InputModuleBPLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Main/MainModule.h"
#include "Math/MathBPLibrary.h"
#include "Media/MediaModuleNetworkComponent.h"
#include "Network/NetworkModuleNetworkComponent.h"
#include "Procedure/ProcedureModule.h"
#include "Procedure/ProcedureModuleNetworkComponent.h"

AWHPlayerController::AWHPlayerController()
{
	WidgetInteractionComp = CreateDefaultSubobject<UWidgetInteractionComponent>(FName("WidgetInteractionComp"));
	WidgetInteractionComp->SetupAttachment(RootComponent);

	ModuleNetCompMap = TMap<TSubclassOf<UModuleNetworkComponentBase>, UModuleNetworkComponentBase*>();
	
	AudioModuleNetComp = CreateDefaultSubobject<UAudioModuleNetworkComponent>(FName("AudioModuleNetComp"));
	ModuleNetCompMap.Add(UAudioModuleNetworkComponent::StaticClass(), AudioModuleNetComp);
	
	CameraModuleNetComp = CreateDefaultSubobject<UCameraModuleNetworkComponent>(FName("CameraModuleNetComp"));
	ModuleNetCompMap.Add(UCameraModuleNetworkComponent::StaticClass(), CameraModuleNetComp);
	
	CharacterModuleNetComp = CreateDefaultSubobject<UCharacterModuleNetworkComponent>(FName("CharacterModuleNetComp"));
	ModuleNetCompMap.Add(UCharacterModuleNetworkComponent::StaticClass(), CharacterModuleNetComp);
	
	EventModuleNetComp = CreateDefaultSubobject<UEventModuleNetworkComponent>(FName("EventModuleNetComp"));
	ModuleNetCompMap.Add(UEventModuleNetworkComponent::StaticClass(), EventModuleNetComp);
	
	MediaModuleNetComp = CreateDefaultSubobject<UMediaModuleNetworkComponent>(FName("MediaModuleNetComp"));
	ModuleNetCompMap.Add(UMediaModuleNetworkComponent::StaticClass(), MediaModuleNetComp);
	
	NetworkModuleNetComp = CreateDefaultSubobject<UNetworkModuleNetworkComponent>(FName("NetworkModuleNetComp"));
	ModuleNetCompMap.Add(UNetworkModuleNetworkComponent::StaticClass(), NetworkModuleNetComp);
	
	ProcedureModuleNetComp = CreateDefaultSubobject<UProcedureModuleNetworkComponent>(FName("ProcedureModuleNetComp"));
	ModuleNetCompMap.Add(UProcedureModuleNetworkComponent::StaticClass(), ProcedureModuleNetComp);
}

void AWHPlayerController::OnInitialize_Implementation()
{
	
}

void AWHPlayerController::OnPreparatory_Implementation(EPhase InPhase)
{
	
}

void AWHPlayerController::OnRefresh_Implementation(float DeltaSeconds)
{
	
}

void AWHPlayerController::OnTermination_Implementation()
{
	
}

void AWHPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if(Execute_IsDefaultLifecycle(this))
	{
		Execute_OnInitialize(this);
		Execute_OnPreparatory(this, EPhase::Final);
	}
}

void AWHPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if(Execute_IsDefaultLifecycle(this))
	{
		Execute_OnTermination(this);
	}
}

void AWHPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
}

void AWHPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if(InPawn->Implements<UWHPlayerInterface>())
	{
		SetPlayerPawn(InPawn);
	}
}

void AWHPlayerController::OnUnPossess()
{
	Super::OnUnPossess();
}

void AWHPlayerController::Tick(float DeltaSeconds) 
{
	Super::Tick(DeltaSeconds);

	if(Execute_IsDefaultLifecycle(this))
	{
		Execute_OnRefresh(this, DeltaSeconds);
	}
}

bool AWHPlayerController::RaycastSingleFromAimPoint(float InRayDistance, ECollisionChannel InGameTraceType, const TArray<AActor*>& InIgnoreActors, FHitResult& OutHitResult) const
{
	int32 viewportSizeX, viewportSizeY;
	FVector sightPos, rayDirection;
	GetViewportSize(viewportSizeX, viewportSizeY);
	if(DeprojectScreenPositionToWorld(viewportSizeX * 0.5f, viewportSizeY * 0.5f, sightPos, rayDirection))
	{
		const FVector rayStart = PlayerCameraManager->GetCameraLocation();
		const FVector rayEnd = rayStart + rayDirection * InRayDistance;
		TArray<AActor*> ignoreActors = InIgnoreActors;
		ignoreActors.AddUnique(GetPawn());
		ignoreActors.AddUnique(GetPlayerPawn());
		return UKismetSystemLibrary::LineTraceSingle(this, rayStart, rayEnd, UGlobalBPLibrary::GetGameTraceChannel(InGameTraceType), false, ignoreActors, EDrawDebugTrace::None, OutHitResult, true);
	}
	return false;
}

void AWHPlayerController::SetPlayerPawn(APawn* InPlayerPawn)
{
	if(PlayerPawn != InPlayerPawn)
	{
		PlayerPawn = InPlayerPawn;
		OnPlayerPawnChanged.Broadcast(PlayerPawn);
	}
}