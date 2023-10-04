// Copyright Five Guys Team.


#include "UI/BGUserWidget_RecordItem.h"
#include "Components/TextBlock.h"

void UBGUserWidget_RecordItem::UpdateRecordItem(const FString& InTeamName, const int32 InTeamScore, bool LatestRecord)
{
	Text_TeamName->SetText(FText::FromString(InTeamName));
	Text_TeamScore->SetText(FText::AsNumber(InTeamScore));

	FColor GoldColor(255, 125, 0);

	if(LatestRecord)
	{
		Text_TeamName->SetColorAndOpacity(GoldColor);
		Text_TeamScore->SetColorAndOpacity(GoldColor);
	}
	else
	{
		Text_TeamName->SetColorAndOpacity(FColor::White);
		Text_TeamScore->SetColorAndOpacity(FColor::White);
	}
}
