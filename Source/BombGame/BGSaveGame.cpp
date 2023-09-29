// Fill out your copyright notice in the Description page of Project Settings.


#include "BGSaveGame.h"

UBGSaveGame::UBGSaveGame()
{
	for (int32 Index = 0; Index < 10; ++Index)
	{
		Top10Records.Add({ Index, FRecordDetail()});
	}
}