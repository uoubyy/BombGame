// Copyright Five Guys Team.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BGUserWidget_GameStatistics.generated.h"

/**
 * 
 */
UCLASS()
class BOMBGAME_API UBGUserWidget_GameStatistics : public UUserWidget
{
	GENERATED_BODY()
	
protected:

	virtual void NativeConstruct() override;

	virtual bool Initialize() override;

protected:
	
	UPROPERTY(BlueprintReadOnly)
	TMap<FString, class UTextBlock*> Text_PlayerName;

	UPROPERTY(BlueprintReadOnly)
	TMap<FString, class UTextBlock*> Text_PlayerTapCnt;

	UPROPERTY(BlueprintReadOnly)
	TMap<FString, class UTextBlock*> Text_PlayerFrozenCnt;

	UPROPERTY(BlueprintReadOnly)
	TMap<FString, class UTextBlock*> Text_PlayerStunCnt;

	class UTextBlock* Text_MPV;

	int32 MPVScore = 0;

public:

	UFUNCTION(BlueprintCallable)
	void UpdatePlayerStatisticInfo(FString TargetPlayer, FString PlayerName, int32 TapCnt, int32 FrozenCnt, int32 StunCnt);
};
