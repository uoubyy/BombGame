// Copyright Five Guys Team.


#include "BGBombReverseDir.h"

ABGBombBase::ABGBombReverseDir()
{
	PrimaryActorTick.bCanEverTick = true;
// 
// 	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
// 	SetRootComponent(SceneRoot);
// 	SceneRoot->SetMobility(EComponentMobility::Movable);
// 
// 	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
// 	MeshComponent->SetupAttachment(SceneRoot);
// 
// 	TriggerComponent = CreateDefaultSubobject<USphereComponent>(TEXT("TriggerBox"));
// 	TriggerComponent->SetupAttachment(SceneRoot);
// 
// 	BombStatus = EBombStatus::BS_Idle;
}

void ABGBombReverseDir::BeginPlay()
{

}

void ABGBombReverseDir::ReverseAllBombsDirection()
{
	
}
