// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "SlateWidgets/ToolbarCommands.h"

#define LOCTEXT_NAMESPACE "FToolbarModule"

void FToolbarCommands::RegisterCommands()
{
	UI_COMMAND(PluginAction, "Delete Assets Window", "Opens the Delete Assets Window", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE