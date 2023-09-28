// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../BGGameplayEnum.h"
#include "BGBombBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnBombExplodedDelegate, const EConveyorDirection, Direction, const int32, ConveyorId, const int32, Damage, const int32, BombId);

UCLASS()
class BOMBGAME_API ABGBombBase : public AActor
{
	GENERATED_BODY()
	
public:	

	ABGBombBase();

protected:

	virtual void BeginPlay() override;

public:	

	virtual void PostInitializeComponents() override;

	virtual void Tick(float DeltaSeconds) override;

	virtual void BeginDestroy() override;

	UFUNCTION(BlueprintCallable, Category = "Bomb Game|Bomb")
	virtual void InitBomb(int32 BombId, float InitSpeed, EConveyorDirection InitMovingDirection, class ABGConveyorBase* ParentConveyor);

	UFUNCTION(BlueprintCallable, Category = "Bomb Game|Bomb")
	void ReverseMovingDirection();

	UPROPERTY(BlueprintAssignable)
	FOnBombExplodedDelegate OnBombExplodedDelegate;

	UFUNCTION(BlueprintCallable, Category = "Bomb Game|Bomb")
	void OnConveyorDirectionChanged(EConveyorDirection NewDirection);

	UFUNCTION(BlueprintCallable, Category = "Bomb Game|Bomb")
	const EConveyorDirection GetMovingDirection() const { return CurrentMovingDirection; }

	UFUNCTION(BlueprintCallable, Category = "Bomb Game|Bomb")
	float GetMovingSpeed() const { return CurrentMovingSpeed; }

	UFUNCTION(BlueprintCallable, Category = "Bomb Game|Bomb")
	void SetMovingSpeed(float NewSpeed) { CurrentMovingSpeed = NewSpeed; }

	UFUNCTION(BlueprintCallable, Category = "Bomb Game|Bomb")
	int32 GetDamageAmount() const { return DamageAmount; }

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USceneComponent* SceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USphereComponent* TriggerComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Bomb Game|Bomb")
	int32 AttachedConveyorId;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Bomb Game|Bomb")
	EBombType BombType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Bomb Game|Bomb")
	float MaxLifeTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Bomb Game|Bomb")
	float CurrentMovingSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Bomb Game|Bomb")
	int32 BombUniqueId;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Bomb Game|Bomb")
	void OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void OnBombExploded();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Bomb Game|Bomb")
	void K2_OnBombExploded();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Bomb Game|Bomb")
	void PostBombExploded();

private:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Bomb Game|Bomb")
	int32 DamageAmount;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Bomb Game|Bomb")
	EBombStatus BombStatus;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Bomb Game|Bomb")
	float BombLifeTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Bomb Game|Bomb")
	EConveyorDirection CurrentMovingDirection;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Bomb Game|Bomb")
	FVector TargetPosition;

	UPROPERTY()
	TObjectPtr<class ABGConveyorBase> AttachedConveyor;

	void RecalculateTargetPosition();
};
