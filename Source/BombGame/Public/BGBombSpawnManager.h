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

	virtual void PostInitializeComponents() override;

public:
	
	UFUNCTION(BlueprintCallable, Category = "Bomb Game|Spawn Manager")
	class ABGBombBase* RequestSpawnNewBomb(int32 ConveyorId);

	UFUNCTION(BlueprintCallable, Category = "Bomb Game|Spawn Manager")
	class ABGConveyorBase* GetConveyorrefById(int32 ConveyorId);

	UFUNCTION(BlueprintCallable, Category = "Bomb Game|Spawn Manager")
	TArray<class ABGBombBase*> GetAllActiveBombsToTeam(ETeamId TargetTeam);

protected:

	UFUNCTION(BlueprintCallable, Category = "Bomb Game|Spawn Manager")
	void SpawnBombForAllConveyors();

	UFUNCTION(BlueprintCallable, Category = "Bomb Game|Spawn Manager")
	void OnBombDestroyed(const EConveyorDirection MovingDirection, const int32 ConveyorId, const int32 DamageAmount, const int32 BombId);

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Bomb Game|Spawn Manager")
	TArray<TSubclassOf<class ABGBombBase>> AllBombTypeClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Bomb Game|Spawn Manager")
	TMap<int32, class ABGConveyorBase*> AllConveyors;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Bomb Game|Spawn Manager")
	TMap<int32, class ABGBombBase*> AllActiveBombs;

	UPROPERTY()
	TObjectPtr<class ABGGameMode> GameModeRef;
};
