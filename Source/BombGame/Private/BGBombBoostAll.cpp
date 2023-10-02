// Copyright Five Guys Team.


#include "BGBombBoostAll.h"
#include "BGBlueprintFunctionLibrary.h"
#include "../BGGameplayEnum.h"
#include "TimerManager.h"
#include "BGConveyorBase.h"

ABGBombBoostAll::ABGBombBoostAll()
{
	BombType = EBombType::BT_BoostAll;
}

void ABGBombBoostAll::OnBombExploded()
{
	Super::OnBombExploded();

	BoostAllBombs(BoostDuration, BoostRate);

	PostBombExploded();
}

void ABGBombBoostAll::BoostAllBombs(float Duration, float Rate)
{
	ETeamId TargetTeam = GetMovingDirection() == EConveyorDirection::CD_Left ? ETeamId::TI_Left : ETeamId::TI_Right;
	TArray<ABGBombBase*> AllBombs = UBGBlueprintFunctionLibrary::GetAllBombsMovingToTeam(this, TargetTeam);
/*	AllBombs = UBGBlueprintFunctionLibrary::GetAllBombsMovingToTeam(this, TargetTeam);*/
	float TargetMovingSpeed = GetMovingSpeed() * Rate;
	for (auto Bomb : AllBombs)
	{
		UE_LOG(LogTemp, Warning, TEXT("Speed up speed %d "), GetMovingSpeed());
		Bomb->SetMovingSpeed(TargetMovingSpeed);
	}

// 	GetWorldTimerManager().SetTimer(BoostDurationTimerHandler, this, &ABGBombBoostAll::ReturnToOriginSpeed, 0, false, Duration);
	GetWorldTimerManager().SetTimer(BoostDurationTimerHandler, this, &ABGBombBoostAll::ReturnToOriginSpeed, Duration, false, 0);
}

void ABGBombBoostAll::ReturnToOriginSpeed()
{
// 	for (auto Bomb : AllBombs)
// 	{
// 		if (Bomb)
// 		{
// 			UE_LOG(LogTemp, Warning, TEXT("Return to original speed, bomb %d"), Bomb);
// 			float TempMovingSpeed = Bomb->GetMovingSpeed() / 2;
// 			Bomb->SetMovingSpeed(TempMovingSpeed);
// 		}
// 		
// 	}
}