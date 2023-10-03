// Copyright Epic Games, Inc. All Rights Reserved.

#include "BGCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include <Kismet/GameplayStatics.h>

ABGCharacter::ABGCharacter()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	PrimaryActorTick.bCanEverTick = false;

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ABGCharacter::OnCollision);
}

void ABGCharacter::BeginPlay()
{
	if (ActorHasTag("LeftTeam"))
	{
		TeamId = ETeamId::TI_Left;
	}
	else
	{
		TeamId = ETeamId::TI_Right;
	}

	Super::BeginPlay();
}


void ABGCharacter::PostInitializeComponents()
{
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		int32 ControllerId = UGameplayStatics::GetPlayerControllerID(PlayerController);
		EAutoReceiveInput::Type AutoPossesTarget = EAutoReceiveInput::Type(ControllerId + 1);
		AutoReceiveInput = AutoPossesTarget;
		AutoPossessPlayer = AutoPossesTarget;
	}

	Super::PostInitializeComponents();
}

void ABGCharacter::OnCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{

}

void ABGCharacter::SetPlayerId(FString NewPlayerId)
{
	PlayerId = NewPlayerId;
	K2_OnPlayerIdSet();
}
