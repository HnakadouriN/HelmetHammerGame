#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "JankenTypes.h"
#include "RuleBase.h"
#include "JankenGameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPhaseChanged, EPhase, NewPhase);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCountdownTick);

UCLASS()
class HELMETANDHAMMERGAME_API AJankenGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	AJankenGameState();

	//���͎���̊֐�
	void SetPlayerHand(int32 Id,EHand NewHand);
	void SetPlayerAction(int32 Id,bool bAttack);

	//���[������̊֐�
	void AddRule(URuleBase* Rule)
	{
		ActiveRules.Add(Rule);
	}
	void ClearRule()
	{
		ActiveRules.Empty();
	}

	//���[���̐ݒ�
	UPROPERTY(BlueprintReadOnly) TArray<FPlayerRoundInfo> Players;
	UPROPERTY(BlueprintReadOnly) EPhase Phase = EPhase::WaitingInput;
	UPROPERTY(BlueprintReadOnly) int32 AttackerId = -1;

	UPROPERTY(BlueprintReadOnly)
	int32 CountdownSec = 0;

	//�C�x���g
	UPROPERTY(BlueprintAssignable) FOnPhaseChanged OnPhaseChanged;
	UPROPERTY(BlueprintAssignable) FOnCountdownTick OnCountdownTick;


protected:
	virtual void BeginPlay() override;

private:
	void TryResolveHands();
	void EnterActionPhase();
	void TryResolveActions();
	void NextRound();
	int32 CalcResultRaw() const;
	int32 ApplyRulesToResult(int32) const;
	void TickCountdown();

	TArray<URuleBase*> ActiveRules;
	FTimerHandle CountdownTimerHandle;

	/*�f�o�b�O�p*/
	UFUNCTION()
	void HandlePhaseChanged(EPhase NewPhase);
};
