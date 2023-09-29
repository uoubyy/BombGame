// Copyright Epic Games, Inc. All Rights Reserved.

#include "BGGameMode.h"
#include "UObject/ConstructorHelpers.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "BGBombSpawnManager.h"
#include "BGPlayerState.h"
#include <GameFramework/PlayerStart.h>

ABGGameMode::ABGGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
	// use our custom PlayerController class
	PlayerControllerClass = ABGPlayerController::StaticClass();

	// Initialize variables
	ElapsedTime = 0.0f;
	CountdownTime = 10000;

	ReadyPlayers = 0;
}

void ABGGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	{
		TArray<AActor*> OutActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), OutActors);

		for (auto ActorRef : OutActors)
		{
			if (APlayerStart* StartPoint = Cast<APlayerStart>(ActorRef))
			{
				AllStartPoints.Add({ StartPoint->PlayerStartTag, StartPoint });
			}
		}

		ensureMsgf(AllStartPoints.Num() >= PlayerNums, TEXT("Not enough PlayerStart, needs %d but only finds %d."), PlayerNums, AllStartPoints.Num());
	}
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
				if(ABGPlayerState* BGPlayerState = BGCharacter->GetPlayerState<ABGPlayerState>())
				{ 
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

	TeamsHealthPoints.Add({ ETeamId::TI_Left, MaxHealthPoints });
	TeamsHealthPoints.Add({ ETeamId::TI_Right, MaxHealthPoints });
}

APlayerController* ABGGameMode::Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	APlayerController* NewPlayerController = Super::Login(NewPlayer, InRemoteRole, Portal, Options, UniqueId, ErrorMessage);

	if (ULocalPlayer* NewLocalPlayer = Cast<ULocalPlayer>(NewPlayer))
	{
		EAutoReceiveInput::Type AutoPossesTarget = EAutoReceiveInput::Type(NewLocalPlayer->GetControllerId() + 1);
		NewPlayerController->AutoReceiveInput = AutoPossesTarget;
	}

	return NewPlayerController;
}

AActor* ABGGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	return Super::ChoosePlayerStart_Implementation(Player);
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
}

APawn* ABGGameMode::SpawnDefaultPawnFor_Implementation(AController* NewPlayer, AActor* StartSpot)
{
	int32 ControllerId = 0;
	if (APlayerController* PlayerController = Cast<APlayerController>(NewPlayer))
	{
		ControllerId = UGameplayStatics::GetPlayerControllerID(PlayerController);
		FString TargetPoint = FString::Printf(TEXT("P%d"), ControllerId);

		StartSpot = AllStartPoints[FName(TargetPoint)];
	}

	APawn* NewPawn = Super::SpawnDefaultPawnFor_Implementation(NewPlayer, StartSpot);
	if(ControllerId < PlayerNums / 2)
	{ 
		NewPawn->Tags.Add("LeftTeam");
	}
	else
	{
		NewPawn->Tags.Add("RightTeam");
	}

	return NewPawn;
}

void ABGGameMode::SetGameState(const EGameState NewGameState)
{
	CurrentGameState = NewGameState;
	OnGameStateChanged.Broadcast(CurrentGameState);
}

ABGConveyorBase* ABGGameMode::GetConveyorrefById(int32 ConveyorId)
{
	return BombSpawnManager ? BombSpawnManager->GetConveyorrefById(ConveyorId) : nullptr;
}

void ABGGameMode::UpdateReadyPlayers()
{
	ReadyPlayers++;
	UE_LOG(LogTemp, Warning, TEXT("Current readyPlayer : %d"), ReadyPlayers);

	if (ReadyPlayers == PlayerNums)
	{
		SetGameState(EGameState::GS_Ready);

		GetWorldTimerManager().SetTimer(ReadyCountdownTimerHandle, this, &ABGGameMode::ReadyCountDown, 0, false, ReadyCountDownTime);
	}
}

void ABGGameMode::ReadyCountDown()
{
	SetGameState(EGameState::GS_Start);
}

EGameState ABGGameMode::GetGameState()
{
	return CurrentGameState;
}

bool ABGGameMode::AllPlayersReady()
{
	return ReadyPlayers == PlayerNums;
}

void ABGGameMode::ApplyDamage(ETeamId TargetTeam, int32 DamageAmount)
{
	TeamsHealthPoints[TargetTeam] -= DamageAmount;

	if (TeamsHealthPoints[TargetTeam] <= 0)
	{
		SetGameState(EGameState::GS_End);
		TeamsHealthPoints[TargetTeam] = 0;
	}

	OnTeamHealthChanged.Broadcast(TeamsHealthPoints[ETeamId::TI_Left], TeamsHealthPoints[ETeamId::TI_Right]);
}

ABGBombSpawnManager* ABGGameMode::GetBombSpawnManager()
{
	return BombSpawnManager;
}

int32 ABGGameMode::GetTeamHealthPoints(ETeamId TargetTeam)
{
	return TeamsHealthPoints[TargetTeam];
}

void ABGGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ElapsedTime += DeltaTime;

	if (CurrentGameState == EGameState::GS_Start)
	{
		CountdownTime -= DeltaTime;
	}
}
