// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../BGGameplayEnum.h"
#include "BGBombSpawnManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnRandomEventActivatedDelegate, ERandomEventType, EventType, const FString&, EventName, const FString&, EventDes);

UCLASS()
class BOMBGAME_API ABGBombSpawnManager : public AActor
{
	GENERATED_BODY()
	
public:	
	ABGBombSpawnManager();

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	virtual void PostInitializeComponents() override;

public:
	
	UFUNCTION(BlueprintCallable, Category = "Bomb Game|Spawn Manager")
	class ABGBombBase* RequestSpawnNewBomb(int32 ConveyorId);

	UFUNCTION(BlueprintCallable, Category = "Bomb Game|Spawn Manager")
	class ABGBombBase* RequestSpawnNewBombByType(int32 ConveyorId, TSubclassOf<class ABGBombBase> BombClass);

	UFUNCTION(BlueprintCallable, Category = "Bomb Game|Spawn Manager")
	class ABGConveyorBase* GetConveyorrefById(int32 ConveyorId);

	UFUNCTION(BlueprintCallable, Category = "Bomb Game|Spawn Manager")
	TArray<class ABGBombBase*> GetAllActiveBombsToTeam(ETeamId TargetTeam);

	UFUNCTION(BlueprintCallable, Category = "Bomb Game|Spawn Manager")
	TArray<class ABGConveyorBase*> GetAllConveyorsToTeam(ETeamId TargetTeam);

	UPROPERTY(BlueprintAssignable, Category = "Bomb Game|Spawn Manager")
	FOnRandomEventActivatedDelegate OnRandomEventActivated;
	 
protected:

	UFUNCTION(BlueprintCallable, Category = "Bomb Game|Spawn Manager")
	void SpawnBombForAllConveyors();

	UFUNCTION(BlueprintCallable, Category = "Bomb Game|Spawn Manager")
	void OnBombDestroyed(const EConveyorDirection MovingDirection, const int32 ConveyorId, const int32 DamageAmount, const int32 BombId);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Bomb Game|Spawn Manager")
	void K2_OnRandomEventLaneSwitch(ABGBombBase* BombOne, ABGBombBase* BombTwo);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Bomb Game|Spawn Manager")
	void K2_OnBlackHoleActivated();

	UFUNCTION(BlueprintCallable, Category = "Bomb Game|Spawn Manager")
	void PostSwitchLane(ABGBombBase* BombOne, ABGBombBase* BombTwo);

	UFUNCTION(BlueprintCallable, Category = "Bomb Game|Spawn Manager")
	void PostBlackHole();

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Bomb Game|Spawn Manager")
	TArray<TSubclassOf<class ABGBombBase>> AllBombTypeClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Bomb Game|Spawn Manager")
	float MinInitSpeed = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Bomb Game|Spawn Manager")
	float MaxInitSpeed = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Bomb Game|Spawn Manager")
	float BoostRate = 1.5f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Bomb Game|Spawn Manager")
	TMap<int32, class ABGConveyorBase*> AllConveyors;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Bomb Game|Spawn Manager")
	TMap<int32, class ABGBombBase*> AllActiveBombs;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Bomb Game|Spawn Manager")
	TArray<FRandomEventInfo> AllRandomEvents;

	UPROPERTY()
	TObjectPtr<class ABGGameMode> GameModeRef;

	float ElapsedTime;

	// TODO: Do not modify
	int32 BombUniqueId = 1;

};
