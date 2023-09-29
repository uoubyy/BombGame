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
						float TargetSpeed = BombInfo.Value->GetMovingSpeed() * BoostRate;
						BombInfo.Value->SetMovingSpeed(TargetSpeed);
						UE_LOG(LogTemp, Warning, TEXT("Boost bomb boost speed: %f"), TargetSpeed);
					}
				}

				break;

			case ERandomEventType::RET_SwitchLane:
			{
				int32 BombMapSize = AllActiveBombs.Num();
				int32 BombOneIndex = FMath::RandRange(0, BombMapSize - 1);
				int32 BombTwoIndex = BombOneIndex;

				while (BombOneIndex == BombTwoIndex)
				{
					BombTwoIndex = FMath::RandRange(0, BombMapSize - 1);
				}

				ABGBombBase* BombOne = nullptr;
				ABGBombBase* BombTwo = nullptr;

				int32 Index = 0;
				auto It = AllActiveBombs.CreateConstIterator();
				for (It, Index; It; ++It, ++Index)
				{
					if (Index == BombOneIndex)
					{
						BombOne = It.Value();
					}

					if (Index == BombTwoIndex)
					{
						BombTwo = It.Value();
					}
				}

				if (BombOne && BombTwo)
				{
					K2_OnRandomEventLaneSwitch(BombOne, BombTwo);
				}
			}

			break;
			case ERandomEventType::RET_AddBomb:
				for (auto ConveyorInfo : AllConveyors) {
					if (ConveyorInfo.Value) {
						TSubclassOf<class ABGBombBase> ChildBombClass;
						for (auto& BombInfo : AllActiveBombs) {
							if (BombInfo.Value->GetAttachedConveyor()->GetConveyorId() == ConveyorInfo.Value->GetConveyorId()) {
								if (BombInfo.Value->GetDamageAmount() > 0){ // don't add another damage bomb if there is already one on the conveyor
									ChildBombClass = RandomEvent.ChildBombClass;
								}else{
									int32 BombTypeCnt = AllBombTypeClass.Num();
									int32 NewBombTypeIndex = FMath::RandRange(0, BombTypeCnt - 1);
									ChildBombClass = AllBombTypeClass[NewBombTypeIndex];
								}
							}
						}
						ABGBombBase* NewBomb = RequestSpawnNewBombByType(ConveyorInfo.Value->GetConveyorId(), ChildBombClass);
						NewBomb->SetBombType(EBombType::BT_Child);
					}
				}
				break;
			case ERandomEventType::RET_BlackHole:
				K2_OnBlackHoleActivated();
				for (auto BombInfo : AllActiveBombs)
				{
					if (BombInfo.Value)
					{
						BombInfo.Value->ToggleMovement(false);
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
		float NewMaxInitSpeed = MaxInitSpeed;
		for (auto& BombInfo : AllActiveBombs) {
			if (BombInfo.Value->GetAttachedConveyor()->GetConveyorId() == ConveyorId) {
			// TODO: fix bomb overlap issue
				NewMaxInitSpeed = BombInfo.Value->GetMovingSpeed(); 
			}
		}
		// TODO: Init Speed
		float InitSpeed = FMath::RandRange(MinInitSpeed, NewMaxInitSpeed);
		NewBomb->InitBomb(BombUniqueId, InitSpeed, ConveyorRef->GetCurrentMovingDirection(), ConveyorRef);

		NewBomb->OnBombExplodedDelegate.AddDynamic(this, &ThisClass::OnBombDestroyed);

		AllActiveBombs.Add({ BombUniqueId, NewBomb });

		++BombUniqueId;
	}

	return NewBomb;
}

ABGBombBase* ABGBombSpawnManager::RequestSpawnNewBombByType(int32 ConveyorId, TSubclassOf<class ABGBombBase> BombClass)
{
	ensureMsgf(AllConveyors.Contains(ConveyorId), TEXT("Request to Spawn New Bomb at the invalid conveyor %d"), ConveyorId);
	UE_LOG(LogTemp, Warning, TEXT("Conveyor %i request to spawn another bomb"), ConveyorId);
	int32 BombTypeCnt = AllBombTypeClass.Num();

	if (BombClass == nullptr)
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
		// set child bomb speed to be equal to parent bomb speed
		for (auto& BombInfo : AllActiveBombs) {
			if (BombInfo.Value->GetAttachedConveyor()->GetConveyorId() == ConveyorId) {
				InitSpeed = BombInfo.Value->GetMovingSpeed();
			}
		}
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
	UE_LOG(LogTemp, Warning, TEXT("BombSpawnManager OnBombDestroyed %d"), BombId);
	if (!AllActiveBombs.Contains(BombId))
	{
		return;
	}
	// ensureMsgf(AllActiveBombs.Contains(BombId), TEXT("OnBombDestroyed with Invalid Id %d"), BombId);

	ABGBombBase* BombRef = AllActiveBombs[BombId];
	if (BombRef->GetBombType() != EBombType::BT_Child)
	{
		RequestSpawnNewBomb(ConveyorId);
	}

	ETeamId TargetTeam = AllActiveBombs[BombId]->GetMovingDirection() == EConveyorDirection::CD_Left ? ETeamId::TI_Left : ETeamId::TI_Right;

	GameModeRef->ApplyDamage(TargetTeam, AllActiveBombs[BombId]->GetDamageAmount());

	AllActiveBombs.Remove(BombId);
}

void ABGBombSpawnManager::PostSwitchLane(ABGBombBase* BombOne, ABGBombBase* BombTwo)
{
	if (BombOne && BombTwo)
	{
		ABGConveyorBase* ConveyorOfBombOne = BombOne->GetAttachedConveyor();
		ABGConveyorBase* ConveyorOfBombTwo = BombTwo->GetAttachedConveyor();

		ConveyorOfBombOne->OnConveyorDirectionChanged.RemoveDynamic(BombOne, &ABGBombBase::OnConveyorDirectionChanged);
		ConveyorOfBombTwo->OnConveyorDirectionChanged.RemoveDynamic(BombTwo, &ABGBombBase::OnConveyorDirectionChanged);

		BombOne->SetAttachedConveyor(ConveyorOfBombTwo);
		BombTwo->SetAttachedConveyor(ConveyorOfBombOne);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PostSwitchLane with Invalid Bomb!"));
	}
}
