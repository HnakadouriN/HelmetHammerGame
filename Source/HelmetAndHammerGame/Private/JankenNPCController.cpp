// Fill out your copyright notice in the Description page of Project Settings.


#include "JankenNPCController.h"
#include "JankenGameState.h"
#include "TimerManager.h"


void AJankenNPCController::BeginPlay()
{
    Super::BeginPlay();
    PrimaryActorTick.bCanEverTick = true;          // ���t���[��������
}

/* ---------------- ���t���[�� Tick ---------------- */
void AJankenNPCController::Tick(float)
{
    AJankenGameState* GS = GetGS();
    if (!GS || GS->PlayersInfo.Num() < 2) return;

    switch (GS->Phase)
    {
    case EPhase::WaitingInput: HandleWaitingInput(GS);  break;
    case EPhase::ActionSelect: HandleActionSelect(GS);  break;
    case EPhase::Resolve:      HandleResolve(GS);       break;
    default: break;
    }
}

/* ---------- ����o���t�F�[�Y ---------- */
void AJankenNPCController::HandleWaitingInput(AJankenGameState* GS)
{
    if (GS->PlayersInfo[PlayerIndex].PlayerHand != EHand::None) return;
    if (GetWorld()->GetTimerManager().IsTimerActive(HandTimer)) return;

    Delay(HandDelay, [this, GS]()
        {
            GS->SetPlayerHand(PlayerIndex, DecideHand());
            UE_LOG(LogTemp, Log, TEXT("NPC: Hand sent"));
        });
}

/* ---------- �U�h�������t�F�[�Y ---------- */
void AJankenNPCController::HandleActionSelect(AJankenGameState* GS)
{
    const auto& P = GS->PlayersInfo[PlayerIndex];
    if (P.bPlayerAttack || P.bPlayerDefend) return;
    if (GetWorld()->GetTimerManager().IsTimerActive(ActionTimer)) return;

    Delay(ActionDelay, [this, GS]()
        {
            bool bAtk = ShouldAttack();                       // �U�����Ȃ� Attack
            GS->SetPlayerAction(PlayerIndex, bAtk);
            UE_LOG(LogTemp, Log, TEXT("NPC: Action sent after 3s (%s)"),
                bAtk ? TEXT("Attack") : TEXT("Defend"));
        });
}

/* ---------- ���E���h�I�� ---------- */
void AJankenNPCController::HandleResolve(AJankenGameState*)
{
    /* �����E���h�p�Ƀ^�C�}�[���N���A */
    GetWorld()->GetTimerManager().ClearTimer(HandTimer);
    GetWorld()->GetTimerManager().ClearTimer(ActionTimer);
}

/* ---------------- Utility ---------------- */
AJankenGameState* AJankenNPCController::GetGS() const
{
    return GetWorld()->GetGameState<AJankenGameState>();
}

void AJankenNPCController::Delay(float Sec, TFunction<void()> Fn)
{
    FTimerHandle& Th = (Sec == HandDelay) ? HandTimer : ActionTimer;
    GetWorld()->GetTimerManager().SetTimer(Th, MoveTemp(Fn), Sec, false);
}

EHand AJankenNPCController::DecideHand() const
{
    return static_cast<EHand>(FMath::RandRange(0, 2));    // �����_��
}
bool AJankenNPCController::ShouldAttack() const
{
    if (auto* GS = GetGS())
        return GS->AttackerId == PlayerIndex;
    return true;
}