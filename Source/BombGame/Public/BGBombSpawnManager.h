// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BGBombSpawnManager.generated.h"

UCLASS()
class BOMBGAME_API ABGBombSpawnManager : public AActor
{
	GENERATED_BODY()
	
public:	
	ABGBombSpawnManager();

protected:
	virtual void BeginPlay() override;

public:
	
	UFUNCTION(BlueprintCallable, Category = "Bomb Game|Spawn Manager")
	class ABGBombBase* RequestSpawnNewBomb(int32 ConveyorId);

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Bomb Game|Spawn Manager")
	TArray<TSubclassOf<class ABGBombBase>> AllBombTypeClass;

	UPROPERTY()
	TObjectPtr<class ABGGameMode> BGGameModeRef;
};
