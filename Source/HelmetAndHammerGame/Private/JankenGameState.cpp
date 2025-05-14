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
	if (Players[Id].Hand != EHand::None)  return; // 2 �x�����h�~

	Players[Id].Hand = NewHand;
	TryResolveHands();
}
void AJankenGameState::SetPlayerAction(int32 Id,bool bAttack)
{
	if (Phase != EPhase::ActionSelect || Id > 1) return;

	// �U�������� Attack ���󂯕t�����A�h�䑤���� Defend ���󂯕t���Ȃ�
	const bool bIsAttacker = (Id == AttackerId);
	if (bAttack != bIsAttacker)  return;   // �s�����͈͂���Ԃ�

	FPlayerRoundInfo& P = Players[Id];
	if (P.bAttack || P.bDefend) return;            // 2 �x�����h�~

	P.bAttack = bAttack;                       // true=Attack, false=Defend
	P.bDefend = !bAttack;
	P.AttackTime = GetWorld()->GetTimeSeconds();

	TryResolveActions();
}
void AJankenGameState::EnterActionPhase()
{
	Phase = EPhase::HandResult;
	OnPhaseChanged.Broadcast(Phase);

	const int32 Result = CalcResultRaw();             // -1:P1��, 1:P0��, 0:������
	if (Result == 0)                               // ������ �� �����Z�b�g
	{
		Players[0] = FPlayerRoundInfo{};
		Players[1] = FPlayerRoundInfo{};
		Phase = EPhase::WaitingInput;
		OnPhaseChanged.Broadcast(Phase);
		return;
	}

	AttackerId = (Result == 1) ? 0 : 1;           // ���ҁ��U����
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
	//�U�����̔�������ݍ���======================
	 
	

	//=========================================
	
	if (Players[0].Hand != EHand::None && Players[1].Hand != EHand::None)
	{
		Phase = EPhase::CountingDown;
		CountdownSec = 5;
		OnPhaseChanged.Broadcast(Phase);
		OnCountdownTick.Broadcast();            // ���� 5 �𑗂�

		GetWorldTimerManager().SetTimer(
			CountdownTimerHandle, this, &AJankenGameState::TickCountdown, 1.f, true);
	}

	//if (Players[0].Hand == EHand::None || Players[1].Hand == EHand::None) return;

	//Phase = EPhase::CountingDown;
	//OnPhaseChanged.Broadcast(Phase);

	///* 5 �b��Ɏ�����J���U�h�t�F�[�Y�� */
	//GetWorldTimerManager().SetTimer(
	//	CountdownTimerHandle, this, &AJankenGameState::EnterActionPhase, 5.f, false);
}
void AJankenGameState::TickCountdown()
{
	CountdownSec--;
	OnCountdownTick.Broadcast();      // HUD �Ɂu�c����b�v��ʒm

	if (CountdownSec <= 0)
	{
		GetWorldTimerManager().ClearTimer(CountdownTimerHandle);

		// �����̏����F��D�����J���čU�h�t�F�[�Y��
		EnterActionPhase();
	}
}
void AJankenGameState::TryResolveActions()
{

	const int32 DefenderId = 1 - AttackerId;
	//�U�����̔�������ݍ���======================

	bool AttackerOK = Players[AttackerId].bAttack;
	bool DefenderOK = Players[DefenderId].bDefend;

	for (URuleBase* Rl : ActiveRules)
		Rl->ModifyAttack(AttackerOK, DefenderOK);
	//=========================================

	if (!(Players[0].bAttack || Players[0].bDefend) ||
		!(Players[1].bAttack || Players[1].bDefend))
		return;

	//const bool AttackerOK = Players[AttackerId].bAttack;   // ���҂� Attack
	//const bool DefenderOK = Players[DefenderId].bDefend;   // �s�҂� Defend

	int32 Winner = -1;
	if (AttackerOK && !DefenderOK)           Winner = AttackerId;        // �U������
	else if (!AttackerOK && DefenderOK)      Winner = DefenderId;        // �h�䐬��
	else if (AttackerOK && DefenderOK)       // ����������
		Winner = (Players[AttackerId].AttackTime < Players[DefenderId].AttackTime)
		? AttackerId : DefenderId;
	else                                     Winner = AttackerId;        // ���~�X �� �����

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

	/* 2) RuleIdx �����ƂɐV�������[�����C���X�^���X�� */
	for (int32 Idx : RuleIdx)
	{
		if (AvailableRuleClasses.IsValidIndex(Idx))
		{
			URuleBase* NewRule = NewObject<URuleBase>(this, AvailableRuleClasses[Idx]);
			ActiveRules.Add(NewRule);
		}
	}

	/* 3) ��D��o�^�i�����֐����ė��p�j */
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

		FString Msg = FString::Printf(TEXT("Winner: P%d   ��=%.3f sec"), Winner, Delta);
		UE_LOG(LogTemp, Log, TEXT("%s"), *Msg);
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, Msg);
	}

	/* 2) ��ʍ��� */
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			/*Key*/   1,               /* �㏑�� */
			/*Time*/  4.f,
			/*Color*/ FColor::Yellow,
			FString::Printf(TEXT("Phase: %s"), *UEnum::GetValueAsString(NewPhase)));
	}
}
