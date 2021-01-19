// Fill out your copyright notice in the Description page of Project Settings.


#include "Helper/Helper.h"

namespace Helper
{
	ARoomEscapeFPSPlayerController* GetPlayerController(APawn* pawn)
	{
		ARoomEscapeFPSPlayerController* pc = nullptr;
		if (pawn)
		{
			pc = Cast<ARoomEscapeFPSPlayerController>(pawn->Controller);
		}
		return pc;
	}
}