// Copyright Epic Games, Inc. All Rights Reserved.

#include "BGGameMode.h"
#include "BGPlayerController.h"
#include "BGCharacter.h"
#include "UObject/ConstructorHelpers.h"

ABGGameMode::ABGGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = ABGPlayerController::StaticClass();
}