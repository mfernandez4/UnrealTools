// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FUEditorExtensionModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
#pragma region ContentBrowserMenuExtension

	/** Delegate for when the context menu is being built */
	void InitCBMenuExtension();

#pragma endregion 
};
