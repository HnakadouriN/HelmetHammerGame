// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "JankenTypes.h"
#include "RuleBase.h"
#include "JankenGameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPhaseChanged, EPhase, NewPhase);

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
	UPROPERTY(BlueprintReadOnly) TArray<FPlayerRoundInfo> PlayersInfo;
	UPROPERTY(BlueprintReadOnly) EPhase Phase = EPhase::WaitingInput;
	UPROPERTY(BlueprintReadOnly) int32 AttackerId = -1;

	//イベント
	UPROPERTY(BlueprintAssignable) FOnPhaseChanged OnPhaseChanged;


protected:
	virtual void BeginPlay() override;

private:
	void TryResolveHands();
	void EnterActionPhase();
	void TryResolveActions();
	void NextRound();
	int32 CalcResultRaw() const;
	int32 ApplyRulesToResult(int32) const;

	TArray<URuleBase*> ActiveRules;
	FTimerHandle CountdownTimerHandle;

	/*デバッグ用*/
	UFUNCTION()
	void HandlePhaseChanged(EPhase NewPhase);
};
