// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../BGGameplayEnum.h"
#include "Components/BoxComponent.h"
#include "BGConveyorBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnConveyorDirectionChangedDelegate, const EConveyorDirection, Direction);

UCLASS()
class BOMBGAME_API ABGConveyorBase : public AActor
{
	GENERATED_BODY()
	
public:	

	ABGConveyorBase();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USceneComponent* SceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UBoxComponent* LeftSideEndPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UBoxComponent* RightSideEndPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USphereComponent* LeftTriggerComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USphereComponent* RightTriggerComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bomb Game|Conveyor")
	ETeamId LastPressedTeam;

	virtual void BeginPlay() override;

public:	

	virtual void Tick(float DeltaTime) override;

	virtual void PostInitializeComponents() override;

	UFUNCTION(BlueprintCallable, Category = "Bomb Game|Conveyor")
	const FVector GetConveyorRightDirection() const;

	UFUNCTION(BlueprintCallable, Category = "Bomb Game|Conveyor")
	const int32 GetConveyorId() const { return ConveyorId; }

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Bomb Game|Conveyor")
	const FVector GetNewBombSpawnPosition();

	UFUNCTION(BlueprintCallable, Category = "Bomb Game|Conveyor")
	const FVector GetLeftSideEndPosition();

	UFUNCTION(BlueprintCallable, Category = "Bomb Game|Conveyor")
	const FVector GetRightSideEndPosition();

	UFUNCTION(BlueprintCallable, Category = "Bomb Game|Conveyor")
	const EConveyorDirection GetCurrentMovingDirection() const { return CurrentMovingDirection; }

	UFUNCTION(BlueprintCallable, Category = "Bomb Game|Conveyor")
	void SetCurrentMovingDirection(EConveyorDirection NewDirection);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Bomb Game|Conveyor")
	void K2_OnMovingDirectionChanged();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Bomb Game|Conveyor")
	void K2_OnMovingSpeedChanged(float NewMovingSpeed);

	UFUNCTION(BlueprintCallable, Category = "Bomb Game|Conveyor")
	bool OnConveyorTapped(class ABGCharacter* SourcePlayer);

	UFUNCTION(BlueprintCallable, Category = "Bomb Game|Conveyor")
	void ReverseMovingDirection();

	UPROPERTY(BlueprintAssignable)
	FOnConveyorDirectionChangedDelegate OnConveyorDirectionChanged;

	UPROPERTY(EditDefaultsOnly, Category = "Bomb Game|Conveyor")
	USoundWave* TapSuccessSoundWave;

protected:
	
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Bomb Game|Conveyor")
	int32 ConveyorId;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Bomb Game|Conveyor")
	EConveyorDirection InitMovingDirection;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Bomb Game|Conveyor")
	EConveyorDirection CurrentMovingDirection;

	void UpdateDirectionBasedOnTeam();

	void UpdateTeamBasedOnDirection();
};
