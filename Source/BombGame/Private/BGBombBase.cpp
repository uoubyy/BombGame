// Fill out your copyright notice in the Description page of Project Settings.


#include "BGBombBase.h"
#include <Components/SceneComponent.h>
#include <Components/StaticMeshComponent.h>
#include <Components/SphereComponent.h>
#include "BGConveyorBase.h"

#include "Math/UnrealMathUtility.h"
#include "Math/Vector.h"

ABGBombBase::ABGBombBase()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);
	SceneRoot->SetMobility(EComponentMobility::Movable);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComponent->SetupAttachment(SceneRoot);

	TriggerComponent = CreateDefaultSubobject<USphereComponent>(TEXT("TriggerBox"));
	TriggerComponent->SetupAttachment(SceneRoot);

	BombStatus = EBombStatus::BS_Idle;
}

void ABGBombBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABGBombBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	TriggerComponent->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnTriggerBeginOverlap);
}

void ABGBombBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (BombStatus != EBombStatus::BS_Moving)
	{
		return;
	}

	if (BombType == EBombType::BT_Timer)
	{
		BombLifeTime += DeltaSeconds;

		if (BombLifeTime >= MaxLifeTime)
		{
			OnBombExploded();
		}
	}

	FVector NewActorLocation = FMath::VInterpConstantTo(GetActorLocation(), TargetPosition, DeltaSeconds, CurrentMovingSpeed);
	SetActorLocation(NewActorLocation);
}

void ABGBombBase::BeginDestroy()
{
	OnBombExplodedDelegate.Clear();

	Super::BeginDestroy();
}

void ABGBombBase::InitBomb(float InitSpeed, EConveyorDirection InitMovingDirection, ABGConveyorBase* ParentConveyor)
{
	ensureMsgf(ParentConveyor, TEXT("InitBomb %s with invalid Conveyor."), *GetName());

	CurrentMovingSpeed = InitSpeed;
	CurrentMovingDirection = InitMovingDirection;

	AttachedConveyor = ParentConveyor;
	AttachedConveyorId = AttachedConveyor->GetConveyorId();

	BombStatus = EBombStatus::BS_Moving;

	RecalculateTargetPosition();

	AttachedConveyor->OnConveyorDirectionChanged.AddDynamic(this, &ThisClass::OnConveyorDirectionChanged);
}

void ABGBombBase::ReverseMovingDirection()
{
	// TODO: Safety check: disable this when Exploded
	CurrentMovingDirection = CurrentMovingDirection == EConveyorDirection::CD_Left ? EConveyorDirection::CD_Right : EConveyorDirection::CD_Left;

	RecalculateTargetPosition();

	// TODO: rotation animation
}

void ABGBombBase::OnConveyorDirectionChanged(EConveyorDirection NewDirection)
{
	CurrentMovingDirection = NewDirection;
	RecalculateTargetPosition();
}

void ABGBombBase::OnTriggerBeginOverlap_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("ABGBombBase %s OnTriggerBeginOverlap with %s."), *GetName(), *OtherActor->GetName());

	OnBombExploded();
}

void ABGBombBase::OnBombExploded_Implementation()
{
	AttachedConveyor = nullptr;
	BombStatus = EBombStatus::BS_Exploded;

	OnBombExplodedDelegate.Broadcast(CurrentMovingDirection, AttachedConveyorId);

	// TODO: Timer type bomb we need test the position to decide damage direction

	Destroy();
}

void ABGBombBase::RecalculateTargetPosition()
{
	TargetPosition = CurrentMovingDirection == EConveyorDirection::CD_Left ? AttachedConveyor->GetLeftSideEndPosition() : AttachedConveyor->GetRightSideEndPosition();
	TargetPosition -= GetActorLocation();

	float Length = FVector::DotProduct(TargetPosition, AttachedConveyor->GetConveyorRightDirection());
	TargetPosition = GetActorLocation() + Length * AttachedConveyor->GetConveyorRightDirection();

	DrawDebugSphere(GetWorld(), TargetPosition, 100, 20, FColor::Red, false, 10);
}
