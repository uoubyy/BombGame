// Copyright Five Guys Team.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "BGPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class BOMBGAME_API ABGPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Bomb Game|Player State")
	const ETeamId GetPlayerTeamId() const { return TeamId; }

	UFUNCTION(BlueprintCallable, Category = "Bomb Game|Player State")
	void SetPlayerTeamId(const ETeamId NewTeamId) { TeamId = NewTeamId; }
	
protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Bomb Game|Player State")
	ETeamId TeamId;
};
