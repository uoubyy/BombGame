#pragma once
#include "CoreMinimal.h"
#include "BGGameplayEnum.generated.h"

UENUM(BlueprintType)
enum class EBombType : uint8
{
	BT_None		UMETA(DisplayName = "None"),
	BT_Regular	UMETA(DisplayName = "Regular Bomb"),
	BT_Timer	UMETA(DisplayName = "Timer Bomb"),
	Count		UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EConveyorDirection : uint8
{
	BT_None		UMETA(DisplayName = "None"),
	CD_Left		UMETA(DisplayName = "Left"),
	CD_Right	UMETA(DisplayName = "Right"),
	Count		UMETA(Hidden)
};