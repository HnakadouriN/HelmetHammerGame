#include "VersusNPCMode.h"
#include "Kismet/GameplayStatics.h"
#include "JankenNPCController.h"
#include "EngineUtils.h"
#include "GameFramework/PlayerStart.h"
#include "GameFramework/PlayerState.h"

void AVersusNPCMode::BeginPlay()
{
    Super::BeginPlay();

    // ���ł� 2 �l������ NPC ��ɂ��Ȃ�
    if (GetGameInstance()->GetNumLocalPlayers() >= 2) return;

    // 1 �s�� NPCController ���X�|�[���i�ʒu�͌��_�ŏ\���j
    GetWorld()->SpawnActor<AJankenNPCController>(
        AJankenNPCController::StaticClass(),
        FVector::ZeroVector, FRotator::ZeroRotator);
}