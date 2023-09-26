// Fill out your copyright notice in the Description page of Project Settings.


#include "BGBombSpawnManager.h"
#include "../BGGameMode.h"

#include "Math/UnrealMathUtility.h"

ABGBombSpawnManager::ABGBombSpawnManager()
{
	PrimaryActorTick.bCanEverTick = false;

}

void ABGBombSpawnManager::BeginPlay()
{
	Super::BeginPlay();
	

	BGGameModeRef = Cast<ABGGameMode>(GetWorld()->GetAuthGameMode());
}

void ABGBombSpawnManager::RequestSpawnNewBomb(int32 ConveyorId)
{
	if (!BGGameModeRef)
	{
		return;
	}

	int32 BombTypeCnt = AllBombTypeClass.Num();

	int32 NewBombTypeIndex = FMath::RandRange(0, BombTypeCnt - 1);

	ABGConveyorBase* ConveyorRef = BGGameModeRef->GetConveyorrefById(ConveyorId);
}

