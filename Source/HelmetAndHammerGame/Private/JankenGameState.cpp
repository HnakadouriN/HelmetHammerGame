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
	if (Phase != EPhase::WaitingInput || !PlayersInfo.IsValidIndex(Id))
	{
		return;
	}
	PlayersInfo[Id].PlayerHand = NewHand;
	TryResolveHands();
}
void AJankenGameState::SetPlayerAction(int32 Id,bool bAttack)
{
	if (Phase != EPhase::ActionSelect || !PlayersInfo.IsValidIndex(Id))
	{
		return;
	}
	PlayersInfo[Id].bPlayerAttack = bAttack;
	PlayersInfo[Id].bPlayerDefend = !bAttack;
	TryResolveActions();
}
void AJankenGameState::EnterActionPhase()
{
	Phase = EPhase::ActionSelect;
	OnPhaseChanged.Broadcast(Phase);

	const int32 ResultRaw = CalcResultRaw();
	int32 Result = ApplyRulesToResult(ResultRaw);

	if (Result == 0)
	{
		// Draw
		// じゃんけんの結果が引き分けの場合、次のラウンドへ移行
		Phase = EPhase::WaitingInput;
		PlayersInfo[0].PlayerHand = PlayersInfo[1].PlayerHand = EHand::None;
		OnPhaseChanged.Broadcast(Phase);
		return;
	}
	AttackerId = (Result == 1) ? 0 : 1; // Player wins if Result is 1, otherwise Opponent wins
	Phase = EPhase::ActionSelect;
	OnPhaseChanged.Broadcast(Phase);
	
	for (URuleBase* Rule : ActiveRules)
	{
		Rule->OnEnterAction(this);
	}
}
void AJankenGameState::NextRound()
{
	// 次のラウンドへ移行
	//プレイヤーとルールの情報を初期化
	PlayersInfo = { FPlayerRoundInfo{}, FPlayerRoundInfo{} };
	ActiveRules.Empty();
	AttackerId = -1;
	Phase = EPhase::WaitingInput;
	OnPhaseChanged.Broadcast(Phase);
	
}
int32 AJankenGameState::CalcResultRaw() const
{
	static const int32 T[3][3] = { {0,1,-1},{-1,0,1},{1,-1,0} };
	int32 r = T[(int32)PlayersInfo[0].PlayerHand][(int32)PlayersInfo[1].PlayerHand];
	if (PlayersInfo[0].bReverse) r *= -1;
	if (PlayersInfo[1].bReverse) r *= -1;
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
	int32 Winner = -1;

	auto AttackSuccess = [&](int32 AtkId, int32 DefId)->bool
		{
			if (!PlayersInfo[AtkId].bPlayerAttack)
			{
				return false;
			}
			if (!PlayersInfo[DefId].bPlayerDefend)
			{
				return true;
			}
			return false;
		};
	if (AttackSuccess(AttackerId, 1 - AttackerId))
	{
		Winner = AttackerId;
	}
	else
	{
		Winner = 1 - AttackerId;
	}

	for (URuleBase* Rule : ActiveRules)
	{
		Rule->PostResolve(this, Winner);
	}
	PlayersInfo[0].bPlayerWin = (Winner == 0);  PlayersInfo[1].bPlayerWin = (Winner == 1);

	Phase = EPhase::Resolve;
	OnRoundFinished.Broadcast(Winner);
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
