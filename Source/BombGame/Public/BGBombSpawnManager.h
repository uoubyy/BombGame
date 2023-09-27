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

protected:

	UFUNCTION(BlueprintCallable, Category = "Bomb Game|Spawn Manager")
	void SpawnBombForAllConveyors();

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Bomb Game|Spawn Manager")
	TArray<TSubclassOf<class ABGBombBase>> AllBombTypeClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Bomb Game|Spawn Manager")
	TMap<int32, class ABGConveyorBase*> AllConveyors;
};
