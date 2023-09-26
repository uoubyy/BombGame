// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BGCharacter.generated.h"

UCLASS(Blueprintable)
class ABGCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABGCharacter();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION()
	void OnCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit);

private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BombGame, meta = (AllowPrivateAccess = "true"))
	int TeamId;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BombGame, meta = (AllowPrivateAccess = "true"))
	int LaneId;
};

