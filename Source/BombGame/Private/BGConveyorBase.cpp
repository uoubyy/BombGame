// Fill out your copyright notice in the Description page of Project Settings.


#include "BGConveyorBase.h"
#include <Components/SphereComponent.h>

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
}

void ABGConveyorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABGConveyorBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SetCurrentMovingDirection(InitMovingDirection);
}

void ABGConveyorBase::SetCurrentMovingDirection(EConveyorDirection NewDirection)
{
	CurrentMovingDirection = NewDirection;
	UpdateTeamBasedOnDirection();

	K2_OnMovingDirectionChanged();
	OnConveyorDirectionChanged.Broadcast(CurrentMovingDirection);
}

void ABGConveyorBase::OnConveyorTapped(class ABGCharacter* SourcePlayer)
{
	ABGPlayerState* BGPlayerState = SourcePlayer->GetPlayerState<ABGPlayerState>();
	
	ETeamId NewTeamId = SourcePlayer->GetPlayerState<ABGPlayerState>()->GetPlayerTeamId();
	if(NewTeamId != LastPressedTeam)
	{ 
		BGPlayerState->OnTapSucceed();
		LastPressedTeam = SourcePlayer->GetPlayerState<ABGPlayerState>()->GetPlayerTeamId();
		UpdateDirectionBasedOnTeam();
	}
}

void ABGConveyorBase::ReverseMovingDirection()
{
	EConveyorDirection NewDirection = CurrentMovingDirection == EConveyorDirection::CD_Left ? EConveyorDirection::CD_Right : EConveyorDirection::CD_Left;

	SetCurrentMovingDirection(NewDirection);
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
	CurrentMovingDirection = (LastPressedTeam == ETeamId::TI_Left) ? EConveyorDirection::CD_Right : EConveyorDirection::CD_Left;
	K2_OnMovingDirectionChanged();

	OnConveyorDirectionChanged.Broadcast(CurrentMovingDirection);
}

void ABGConveyorBase::UpdateTeamBasedOnDirection()
{
	LastPressedTeam = CurrentMovingDirection == EConveyorDirection::CD_Left ? ETeamId::TI_Right : ETeamId::TI_Left;
}

const FVector ABGConveyorBase::GetLeftSideEndPosition()
{
	return LeftSideEndPoint->GetComponentLocation();
}

const FVector ABGConveyorBase::GetConveyorRightDirection() const
{
	return GetActorRightVector();
}

