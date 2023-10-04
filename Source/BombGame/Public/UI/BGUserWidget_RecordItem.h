// Copyright Five Guys Team.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BGUserWidget_RecordItem.generated.h"

/**
 * 
 */
UCLASS()
class BOMBGAME_API UBGUserWidget_RecordItem : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UTextBlock> Text_TeamName;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UTextBlock> Text_TeamScore;

public:
	UFUNCTION(BlueprintCallable)
	void UpdateRecordItem(const FString& InTeamName, const int32 InTeamScore, bool LatestRecord);
};
