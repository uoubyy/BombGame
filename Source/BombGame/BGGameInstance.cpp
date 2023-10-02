// Copyright Five Guys Team.


#include "BGGameInstance.h"

void UBGGameInstance::SetTeamName(const ETeamId TargetTeam, const FString& InTeamName)
{
	FString& TeamName = TeamNames.FindOrAdd(TargetTeam);
	TeamName = InTeamName;
}

const FString UBGGameInstance::GetTeamName(const ETeamId TargetTeam)
{
	FString& TeamName = TeamNames.FindOrAdd(TargetTeam);
	return TeamName;
}
