// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../BGGameplayEnum.h"
#include "BGBombBase.generated.h"

UCLASS()
class BOMBGAME_API ABGBombBase : public AActor
{
	GENERATED_BODY()
	
public:	

	ABGBombBase();

protected:

	virtual void BeginPlay() override;

public:	

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USceneComponent* SceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USphereComponent* TriggerSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Bomb Game|Bomb")
	int32 ConveyorId;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Bomb Game|Bomb")
	EBombType BombType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Bomb Game|Bomb")
	float MaxLifeTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Bomb Game|Bomb")
	EConveyorDirection CurrentMovingDirection;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Bomb Game|Bomb")
	float CurrentMovingSpeed;
	
};
