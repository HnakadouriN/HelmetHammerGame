#pragma once
#include "JankenTypes.generated.h"

//じゃんけんの手の種類
UENUM(BlueprintType)
enum class EHand:uint8
{
	Rock,
	Paper,
	Scissors,
	None
};

//じゃんけんのルールの種類
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

//1プレーヤー分のラウンド情報
USTRUCT(BlueprintType)
struct FPlayerRoundInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	EHand Hand = EHand::None;

	UPROPERTY(BlueprintReadOnly)
	bool bAttack = false;
	UPROPERTY(BlueprintReadOnly)
	bool bDefend = false;
	UPROPERTY(BlueprintReadOnly)
	bool bWin = false;

	float AttackTime = 0.0f;
};
