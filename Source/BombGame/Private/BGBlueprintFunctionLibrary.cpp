// Copyright Five Guys Team.


#include "BGBlueprintFunctionLibrary.h"
#include <GameFramework/GameStateBase.h>
#include "../BGPlayerState.h"

#include "Math/Vector.h"

AActor* UBGBlueprintFunctionLibrary::FindClosestPlayerOfTeam(AActor* RequestActor, const ETeamId TargetTeam)
{
	AActor* ClosestPawn = false;
	double ClosestDist = 0.0f;
	if (AGameStateBase* GameStateBase = RequestActor->GetWorld()->GetGameState())
	{
		for (APlayerState* PlayerState : GameStateBase->PlayerArray)
		{
			if (ABGPlayerState* BGPlayerState = Cast<ABGPlayerState>(PlayerState))
			{
				if (BGPlayerState->GetPlayerTeamId() != TargetTeam)
				{
					continue;
				}

				APawn* Pawn = BGPlayerState->GetPawn();
				if (!Pawn)
				{
					continue; // un-possess will case no pawn of PlayerState
				}
				double DistToPawn = FVector::Distance(Pawn->GetActorLocation(), RequestActor->GetActorLocation());
				if (!ClosestPawn || DistToPawn <= ClosestDist)
				{
					ClosestDist = DistToPawn;
					ClosestPawn = Pawn;
				}
			}
		}
	}

	return ClosestPawn;
}