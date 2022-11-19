// Copyright Epic Games, Inc. All Rights Reserved.

#include "UnrealToolsGameMode.h"
#include "UnrealToolsCharacter.h"
#include "UObject/ConstructorHelpers.h"

AUnrealToolsGameMode::AUnrealToolsGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
