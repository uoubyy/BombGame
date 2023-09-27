#pragma once
#include "CoreMinimal.h"
#include "BGGameplayEnum.generated.h"

UENUM(BlueprintType)
enum class EBombType : uint8
{
	BT_None		UMETA(DisplayName = "None"),
	BT_Regular	UMETA(DisplayName = "Regular Bomb"),
	BT_Timer	UMETA(DisplayName = "Timer Bomb"),
	BT_Reverse	UMETA(DisplayName = "Reverse Bomb"),
	Count		UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EBombStatus : uint8
{
	BS_None		UMETA(DisplayName = "None"),
	BS_Idle		UMETA(DisplayName = "Idle"),
	BS_Moving	UMETA(DisplayName = "Moving"),
	BS_Exploded	UMETA(DisplayName = "Exploded"),
	Count		UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EConveyorDirection : uint8
{
	CD_None		UMETA(DisplayName = "None"),
	CD_Left		UMETA(DisplayName = "Left"),
	CD_Right	UMETA(DisplayName = "Right"),
	Count		UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EGameState : uint8
{
	GS_Idle		UMETA(DisplayName = "Idle"),
	GS_Ready	UMETA(DisplayName = "Ready"),
	GS_Start	UMETA(DisplayName = "Start"),
	GS_End	    UMETA(DisplayName = "End"),
	Count		UMETA(Hidden)
};

UENUM(BlueprintType)
enum class ETeamId : uint8
{
	TI_None		UMETA(DisplayName = "None"),
	TI_Left		UMETA(DisplayName = "Left Team"),
	TI_Right	UMETA(DisplayName = "Right Team"),
	Count		UMETA(Hidden)
};