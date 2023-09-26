// Copyright Epic Games, Inc. All Rights Reserved.

#include "BGGameMode.h"
#include "BGPlayerController.h"
#include "BGCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "BGConveyorBase.h"

#include "Kismet/GameplayStatics.h"
#include "BGBombSpawnManager.h"
#include "BGPlayerState.h"

ABGGameMode::ABGGameMode()
{
}

void ABGGameMode::StartPlay()
{
	Super::StartPlay();

	{
		TArray<AActor*> OutActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABGBombSpawnManager::StaticClass(), OutActors);

		ensureMsgf(OutActors.Num() == 1, TEXT("Failed to find a Bomb Spawn Manager in the Level"));

		if (OutActors.Num() == 1)
		{
			BombSpawnManager = Cast<ABGBombSpawnManager>(OutActors[0]);
		}
	}

	
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

ABGConveyorBase* ABGGameMode::GetConveyorrefById(int32 ConveyorId)
{
	if (AllConveyors.Contains(ConveyorId))
	{
		return AllConveyors[ConveyorId];
	}

	return nullptr;
}
