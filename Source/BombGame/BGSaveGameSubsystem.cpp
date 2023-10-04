// Fill out your copyright notice in the Description page of Project Settings.


#include "BGSaveGameSubsystem.h"
#include "BGSaveGame.h"
#include <Kismet/GameplayStatics.h>
#include <GameFramework/GameStateBase.h>

void UBGSaveGameSubsystem::SetSlotName(FString NewSlotName)
{
	if (NewSlotName.Len() > 0)
	{
		CurrentSlotName = NewSlotName;
	}
}

void UBGSaveGameSubsystem::WriteSaveGame()
{
	if (!UGameplayStatics::SaveGameToSlot(CurrentSaveGame, CurrentSlotName, 0))
	{
		UE_LOG(LogTemp, Warning, TEXT("Try to WriteSaveGame Failed"));
	}
}

void UBGSaveGameSubsystem::LoadSaveGame(FString InSlotName)
{
	SetSlotName(InSlotName);

	if (UGameplayStatics::DoesSaveGameExist(CurrentSlotName, 0))
	{ 
		CurrentSaveGame = Cast<UBGSaveGame>(UGameplayStatics::LoadGameFromSlot(CurrentSlotName, 0));

		UE_LOG(LogTemp, Log, TEXT("Loaded SaveGame Data."));
	}

	if (!CurrentSaveGame)
	{
		CurrentSaveGame = Cast<UBGSaveGame>(UGameplayStatics::CreateSaveGameObject(UBGSaveGame::StaticClass()));
		UE_LOG(LogTemp, Log, TEXT("Created New SaveGame Data."));
	}
}

class UBGSaveGame* UBGSaveGameSubsystem::GetTop10Records()
{
	if (!CurrentSaveGame)
	{
		LoadSaveGame(CurrentSlotName);
	}

	return CurrentSaveGame;
}

bool UBGSaveGameSubsystem::TryAddRecordToTop10(const FString& TeamName, const int32 TeamScore)
{
	UE_LOG(LogTemp, Warning, TEXT("TryAddRecordToTop10 %s %d"), *TeamName, TeamScore);
	// In case no save game loaded or created
	GetTop10Records();
	
	LatestRecordRank = CurrentSaveGame->TryAddRecordToTop10(FRecordDetail(TeamName, TeamScore));

	return LatestRecordRank < 10;
}
