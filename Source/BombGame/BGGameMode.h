// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "BGPlayerController.h"
#include "BGCharacter.h"
#include "BGConveyorBase.h"

#include "BGGameplayEnum.h"
#include "BGGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameStateChangedDelegate, const EGameState, NewGameState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTeamScoreChangedDelegate, int32, LeftTeamScore, int32, RightTeamScore);

UCLASS(minimalapi)
class ABGGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ABGGameMode();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void StartPlay() override;

	virtual APlayerController* Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;

	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

	UFUNCTION(BlueprintCallable, Category = "Bomb Game|Game Mode")
	class ABGConveyorBase* GetConveyorrefById(int32 ConveyorId);

	UFUNCTION(BlueprintCallable, Category = "Bomb Game|Game Mode")
	void RegisterCharacter(int CharacterId, class ABGCharacter* CharacterRef);

	UFUNCTION(BlueprintCallable, Category = "Bomb Game|Game Mode")
	class ABGCharacter* GetCharacterRefById(int32 CharacterId);

	UFUNCTION(BlueprintCallable, Category = "Bomb Game|Game Mode")
	void UpdateReadyPlayers();

	UFUNCTION(BlueprintCallable, Category = "Bomb Game|Game Mode")
	void ReadyCountDown();

	UFUNCTION(BlueprintCallable, Category = "Bomb Game|Game Mode")
	EGameState GetGameState();

	UFUNCTION(BlueprintCallable)
	bool AllPlayersReady();

	UFUNCTION(BlueprintCallable, Category = "Bomb Game|Game Mode")
	void ApplyDamage(ETeamId TargetTeam, int32 DamageAmount);

	UFUNCTION(BlueprintCallable, Category = "Bomb Game|Game Mode")
	class ABGBombSpawnManager* GetBombSpawnManager();

	UFUNCTION(BlueprintCallable, Category = "Bomb Game|Game Mode")
	int32 GetTeamScore(ETeamId TargetTeam);

	// Total countdown time
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bomb Game|Game Mode")
	float CountdownTime;

	// Ready start countdown time
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bomb Game|Game Mode")
	float ReadyCountDownTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bomb Game|Game Mode")
	int PlayerNums;

	UPROPERTY(BlueprintAssignable, Category = "Bomb Game|Game Mode")
	FOnGameStateChangedDelegate OnGameStateChanged;

	UPROPERTY(BlueprintAssignable, Category = "Bomb Game|Game Mode")
	FOnTeamScoreChangedDelegate OnTeamScoreChanged;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Bomb Game|Game Mode")
	TMap<int32, class ABGCharacter*> AllCharacters;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Bomb Game|Game Mode")
	TMap<ETeamId, int32> TeamsHealthPoints;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Bomb Game|Game Mode")
	int32 MaxHealthPoints = 5;

	// Elapsed time
	float ElapsedTime;
	// Timer handle
	FTimerHandle CountdownTimerHandle;
	FTimerHandle ReadyCountdownTimerHandle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Bomb Game|Game Mode")
	EGameState CurrentGameState = EGameState::GS_Idle;

	int ReadyPlayers;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Bomb Game|Game Mode")
	TObjectPtr<class ABGBombSpawnManager> BombSpawnManager;

	void SetGameState(const EGameState NewGameState);
};



