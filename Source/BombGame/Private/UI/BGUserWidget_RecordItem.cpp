// Copyright Five Guys Team.


#include "UI/BGUserWidget_RecordItem.h"
#include "Components/TextBlock.h"

void UBGUserWidget_RecordItem::UpdateRecordItem(const FString& InTeamName, const int32 InTeamScore)
{
	Text_TeamName->SetText(FText::FromString(InTeamName));
	Text_TeamScore->SetText(FText::AsNumber(InTeamScore));
}
