// Copyright 2021 Sam Carey. All Rights Reserved.

#include "Common/Module/EditorModuleBase.h"

#define LOCTEXT_NAMESPACE "FEditorModuleBase"

FEditorModuleBase::FEditorModuleBase()
{
	AppIdentifier = FName("EditorModuleBase_AppIdentifier");
}

FEditorModuleBase::~FEditorModuleBase()
{
}

void FEditorModuleBase::StartupModule()
{
}

void FEditorModuleBase::ShutdownModule()
{
}

void FEditorModuleBase::RegisterSettings(ISettingsModule* SettingsModule)
{
	
}

void FEditorModuleBase::UnRegisterSettings(ISettingsModule* SettingsModule)
{
}

bool FEditorModuleBase::HandleSettingsSaved()
{
	return false;
}

void FEditorModuleBase::RegisterCommands(const TSharedPtr<FUICommandList>& Commands)
{
	
}

void FEditorModuleBase::RegisterMenus(const TSharedPtr<FUICommandList>& Commands)
{
}

void FEditorModuleBase::RegisterAssetTypeAction(IAssetTools& AssetTools, EAssetTypeCategories::Type AssetCategory, TArray<TSharedPtr<IAssetTypeActions>>& AssetTypeActions)
{
	
}

void FEditorModuleBase::RegisterCustomClassLayout(FPropertyEditorModule& PropertyEditor)
{
}

void FEditorModuleBase::UnRegisterCustomClassLayout(FPropertyEditorModule& PropertyEditor)
{
}

void FEditorModuleBase::AddWindowMenu(const TSharedPtr<const FUICommandInfo>& Command, const TSharedPtr<const FUICommandList>& Commands)
{
	UToolMenu* oolMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window.WHFramework");
	{
		FToolMenuSection& Section = oolMenu->FindOrAddSection("WHFramework");
		{
			Section.AddMenuEntryWithCommandList(Command, Commands);
		}
	}
}

void FEditorModuleBase::AddToolbarMenu(const TSharedPtr<const FUICommandInfo>& Command, const TSharedPtr<const FUICommandList>& Commands)
{
	UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
	{
		FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("WHFramework");
		{
			FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(Command));
			Entry.SetCommandList(Commands);
		}
	}
}

#undef LOCTEXT_NAMESPACE
