#pragma once
#include "JankenTypes.generated.h"

//����񂯂�̎�̎��
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

//����񂯂�̃��[���̎��
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

//1�v���[���[���̃��E���h���
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
