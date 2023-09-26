// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BGPlayerController.h"
#include "BGCharacter.h"
#include "BGConveyorBase.h"
#include "BGGameplayEnum.h"
#include "BGGameMode.generated.h"

UCLASS(minimalapi)
class ABGGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ABGGameMode();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

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
	EGameState GetGameState();

	UFUNCTION(BlueprintCallable)
	bool AllPlayersReady();


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
	// Total countdown time
	int32 CountdownTime;
	// Timer handle
	FTimerHandle CountdownTimerHandle;

	EGameState GameState;

	int ReadyPlayers;
};



