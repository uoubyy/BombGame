// Copyright Epic Games, Inc. All Rights Reserved.

#include "BGGameMode.h"
#include "UObject/ConstructorHelpers.h"

ABGGameMode::ABGGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = ABGPlayerController::StaticClass();

	// Initialize variables
	ElapsedTime = 0.0f;
	CountdownTime = 10000;
	GameState = EGameState::GS_Idle;
	ReadyPlayers = 0;
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

void ABGGameMode::BeginPlay()
{
	while (!AllPlayersReady())
	{

	}
}

ABGConveyorBase* ABGGameMode::GetConveyorrefById(int32 ConveyorId)
{
	if (AllConveyors.Contains(ConveyorId))
	{
		return AllConveyors[ConveyorId];
	}

	return nullptr;
}

// Register the character when it joins at first time
void ABGGameMode::RegisterCharacter(int CharacterId, class ABGCharacter* CharacterRef)
{
	bool DuplicatedCheck = AllCharacters.Contains(CharacterId);
	ensureMsgf(DuplicatedCheck == false, TEXT("Register duplicated conveyors with the same id %d"), CharacterId);

	if (DuplicatedCheck)
	{
		return;
	}

	AllCharacters.Add({CharacterId , CharacterRef});
}

// Get the character reference by Id
ABGCharacter* ABGGameMode::GetCharacterRefById(int32 CharacterId)
{
	if (AllCharacters.Contains(CharacterId))
	{
		return AllCharacters[CharacterId];
	}

	return nullptr;
}

EGameState ABGGameMode::GetGameState()
{
	return GameState;
}

bool ABGGameMode::AllPlayersReady()
{

}

void ABGGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ElapsedTime += DeltaTime;
	if (GameState == EGameState::GS_Start)
	{
		CountdownTime -= DeltaTime;
	}
}
