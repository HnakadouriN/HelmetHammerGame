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

	//入力周りの関数
	void SetPlayerHand(int32 Id,EHand NewHand);
	void SetPlayerAction(int32 Id,bool bAttack);

	//ルール周りの関数
	void AddRule(URuleBase* Rule)
	{
		ActiveRules.Add(Rule);
	}
	void ClearRule()
	{
		ActiveRules.Empty();
	}

	//ルールの設定
	UPROPERTY(BlueprintReadOnly) TArray<FPlayerRoundInfo> Players;
	UPROPERTY(BlueprintReadOnly) EPhase Phase = EPhase::WaitingInput;
	UPROPERTY(BlueprintReadOnly) int32 AttackerId = -1;

	UPROPERTY(BlueprintReadOnly)
	int32 CountdownSec = 0;

	//イベント
	UPROPERTY(BlueprintAssignable) FOnPhaseChanged OnPhaseChanged;
	UPROPERTY(BlueprintAssignable) FOnCountdownTick OnCountdownTick;

    UPROPERTY(BlueprintReadOnly)
	TArray<TSubclassOf<URuleBase>> AvailableRuleClasses;

	UFUNCTION(BlueprintCallable)
	void SetRulesAndStart(const TArray<int32>& RuleIdx);


	void ApplySelectedRules(const TArray<int32>& Indices);
	void ApplyRulesAndHand(int32 PlayerId, EHand Hand, const TArray<int32>& RuleIdx);

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


	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<URuleBase*> ActiveRules;
	FTimerHandle CountdownTimerHandle;

	
	/*デバッグ用*/
	UFUNCTION()
	void HandlePhaseChanged(EPhase NewPhase);
};
