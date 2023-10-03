// Copyright Five Guys Team.

#pragma once

#include "CoreMinimal.h"
//#include "Engine/DataAsset.h"
#include "Engine/DataTable.h"
#include "BGDataTable_MapInfo.generated.h"

//USTRUCT(BlueprintType)
//struct FBGMapConfig
//{
//	GENERATED_BODY()
//
//public:
//	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
//	FString MapName;
//
//	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
//	int32 MaxPlayerCnt = 4;
//
//	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
//	class UTexture2D* MapIcon = nullptr;
//
//	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
//	TSoftObjectPtr<class UWorld> LevelSoftRef = nullptr;
//};

/**
 * 
 */
USTRUCT(BlueprintType)
struct BOMBGAME_API FBGDataTable_MapInfo : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString MapName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 MaxPlayerCnt = 4;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UTexture2D* MapIcon = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<class UWorld> LevelSoftRef = nullptr;
};
