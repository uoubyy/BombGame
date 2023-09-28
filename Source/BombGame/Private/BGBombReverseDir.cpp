// Copyright Five Guys Team.


#include "BGBombReverseDir.h"
#include "BGBlueprintFunctionLibrary.h"
#include "../BGGameplayEnum.h"
#include "BGConveyorBase.h"


ABGBombReverseDir::ABGBombReverseDir()
{
	BombType = EBombType::BT_Reverse;
}

void ABGBombReverseDir::OnBombExploded()
{
	Super::OnBombExploded();

	ReverseAllBombsDirection();

	PostBombExploded();
}

void ABGBombReverseDir::ReverseAllBombsDirection()
{
	ETeamId TargetTeam = GetMovingDirection() == EConveyorDirection::CD_Left ? ETeamId::TI_Right : ETeamId::TI_Left;
	// if left team get a bomb, making all bombs moving to right team reverse
	// the source of direction is conveyor so we need reverse conveyor's direction
	TArray<ABGConveyorBase*> AllConveyors = UBGBlueprintFunctionLibrary::GetAllConveyosMovingToTeam(this, TargetTeam);
	{
		for (auto Conveyor : AllConveyors)
		{
			EConveyorDirection NewDirection = Conveyor->GetCurrentMovingDirection() == EConveyorDirection::CD_Left ? EConveyorDirection::CD_Right : EConveyorDirection::CD_Left;

			Conveyor->SetCurrentMovingDirection(NewDirection);
		}
	}
}


