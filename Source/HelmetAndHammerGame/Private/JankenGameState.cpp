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

	PlayersInfo.Init(FPlayerRoundInfo{}, 2);

	OnPhaseChanged.AddDynamic(this, &AJankenGameState::HandlePhaseChanged);

	OnPhaseChanged.Broadcast(Phase);
}
void AJankenGameState::SetPlayerHand(int32 Id,EHand NewHand)
{
	if (Phase != EPhase::WaitingInput || Id > 1) return;
	PlayersInfo[Id].PlayerHand = NewHand; TryResolveHands();
}
void AJankenGameState::SetPlayerAction(int32 Id,bool bAttack)
{
	if (Phase != EPhase::ActionSelect) return;
	PlayersInfo[Id].bPlayerAttack = bAttack; PlayersInfo[Id].bPlayerDefend = !bAttack; TryResolveActions();
}
void AJankenGameState::EnterActionPhase()
{
	Phase = EPhase::HandResult; OnPhaseChanged.Broadcast(Phase);

	/* ===== ここで両者の手を表示 ===== */
	auto HandToString = [](EHand H)
		{
			switch (H)
			{
			case EHand::Rock:     return TEXT("Rock");
			case EHand::Scissors: return TEXT("Scissors");
			case EHand::Paper:    return TEXT("Paper");
			default:              return TEXT("None");
			}
		};

	const FString Msg = FString::Printf(TEXT("You: %s   NPC: %s"),
		HandToString(PlayersInfo[0].PlayerHand),
		HandToString(PlayersInfo[1].PlayerHand));

	UE_LOG(LogTemp, Log, TEXT("%s"), *Msg);                 // Output Log
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(
			42 /*Key*/, 3.f, FColor::Cyan, Msg);            // 画面左上 3 秒表示
	/* ================================= */

	int32 R = CalcResultRaw();   // -1:p1勝 0:あいこ 1:p0勝
	if (R == 0) { Phase = EPhase::WaitingInput; PlayersInfo[0].PlayerHand = PlayersInfo[1].PlayerHand = EHand::None; OnPhaseChanged.Broadcast(Phase); return; }
	AttackerId = (R == 1) ? 0 : 1;
	Phase = EPhase::ActionSelect; OnPhaseChanged.Broadcast(Phase);
}
void AJankenGameState::NextRound()
{
	// 次のラウンドへ移行
	//プレイヤーとルールの情報を初期化
	PlayersInfo = { FPlayerRoundInfo{}, FPlayerRoundInfo{} };
	AttackerId = -1;
	Phase = EPhase::WaitingInput;
	OnPhaseChanged.Broadcast(Phase);
	
}
int32 AJankenGameState::CalcResultRaw() const
{
	static const int32 T[3][3] = { {0,1,-1},{-1,0,1},{1,-1,0} };
	int32 r = T[(int32)PlayersInfo[0].PlayerHand][(int32)PlayersInfo[1].PlayerHand];
	for (auto* Rule : ActiveRules) r = Rule->ModifyJankenResult(r, 0);
	return r;
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
	if (PlayersInfo[0].PlayerHand == EHand::None || PlayersInfo[1].PlayerHand == EHand::None)
	{
		return;
	}
	Phase = EPhase::CountingDown;
	OnPhaseChanged.Broadcast(Phase);

	GetWorldTimerManager().SetTimer(
		CountdownTimerHandle,
		this,
		&AJankenGameState::EnterActionPhase,
		5.0f, // 5秒後に
		false // 1回だけ
	);
}
void AJankenGameState::TryResolveActions()
{
	if (!(PlayersInfo[0].bPlayerAttack||PlayersInfo[1].bPlayerAttack) ||
	    !(PlayersInfo[1].bPlayerAttack||PlayersInfo[1].bPlayerDefend))
	{
		return;
	}

	bool AttHit = PlayersInfo[AttackerId].bPlayerAttack && !PlayersInfo[1 - AttackerId].bPlayerDefend;
	int32 Win = AttHit ? AttackerId : 1 - AttackerId;

	PlayersInfo[Win].bPlayerWin = true;
	Phase = EPhase::Resolve;
	OnPhaseChanged.Broadcast(Phase);

	GetWorldTimerManager().SetTimer(CountdownTimerHandle, this, &AJankenGameState::NextRound, 2.f, false);
}


void URule_GuardBreak::PostResolve(AJankenGameState* GS, int32 WinnerId)
{
	if (!GS) return;
	const bool bDefenderWon = (WinnerId != -1 && WinnerId != GS->AttackerId);
	if (bDefenderWon && FMath::FRand() < 0.5f)      // 50% で破壊
		WinnerId = GS->AttackerId;
}

void AJankenGameState::HandlePhaseChanged(EPhase NewPhase)
{
	UE_LOG(LogTemp, Log, TEXT("[Phase] %s  P0=%s  P1=%s  Attacker=%d"),
		*UEnum::GetValueAsString(NewPhase),
		*UEnum::GetValueAsString(PlayersInfo[0].PlayerHand),
		*UEnum::GetValueAsString(PlayersInfo[1].PlayerHand),
		AttackerId);

	/* 2) 画面左上 */
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			/*Key*/   1,               /* 上書き */
			/*Time*/  4.f,
			/*Color*/ FColor::Yellow,
			FString::Printf(TEXT("Phase: %s"), *UEnum::GetValueAsString(NewPhase)));
	}
}
