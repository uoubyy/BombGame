// Copyright Five Guys Team.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
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

	//--------------------------------------------------

	UFUNCTION()
	void OnTeamScoreChanged(int32 LeftTeamScore, int32 RightTeamScore);
};
