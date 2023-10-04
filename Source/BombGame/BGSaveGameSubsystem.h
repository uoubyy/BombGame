// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "BGSaveGameSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class BOMBGAME_API UBGSaveGameSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
protected:

	FString CurrentSlotName = "BombGameSaveGame";

	UPROPERTY()
	TObjectPtr<class UBGSaveGame> CurrentSaveGame;

	UPROPERTY()
	int32 LatestRecordRank = 0;

public:
	
	UFUNCTION(BlueprintCallable, Category = "Bomb Game|SaveGameSystem")
	void SetSlotName(FString NewSlotName);

	UFUNCTION(BlueprintCallable, Category = "Bomb Game|SaveGameSystem")
	void WriteSaveGame();

	void LoadSaveGame(FString InSlotName = "");

	UFUNCTION(BlueprintCallable, Category = "Bomb Game|SaveGameSystem")
	class UBGSaveGame* GetTop10Records();

	UFUNCTION(BlueprintCallable, Category = "Bomb Game|SaveGameSystem")
	bool TryAddRecordToTop10(const FString& TeamName, const int32 TeamScore);

	UFUNCTION(BlueprintCallable, Category = "Bomb Game|SaveGameSystem")
	const int32 GetLatestRecordRank() const { return LatestRecordRank; }
};
