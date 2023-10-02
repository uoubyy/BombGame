// Copyright Five Guys Team.


#include "UI/BGUserWidget_GameStatistics.h"
#include "Components/TextBlock.h"
#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanelSlot.h"

void UBGUserWidget_GameStatistics::NativeConstruct()
{
	Super::NativeConstruct();
}

bool UBGUserWidget_GameStatistics::Initialize()
{
	bool Result = Super::Initialize();

	Text_PlayerName.Empty();
	Text_PlayerTapCnt.Empty();
	Text_PlayerFrozenCnt.Empty();
	Text_PlayerStunCnt.Empty();

	Text_MPV = Cast<UTextBlock>(WidgetTree->FindWidget(FName("Text_MPV")));

	for (int Id = 0; Id < 4; ++Id)
	{
		FString PlayerId = FString::Printf(TEXT("P%d"), Id);

		FString Text_PlayerTapCntName = FString::Printf(TEXT("Text_P%d_TapCnt"), Id);
		FString Text_PlayerFrozenCntName = FString::Printf(TEXT("Text_P%d_FrozenCnt"), Id);
		FString	Text_PlayerStunCntName = FString::Printf(TEXT("Text_P%d_StunCnt"), Id);

		if (UTextBlock* TextItem = Cast<UTextBlock>(WidgetTree->FindWidget(FName(FString::Printf(TEXT("Text_P%d_Name"), Id)))))
		{
			Text_PlayerName.Add({ PlayerId , TextItem });
		}

		if (UTextBlock* TextItem = Cast<UTextBlock>(WidgetTree->FindWidget(FName(Text_PlayerTapCntName))))
		{
			Text_PlayerTapCnt.Add({ PlayerId , TextItem });
		}

		if (UTextBlock* TextItem = Cast<UTextBlock>(WidgetTree->FindWidget(FName(Text_PlayerFrozenCntName))))
		{
			Text_PlayerFrozenCnt.Add({ PlayerId , TextItem });
		}

		if (UTextBlock* TextItem = Cast<UTextBlock>(WidgetTree->FindWidget(FName(Text_PlayerStunCntName))))
		{
			Text_PlayerStunCnt.Add({ PlayerId , TextItem });
		}
	}

	return Result;
}

void UBGUserWidget_GameStatistics::UpdatePlayerStatisticInfo(FString TargetPlayer, FString PlayerName, int32 TapCnt, int32 FrozenCnt, int32 StunCnt)
{
	if (auto TextItem = Text_PlayerTapCnt.Find(TargetPlayer))
	{
		(*TextItem)->SetText(FText::AsNumber(TapCnt));
	}

	if (auto TextItem = Text_PlayerFrozenCnt.Find(TargetPlayer))
	{
		(*TextItem)->SetText(FText::AsNumber(FrozenCnt));
	}

	if (auto TextItem = Text_PlayerStunCnt.Find(TargetPlayer))
	{
		(*TextItem)->SetText(FText::AsNumber(StunCnt));
	}

	if (Text_MPV && TapCnt > MPVScore)
	{
		Text_MPV->SetVisibility(ESlateVisibility::HitTestInvisible);
		MPVScore = TapCnt;
		if (auto TextItem = Text_PlayerName.Find(TargetPlayer))
		{
			UWidget* Widget = *TextItem;
			FGeometry Geometry = GetCachedGeometry();
			FVector2D Position = Geometry.AbsoluteToLocal(Widget->GetCachedGeometry().GetAbsolutePosition()) + Widget->GetCachedGeometry().GetLocalSize() / 2.0f;

			UCanvasPanelSlot* Text_MPV_Slot = Cast<UCanvasPanelSlot>(Text_MPV->Slot);
			Text_MPV_Slot->SetPosition(Position);
		}
	}
}
