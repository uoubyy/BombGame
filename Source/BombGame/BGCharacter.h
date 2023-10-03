// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BGGameplayEnum.h"
#include "BGCharacter.generated.h"

UCLASS(Blueprintable)
class ABGCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABGCharacter();

	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

	UFUNCTION()
	void OnCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit);

	UFUNCTION(BlueprintCallable)
	void SetPlayerId(FString NewPlayerId);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void K2_OnPlayerIdSet();

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = BombGame, meta = (AllowPrivateAccess = "true"))
	ETeamId TeamId;

protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = BombGame, meta = (AllowPrivateAccess = "true"))
	FString PlayerId;
};

