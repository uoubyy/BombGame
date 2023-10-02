// Copyright Five Guys Team.


#include "UI/BGUserWidget_Top10Records.h"
#include "UI/BGUserWidget_RecordItem.h"
#include "../BGGameplayEnum.h"
#include "Blueprint/WidgetTree.h"

bool UBGUserWidget_Top10Records::Initialize()
{
	bool Result = Super::Initialize();

	Top10Records.Empty();

	if (WidgetTree)
	{
		for(int Id = 0; Id < 10; ++Id)
		{ 
			FString WidgetName = FString::Printf(TEXT("WBP_RecordItem%d"), Id);
			if (WidgetTree->FindWidget(FName(WidgetName)))
			{
				UE_LOG(LogTemp, Warning, TEXT(""));
			}
			if (UBGUserWidget_RecordItem* RecordWidget = Cast<UBGUserWidget_RecordItem>(WidgetTree->FindWidget(FName(WidgetName))))
			{
				Top10Records.Add(RecordWidget);
			}
		}
	}

	return Result;
}

void UBGUserWidget_Top10Records::UpdateTop10Records(const TMap<int32, struct FRecordDetail>& InTop10Records)
{
	for (int Id = 0; Id < 10; ++Id)
	{
		if (InTop10Records[Id].TeamName.IsEmpty())
		{
			Top10Records[Id]->SetVisibility(ESlateVisibility::Collapsed);
		}
		else
		{
			Top10Records[Id]->SetVisibility(ESlateVisibility::HitTestInvisible);
			Top10Records[Id]->UpdateRecordItem(InTop10Records[Id].TeamName, InTop10Records[Id].TeamScore);
		}
	}
}
