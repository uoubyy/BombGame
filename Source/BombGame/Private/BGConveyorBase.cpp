// Fill out your copyright notice in the Description page of Project Settings.


#include "BGConveyorBase.h"
#include "../BGGameMode.h"

ABGConveyorBase::ABGConveyorBase()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);
	SceneRoot->SetMobility(EComponentMobility::Static);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComponent->SetupAttachment(SceneRoot);

	LeftSideEndPoint = CreateDefaultSubobject<USceneComponent>(TEXT("LeftSideEndPoint"));
	LeftSideEndPoint->SetupAttachment(SceneRoot);

	RightSideEndPoint = CreateDefaultSubobject<USceneComponent>(TEXT("RightSideEndPoint"));
	RightSideEndPoint->SetupAttachment(SceneRoot);
}

void ABGConveyorBase::BeginPlay()
{
	Super::BeginPlay();

	CurrentMovingDirection = InitMovingDirection;
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

const FVector ABGConveyorBase::GetNewBombSpawnPosition_Implementation()
{
	return FVector::Zero(); // TODO: override in Blueprint
}

const FVector ABGConveyorBase::GetRightSideEndPosition()
{
	return RightSideEndPoint->GetComponentLocation();
}

const FVector ABGConveyorBase::GetLeftSideEndPosition()
{
	return LeftSideEndPoint->GetComponentLocation();
}

const FVector ABGConveyorBase::GetConveyorRightDirection() const
{
	return GetActorRightVector();
}

