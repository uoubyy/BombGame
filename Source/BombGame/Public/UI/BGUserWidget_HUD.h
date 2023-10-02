// Copyright Five Guys Team.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../BGGameplayEnum.h"
#include "BGUserWidget_HUD.generated.h"

/**
 * 
 */
UCLASS()
class BOMBGAME_API UBGUserWidget_HUD : public UUserWidget
{
	GENERATED_BODY()

protected:

	virtual void NativeConstruct() override;
	
	virtual bool Initialize() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

protected:

	UPROPERTY(BlueprintReadOnly)
	TMap<int32, class UImage*> RightTeamPoints;

	UPROPERTY(BlueprintReadOnly)
	TMap<int32, class UImage*> LeftTeamPoints;

	UPROPERTY(BlueprintReadOnly)
	float ElapsedTime;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UTextBlock> Text_LeftTeamScore;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UTextBlock> Text_RightTeamScore;

	//--------------------------------------------------

	UFUNCTION()
	void OnTeamHealthChanged(int32 LeftTeamHealth, int32 RightTeamHealth);

	UFUNCTION()
	void OnTeamScoreChanged(ETeamId TargetTeam, int32 NewScore, int32 DeltaScore);

	UFUNCTION()
	void OnGameStateChanged(const EGameState NewGameState);

	UFUNCTION(BlueprintImplementableEvent)
	void K2_OnGameStateChanged(const EGameState NewGameState);

	UFUNCTION(BlueprintImplementableEvent)
	void OnRandomEventActivated(ERandomEventType EventType, const FString& EventName, const FString& EventDes);

	EGameState CurrentGameState;
};
