// Copyright Five Guys Team.

#pragma once

#include "CoreMinimal.h"
#include "BGBombBase.h"
#include "BGBombSpawnManager.h"
#include "BGGameplayEnum.h"
#include "BGBombReverseDir.generated.h"

/**
 * 
 */
UCLASS()
class BOMBGAME_API ABGBombReverseDir : public ABGBombBase
{
	GENERATED_BODY()

public:

	ABGBombReverseDir();

// protected:
// 
// 	virtual void BeginPlay() override;

public:

	UFUNCTION(BlueprintCallable)
	void ReverseAllBombsDirection();

private:

	EBombType BombType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Bomb Game|Bomb")
	EConveyorDirection CurrentMovingDirection;

};
