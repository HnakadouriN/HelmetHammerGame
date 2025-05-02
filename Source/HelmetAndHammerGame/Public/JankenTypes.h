// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "JankenTypes.generated.h"

UENUM(BlueprintType)
enum class EHand:uint8
{
	Rock   UMETA(DisplayName = "Gu-"),
	Paper  UMETA(DisplayName = "Pa-"),
	Scissors UMETA(DisplayName = "Choki-"),
	None   UMETA(DisplayName = "No")
};

UENUM(BlueprintType)
enum class EPhase :uint8
{
	WaitingInput,
	CountingDown,
	HandResult,
	ActionSelect,
	Resolve,
	Finished
};

USTRUCT(BlueprintType)
struct FJankenResult
{
	GENERATED_BODY()

	bool bReverse = false;
	EHand PlayerHand = EHand::None;
	EHand OpponentHand = EHand::None;
	bool bPlayerAttack = false;
	bool bPlayerDefend = false;
	bool bPlayerWin = false;
};
