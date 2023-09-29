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

	virtual bool Initialize() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

protected:

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UImage> LeftTeam_H1;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UImage> LeftTeam_H2;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UImage> LeftTeam_H3;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UImage> LeftTeam_H4;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UImage> LeftTeam_H5;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UImage> LeftTeam_H6;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UImage> LeftTeam_H7;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UImage> LeftTeam_H8;

	//-------------------------------------------------

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UImage> RightTeam_H1;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UImage> RightTeam_H2;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UImage> RightTeam_H3;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UImage> RightTeam_H4;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UImage> RightTeam_H5;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UImage> RightTeam_H6;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UImage> RightTeam_H7;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UImage> RightTeam_H8;

	UPROPERTY()
	TMap<int32, class UImage*> RightTeamPoints;

	UPROPERTY()
	TMap<int32, class UImage*> LeftTeamPoints;

	UPROPERTY(BlueprintReadOnly)
	float ElapsedTime;

	//--------------------------------------------------

	UFUNCTION()
	void OnTeamHealthChanged(int32 LeftTeamHealth, int32 RightTeamHealth);

	UFUNCTION()
	void OnGameStateChanged(const EGameState NewGameState);

	UFUNCTION(BlueprintImplementableEvent)
	void K2_OnGameStateChanged(const EGameState NewGameState);

	UFUNCTION(BlueprintImplementableEvent)
	void OnRandomEventActivated(ERandomEventType EventType, const FString& EventName, const FString& EventDes);

	EGameState CurrentGameState;
};
