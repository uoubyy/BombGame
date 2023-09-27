// Copyright Epic Games, Inc. All Rights Reserved.

#include "BGGameMode.h"
#include "UObject/ConstructorHelpers.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "BGBombSpawnManager.h"
#include "BGPlayerState.h"

ABGGameMode::ABGGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
	// use our custom PlayerController class
	PlayerControllerClass = ABGPlayerController::StaticClass();

	// Initialize variables
	ElapsedTime = 0.0f;
	CountdownTime = 10000;
	GameState = EGameState::GS_Idle;
	ReadyPlayers = 0;
/*	
.AddDynamic(this, &ABGGameMode::UpdateReadyPlayers);*/
}

void ABGGameMode::StartPlay()
{
	Super::StartPlay();
	
	{
		TArray<AActor*> OutActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABGCharacter::StaticClass(), OutActors);

		ensureMsgf(OutActors.Num() > 0, TEXT("Failed to find any Player in the Level"));

		for (auto TargetActor : OutActors)
		{
			if (ABGCharacter* BGCharacter = Cast<ABGCharacter>(TargetActor))
			{
				ABGPlayerState* BGPlayerState = BGCharacter->GetPlayerState<ABGPlayerState>();

				if (BGCharacter->ActorHasTag(FName("LeftTeam")))
				{
					BGPlayerState->SetPlayerTeamId(ETeamId::TI_Left);
				}
				else
				{
					BGPlayerState->SetPlayerTeamId(ETeamId::TI_Right);
				}
			}
		}
	}
}

APlayerController* ABGGameMode::Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	return Super::Login(NewPlayer, InRemoteRole, Portal, Options, UniqueId, ErrorMessage);
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
	Super::BeginPlay();

	{
		TArray<AActor*> OutActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABGBombSpawnManager::StaticClass(), OutActors);

		ensureMsgf(OutActors.Num() == 1, TEXT("Failed to find a Bomb Spawn Manager in the Level"));

		if (OutActors.Num() == 1)
		{
			BombSpawnManager = Cast<ABGBombSpawnManager>(OutActors[0]);
		}
	}

	for (auto& ConveyorInfo : AllConveyors)
	{
		BombSpawnManager->RequestSpawnNewBomb(ConveyorInfo.Key);
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

void ABGGameMode::UpdateReadyPlayers()
{
	ReadyPlayers ++;
	UE_LOG(LogTemp, Warning, TEXT("Current readyPlayer : %d"), ReadyPlayers);
// 	bool PlayerNumsCheck = ReadyPlayers > PlayerNums ? true : false;
// 	ensureMsgf(PlayerNumsCheck == false, TEXT("Ready Player numbers are out of range: %d"), ReadyPlayers);
// 
// 	if (PlayerNumsCheck)
// 	{
// 		return;
// 	}

	if (ReadyPlayers == PlayerNums)
	{
		GameState = EGameState::GS_Ready;
		AllPlayersReadyDelegate.Broadcast();
		UE_LOG(LogTemp, Warning, TEXT("start count down"));
// 		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("start count down")));

		FTimerDelegate ReadyCountDownTimerDelegate;
		ReadyCountDownTimerDelegate.BindUFunction(this, FName("ReadyCountDown"));
		GetWorldTimerManager().SetTimer(ReadyCountdownTimerHandle, ReadyCountDownTimerDelegate, ReadyCountDownTime, false);
	}
}

void ABGGameMode::ReadyCountDown()
{
	UE_LOG(LogTemp, Warning, TEXT("count down over"));
// 	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("count down over")));
	GameState = EGameState::GS_Start;
	GameStartDelegate.Broadcast();
}

EGameState ABGGameMode::GetGameState()
{
	return GameState;
}

bool ABGGameMode::AllPlayersReady()
{
	return ReadyPlayers == PlayerNums ? true : false;
}

void ABGGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

//  	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("GameState is : %d"), GameState));

	ElapsedTime += DeltaTime;
	if (GameState == EGameState::GS_Start)
	{
		CountdownTime -= DeltaTime;
	}
}
