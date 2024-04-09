// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#define GENERATED_MAIN_MANAGER(ManagerClass) \
protected: \
	static ManagerClass* Instance; \
public: \
	static ManagerClass& Get(); \
	static ManagerClass* GetPtr();

#define GENERATED_MANAGER(ManagerClass) \
protected: \
	static ManagerClass* Instance; \
public: \
	static ManagerClass& Get(); \
	static ManagerClass* GetPtr(); \
	static void Register(); \
	static void UnRegister();

#define IMPLEMENTATION_MANAGER(ManagerClass) \
ManagerClass* ManagerClass::Instance = nullptr; \
ManagerClass& ManagerClass::Get() \
{ \
	return *ManagerClass::GetPtr(); \
} \
ManagerClass* ManagerClass::GetPtr() \
{ \
	if(!Instance) \
	{ \
		Instance = FMainManager::GetManager<ManagerClass>() ? FMainManager::GetManager<ManagerClass>() : new ManagerClass(); \
	} \
	return Instance; \
} \
void ManagerClass::Register() \
{ \
	FMainManager::RegisterManager<ManagerClass>(); \
} \
void ManagerClass::UnRegister() \
{ \
	FMainManager::UnRegisterManager<ManagerClass>(); \
}

#define IMPLEMENTATION_MAIN_MANAGER(ManagerClass) \
ManagerClass* ManagerClass::Instance = nullptr; \
ManagerClass& ManagerClass::Get() \
{ \
	return *ManagerClass::GetPtr(); \
} \
ManagerClass* ManagerClass::GetPtr() \
{ \
	if(!Instance) \
	{ \
		Instance = new ManagerClass(); \
		Instance->OnInitialize(); \
	} \
	return Instance; \
}