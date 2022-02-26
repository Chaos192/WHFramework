// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/WidgetModule.h"

#include "Kismet/GameplayStatics.h"
#include "Widget/WidgetModuleBPLibrary.h"

// Sets default values
AWidgetModule::AWidgetModule()
{
	ModuleName = FName("WidgetModule");

	bPreloadWidgets = false;
}

#if WITH_EDITOR
void AWidgetModule::OnGenerate_Implementation()
{
	Super::OnGenerate_Implementation();
}

void AWidgetModule::OnDestroy_Implementation()
{
	Super::OnDestroy_Implementation();
}
#endif

void AWidgetModule::OnInitialize_Implementation()
{
	Super::OnInitialize_Implementation();

	for(auto Iter : UserWidgetClasses)
	{
		if(Iter)
		{
			const FName WidgetName = Iter->GetDefaultObject<UUserWidgetBase>()->GetWidgetName();
			if(!UserWidgetClassMap.Contains(WidgetName))
			{
				UserWidgetClassMap.Add(WidgetName, Iter);
			}
			if(bPreloadWidgets)
			{
				UWidgetModuleBPLibrary::CreateUserWidget<UUserWidgetBase>(nullptr, Iter);
			}
		}
	}
}

void AWidgetModule::OnRefresh_Implementation(float DeltaSeconds)
{
	Super::OnRefresh_Implementation(DeltaSeconds);
}

void AWidgetModule::OnPause_Implementation()
{
	Super::OnPause_Implementation();
}

void AWidgetModule::OnUnPause_Implementation()
{
	Super::OnUnPause_Implementation();
}

bool AWidgetModule::K2_HasUserWidget(TSubclassOf<UUserWidgetBase> InWidgetClass) const
{
	return HasUserWidget<UUserWidgetBase>(InWidgetClass);
}

UUserWidgetBase* AWidgetModule::K2_GetUserWidget(TSubclassOf<UUserWidgetBase> InWidgetClass) const
{
	return GetUserWidget<UUserWidgetBase>(InWidgetClass);
}

UUserWidgetBase* AWidgetModule::K2_GetUserWidgetByName(FName InName, TSubclassOf<UUserWidgetBase> InWidgetClass) const
{
	if(AllUserWidgets.Contains(InName))
	{
		return AllUserWidgets[InName];
	}
	return nullptr;
}

UUserWidgetBase* AWidgetModule::K2_CreateUserWidget(TSubclassOf<UUserWidgetBase> InWidgetClass, AActor* InOwner)
{
	return CreateUserWidget<UUserWidgetBase>(InOwner, InWidgetClass);
}

bool AWidgetModule::K2_InitializeUserWidget(TSubclassOf<UUserWidgetBase> InWidgetClass, AActor* InOwner)
{
	return InitializeUserWidget<UUserWidgetBase>(InOwner, InWidgetClass);
}

bool AWidgetModule::K2_OpenUserWidget(TSubclassOf<UUserWidgetBase> InWidgetClass, const TArray<FParameter>& InParams,  bool bInstant)
{
	return OpenUserWidget<UUserWidgetBase>(&InParams, bInstant, InWidgetClass);
}

bool AWidgetModule::K2_CloseUserWidget(TSubclassOf<UUserWidgetBase> InWidgetClass, bool bInstant)
{
	return CloseUserWidget<UUserWidgetBase>(bInstant, InWidgetClass);
}

bool AWidgetModule::K2_ToggleUserWidget(TSubclassOf<UUserWidgetBase> InWidgetClass, bool bInstant)
{
	return ToggleUserWidget<UUserWidgetBase>(bInstant, InWidgetClass);
}

bool AWidgetModule::K2_DestroyUserWidget(TSubclassOf<UUserWidgetBase> InWidgetClass)
{
	return DestroyUserWidget<UUserWidgetBase>(InWidgetClass);
}

void AWidgetModule::CloseAllUserWidget(EWidgetType InWidgetType, bool bInstant)
{
	for (auto Iter : AllUserWidgets)
	{
		if(!Iter.Value) continue;
		if (InWidgetType == EWidgetType::None || Iter.Value->GetWidgetType() == InWidgetType)
		{
			if (Iter.Value->GetWidgetType() == EWidgetType::Temporary)
			{
				TemporaryUserWidget = nullptr;
			}
			Iter.Value->OnClose(bInstant);
		}
	}
}

void AWidgetModule::CloseAllSlateWidget(EWidgetType InWidgetType, bool bInstant)
{
	for (auto Iter : AllSlateWidgets)
	{
		if(!Iter.Value) continue;
		if (InWidgetType == EWidgetType::None || Iter.Value->GetWidgetType() == InWidgetType)
		{
			Iter.Value->SetLastWidget(nullptr);
			Iter.Value->OnClose(bInstant);
		}
		TemporarySlateWidget = nullptr;
	}
}

EInputMode AWidgetModule::GetNativeInputMode() const
{
	EInputMode ReturnValue = EInputMode::None;
    for (auto Iter : AllUserWidgets)
    {
    	if(!Iter.Value) continue;
    	UUserWidgetBase* UserWidget = Iter.Value;
    	if (UserWidget && UserWidget->GetWidgetState() == EWidgetState::Opened && !UserWidget->GetParentWidget() && (int32)UserWidget->GetInputMode() > (int32)ReturnValue)
    	{
    		ReturnValue = UserWidget->GetInputMode();
    	}
    }
   return ReturnValue;
}
