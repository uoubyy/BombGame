// Fill out your copyright notice in the Description page of Project Settings.


#include "BGBombSpawnManager.h"
#include "../BGGameMode.h"

#include "Math/UnrealMathUtility.h"
#include "BGConveyorBase.h"
#include "BGBombBase.h"

#include "Kismet/GameplayStatics.h"

ABGBombSpawnManager::ABGBombSpawnManager()
{
	PrimaryActorTick.bCanEverTick = false;

}

void ABGBombSpawnManager::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABGConveyorBase::StaticClass(), OutActors);

	ensureMsgf(OutActors.Num() > 0, TEXT("Failed to find any Conveyor in the Level"));

	for (auto TargetActor : OutActors)
	{
		if (ABGConveyorBase* ConveyorRef = Cast<ABGConveyorBase>(TargetActor))
		{
			int32 ConveyorId = ConveyorRef->GetConveyorId();
			ensureMsgf(!AllConveyors.Contains(ConveyorId), TEXT("Register duplicated conveyors with the same id %d"), ConveyorId);

			AllConveyors.Add({ ConveyorId , ConveyorRef });
		}
	}
}

void ABGBombSpawnManager::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	BGGameModeRef = Cast<ABGGameMode>(GetWorld()->GetAuthGameMode());
}

ABGBombBase* ABGBombSpawnManager::RequestSpawnNewBomb(int32 ConveyorId)
{
	ensureMsgf(AllConveyors.Contains(ConveyorId), TEXT("Register duplicated conveyors with the same id %d"), ConveyorId)

	int32 BombTypeCnt = AllBombTypeClass.Num();

	int32 NewBombTypeIndex = FMath::RandRange(0, BombTypeCnt - 1);

	ABGConveyorBase* ConveyorRef = BGGameModeRef->GetConveyorrefById(ConveyorId);

	FActorSpawnParameters* SpawnParameters = new FActorSpawnParameters;
	SpawnParameters->SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ABGBombBase* NewBomb = Cast<ABGBombBase>(GetWorld()->SpawnActor<AActor>(AllBombTypeClass[NewBombTypeIndex], ConveyorRef->GetNewBombSpawnPosition(), FRotator::ZeroRotator, *SpawnParameters));

	if (NewBomb)
	{
		// TODO: Init Speed
		NewBomb->InitBomb(50.0f, ConveyorRef->GetCurrentMovingDirection(), ConveyorRef);
	}

	return NewBomb;
}

