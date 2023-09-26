// Fill out your copyright notice in the Description page of Project Settings.


#include "BGConveyorBase.h"

// Sets default values
ABGConveyorBase::ABGConveyorBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABGConveyorBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABGConveyorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

