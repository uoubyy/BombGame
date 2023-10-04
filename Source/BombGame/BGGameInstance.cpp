// Copyright Five Guys Team.


#include "BGGameInstance.h"
#include "GenericPlatform/GenericPlatformInputDeviceMapper.h"

void UBGGameInstance::Init()
{
	Super::Init();
	IPlatformInputDeviceMapper::Get().GetOnInputDeviceConnectionChange().AddUObject(this, &ThisClass::OnControllerConnectionChanged);
}

void UBGGameInstance::SetTeamName(const ETeamId TargetTeam, const FString& InTeamName)
{
	FString& TeamName = TeamNames.FindOrAdd(TargetTeam);
	TeamName = InTeamName;
	UE_LOG(LogTemp, Warning, TEXT("Set Team Name: %s"), *InTeamName);
}

void UBGGameInstance::OnControllerConnectionChanged(EInputDeviceConnectionState IsConnected, FPlatformUserId InUserId, FInputDeviceId InDeviceId)
{
	FString StrIsConnected = IsConnected == EInputDeviceConnectionState::Connected ? "Connected" : "DisConnected";
	UE_LOG(LogTemp, Warning, TEXT("OnControllerConnectionChanged: UserId %d DeviceId %d %s"), InUserId, InDeviceId.GetId(), *StrIsConnected);

	OnControllerConnectionChangedDelegate.Broadcast(IsConnected == EInputDeviceConnectionState::Connected, InUserId, InDeviceId.GetId());
}

const FString UBGGameInstance::GetTeamName(const ETeamId TargetTeam)
{
	FString DefaultTeamName = TargetTeam == ETeamId::TI_Left ? "AAA" : "BBB";
	FString& TeamName = TeamNames.FindOrAdd(TargetTeam, DefaultTeamName);
	return TeamName;
}
