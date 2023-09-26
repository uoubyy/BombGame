// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BGConveyorBase.generated.h"

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
	class USceneComponent* LeftSideEndPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USceneComponent* RightSideEndPoint;

	virtual void BeginPlay() override;

public:	

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Bomb Game|Conveyor")
	const FVector GetConveyorRightDirection() const;

	UFUNCTION(BlueprintCallable, Category = "Bomb Game|Conveyor")
	const int32 GetConveyorId() const { return ConveyorId; }

	UFUNCTION(BlueprintCallable, Category = "Bomb Game|Conveyor")
	const FVector GetLeftSideEndPosition();

	UFUNCTION(BlueprintCallable, Category = "Bomb Game|Conveyor")
	const FVector GetRightSideEndPosition();

protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Bomb Game|Conveyor")
	int32 ConveyorId;
};
