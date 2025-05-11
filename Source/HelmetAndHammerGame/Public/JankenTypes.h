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
	EHand PlayerHand = EHand::None;

	UPROPERTY(BlueprintReadOnly)
	bool bPlayerAttack = false;
	UPROPERTY(BlueprintReadOnly)
	bool bPlayerDefend = false;
	UPROPERTY(BlueprintReadOnly)
	bool bPlayerWin = false;

	float AttackTime = 0.0f;
};
