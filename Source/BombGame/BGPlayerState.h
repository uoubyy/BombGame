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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Bomb Game|Player State")
	int32 SuccessTapCnt = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Bomb Game|Player State")
	int32 StunCnt = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Bomb Game|Player State")
	int32 FrozeCnt = 0;

public:
	UFUNCTION(BlueprintCallable, Category = "Bomb Game|Player State")
	void OnTapSucceed() { ++SuccessTapCnt; }

	UFUNCTION(BlueprintCallable, Category = "Bomb Game|Player State")
	void OnBeingFrozen() { ++FrozeCnt; }

	UFUNCTION(BlueprintCallable, Category = "Bomb Game|Player State")
	void OnBeingStun() { ++StunCnt; }

	UFUNCTION(BlueprintCallable, Category = "Bomb Game|Player State")
	const int32 GetTapCnt() const { return SuccessTapCnt; }

	UFUNCTION(BlueprintCallable, Category = "Bomb Game|Player State")
	const int32 GetFrozenCnt() const { return FrozeCnt; }

	UFUNCTION(BlueprintCallable, Category = "Bomb Game|Player State")
	const int32 GetStunCnt() const { return StunCnt; }
	
};
