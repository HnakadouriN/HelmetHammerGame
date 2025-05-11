// Fill out your copyright notice in the Description page of Project Settings.


#include "JankenNPCController.h"
#include "JankenGameState.h"


void AJankenNPCController::BeginPlay()
{
    Super::BeginPlay();
    PrimaryActorTick.bCanEverTick = true;          // Tick ‚ðŽg‚¤
}

void AJankenNPCController::Tick(float)
{
    AJankenGameState* GS = GetWorld()->GetGameState<AJankenGameState>();
    if (!GS || GS->PlayersInfo.Num() < 2) return;

    switch (GS->Phase)
    {
    case EPhase::WaitingInput:
        if (GS->PlayersInfo[PlayerIndex].PlayerHand == EHand::None)
            GS->SetPlayerHand(PlayerIndex, DecideHand());
        break;

    case EPhase::ActionSelect:
        if (!(GS->PlayersInfo[PlayerIndex].bPlayerAttack || GS->PlayersInfo[PlayerIndex].bPlayerDefend))
            GS->SetPlayerAction(PlayerIndex, ShouldAttack());
        break;

    default: break;
    }
}

EHand AJankenNPCController::DecideHand() const
{
    return static_cast<EHand>(FMath::RandRange(0, 2));
}
bool AJankenNPCController::ShouldAttack() const
{
    if (auto* GS = GetWorld()->GetGameState<AJankenGameState>())
        return GS->AttackerId == PlayerIndex;
    return true;
}