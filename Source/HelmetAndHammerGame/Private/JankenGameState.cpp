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

	PlayersInfo.Init(FPlayerRoundInfo{}, 2);

	OnPhaseChanged.Broadcast(Phase);
}
void AJankenGameState::SetPlayerHand(EHand NewHand)
{
	PlayersInfo[0].PlayerHand = NewHand;
}
void AJankenGameState::SetPlayerAction(bool bAttack)
{
	PlayersInfo[0].bPlayerAttack = bAttack;
}
void AJankenGameState::AddRule(URuleBase* Rule)
{
	if (Rule)
	{
		ActiveRules.Add(Rule);
	}
}
void AJankenGameState::ClearRule()
{
	ActiveRules.Empty();
}
void AJankenGameState::TryResolveHands()
{
	// じゃんけんの手を決定
	PlayersInfo[0].PlayerHand = PlayersInfo[0].PlayerHand;
	PlayersInfo[1].OpponentHand = PlayersInfo[1].OpponentHand;
	// ルールを適用
	int32 Result = CalcResultRaw();
	Result = ApplyRulesToResult(Result);
	// 勝敗を決定
	if (Result == 0)
	{
		OnRoundFinished.Broadcast(-1);
	}
	else
	{
		OnRoundFinished.Broadcast(Result);
	}
}
void AJankenGameState::EnterActionPhase()
{
	Phase = EPhase::ActionSelect;
	OnPhaseChanged.Broadcast(Phase);
}
void AJankenGameState::TryResolveActions()
{
	// アクションを決定
	PlayersInfo[0].bPlayerDefend = PlayersInfo[0].bPlayerDefend;
	PlayersInfo[1].bPlayerAttack = PlayersInfo[1].bPlayerAttack;
	// ルールを適用
	int32 Result = CalcResultRaw();
	Result = ApplyRulesToResult(Result);
	// 勝敗を決定
	if (Result == 0)
	{
		OnRoundFinished.Broadcast(-1);
	}
	else
	{
		OnRoundFinished.Broadcast(Result);
	}
}
void AJankenGameState::NextRound()
{
	// 次のラウンドへ
	Phase = EPhase::WaitingInput;
	OnPhaseChanged.Broadcast(Phase);
}
int32 AJankenGameState::CalcResultRaw() const
{
	// じゃんけんの手を比較
	if (PlayersInfo[0].PlayerHand == PlayersInfo[1].OpponentHand)
	{
		return 0; // Draw
	}
	else if ((PlayersInfo[0].PlayerHand == EHand::Rock && PlayersInfo[1].OpponentHand == EHand::Scissors) ||
		(PlayersInfo[0].PlayerHand == EHand::Paper && PlayersInfo[1].OpponentHand == EHand::Rock) ||
		(PlayersInfo[0].PlayerHand == EHand::Scissors && PlayersInfo[1].OpponentHand == EHand::Paper))
	{
		return 1; // Player wins
	}
	else
	{
		return 2; // Opponent wins
	}
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

//void AJankenGameState::HandlePhaseChanged(EPhase NewPhase)
//{
//	/* 1) コンソールログ */
//	UE_LOG(LogTemp, Log, TEXT("[Phase] %s  Hand=%s  Opp=%s  Rev=%d  Win=%d"),
//		*UEnum::GetValueAsString(NewPhase),
//		*UEnum::GetValueAsString(RoundResult.PlayerHand),
//		*UEnum::GetValueAsString(RoundResult.OpponentHand),
//		RoundResult.bReverse,
//		RoundResult.bPlayerWin);
//
//	/* 2) 画面左上に 4 秒表示 */
//	if (GEngine)
//	{
//		FString Msg = FString::Printf(TEXT("Phase:%s  Reverse:%s"),
//			*UEnum::GetValueAsString(NewPhase),
//			RoundResult.bReverse ? TEXT("ON") : TEXT("OFF"));
//
//		GEngine->AddOnScreenDebugMessage(
//			/*Key*/   1,          // 同じキーなら上書き
//			/*Time*/  4.f,
//			/*Color*/ FColor::Yellow,
//			Msg);
//	}
//}
