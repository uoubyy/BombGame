// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BGGameplayEnum.h"
#include "BGGameMode.generated.h"

UCLASS(minimalapi)
class ABGGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ABGGameMode();

	virtual void StartPlay() override;

	virtual APlayerController* Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;

	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

	UFUNCTION(BlueprintCallable, Category = "Bomb Game|Game Mode")
	void RegisterConveyor(int32 ConveyorId, class ABGConveyorBase* ConveyorRef);

	UFUNCTION(BlueprintCallable, Category = "Bomb Game|Game Mode")
	class ABGConveyorBase* GetConveyorrefById(int32 ConveyorId);

private:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Bomb Game|Game Mode")
	TMap<int32, class ABGConveyorBase*> AllConveyors;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Bomb Game|Game Mode")
	TObjectPtr<class ABGBombSpawnManager> BombSpawnManager;
};



