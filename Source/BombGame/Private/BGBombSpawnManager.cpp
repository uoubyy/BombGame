// Fill out your copyright notice in the Description page of Project Settings.


#include "BGBombSpawnManager.h"
#include "../BGGameMode.h"

#include "Math/UnrealMathUtility.h"
#include "BGConveyorBase.h"
#include "BGBombBase.h"

ABGBombSpawnManager::ABGBombSpawnManager()
{
	PrimaryActorTick.bCanEverTick = false;

}

void ABGBombSpawnManager::BeginPlay()
{
	Super::BeginPlay();
	

	BGGameModeRef = Cast<ABGGameMode>(GetWorld()->GetAuthGameMode());
}

ABGBombBase* ABGBombSpawnManager::RequestSpawnNewBomb(int32 ConveyorId)
{
	if (!BGGameModeRef)
	{
		return nullptr;
	}

	int32 BombTypeCnt = AllBombTypeClass.Num();

	int32 NewBombTypeIndex = FMath::RandRange(0, BombTypeCnt - 1);

	ABGConveyorBase* ConveyorRef = BGGameModeRef->GetConveyorrefById(ConveyorId);

	FActorSpawnParameters* SpawnParameters = new FActorSpawnParameters;
	SpawnParameters->SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ABGBombBase* NewBomb = Cast<ABGBombBase>(GetWorld()->SpawnActor<AActor>(AllBombTypeClass[NewBombTypeIndex], ConveyorRef->GetNewBombSpawnPosition(), FRotator::ZeroRotator, *SpawnParameters));

	if (NewBomb)
	{
		// TODO: Init Speed
		NewBomb->InitBomb(200.0f, ConveyorRef->GetCurrentMovingDirection(), ConveyorRef);
	}

	return NewBomb;
}

