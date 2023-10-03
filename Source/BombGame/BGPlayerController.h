// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "BGPlayerController.generated.h"

/** Forward declaration to improve compiling times */
class UNiagaraSystem;

UCLASS()
class ABGPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ABGPlayerController();

	virtual void PostInitializeComponents() override;

	UFUNCTION()
	void OnControllerConnectionChanged(bool IsConnected, FPlatformUserId InUserId, int32 InDeviceId);

	UFUNCTION(BlueprintImplementableEvent, Category = "Bomb Game|PlayerController")
	void K2_OnControllerConnectionChanged(bool IsConnected, FPlatformUserId InUserId, int32 InDeviceId);
};


