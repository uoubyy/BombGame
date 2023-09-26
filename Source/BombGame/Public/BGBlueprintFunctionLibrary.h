// Copyright Five Guys Team.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "../BGGameplayEnum.h"
#include "BGBlueprintFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class BOMBGAME_API UBGBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	
	UFUNCTION(BlueprintCallable, Category = "Bomb Game|Gameplay")
	static AActor* FindClosestPlayerOfTeam(AActor* RequestActor, const ETeamId TargetTeam);
};
