// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "BGSaveGame.generated.h"

USTRUCT(BlueprintType)
struct FRecordDetail
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Record Detail")
	FString TeamName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Record Detail")
	int32 TeamScore;

	FRecordDetail() 
		: TeamName("")
		, TeamScore(0)
	{
	}

	FRecordDetail(const FString& InTeamName, int32 InTeamScore)
		: TeamName(InTeamName)
		, TeamScore(InTeamScore)
	{
	}
};

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
	bool TryAddRecordToTop10(const FRecordDetail& RecordDetail);
};
