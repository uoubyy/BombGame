// Fill out your copyright notice in the Description page of Project Settings.


#include "BGConveyorBase.h"
#include <Components/SphereComponent.h>
#include "../BGGameMode.h"
#include "../BGPlayerState.h"
#include "../BGCharacter.h"

ABGConveyorBase::ABGConveyorBase()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);
	SceneRoot->SetMobility(EComponentMobility::Static);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComponent->SetupAttachment(SceneRoot);

	LeftSideEndPoint = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftSideEndPoint"));
	LeftSideEndPoint->SetupAttachment(SceneRoot);

	RightSideEndPoint = CreateDefaultSubobject<UBoxComponent>(TEXT("RightSideEndPoint"));
	RightSideEndPoint->SetupAttachment(SceneRoot);

	LeftTriggerComponent = CreateDefaultSubobject<USphereComponent>(TEXT(" Left TriggerBox"));
	LeftTriggerComponent->SetupAttachment(SceneRoot);

	RightTriggerComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Right TriggerBox"));
	RightTriggerComponent->SetupAttachment(SceneRoot);
	
}

void ABGConveyorBase::BeginPlay()
{
	Super::BeginPlay();

	CurrentMovingDirection = InitMovingDirection;
	LastPressedTeam = CurrentMovingDirection == EConveyorDirection::CD_Left ? ETeamId::TI_Left : ETeamId::TI_Right;

	K2_OnMovingDirectionChanged();
}

void ABGConveyorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABGConveyorBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	ABGGameMode* BGGameMode = Cast<ABGGameMode>(GetWorld()->GetAuthGameMode());

	if (BGGameMode)
	{
		BGGameMode->RegisterConveyor(ConveyorId, this);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to register Conveyor %d, Game mode not detected."), ConveyorId);
	}
}

void ABGConveyorBase::OnConveyorTapped(class ABGCharacter* SourcePlayer)
{
	LastPressedTeam = SourcePlayer->GetPlayerState<ABGPlayerState>()->GetPlayerTeamId();
	UpdateDirectionBasedOnTeam();
}

const FVector ABGConveyorBase::GetNewBombSpawnPosition_Implementation()
{
	return FVector::Zero(); // TODO: override in Blueprint
}

const FVector ABGConveyorBase::GetRightSideEndPosition()
{
	return RightSideEndPoint->GetComponentLocation();
}

void ABGConveyorBase::UpdateDirectionBasedOnTeam()
{

	ABGGameMode* BGGameMode = Cast<ABGGameMode>(GetWorld()->GetAuthGameMode());

	CurrentMovingDirection = (LastPressedTeam == ETeamId::TI_Left) ? EConveyorDirection::CD_Left : EConveyorDirection::CD_Right;
	K2_OnMovingDirectionChanged();


}

const FVector ABGConveyorBase::GetLeftSideEndPosition()
{
	return LeftSideEndPoint->GetComponentLocation();
}

const FVector ABGConveyorBase::GetConveyorRightDirection() const
{
	return GetActorRightVector();
}

