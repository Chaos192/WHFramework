// Fill out your copyright notice in the Description page of Project Settings.


#include "Main/Base/ManagerBase.h"

const FUniqueType FManagerBase::Type = FUniqueType();

// Sets default values
FManagerBase::FManagerBase() : FUniqueClass(Type)
{
}

FManagerBase::~FManagerBase()
{
}

void FManagerBase::OnInitialize()
{
	int32 a = 0;
}
