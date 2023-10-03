// Copyright Five Guys Team.


#include "BGGameInstance.h"
#include "Misc/CoreDelegates.h"

void UBGGameInstance::Init()
{
	Super::Init();
	FCoreDelegates::OnControllerConnectionChange.AddUObject(this, &ThisClass::OnControllerConnectionChanged);
}

void UBGGameInstance::SetTeamName(const ETeamId TargetTeam, const FString& InTeamName)
{
	FString& TeamName = TeamNames.FindOrAdd(TargetTeam);
	TeamName = InTeamName;
	UE_LOG(LogTemp, Warning, TEXT("Set Team Name: %s"), *InTeamName);
}

void UBGGameInstance::OnControllerConnectionChanged(bool IsConnected, FPlatformUserId InUserId, int32 InDeviceId)
{
	FString StrIsConnected = IsConnected ? "Connected" : "DisConnected";
	UE_LOG(LogTemp, Warning, TEXT("OnControllerConnectionChanged: UserId %d DeviceId %d %s"), InUserId, InDeviceId, *StrIsConnected);

	OnControllerConnectionChangedDelegate.Broadcast(IsConnected, InUserId, InDeviceId);
}

const FString UBGGameInstance::GetTeamName(const ETeamId TargetTeam)
{
	FString DefaultTeamName = TargetTeam == ETeamId::TI_Left ? "AAA" : "BBB";
	FString& TeamName = TeamNames.FindOrAdd(TargetTeam, DefaultTeamName);
	return TeamName;
}
