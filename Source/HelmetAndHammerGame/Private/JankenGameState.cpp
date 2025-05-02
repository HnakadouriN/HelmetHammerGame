// Fill out your copyright notice in the Description page of Project Settings.


#include "JankenGameState.h"
#include "TimerManager.h"
#include "Engine/Engine.h"

AJankenGameState::AJankenGameState()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AJankenGameState::BeginPlay()
{
	Super::BeginPlay();

	OnPhaseChanged.AddDynamic(this, &AJankenGameState::HandlePhaseChanged);

	OnPhaseChanged.Broadcast(Phase);
}

void AJankenGameState::SetPlayerHand(EHand NewHand)
{
	if (Phase != EPhase::WaitingInput) return;

	RoundResult.PlayerHand = NewHand;
	RoundResult.OpponentHand = static_cast<EHand>(FMath::RandRange(0, 2));

	Phase = EPhase::CountingDown;
	OnPhaseChanged.Broadcast(Phase);
	StartCountdown();
}

void AJankenGameState::StartCountdown()
{
	GetWorld()->GetTimerManager().SetTimer(CountdownTimerHandle, this, &AJankenGameState::FinishCountdown, 3.0f, false);
}

void AJankenGameState::FinishCountdown()
{
	ShowHandResult();
}

void AJankenGameState::SetRule(URuleBase* NewRule)
{
	if (Phase == EPhase::WaitingInput)
	{
		CurrentRule = NewRule;
		if (CurrentRule) CurrentRule->PreHand(this);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot set rule while in phase: %s"), *UEnum::GetValueAsString(Phase));
	}
}

int32 AJankenGameState::CalcJankenResult() const
{
	static const int32 Table[3][3] = { {0,1,-1},{-1,0,1},{1,-1,0} };
	int32 Base = Table[(int32)RoundResult.PlayerHand][(int32)RoundResult.OpponentHand];
	return Base;
};

void AJankenGameState::ShowHandResult()
{
	Phase = EPhase::HandResult;
	OnPhaseChanged.Broadcast(Phase);

	int32 Result = CalcJankenResult();
	if (Result == 0)
	{
		Phase = EPhase::WaitingInput;
		OnPhaseChanged.Broadcast(Phase);
		return;
	}

	Phase = EPhase::ActionSelect;
	OnPhaseChanged.Broadcast(Phase);

	if (CurrentRule) CurrentRule->OnEnterAction(this);
}

void AJankenGameState::SetPlayerAction(bool bAttack)
{
	if (Phase != EPhase::ActionSelect) return;
	RoundResult.bPlayerAttack = bAttack;
	RoundResult.bPlayerDefend = !bAttack;
	ResolveAction();
}

void AJankenGameState::ResolveAction()
{
	const bool bPlayerJankenWin = CalcJankenResult() == 1;

	if (bPlayerJankenWin)
	{
		RoundResult.bPlayerWin = RoundResult.bPlayerAttack;
	}
	else
	{
		RoundResult.bPlayerWin = !RoundResult.bPlayerAttack;
	}
	if (CurrentRule) CurrentRule->PostResolve(this);

	Phase = EPhase::Resolve;
	OnPhaseChanged.Broadcast(Phase);

	GetWorldTimerManager().SetTimer(CountdownTimerHandle, [this]() {
		RoundResult = FJankenResult{};
		CurrentRule = nullptr;
		Phase = EPhase::WaitingInput;
		OnPhaseChanged.Broadcast(Phase);
		}, 3.0f, false);
}
void AJankenGameState::HandlePhaseChanged(EPhase NewPhase)
{
	/* 1) コンソールログ */
	UE_LOG(LogTemp, Log, TEXT("[Phase] %s  Hand=%s  Opp=%s  Rev=%d  Win=%d"),
		*UEnum::GetValueAsString(NewPhase),
		*UEnum::GetValueAsString(RoundResult.PlayerHand),
		*UEnum::GetValueAsString(RoundResult.OpponentHand),
		RoundResult.bReverse,
		RoundResult.bPlayerWin);

	/* 2) 画面左上に 4 秒表示 */
	if (GEngine)
	{
		FString Msg = FString::Printf(TEXT("Phase:%s  Reverse:%s"),
			*UEnum::GetValueAsString(NewPhase),
			RoundResult.bReverse ? TEXT("ON") : TEXT("OFF"));

		GEngine->AddOnScreenDebugMessage(
			/*Key*/   1,          // 同じキーなら上書き
			/*Time*/  4.f,
			/*Color*/ FColor::Yellow,
			Msg);
	}
}
