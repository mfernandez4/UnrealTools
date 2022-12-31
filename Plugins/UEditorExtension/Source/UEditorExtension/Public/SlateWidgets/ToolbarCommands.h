// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "SlateWidgets/UEditorExtensionStyle.h"

class FToolbarCommands : public TCommands<FToolbarCommands>
{
public:

	FToolbarCommands()
		: TCommands<FToolbarCommands>(TEXT("UEditorExtension"), NSLOCTEXT("Contexts", "UEditorExtension", "UEditorExtension Plugin"), NAME_None, FUEditorExtensionStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > PluginAction;
};