// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "BGGameplayEnum.h"
#include "BGSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class BOMBGAME_API UBGSaveGame : public USaveGame
{
	GENERATED_BODY()

private:
	int32 MaxRecordCnt = 10;
	
public:
	UBGSaveGame();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bomb Game|SaveGame")
	TMap<int32, FRecordDetail> Top10Records;

	UFUNCTION(BlueprintCallable, Category = "Bomb Game|SaveGame")
	int32 TryAddRecordToTop10(const FRecordDetail& RecordDetail);
};
