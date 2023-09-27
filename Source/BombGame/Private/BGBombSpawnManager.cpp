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

	SpawnBombForAllConveyors();
}

void ABGBombSpawnManager::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

ABGBombBase* ABGBombSpawnManager::RequestSpawnNewBomb(int32 ConveyorId)
{
	ensureMsgf(AllConveyors.Contains(ConveyorId), TEXT("Request to Spawn New Bomb at the invalid conveyor %d"), ConveyorId);

	int32 BombTypeCnt = AllBombTypeClass.Num();

	int32 NewBombTypeIndex = FMath::RandRange(0, BombTypeCnt - 1);

	ABGConveyorBase* ConveyorRef = AllConveyors[ConveyorId];

	FActorSpawnParameters* SpawnParameters = new FActorSpawnParameters;
	SpawnParameters->SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ABGBombBase* NewBomb = Cast<ABGBombBase>(GetWorld()->SpawnActor<AActor>(AllBombTypeClass[NewBombTypeIndex], ConveyorRef->GetNewBombSpawnPosition(), FRotator::ZeroRotator, *SpawnParameters));

	if (NewBomb)
	{
		// TODO: Init Speed
		NewBomb->InitBomb(200.0f, ConveyorRef->GetCurrentMovingDirection(), ConveyorRef);

		NewBomb->OnBombExplodedDelegate.AddDynamic(this, &ThisClass::OnBombDestroyed);
	}

	return NewBomb;
}

ABGConveyorBase* ABGBombSpawnManager::GetConveyorrefById(int32 ConveyorId)
{
	if (AllConveyors.Contains(ConveyorId))
	{
		return AllConveyors[ConveyorId];
	}

	return nullptr;
}

void ABGBombSpawnManager::SpawnBombForAllConveyors()
{
	for (int32 ConveyorId = 0; ConveyorId < AllConveyors.Num(); ++ConveyorId)
	{
		RequestSpawnNewBomb(ConveyorId);
	}
}

void ABGBombSpawnManager::OnBombDestroyed(const EConveyorDirection MovingDirection, const int32 ConveyorId)
{
	RequestSpawnNewBomb(ConveyorId);
}

