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

UENUM(BlueprintType)
enum class EAction :uint8
{
	None,
	Attack,
	Defend
};

//じゃんけんのルールの種類
UENUM(BlueprintType)
enum class EPhase :uint8
{
	RuleSelect UMETA(DisplayName = "ルール選択中"),
	WaitingInput UMETA(DisplayName = "出す手を選択してください"),
	CountingDown UMETA(DisplayName = "カウントダウン"),
	HandResult UMETA(DisplayName = "結果表示"),
	ActionSelect UMETA(DisplayName = "攻防選択中"),
	Resolve UMETA(DisplayName = "リザルト"),
	Finished UMETA(DisplayName = "")
};

//1プレーヤー分のラウンド情報
USTRUCT(BlueprintType)
struct FPlayerRoundInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	EHand Hand = EHand::None;
	EAction Action = EAction::None;

	UPROPERTY(BlueprintReadOnly)
	bool bAttack = false;
	UPROPERTY(BlueprintReadOnly)
	bool bDefend = false;
	UPROPERTY(BlueprintReadOnly)
	bool bWin = false;

	float AttackTime = 0.0f;
};
