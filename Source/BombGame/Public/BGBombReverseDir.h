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

public:

	UFUNCTION(BlueprintCallable)
	void ReverseAllBombsDirection();

protected:

	virtual void OnBombExploded() override;

private:

	EBombType BombType;

};
