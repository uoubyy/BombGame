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
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTeamHealthChangedDelegate, int32, LeftTeamHealth, int32, RightTeamHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnTeamScoreChangedDelegate, ETeamId, TargetTeam, int32, NewScore, int32, DeltaValue);

UCLASS(minimalapi)
class ABGGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ABGGameMode();

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	virtual void Tick(float DeltaTime) override;

	virtual void StartPlay() override;

	virtual APlayerController* Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;

	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

	UFUNCTION(BlueprintCallable, Category = "Bomb Game|Game Mode")
	class ABGConveyorBase* GetConveyorrefById(int32 ConveyorId);

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
	void AddRewards(ETeamId TargetTeam, int32 RewardAmount);

	UFUNCTION(BlueprintCallable, Category = "Bomb Game|Game Mode")
	class ABGBombSpawnManager* GetBombSpawnManager();

	UFUNCTION(BlueprintCallable, Category = "Bomb Game|Game Mode")
	int32 GetTeamHealthPoints(ETeamId TargetTeam);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bomb Game|Game Mode")
	bool IsDebugMode = false;

	// Total countdown time
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bomb Game|Game Mode")
	float CountdownTime;

	// Ready start countdown time
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bomb Game|Game Mode")
	float ReadyCountDownTime = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bomb Game|Game Mode")
	int PlayerNums = 4;

	UPROPERTY(BlueprintAssignable, Category = "Bomb Game|Game Mode")
	FOnGameStateChangedDelegate OnGameStateChanged;

	UPROPERTY(BlueprintAssignable, Category = "Bomb Game|Game Mode")
	FOnTeamHealthChangedDelegate OnTeamHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "Bomb Game|Game Mode")
	FOnTeamScoreChangedDelegate OnTeamScoreChanged;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual APawn* SpawnDefaultPawnFor_Implementation(AController* NewPlayer, AActor* StartSpot) override;

private:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Bomb Game|Game Mode")
	TMap<ETeamId, int32> TeamsHealthPoints;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Bomb Game|Game Mode")
	TMap<ETeamId, int32> TeamsScore;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Bomb Game|Game Mode")
	TMap<FName, TObjectPtr<class APlayerStart>> AllStartPoints;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Bomb Game|Game Mode")
	int32 MaxHealthPoints = 8;

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



