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