#include "VersusNPCMode.h"
#include "Kismet/GameplayStatics.h"
#include "JankenNPCController.h"
#include "EngineUtils.h"
#include "GameFramework/PlayerStart.h"
#include "GameFramework/PlayerState.h"

void AVersusNPCMode::BeginPlay()
{
    Super::BeginPlay();

    // すでに 2 人いたら NPC 戦にしない
    if (GetGameInstance()->GetNumLocalPlayers() >= 2) return;

    // 1 行で NPCController をスポーン（位置は原点で十分）
    GetWorld()->SpawnActor<AJankenNPCController>(
        AJankenNPCController::StaticClass(),
        FVector::ZeroVector, FRotator::ZeroRotator);
}