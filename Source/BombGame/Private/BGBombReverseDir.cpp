// Copyright Five Guys Team.


#include "BGBombReverseDir.h"
#include "BGBlueprintFunctionLibrary.h"
#include "../BGGameplayEnum.h"


ABGBombReverseDir::ABGBombReverseDir()
{
	BombType = EBombType::BT_Reverse;
}

void ABGBombReverseDir::ReverseAllBombsDirection()
{
	ETeamId TargetTeam = GetMovingDirection() == EConveyorDirection::CD_Left ? ETeamId::TI_Right : ETeamId::TI_Left;
	TArray<ABGBombBase*> AllActiveBombs = UBGBlueprintFunctionLibrary::GetAllBombsMovingToTeam(this, TargetTeam);
	for (auto Bomb : AllActiveBombs)
	{
		Bomb->ReverseMovingDirection();
	}
}
