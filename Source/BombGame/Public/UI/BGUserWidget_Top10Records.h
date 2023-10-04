// Copyright Five Guys Team.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BGUserWidget_Top10Records.generated.h"

/**
 * 
 */
UCLASS()
class BOMBGAME_API UBGUserWidget_Top10Records : public UUserWidget
{
	GENERATED_BODY()

protected:

	virtual bool Initialize() override;

	UPROPERTY(BlueprintReadOnly)
	TArray<TObjectPtr<class UBGUserWidget_RecordItem>> Top10Records;

public:
	UFUNCTION(BlueprintCallable)
	void UpdateTop10Records(const TMap<int32, struct FRecordDetail>& InTop10Records, int32 CurrentRecordRank);
	
};
