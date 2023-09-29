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

public:
	
	UFUNCTION(BlueprintCallable, Category = "Bomb Game|SaveGame")
	void SetSlotName(FString NewSlotName);

	UFUNCTION(BlueprintCallable, Category = "Bomb Game|SaveGame")
	void WriteSaveGame();

	void LoadSaveGame(FString InSlotName = "");

	UFUNCTION(BlueprintCallable, Category = "Bomb Game|SaveGame")
	class UBGSaveGame* GetTop10Records();
};
