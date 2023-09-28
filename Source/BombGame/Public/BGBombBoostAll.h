// Copyright Five Guys Team.

#pragma once

#include "CoreMinimal.h"
#include "BGBombBase.h"
#include "BGBombSpawnManager.h"
#include "BGBombBoostAll.generated.h"

/**
 * 
 */
UCLASS()
class BOMBGAME_API ABGBombBoostAll : public ABGBombBase
{
	GENERATED_BODY()
public:

	ABGBombBoostAll();

public:

	UFUNCTION(BlueprintCallable)
	void BoostAllBombs(float Duration, float Rate);

	UFUNCTION(BlueprintCallable)
	void ReturnToOriginSpeed();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Bomb Game|Boost All Bomb")
	float BoostDuration;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Bomb Game|Boost All Bomb")
	float BoostRate;

protected:

	virtual void OnBombExploded() override;

private:

	FTimerHandle BoostDurationTimerHandler;
	EBombType BombType;
	TArray<ABGBombBase*> AllBombs;
};
