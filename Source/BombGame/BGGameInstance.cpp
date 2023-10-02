// Copyright Five Guys Team.


#include "BGGameInstance.h"

void UBGGameInstance::SetTeamName(const ETeamId TargetTeam, const FString& InTeamName)
{
	FString& TeamName = TeamNames.FindOrAdd(TargetTeam);
	TeamName = InTeamName;
	UE_LOG(LogTemp, Warning, TEXT("Set Team Name: %s"), *InTeamName);
}

const FString UBGGameInstance::GetTeamName(const ETeamId TargetTeam)
{
	FString& TeamName = TeamNames.FindOrAdd(TargetTeam);
	return TeamName;
}
