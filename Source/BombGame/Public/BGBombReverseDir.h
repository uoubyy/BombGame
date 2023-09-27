// Copyright Five Guys Team.

#pragma once

#include "CoreMinimal.h"
#include "BGBombBase.h"
#include "BGBombSpawnManager.h"
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

};
