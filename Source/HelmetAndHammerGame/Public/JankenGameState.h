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

	void SetPlayerHand(EHand NewHand);
	void SetPlayerAction(bool bAttack);
	UFUNCTION(BlueprintCallable) void SetRule(URuleBase* NewRule);


	UPROPERTY(BlueprintAssignable) FOnPhaseChanged OnPhaseChanged;
	UPROPERTY(BlueprintReadOnly) FJankenResult RoundResult;

protected:
	virtual void BeginPlay() override;

private:
	void StartCountdown();
	void FinishCountdown();
	void ShowHandResult();
	void ResolveAction();
	int32 CalcJankenResult() const;

	EPhase Phase = EPhase::WaitingInput;
	FTimerHandle CountdownTimerHandle;

	UPROPERTY(EditAnywhere, Instanced)
	URuleBase* CurrentRule = nullptr;
};
