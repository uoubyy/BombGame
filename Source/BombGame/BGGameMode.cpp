// Copyright Epic Games, Inc. All Rights Reserved.

#include "BGGameMode.h"
#include "BGPlayerController.h"
#include "BGCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "BGConveyorBase.h"

ABGGameMode::ABGGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = ABGPlayerController::StaticClass();
}

AActor* ABGGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	return Super::ChoosePlayerStart_Implementation(Player);
}

void ABGGameMode::RegisterConveyor(int32 ConveyorId, class ABGConveyorBase* ConveyorRef)
{
	bool DuplicatedCheck = AllConveyors.Contains(ConveyorId);
	ensureMsgf(DuplicatedCheck == false, TEXT("Register duplicated conveyors with the same id %d"), ConveyorId);

	if (DuplicatedCheck)
	{
		return;
	}

	AllConveyors.Add({ ConveyorId , ConveyorRef });
}

ABGConveyorBase* ABGGameMode::GetConveyorrefById(int32 ConveyorId)
{
	if (AllConveyors.Contains(ConveyorId))
	{
		return AllConveyors[ConveyorId];
	}

	return nullptr;
}
