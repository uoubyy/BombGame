// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BGGameMode.generated.h"

UCLASS(minimalapi)
class ABGGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ABGGameMode();

	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

	UFUNCTION(BlueprintCallable, Category = "Bomb Game|Game Mode")
	void RegisterConveyor(int32 ConveyorId, class ABGConveyorBase* ConveyorRef);

	UFUNCTION(BlueprintCallable, Category = "Bomb Game|Game Mode")
	class ABGConveyorBase* GetConveyorrefById(int32 ConveyorId);

private:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Bomb Game|Game Mode")
	TMap<int32, class ABGConveyorBase*> AllConveyors;
};



