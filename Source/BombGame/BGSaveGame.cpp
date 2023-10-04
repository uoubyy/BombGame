// Fill out your copyright notice in the Description page of Project Settings.


#include "BGSaveGame.h"

UBGSaveGame::UBGSaveGame()
{
	for (int32 Index = 0; Index < MaxRecordCnt; ++Index)
	{
		Top10Records.Add({ Index, FRecordDetail()});
	}
}

int32 UBGSaveGame::TryAddRecordToTop10(const FRecordDetail& RecordDetail)
{
	int32 Index = 0;
	for (; Index < MaxRecordCnt; ++Index)
	{
		if (RecordDetail.TeamScore >= Top10Records[Index].TeamScore)
		{
			break;
		}
	}

	if (Index < MaxRecordCnt)
	{
		for (int32 i = MaxRecordCnt - 1; i >= Index + 1; --i)
		{
			Top10Records[i] = Top10Records[i - 1];
		}

		Top10Records[Index] = RecordDetail;
	}

	return Index;// < MaxRecordCnt;
}