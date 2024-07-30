// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/PickUp/AbilityPickUpBase.h"

#include "Ability/AbilityModule.h"
#include "Ability/PickUp/AbilityPickerInterface.h"
#include "Common/Components/FallingMovementComponent.h"
#include "Common/Components/FollowingMovementComponent.h"
#include "Common/Interaction/InteractionComponent.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "Components/BoxComponent.h"
#include "ObjectPool/ObjectPoolModuleStatics.h"
#include "Scene/SceneModuleStatics.h"

AAbilityPickUpBase::AAbilityPickUpBase()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetupAttachment(RootComponent);
	BoxComponent->SetCollisionProfileName(TEXT("PickUp"));
	BoxComponent->SetBoxExtent(FVector(10.f));
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AAbilityPickUpBase::OnOverlap);

	RotatingComponent = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("RotatingComponent"));
	RotatingComponent->RotationRate = FRotator(0.f, 180.f, 0.f);

	FallingComponent = CreateDefaultSubobject<UFallingMovementComponent>(TEXT("FallingComponent"));
	
	FollowingComponent = CreateDefaultSubobject<UFollowingMovementComponent>(TEXT("FollowingComponent"));

	Item = FAbilityItem::Empty;
}

void AAbilityPickUpBase::OnInitialize_Implementation()
{
	Super::OnInitialize_Implementation();

	FallingComponent->SetTraceChannel(USceneModuleStatics::GetTraceMapping(FName("PickUp")).GetTraceChannel());
}

void AAbilityPickUpBase::OnSpawn_Implementation(UObject* InOwner, const TArray<FParameter>& InParams)
{
	Super::OnSpawn_Implementation(InOwner, InParams);
}

void AAbilityPickUpBase::OnDespawn_Implementation(bool bRecovery)
{
	Super::OnDespawn_Implementation(bRecovery);
	Item = FAbilityItem::Empty;

	FollowingComponent->SetFollowingTarget(nullptr);
}

void AAbilityPickUpBase::LoadData(FSaveData* InSaveData, EPhase InPhase)
{
	const auto& SaveData = InSaveData->CastRef<FPickUpSaveData>();

	Item = SaveData.Item;
	SetActorLocationAndRotation(SaveData.Location, FRotator(0.f, FMath::FRandRange(0.f, 360.f), 0.f));
}

FSaveData* AAbilityPickUpBase::ToData()
{
	static FPickUpSaveData SaveData;
	SaveData = FPickUpSaveData();

	SaveData.Item = Item;
	SaveData.Location = GetActorLocation();

	return &SaveData;
}

void AAbilityPickUpBase::OnPickUp(IAbilityPickerInterface* InPicker)
{
	if(InPicker && InPicker->OnPickUp(this))
	{
		UObjectPoolModuleStatics::DespawnObject(this);
	}
}

void AAbilityPickUpBase::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(!Item.IsValid()) return;

	if(IAbilityPickerInterface* Picker = Cast<IAbilityPickerInterface>(OtherActor))
	{
		if(OtherComp->IsA<UInteractionComponent>())
		{
			FallingComponent->SetActive(false);
			FollowingComponent->SetFollowingTarget(OtherActor);
		}
		else
		{
			OnPickUp(Picker);
		}
	}
}
