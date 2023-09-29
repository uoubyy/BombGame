// Copyright Five Guys Team.


#include "UI/BGUserWidget_HUD.h"
#include "../BGGameMode.h"
#include "BGBombSpawnManager.h"
#include "Components/Image.h"

bool UBGUserWidget_HUD::Initialize()
{
	bool Result = Super::Initialize();

	if (ABGGameMode* BGGameMod = Cast<ABGGameMode>(GetWorld()->GetAuthGameMode()))
	{
		BGGameMod->OnTeamHealthChanged.AddDynamic(this, &ThisClass::OnTeamHealthChanged);
		BGGameMod->OnGameStateChanged.AddDynamic(this, &ThisClass::OnGameStateChanged);

		BGGameMod->GetBombSpawnManager()->OnRandomEventActivated.AddDynamic(this, &ThisClass::OnRandomEventActivated);
	}
	
	RightTeamPoints.Add({0, RightTeam_H1});
	RightTeamPoints.Add({1, RightTeam_H2});
	RightTeamPoints.Add({2, RightTeam_H3});
	RightTeamPoints.Add({3, RightTeam_H4});
	RightTeamPoints.Add({4, RightTeam_H5});
	RightTeamPoints.Add({5, RightTeam_H6});
	RightTeamPoints.Add({6, RightTeam_H7});
	RightTeamPoints.Add({7, RightTeam_H8});

	LeftTeamPoints.Add({0, LeftTeam_H8});
	LeftTeamPoints.Add({1, LeftTeam_H7});
	LeftTeamPoints.Add({2, LeftTeam_H6});
	LeftTeamPoints.Add({3, LeftTeam_H5});
	LeftTeamPoints.Add({4, LeftTeam_H4});
	LeftTeamPoints.Add({5, LeftTeam_H3});
	LeftTeamPoints.Add({6, LeftTeam_H2});
	LeftTeamPoints.Add({7, LeftTeam_H1});

	return Result;
}

void UBGUserWidget_HUD::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (CurrentGameState == EGameState::GS_Start)
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

void UBGUserWidget_HUD::OnGameStateChanged(const EGameState NewGameState)
{
	CurrentGameState = NewGameState;

	K2_OnGameStateChanged(NewGameState);
}
