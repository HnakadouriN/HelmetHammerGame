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
	EHand PlayerHand = EHand::None;

	UPROPERTY(BlueprintReadOnly)
	bool bPlayerAttack = false;
	UPROPERTY(BlueprintReadOnly)
	bool bPlayerDefend = false;
	UPROPERTY(BlueprintReadOnly)
	bool bPlayerWin = false;

	float AttackTime = 0.0f;
};
