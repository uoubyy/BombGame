// Copyright Five Guys Team.


#include "BGGameInstance.h"

void UBGGameInstance::SetTeamName(const ETeamId TargetTeam, const FString& InTeamName)
{
	FString& TeamName = TeamNames.FindOrAdd(TargetTeam);
	TeamName = InTeamName;
}

const FString UBGGameInstance::GetTeamName(const ETeamId TargetTeam)
{
	FString DefaultTeamName = TargetTeam == ETeamId::TI_Left ? "AAA" : "BBB";
	FString& TeamName = TeamNames.FindOrAdd(TargetTeam, DefaultTeamName);
	return TeamName;
}
