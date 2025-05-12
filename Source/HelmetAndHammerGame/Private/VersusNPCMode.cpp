// Fill out your copyright notice in the Description page of Project Settings.


#include "VersusNPCMode.h"
#include "Kismet/GameplayStatics.h"
#include "JankenNPCController.h"

void AVersusNPCMode::BeginPlay()
{
    Super::BeginPlay();

    /* ‚·‚Å‚É NPC ‚ª‚¢‚é‚© 2 ‚ÂˆÈã LocalPlayer ‚ª‚ ‚é‚È‚ç‰½‚à‚µ‚È‚¢ */
    if (GetGameInstance()->GetNumLocalPlayers() >= 2) return;

    /* ------ NPC Controller ‚ð‚½‚¾‚Ì Actor ‚Æ‚µ‚Ä spawn ------ */
    FActorSpawnParameters P;
    P.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    AJankenNPCController* NPC = GetWorld()->SpawnActor<AJankenNPCController>(
        AJankenNPCController::StaticClass(),
        FVector::ZeroVector, FRotator::ZeroRotator, P);

    if (!NPC) {
        UE_LOG(LogTemp, Error, TEXT("Spawn NPCController failed"));
    }
    else
        UE_LOG(LogTemp, Log, TEXT("NPCController spawned OK"));
}