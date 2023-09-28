// Copyright Five Guys Team.


#include "BGBombReverseDir.h"
#include "BGBlueprintFunctionLibrary.h"
#include "../BGGameplayEnum.h"


ABGBombReverseDir::ABGBombReverseDir()
{
	BombType = EBombType::BT_Reverse;
}

void ABGBombReverseDir::OnBombExploded_Implementation()
{
	Super::OnBombExploded_Implementation();

	ReverseAllBombsDirection();
}

void ABGBombReverseDir::ReverseAllBombsDirection()
{
	ETeamId TargetTeam = GetMovingDirection() == EConveyorDirection::CD_Left ? ETeamId::TI_Right : ETeamId::TI_Left;
	TArray<ABGBombBase*> AllActiveBombs = UBGBlueprintFunctionLibrary::GetAllBombsMovingToTeam(this, TargetTeam);
	for (auto Bomb : AllActiveBombs)
	{
		UE_LOG(LogTemp, Warning, TEXT("reverse bomb"));
		Bomb->ReverseMovingDirection();
	}
}


