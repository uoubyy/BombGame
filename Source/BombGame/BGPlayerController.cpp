// Copyright Epic Games, Inc. All Rights Reserved.

#include "BGPlayerController.h"
#include "GameFramework/Pawn.h"
//#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "BGCharacter.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "BGGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "BGGameMode.h"

ABGPlayerController::ABGPlayerController()
{
}

void ABGPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if(UBGGameInstance* BGGameInstance = Cast<UBGGameInstance>(GetGameInstance()))
	{
		BGGameInstance->OnControllerConnectionChangedDelegate.AddDynamic(this, &ThisClass::OnControllerConnectionChanged);
	}
}

void ABGPlayerController::OnControllerConnectionChanged(bool IsConnected, FPlatformUserId InUserId, int32 InDeviceId)
{
	if (IsConnected && GetPlatformUserId() == InUserId)
	{
		
	}

	K2_OnControllerConnectionChanged(IsConnected, InUserId, InDeviceId);
}
