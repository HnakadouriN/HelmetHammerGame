// Fill out your copyright notice in the Description page of Project Settings.


#include "Rule_GuardBreak.h"
#include "JankenGameState.h"
#include "Kismet/KismetMathLibrary.h"

void URule_GuardBreak::PostResolve(AJankenGameState* GS, int32 WinnerId)
{
	if (!GS || WinnerId == -1) return;
}
