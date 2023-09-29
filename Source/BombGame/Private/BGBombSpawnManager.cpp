// Fill out your copyright notice in the Description page of Project Settings.


#include "BGBombSpawnManager.h"
#include "../BGGameMode.h"

#include "Math/UnrealMathUtility.h"
#include "BGConveyorBase.h"
#include "BGBombBase.h"

#include "Kismet/GameplayStatics.h"

ABGBombSpawnManager::ABGBombSpawnManager()
{
	PrimaryActorTick.bCanEverTick = true;

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

	GameModeRef = Cast<ABGGameMode>(GetWorld()->GetAuthGameMode());
}

void ABGBombSpawnManager::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	ElapsedTime += DeltaSeconds;
	for (auto& RandomEvent : AllRandomEvents)
	{
		if (RandomEvent.HasUsed)
		{
			continue;
		}

		if (RandomEvent.ActiveTime <= ElapsedTime)
		{
			RandomEvent.HasUsed = true;
			OnRandomEventActivated.Broadcast(RandomEvent.EventType, RandomEvent.RandomEventName, RandomEvent.RandomEventDes);

			switch (RandomEvent.EventType)
			{
			case ERandomEventType::RET_ReverseAll:
				for (auto ConveyorInfo : AllConveyors)
				{
					if (ConveyorInfo.Value)
					{
						ConveyorInfo.Value->ReverseMovingDirection();
					}
				}
			break;

			case ERandomEventType::RET_BoostAll:
				for (auto BombInfo : AllActiveBombs)
				{
					if (BombInfo.Value)
					{
						UE_LOG(LogTemp, Warning, TEXT("Boost bomb original speed: %f"), BombInfo.Value->GetMovingSpeed());
						float TargetSpeed = BombInfo.Value->GetMovingSpeed()* BoostRate;
						BombInfo.Value->SetMovingSpeed(TargetSpeed);
						UE_LOG(LogTemp, Warning, TEXT("Boost bomb boost speed: %f"), TargetSpeed);
					}
				}

			break;

			case ERandomEventType::RET_SwitchLane:

			break;
			case ERandomEventType::RET_AddBomb:
				for (auto ConveyorInfo : AllConveyors) {
					if (ConveyorInfo.Value) {
						ABGBombBase* NewBomb = RequestSpawnNewBombByType(ConveyorInfo.Value->GetConveyorId(), RandomEvent.ChildBombClass);
						NewBomb->SetBombType(EBombType::BT_Child);
					}
				}
			break;
			}

		}
	}
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
		float InitSpeed = FMath::RandRange(MinInitSpeed, MaxInitSpeed);
		NewBomb->InitBomb(BombUniqueId, InitSpeed, ConveyorRef->GetCurrentMovingDirection(), ConveyorRef);

		NewBomb->OnBombExplodedDelegate.AddDynamic(this, &ThisClass::OnBombDestroyed);

		AllActiveBombs.Add({ BombUniqueId, NewBomb} );

		++BombUniqueId;
	}

	return NewBomb;
}

ABGBombBase* ABGBombSpawnManager::RequestSpawnNewBombByType(int32 ConveyorId, TSubclassOf<class ABGBombBase> BombClass)
{
	ensureMsgf(AllConveyors.Contains(ConveyorId), TEXT("Request to Spawn New Bomb at the invalid conveyor %d"), ConveyorId);
	UE_LOG(LogTemp, Warning, TEXT("Conveyor %i request to spawn another bomb"), ConveyorId);
	int32 BombTypeCnt = AllBombTypeClass.Num();

	if(BombClass == nullptr)
	{ 
		int32 NewBombTypeIndex = FMath::RandRange(0, BombTypeCnt - 1);
		BombClass = AllBombTypeClass[NewBombTypeIndex];
	}

	ABGConveyorBase* ConveyorRef = AllConveyors[ConveyorId];

	FActorSpawnParameters* SpawnParameters = new FActorSpawnParameters;
	SpawnParameters->SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ABGBombBase* NewBomb = Cast<ABGBombBase>(GetWorld()->SpawnActor<AActor>(BombClass, ConveyorRef->GetNewBombSpawnPosition(), FRotator::ZeroRotator, *SpawnParameters));

	if (NewBomb)
	{
		// TODO: Init Speed
		float InitSpeed = FMath::RandRange(MinInitSpeed, MaxInitSpeed);
		NewBomb->InitBomb(BombUniqueId, InitSpeed, ConveyorRef->GetCurrentMovingDirection(), ConveyorRef);

		NewBomb->OnBombExplodedDelegate.AddDynamic(this, &ThisClass::OnBombDestroyed);

		AllActiveBombs.Add({ BombUniqueId, NewBomb });

		++BombUniqueId;
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

TArray<class ABGBombBase*> ABGBombSpawnManager::GetAllActiveBombsToTeam(ETeamId TargetTeam)
{
	TArray<class ABGBombBase*> AllBombs;

	EConveyorDirection TargetDirection = (TargetTeam == ETeamId::TI_All) ? EConveyorDirection::CD_All : (TargetTeam == ETeamId::TI_Left ? EConveyorDirection::CD_Left : EConveyorDirection::CD_Right);

	for (auto& BombInfo : AllActiveBombs)
	{
		if (BombInfo.Value && (TargetDirection == EConveyorDirection::CD_All || BombInfo.Value->GetMovingDirection() == TargetDirection))
		{
			AllBombs.Add(BombInfo.Value);
		}
	}

	return AllBombs;
}

TArray<class ABGConveyorBase*> ABGBombSpawnManager::GetAllConveyorsToTeam(ETeamId TargetTeam)
{
	TArray<class ABGConveyorBase*> OutConveyors;

	EConveyorDirection TargetDirection = (TargetTeam == ETeamId::TI_All) ? EConveyorDirection::CD_All : (TargetTeam == ETeamId::TI_Left ? EConveyorDirection::CD_Left : EConveyorDirection::CD_Right);

	for (auto& ConveyorInfo : AllConveyors)
	{
		if (ConveyorInfo.Value->GetCurrentMovingDirection() == TargetDirection || TargetDirection == EConveyorDirection::CD_All)
		{
			OutConveyors.Add(ConveyorInfo.Value);
		}
	}

	return OutConveyors;
}

void ABGBombSpawnManager::SpawnBombForAllConveyors()
{
	for (int32 ConveyorId = 0; ConveyorId < AllConveyors.Num(); ++ConveyorId)
	{
		RequestSpawnNewBomb(ConveyorId);
	}
}

void ABGBombSpawnManager::OnBombDestroyed(const EConveyorDirection MovingDirection, const int32 ConveyorId, const int32 DamageAmount, const int32 BombId)
{
	ensureMsgf(AllActiveBombs.Contains(BombId), TEXT("OnBombDestroyed with Invalid Id %d"), BombId);

	ABGBombBase* BombRef = AllActiveBombs[BombId];
	if(BombRef->GetBombType() != EBombType::BT_Child)
	{ 
		RequestSpawnNewBomb(ConveyorId);
	}

	ETeamId TargetTeam = AllActiveBombs[BombId]->GetMovingDirection() == EConveyorDirection::CD_Left ? ETeamId::TI_Left : ETeamId::TI_Right;

	GameModeRef->ApplyDamage(TargetTeam, AllActiveBombs[BombId]->GetDamageAmount());

	AllActiveBombs.Remove(BombId);
}

