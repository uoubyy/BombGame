// Copyright Epic Games, Inc. All Rights Reserved.

#include "BGGameMode.h"
#include "UObject/ConstructorHelpers.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "BGBombSpawnManager.h"
#include "BGPlayerState.h"
#include "GameFramework/PlayerStart.h"
#include "BGGameInstance.h"
#include "BGSaveGameSubsystem.h"

ABGGameMode::ABGGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
	// use our custom PlayerController class
	PlayerControllerClass = ABGPlayerController::StaticClass();

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

	TeamsHealthPoints.Add({ ETeamId::TI_Left, MaxHealthPoints });
	TeamsHealthPoints.Add({ ETeamId::TI_Right, MaxHealthPoints });

	TeamsScore.Add({ ETeamId::TI_Left, 0 });
	TeamsScore.Add({ ETeamId::TI_Right, 0 });
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

	GetBombSpawnManager();
}

APawn* ABGGameMode::SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform)
{
	UClass* PawnClass = nullptr;
	if (APlayerController* PlayerController = Cast<APlayerController>(NewPlayer))
	{
		int32 ControllerId = UGameplayStatics::GetPlayerControllerID(PlayerController);
		FString PlayerTag = FString::Printf(TEXT("P%d"), ControllerId);
		if (OverridePawnClass.Contains(PlayerTag))
		{
			PawnClass = OverridePawnClass[PlayerTag];
		}
	}

	PawnClass = PawnClass ? PawnClass : GetDefaultPawnClassForController(NewPlayer); // Fallback to default pawn class

	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Instigator = GetInstigator();
	SpawnInfo.ObjectFlags |= RF_Transient;	// We never want to save default player pawns into a map
	APawn* ResultPawn = GetWorld()->SpawnActor<APawn>(PawnClass, SpawnTransform, SpawnInfo);
	if (!ResultPawn)
	{
		UE_LOG(LogGameMode, Warning, TEXT("SpawnDefaultPawnAtTransform: Couldn't spawn Pawn of type %s at %s"), *GetNameSafe(PawnClass), *SpawnTransform.ToHumanReadableString());
	}
	return ResultPawn;
}

APawn* ABGGameMode::SpawnDefaultPawnFor_Implementation(AController* NewPlayer, AActor* StartSpot)
{
	int32 ControllerId = 0;
	FString PlayerTag = "P0";
	if (APlayerController* PlayerController = Cast<APlayerController>(NewPlayer))
	{
		ControllerId = UGameplayStatics::GetPlayerControllerID(PlayerController);
		PlayerTag = FString::Printf(TEXT("P%d"), ControllerId);

		StartSpot = AllStartPoints[FName(PlayerTag)];

		if (ABGPlayerState* BGPlayerState = PlayerController->GetPlayerState<ABGPlayerState>())
		{
			BGPlayerState->SetPlayerTeamId(ControllerId < PlayerNums / 2 ? ETeamId::TI_Left : ETeamId::TI_Right);
		}
	}

	FName PlayerTeamTag = ControllerId < PlayerNums / 2 ? FName("LeftTeam") : FName("RightTeam");

	APawn* NewPawn = Super::SpawnDefaultPawnFor_Implementation(NewPlayer, StartSpot);
	NewPawn->Tags.Add(PlayerTeamTag);

	if (ABGCharacter* NewCharacter = Cast<ABGCharacter>(NewPawn))
	{
		NewCharacter->SetPlayerId(PlayerTag);
	}

	return NewPawn;
}

void ABGGameMode::SetGameState(const EGameState NewGameState)
{
	if (CurrentGameState == NewGameState)
	{
		return;
	}
	CurrentGameState = NewGameState;

	if(NewGameState == EGameState::GS_Start)
	{ 
		GetWorld()->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateLambda([this]()
			{
				this->SetGameState(EGameState::GS_InProgress);
			}));
	}
	OnGameStateChanged.Broadcast(CurrentGameState);
}

ABGConveyorBase* ABGGameMode::GetConveyorrefById(int32 ConveyorId)
{
	return BombSpawnManager ? BombSpawnManager->GetConveyorrefById(ConveyorId) : nullptr;
}

void ABGGameMode::PlayerRequestOnReady(AController* InPlayer)
{
	ReadyPlayers++;
	UE_LOG(LogTemp, Warning, TEXT("Current readyPlayer : %d"), ReadyPlayers);

	if (IsDebugMode || ReadyPlayers == PlayerNums)
	{
		SetGameState(EGameState::GS_Start);
	}
}

EGameState ABGGameMode::GetGameState()
{
	return CurrentGameState;
}

bool ABGGameMode::IsAllPlayersReady()
{
	return ReadyPlayers == PlayerNums;
}

void ABGGameMode::ApplyDamage(ETeamId TargetTeam, int32 DamageAmount)
{
	TeamsHealthPoints[TargetTeam] -= DamageAmount;

	if (TeamsHealthPoints[TargetTeam] <= 0)
	{
		ETeamId WinnerTeam = TargetTeam == ETeamId::TI_Left ? ETeamId::TI_Right : ETeamId::TI_Left;
		// TeamsScore[WinnerTeam] += TeamsHealthPoints[WinnerTeam] * 
		SetGameState(EGameState::GS_End);
		TeamsHealthPoints[TargetTeam] = 0;

		// Try save the winner record to save game

		UBGGameInstance* GameInstance = Cast<UBGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
		if (UBGSaveGameSubsystem* BGSaveGameSubsystem = GameInstance->GetSubsystem<UBGSaveGameSubsystem>())
		{
			FString TeamName = GameInstance->GetTeamName(WinnerTeam);
			BGSaveGameSubsystem->TryAddRecordToTop10(TeamName, TeamsScore[WinnerTeam]);

			// TODO: no need write save game file immediately
			BGSaveGameSubsystem->WriteSaveGame();
		}
	}

	OnTeamHealthChanged.Broadcast(TeamsHealthPoints[ETeamId::TI_Left], TeamsHealthPoints[ETeamId::TI_Right]);
}

void ABGGameMode::AddRewards(ETeamId TargetTeam, int32 RewardAmount)
{
	TeamsScore[TargetTeam] += RewardAmount;

	OnTeamScoreChanged.Broadcast(TargetTeam, TeamsScore[TargetTeam], RewardAmount);
}

ABGBombSpawnManager* ABGGameMode::GetBombSpawnManager()
{
	if (!BombSpawnManager)
	{
		TArray<AActor*> OutActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABGBombSpawnManager::StaticClass(), OutActors);

		// ensureMsgf(OutActors.Num() == 1, TEXT("Failed to find a Bomb Spawn Manager in the Level"));
		UE_LOG(LogTemp, Warning, TEXT("Failed to find a Bomb Spawn Manager in the Level."));

		if (OutActors.Num() == 1)
		{
			BombSpawnManager = Cast<ABGBombSpawnManager>(OutActors[0]);
		}
	}
	return BombSpawnManager;
}

int32 ABGGameMode::GetTeamHealthPoints(ETeamId TargetTeam)
{
	return TeamsHealthPoints[TargetTeam];
}

const ETeamId ABGGameMode::GetWinnerTeam() const
{
	return TeamsHealthPoints[ETeamId::TI_Left] == 0 ? ETeamId::TI_Right : ETeamId::TI_Left;
}

void ABGGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ElapsedGameTime += DeltaTime;
}
