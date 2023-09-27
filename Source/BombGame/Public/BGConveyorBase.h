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

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Bomb Game|Conveyor")
	void K2_OnMovingDirectionChanged();

	UFUNCTION(BlueprintCallable, Category = "Bomb Game|Conveyor")
	void OnConveyorTapped(class ABGCharacter* SourcePlayer);

	UPROPERTY(BlueprintAssignable)
	FOnConveyorDirectionChangedDelegate OnConveyorDirectionChanged;

protected:
	
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Bomb Game|Conveyor")
	int32 ConveyorId;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Bomb Game|Conveyor")
	EConveyorDirection InitMovingDirection;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Bomb Game|Conveyor")
	EConveyorDirection CurrentMovingDirection;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Bomb Game|Conveyor")
	//ETeam LastPressedTeam;

	void UpdateDirectionBasedOnTeam();
};
