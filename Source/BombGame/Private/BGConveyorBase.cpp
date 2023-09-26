// Fill out your copyright notice in the Description page of Project Settings.


#include "BGConveyorBase.h"

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
	
}

void ABGConveyorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

const FVector ABGConveyorBase::GetNewBombSpawnPosition()
{
	return FVector::Zero();
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

