// Copyright Five Guys Team.

#pragma once

#include "CoreMinimal.h"
#include "BGBombBase.h"
#include "BGBombReverseDir.generated.h"

/**
 * 
 */
UCLASS()
class BOMBGAME_API ABGBombReverseDir : public ABGBombBase
{
	GENERATED_BODY()

public:

	ABGBombBase();

protected:

	virtual void BeginPlay() override;

public:

	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable)
	void ReverseAllBombsDirection();

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Bomb Game|Bomb")
	EBombStatus BombStatus;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Bomb Game|Bomb")
	EConveyorDirection CurrentMovingDirection;

};
