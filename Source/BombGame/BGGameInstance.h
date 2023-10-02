// Copyright Five Guys Team.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "BGGameplayEnum.h"
#include "BGGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class BOMBGAME_API UBGGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bomb Game|GameInstance")
	TMap<ETeamId, FString> TeamNames;

public:
	UFUNCTION(BlueprintCallable, Category = "Bomb Game|GameInstance")
	void SetTeamName(const ETeamId TargetTeam, const FString& InTeamName);

	UFUNCTION(BlueprintCallable, Category = "Bomb Game|GameInstance")
	const FString GetTeamName(const ETeamId TargetTeam);
};
