// Fill out your copyright notice in the Description page of Project Settings.


#include "BGBombBase.h"
#include <Components/SceneComponent.h>
#include <Components/StaticMeshComponent.h>
#include <Components/SphereComponent.h>
#include "BGConveyorBase.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"
#include "Math/Vector.h"
#include <UMG/Public/Components/WidgetComponent.h>

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

	IndicatorWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("IndicatorWidget"));
	IndicatorWidget->SetWidgetSpace(EWidgetSpace::Screen);
	IndicatorWidget->SetupAttachment(SceneRoot);
	IndicatorWidget->SetRelativeLocation(FVector(0.0f, -140.0f, 360.0f));

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

void ABGBombBase::InitBomb(int32 BombId, float InitSpeed, EConveyorDirection InitMovingDirection, ABGConveyorBase* ParentConveyor)
{
	ensureMsgf(ParentConveyor, TEXT("InitBomb %s with invalid Conveyor."), *GetName());

	BombUniqueId = BombId;

	CurrentMovingSpeed = InitSpeed;
	CurrentMovingDirection = InitMovingDirection;
	MaxMovingSpeed = InitSpeed * 2.0f;

	SetAttachedConveyor(ParentConveyor, false);

	BombStatus = EBombStatus::BS_Moving;
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

void ABGBombBase::SetMovingSpeed(float NewSpeed)
{
	if (BombStatus >= EBombStatus::BS_Exploded)
	{
		return; // Do nothing if pending destroying
	}
	CurrentMovingSpeed = NewSpeed;
	CurrentMovingSpeed = FMath::Clamp(CurrentMovingSpeed, 0.0f, MaxMovingSpeed);
	AttachedConveyor->K2_OnMovingSpeedChanged(CurrentMovingSpeed);
}

void ABGBombBase::SetAttachedConveyor(ABGConveyorBase* NewConveyor, bool ResetPosition)
{
	if (AttachedConveyor == NewConveyor && !ResetPosition)
	{
		return;
	}

	if (AttachedConveyor != NewConveyor)
	{
		if(AttachedConveyor)
		{ 
			AttachedConveyor->OnConveyorDirectionChanged.RemoveDynamic(this, &ThisClass::OnConveyorDirectionChanged);
		}
		if(NewConveyor && BombStatus < EBombStatus::BS_Exploded)
		{ 
			NewConveyor->OnConveyorDirectionChanged.AddDynamic(this, &ThisClass::OnConveyorDirectionChanged);
		}
	}

	AttachedConveyor = NewConveyor;
	AttachedConveyorId = AttachedConveyor->GetConveyorId();
	CurrentMovingDirection = AttachedConveyor->GetCurrentMovingDirection();

	if (ResetPosition)
	{
		SetActorLocation(AttachedConveyor->GetNewBombSpawnPosition());
	}

	RecalculateTargetPosition();

	ToggleMovement(true);
	ToggleVisibilityAndCollision(true);
}

void ABGBombBase::ToggleMovement(bool EnableOrNot)
{
	if (BombStatus >= EBombStatus::BS_Exploded)
	{
		return; // Do nothing if pending destroying
	}

	BombStatus = EnableOrNot ? EBombStatus::BS_Moving : EBombStatus::BS_Paused;
}

void ABGBombBase::RotateMovingToPoint(FVector TargetCenter, float DeltaSeconds, float Speed, float InitDist)
{
	if (BombStatus == EBombStatus::BS_Exploded)
	{
		return; // Do nothing if pending destroying
	}

	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	TriggerComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	FVector Direction = (GetActorLocation() - TargetCenter).GetSafeNormal();
	float Dist = (GetActorLocation() - TargetCenter).Length();
	Dist -= Speed * DeltaSeconds;

	const FRotator Rot(0.0f, Speed * DeltaSeconds, 0.0f);
	FVector NewDirection = Rot.RotateVector(Direction);

	FVector NewPosition = TargetCenter + NewDirection * Dist;
	SetActorLocation(NewPosition);

	// always hide the indicator when rotate moving to target
	IndicatorWidget->SetVisibility(false);

	if (Dist <= 10.0f)
	{
		ToggleVisibilityAndCollision(false);
	}
}

void ABGBombBase::OnTriggerBeginOverlap_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// UE_LOG(LogTemp, Warning, TEXT("ABGBombBase %s id %d OnTriggerBeginOverlap with %s."), *GetName(), BombUniqueId, *OtherActor->GetName());

	OnBombExploded();
}

void ABGBombBase::OnBombExploded()
{
	if (BombStatus >= EBombStatus::BS_Exploded)
	{
		return;
	}

	if (AttachedConveyor)
	{
		AttachedConveyor->OnConveyorDirectionChanged.RemoveDynamic(this, &ThisClass::OnConveyorDirectionChanged);
	}

	BombStatus = EBombStatus::BS_Exploded;

	ToggleVisibilityAndCollision(false);

	if(ExplodeSoundWave)
	{ 
		UGameplayStatics::PlaySound2D(this, ExplodeSoundWave);
	}

	// TODO: Timer type bomb we need test the position to decide damage direction

	K2_OnBombExploded();
}

void ABGBombBase::PostBombExploded_Implementation()
{
	// TODO: OnBombExplodedDelegate -> SpawnManager Request Spawn New Bomb 
	// The AttachedConveyor is set to nullptr before Request Spawn New Bomb will cause issue
	OnBombExplodedDelegate.Broadcast(CurrentMovingDirection, AttachedConveyorId, DamageAmount, BombUniqueId);

	// AttachedConveyor = nullptr;
	Destroy();
}

void ABGBombBase::ToggleVisibilityAndCollision(bool EnableOrNot)
{
	MeshComponent->SetVisibility(EnableOrNot);
	IndicatorWidget->SetVisibility(EnableOrNot);

	MeshComponent->SetCollisionEnabled(EnableOrNot ? ECollisionEnabled::QueryAndPhysics : ECollisionEnabled::NoCollision);
	TriggerComponent->SetCollisionEnabled(EnableOrNot ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);

	K2_ToggleVisibilityAndCollision(EnableOrNot);
}

void ABGBombBase::RecalculateTargetPosition()
{
	if (BombStatus >= EBombStatus::BS_Exploded)
	{
		return;
	}

	if (!AttachedConveyor)
	{
		UE_LOG(LogTemp, Warning, TEXT("RecalculateTargetPosition but AttachedConveyor is nullptr"));
		return;
	}
	TargetPosition = CurrentMovingDirection == EConveyorDirection::CD_Left ? AttachedConveyor->GetLeftSideEndPosition() : AttachedConveyor->GetRightSideEndPosition();
	TargetPosition -= GetActorLocation();

	float Length = FVector::DotProduct(TargetPosition, AttachedConveyor->GetConveyorRightDirection());
	TargetPosition = GetActorLocation() + Length * AttachedConveyor->GetConveyorRightDirection();

	// DrawDebugSphere(GetWorld(), TargetPosition, 100, 20, FColor::Red, false, 10);
}
