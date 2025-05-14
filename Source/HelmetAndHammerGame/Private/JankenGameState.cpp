#include "JankenGameState.h"
#include "Rule_GuardBreak.h"
#include "Rule_Reverse.h"
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

	AvailableRuleClasses = {
		URule_GuardBreak::StaticClass(),
		URule_Reverse::StaticClass()
	};

	OnPhaseChanged.AddDynamic(this, &AJankenGameState::HandlePhaseChanged);

	OnPhaseChanged.Broadcast(Phase);
}
void AJankenGameState::SetPlayerHand(int32 Id,EHand NewHand)
{
	if (Phase != EPhase::WaitingInput || Id > 1) return;
	if (Players[Id].Hand != EHand::None)  return; // 2 度押し防止

	Players[Id].Hand = NewHand;
	TryResolveHands();
}
void AJankenGameState::SetPlayerAction(int32 Id,bool bAttack)
{
	if (Phase != EPhase::ActionSelect || Id > 1) return;

	// 攻撃側しか Attack を受け付けず、防御側しか Defend を受け付けない
	const bool bIsAttacker = (Id == AttackerId);
	if (bAttack != bIsAttacker)  return;   // 不正入力は握りつぶす

	FPlayerRoundInfo& P = Players[Id];
	if (P.bAttack || P.bDefend) return;            // 2 度押し防止

	P.bAttack = bAttack;                       // true=Attack, false=Defend
	P.bDefend = !bAttack;
	P.AttackTime = GetWorld()->GetTimeSeconds();

	TryResolveActions();
}
void AJankenGameState::EnterActionPhase()
{
	Phase = EPhase::HandResult;
	OnPhaseChanged.Broadcast(Phase);

	const int32 Result = CalcResultRaw();             // -1:P1勝, 1:P0勝, 0:あいこ
	if (Result == 0)                               // あいこ → 即リセット
	{
		Players[0] = FPlayerRoundInfo{};
		Players[1] = FPlayerRoundInfo{};
		Phase = EPhase::WaitingInput;
		OnPhaseChanged.Broadcast(Phase);
		return;
	}

	AttackerId = (Result == 1) ? 0 : 1;           // 勝者＝攻撃側
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
	//攻撃時の判定を挟み込む======================
	 
	

	//=========================================
	
	if (Players[0].Hand != EHand::None && Players[1].Hand != EHand::None)
	{
		Phase = EPhase::CountingDown;
		CountdownSec = 5;
		OnPhaseChanged.Broadcast(Phase);
		OnCountdownTick.Broadcast();            // 初回 5 を送る

		GetWorldTimerManager().SetTimer(
			CountdownTimerHandle, this, &AJankenGameState::TickCountdown, 1.f, true);
	}

	//if (Players[0].Hand == EHand::None || Players[1].Hand == EHand::None) return;

	//Phase = EPhase::CountingDown;
	//OnPhaseChanged.Broadcast(Phase);

	///* 5 秒後に手を公開し攻防フェーズへ */
	//GetWorldTimerManager().SetTimer(
	//	CountdownTimerHandle, this, &AJankenGameState::EnterActionPhase, 5.f, false);
}
void AJankenGameState::TickCountdown()
{
	CountdownSec--;
	OnCountdownTick.Broadcast();      // HUD に「残り◯秒」を通知

	if (CountdownSec <= 0)
	{
		GetWorldTimerManager().ClearTimer(CountdownTimerHandle);

		// 既存の処理：手札を公開して攻防フェーズへ
		EnterActionPhase();
	}
}
void AJankenGameState::TryResolveActions()
{

	const int32 DefenderId = 1 - AttackerId;
	//攻撃時の判定を挟み込む======================

	bool AttackerOK = Players[AttackerId].bAttack;
	bool DefenderOK = Players[DefenderId].bDefend;

	for (URuleBase* Rl : ActiveRules)
		Rl->ModifyAttack(AttackerOK, DefenderOK);
	//=========================================

	if (!(Players[0].bAttack || Players[0].bDefend) ||
		!(Players[1].bAttack || Players[1].bDefend))
		return;

	//const bool AttackerOK = Players[AttackerId].bAttack;   // 勝者は Attack
	//const bool DefenderOK = Players[DefenderId].bDefend;   // 敗者は Defend

	int32 Winner = -1;
	if (AttackerOK && !DefenderOK)           Winner = AttackerId;        // 攻撃成功
	else if (!AttackerOK && DefenderOK)      Winner = DefenderId;        // 防御成功
	else if (AttackerOK && DefenderOK)       // 早押し勝負
		Winner = (Players[AttackerId].AttackTime < Players[DefenderId].AttackTime)
		? AttackerId : DefenderId;
	else                                     Winner = AttackerId;        // 両ミス → 守備側

	Players[Winner].bWin = true;
	Phase = EPhase::Resolve;
	OnPhaseChanged.Broadcast(Phase);

	GetWorldTimerManager().SetTimer(
		CountdownTimerHandle, this, &AJankenGameState::NextRound, 2.f, false);
}
void AJankenGameState::ApplySelectedRules(const TArray<int32>& Indices)
{
	for (URuleBase* R : ActiveRules) if (R) R->ConditionalBeginDestroy();
	ActiveRules.Empty();

	for (int32 Idx : Indices)
		if (AvailableRuleClasses.IsValidIndex(Idx))
			ActiveRules.Add(NewObject<URuleBase>(this, AvailableRuleClasses[Idx]));
}
void AJankenGameState::ApplyRulesAndHand(int32 PlayerId, EHand Hand, const TArray<int32>& RuleIdx)
{
	for (URuleBase* R : ActiveRules)
		if (R) R->ConditionalBeginDestroy();
	ActiveRules.Empty();

	/* 2) RuleIdx をもとに新しいルールをインスタンス化 */
	for (int32 Idx : RuleIdx)
	{
		if (AvailableRuleClasses.IsValidIndex(Idx))
		{
			URuleBase* NewRule = NewObject<URuleBase>(this, AvailableRuleClasses[Idx]);
			ActiveRules.Add(NewRule);
		}
	}

	/* 3) 手札を登録（既存関数を再利用） */
	SetPlayerHand(PlayerId, Hand);
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

		FString Msg = FString::Printf(TEXT("Winner: P%d   Δ=%.3f sec"), Winner, Delta);
		UE_LOG(LogTemp, Log, TEXT("%s"), *Msg);
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, Msg);
	}

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
