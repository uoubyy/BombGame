// Copyright Five Guys Team.


#include "UI/BGUserWidget_HUD.h"
#include "../BGGameMode.h"
#include "BGBombSpawnManager.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

#include "Blueprint/WidgetTree.h"

void UBGUserWidget_HUD::NativeConstruct()
{
	Super::NativeConstruct();
}

bool UBGUserWidget_HUD::Initialize()
{
	bool Result = Super::Initialize();

	if (ABGGameMode* BGGameMod = Cast<ABGGameMode>(GetWorld()->GetAuthGameMode()))
	{
		BGGameMod->OnTeamHealthChanged.AddDynamic(this, &ThisClass::OnTeamHealthChanged);
		BGGameMod->OnGameStateChanged.AddDynamic(this, &ThisClass::OnGameStateChanged);
		BGGameMod->OnTeamScoreChanged.AddDynamic(this, &ThisClass::OnTeamScoreChanged);

		BGGameMod->GetBombSpawnManager()->OnRandomEventActivated.AddDynamic(this, &ThisClass::OnRandomEventActivated);
	}

	RightTeamPoints.Empty();
	LeftTeamPoints.Empty();

	if (WidgetTree)
	{
		for (int Id = 1; Id <= 8; ++Id)
		{
			FString RightTeamH = FString::Printf(TEXT("RightTeam_H%d"), Id);
			FString LeftTeamH = FString::Printf(TEXT("LeftTeam_H%d"), Id);

			if (UImage* HealthPoint = Cast<UImage>(WidgetTree->FindWidget(FName(RightTeamH))))
			{
				//UE_LOG(LogTemp, Warning, TEXT("Find widget %s."), *RightTeamH);

				RightTeamPoints.Add({ Id - 1, HealthPoint });
			}

			if (UImage* HealthPoint = Cast<UImage>(WidgetTree->FindWidget(FName(LeftTeamH))))
			{
				//UE_LOG(LogTemp, Warning, TEXT("Find widget %s."), *LeftTeamH);

				LeftTeamPoints.Add({ Id - 1, HealthPoint });
			}
		}
	}

	return Result;
}

void UBGUserWidget_HUD::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (CurrentGameState == EGameState::GS_Start || CurrentGameState == EGameState::GS_InProgress)
	{
		ElapsedTime += InDeltaTime;
	}
}

void UBGUserWidget_HUD::OnTeamHealthChanged(int32 LeftTeamHealth, int32 RightTeamHealth)
{
	for (int32 Index = 7; Index >= LeftTeamHealth; --Index)
	{
		LeftTeamPoints[Index]->SetVisibility(ESlateVisibility::Collapsed);
	}

	for (int32 Index = 7; Index >= RightTeamHealth; --Index)
	{
		RightTeamPoints[Index]->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UBGUserWidget_HUD::OnTeamScoreChanged(ETeamId TargetTeam, int32 NewScore, int32 DeltaScore)
{
	if (TargetTeam == ETeamId::TI_Left)
	{
		Text_LeftTeamScore->SetText(FText::AsNumber(NewScore));
	}
	else
	{
		Text_RightTeamScore->SetText(FText::AsNumber(NewScore));
	}
}

void UBGUserWidget_HUD::OnGameStateChanged(const EGameState NewGameState)
{
	CurrentGameState = NewGameState;

	K2_OnGameStateChanged(NewGameState);
}
