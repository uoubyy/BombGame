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
	BT_SpeedUp	UMETA(DisplayName = "Speed Bomb"),
	BT_BoostAll	UMETA(DisplayName = "Boost All Bombs"),
	BT_Child	UMETA(DisplayName = "Child Bomb"),
	Count		UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EBombStatus : uint8
{
	BS_None		UMETA(DisplayName = "None"),
	BS_Idle		UMETA(DisplayName = "Idle"),
	BS_Moving	UMETA(DisplayName = "Moving"),
	BS_Paused	UMETA(DisplayName = "Paused"),
	BS_Exploded	UMETA(DisplayName = "Exploded"),
	Count		UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EConveyorDirection : uint8
{
	CD_None		UMETA(DisplayName = "None"),
	CD_Left		UMETA(DisplayName = "Left"),
	CD_Right	UMETA(DisplayName = "Right"),
	CD_All		UMETA(DisplayName = "All Direction"),
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
	TI_All		UMETA(DisplayName = "All Teams"),
	Count		UMETA(Hidden)
};

UENUM(BlueprintType)
enum class ERandomEventType : uint8
{
	RET_None		UMETA(DisplayName = "None"),
	RET_SwitchLane	UMETA(DisplayName = "Switch Lane"),
	RET_ReverseAll	UMETA(DisplayName = "Reverse All"),
	RET_BoostAll	UMETA(DisplayName = "Boost All"),
	RET_AddBomb     UMETA(DisplayName = "Add Bomb"),
	RET_BlackHole   UMETA(DisplayName = "Black Hole"),
	Count			UMETA(Hidden)
};

USTRUCT(BlueprintType)
struct FRandomEventInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Random Event")
	ERandomEventType EventType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Random Event")
	float ActiveTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Random Event")
	bool HasUsed = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Random Event")
	FString RandomEventName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Random Event")
	FString RandomEventDes;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Random Event")
	TSubclassOf<class ABGBombBase> ChildBombClass;
};