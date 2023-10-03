// Copyright Five Guys Team.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "BGGameplayEnum.h"
#include "BGGameInstance.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnControllerConnectionChangedDelegate, bool, IsConnected, FPlatformUserId, InUserId, int32, InDeviceId);

/**
 * 
 */
UCLASS()
class BOMBGAME_API UBGGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bomb Game|GameInstance")
	TMap<ETeamId, FString> TeamNames;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bomb Game|GameInstance")
	TSoftObjectPtr<class UWorld> SelectedMapRef;

public:
	virtual void Init() override;

	UFUNCTION(BlueprintCallable, Category = "Bomb Game|GameInstance")
	void SetTeamName(const ETeamId TargetTeam, const FString& InTeamName);

	UFUNCTION(BlueprintCallable, Category = "Bomb Game|GameInstance")
	const FString GetTeamName(const ETeamId TargetTeam);

	UFUNCTION(BlueprintCallable, Category = "Bomb Game|GameInstance")
	TSoftObjectPtr<class UWorld> GetSelectedMapRef() const { return SelectedMapRef; }

	UFUNCTION(BlueprintCallable, Category = "Bomb Game|GameInstance")
	void SetSelectedMap(TSoftObjectPtr<class UWorld> InLevelRef) { SelectedMapRef  = InLevelRef; }

	UFUNCTION()
	void OnControllerConnectionChanged(bool IsConnected, FPlatformUserId InUserId, int32 InDeviceId);

	UPROPERTY(BlueprintAssignable)
	FOnControllerConnectionChangedDelegate OnControllerConnectionChangedDelegate;
};
