// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "BGPlayerController.h"
#include "BGCharacter.h"
#include "BGConveyorBase.h"

#include "BGGameplayEnum.h"
#include "BGGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FReadyPlayerDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGameStartDelegate);

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
	void RegisterConveyor(int32 ConveyorId, class ABGConveyorBase* ConveyorRef);

	UFUNCTION(BlueprintCallable, Category = "Bomb Game|Game Mode")
	class ABGConveyorBase* GetConveyorrefById(int32 ConveyorId);

	UFUNCTION(BlueprintCallable, Category = "Bomb Game|Game Mode")
	void RegisterCharacter(int CharacterId, class ABGCharacter* CharacterRef);

	UFUNCTION(BlueprintCallable, Category = "Bomb Game|Game Mode")
	class ABGCharacter* GetCharacterRefById(int32 CharacterId);

	UFUNCTION(BlueprintCallable)
	void UpdateReadyPlayers();

	UFUNCTION(BlueprintCallable)
	void ReadyCountDown();

	UFUNCTION(BlueprintCallable)
	EGameState GetGameState();

	UFUNCTION(BlueprintCallable)
	bool AllPlayersReady();

	// Total countdown time
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bomb Game|Game Mode")
	float CountdownTime;

	// Ready start countdown time
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bomb Game|Game Mode")
	float ReadyCountDownTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bomb Game|Game Mode")
	int PlayerNums;

	UPROPERTY(BlueprintAssignable)
	FReadyPlayerDelegate AllPlayersReadyDelegate;

	UPROPERTY(BlueprintAssignable)
	FGameStartDelegate GameStartDelegate;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Bomb Game|Game Mode")
	TMap<int32, class ABGConveyorBase*> AllConveyors;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Bomb Game|Game Mode")
	TMap<int32, class ABGCharacter*> AllCharacters;

	// Elapsed time
	float ElapsedTime;
	// Timer handle
	FTimerHandle CountdownTimerHandle;
	FTimerHandle ReadyCountdownTimerHandle;
	EGameState GameState;

	int ReadyPlayers;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Bomb Game|Game Mode")
	TObjectPtr<class ABGBombSpawnManager> BombSpawnManager;
};



