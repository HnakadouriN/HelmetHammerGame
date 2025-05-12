// Fill out your copyright notice in the Description page of Project Settings.


#include "JankenGameState.h"
#include "Rule_GuardBreak.h"
#include "TimerManager.h"
#include "Engine/Engine.h"

AJankenGameState::AJankenGameState()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AJankenGameState::BeginPlay()
{
	Super::BeginPlay();

	Players.Init(FPlayerRoundInfo{}, 2);

	OnPhaseChanged.AddDynamic(this, &AJankenGameState::HandlePhaseChanged);

	OnPhaseChanged.Broadcast(Phase);
}
void AJankenGameState::SetPlayerHand(int32 Id,EHand NewHand)
{
	if (Phase != EPhase::WaitingInput || Id > 1) return;
	if (Players[Id].Hand != EHand::None)  return; // 2 “x‰Ÿ‚µ–h~

	Players[Id].Hand = NewHand;
	TryResolveHands();
}
void AJankenGameState::SetPlayerAction(int32 Id,bool bAttack)
{
	if (Phase != EPhase::ActionSelect || Id > 1) return;

	FPlayerRoundInfo& P = Players[Id];
	if (P.bAttack || P.bDefend) return;            // 2 “x‰Ÿ‚µ–h~

	P.bAttack = bAttack;                       // true=Attack, false=Defend
	P.bDefend = !bAttack;
	P.AttackTime = GetWorld()->GetTimeSeconds();

	TryResolveActions();
}
void AJankenGameState::EnterActionPhase()
{
	Phase = EPhase::HandResult;
	OnPhaseChanged.Broadcast(Phase);

	const int32 Result = CalcResultRaw();             // -1:P1Ÿ, 1:P0Ÿ, 0:‚ ‚¢‚±
	if (Result == 0)                               // ‚ ‚¢‚± ¨ ‘¦ƒŠƒZƒbƒg
	{
		Players[0] = FPlayerRoundInfo{};
		Players[1] = FPlayerRoundInfo{};
		Phase = EPhase::WaitingInput;
		OnPhaseChanged.Broadcast(Phase);
		return;
	}

	AttackerId = (Result == 1) ? 0 : 1;           // ŸÒUŒ‚‘¤
	Phase = EPhase::ActionSelect;
	OnPhaseChanged.Broadcast(Phase);
}
void AJankenGameState::NextRound()
{
	Players[0] = FPlayerRoundInfo{};
	Players[1] = FPlayerRoundInfo{};
	AttackerId = -1;

	Phase = EPhase::WaitingInput;
	OnPhaseChanged.Broadcast(Phase);
}
int32 AJankenGameState::CalcResultRaw() const
{
	static const int32 Table[3][3] = { /*P0 vs P1*/  {0, 1, -1},
										{-1, 0, 1},
										{1, -1, 0} };
	return Table[(int32)Players[0].Hand][(int32)Players[1].Hand];
}
int32 AJankenGameState::ApplyRulesToResult(int32 BaseResult) const
{
	int32 Result = BaseResult;
	for (URuleBase* Rule : ActiveRules)
	{
		Result = Rule->ModifyJankenResult(Result, 0);
	}
	return Result;
}
void AJankenGameState::TryResolveHands()
{
	if (Players[0].Hand == EHand::None || Players[1].Hand == EHand::None) return;

	Phase = EPhase::CountingDown;
	OnPhaseChanged.Broadcast(Phase);

	/* 5 •bŒã‚Éè‚ğŒöŠJ‚µU–hƒtƒF[ƒY‚Ö */
	GetWorldTimerManager().SetTimer(
		CountdownTimerHandle, this, &AJankenGameState::EnterActionPhase, 5.f, false);
}
void AJankenGameState::TryResolveActions()
{
	if (!(Players[0].bAttack || Players[0].bDefend) ||
		!(Players[1].bAttack || Players[1].bDefend))
		return;

	const int32 DefenderId = 1 - AttackerId;
	const bool AttackerOK = Players[AttackerId].bAttack;   // ŸÒ‚Í Attack
	const bool DefenderOK = Players[DefenderId].bDefend;   // ”sÒ‚Í Defend

	int32 Winner = -1;
	if (AttackerOK && !DefenderOK)           Winner = AttackerId;        // UŒ‚¬Œ÷
	else if (!AttackerOK && DefenderOK)      Winner = DefenderId;        // –hŒä¬Œ÷
	else if (AttackerOK && DefenderOK)       // ‘‰Ÿ‚µŸ•‰
		Winner = (Players[AttackerId].AttackTime < Players[DefenderId].AttackTime)
		? AttackerId : DefenderId;
	else                                     Winner = DefenderId;        // —¼ƒ~ƒX ¨ ç”õ‘¤

	Players[Winner].bWin = true;
	Phase = EPhase::Resolve;
	OnPhaseChanged.Broadcast(Phase);

	GetWorldTimerManager().SetTimer(
		CountdownTimerHandle, this, &AJankenGameState::NextRound, 2.f, false);
}


void URule_GuardBreak::PostResolve(AJankenGameState* GS, int32 WinnerId)
{
	if (!GS) return;
	const bool bDefenderWon = (WinnerId != -1 && WinnerId != GS->AttackerId);
	if (bDefenderWon && FMath::FRand() < 0.5f)      // 50% ‚Å”j‰ó
		WinnerId = GS->AttackerId;
}

void AJankenGameState::HandlePhaseChanged(EPhase NewPhase)
{
	UE_LOG(LogTemp, Log, TEXT("[Phase] %s  P0=%s  P1=%s  Attacker=%d"),
		*UEnum::GetValueAsString(NewPhase),
		*UEnum::GetValueAsString(Players[0].Hand),
		*UEnum::GetValueAsString(Players[1].Hand),
		AttackerId);

	if (NewPhase == EPhase::Resolve)
	{
		int32 Winner = Players[0].bWin ? 0 : 1;
		float Delta = FMath::Abs(Players[0].AttackTime - Players[1].AttackTime);

		FString Msg = FString::Printf(TEXT("Winner: P%d   ƒ¢=%.3f sec"), Winner, Delta);
		UE_LOG(LogTemp, Log, TEXT("%s"), *Msg);
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, Msg);
	}

	/* 2) ‰æ–Ê¶ã */
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			/*Key*/   1,               /* ã‘‚« */
			/*Time*/  4.f,
			/*Color*/ FColor::Yellow,
			FString::Printf(TEXT("Phase: %s"), *UEnum::GetValueAsString(NewPhase)));
	}
}
