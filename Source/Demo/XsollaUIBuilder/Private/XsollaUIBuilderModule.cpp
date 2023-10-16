// Copyright 2023 Xsolla Inc. All Rights Reserved.

#include "XsollaUIBuilderModule.h"
#include "XsollaUIBuilderDefines.h"
#include "XsollaUIBuilderSettings.h"

#include "Developer/Settings/Public/ISettingsModule.h"

#define LOCTEXT_NAMESPACE "FXsollaUIBuilderModule"

const FName FXsollaUIBuilderModule::ModuleName = "XsollaUIBuilder";

void FXsollaUIBuilderModule::StartupModule()
{
	// Register settings
	XsollaUIBuilderSettings = GetMutableDefault<UXsollaUIBuilderSettings>();

	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->RegisterSettings("Project", "Plugins", ModuleName,
			LOCTEXT("RuntimeSettingsName", "Xsolla UIBuilder"),
			LOCTEXT("RuntimeSettingsDescription", "Configure Xsolla UIBuilder"),
			XsollaUIBuilderSettings);
	}

	UE_LOG(LogXsollaUIBuilder, Log, TEXT("%s: XsollaUIBuilder module started"), *VA_FUNC_LINE);
}

void FXsollaUIBuilderModule::ShutdownModule()
{
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->UnregisterSettings("Project", "Plugins", ModuleName);
	}

	if (!GExitPurge)
	{
		// If we're in exit purge, this object has already been destroyed
		XsollaUIBuilderSettings->RemoveFromRoot();
		if (XsollaUIBuilderSettings->IsRooted())
		{
			XsollaUIBuilderSettings->RemoveFromRoot();
		}
	}
	else
	{
		XsollaUIBuilderSettings = nullptr;
	}
}

UXsollaUIBuilderSettings* FXsollaUIBuilderModule::GetSettings() const
{
	check(XsollaUIBuilderSettings);
	return XsollaUIBuilderSettings;
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FXsollaUIBuilderModule, XsollaUIBuilder);

DEFINE_LOG_CATEGORY(LogXsollaUIBuilder);