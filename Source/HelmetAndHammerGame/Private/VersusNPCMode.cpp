// Fill out your copyright notice in the Description page of Project Settings.


#include "VersusNPCMode.h"
#include "Kismet/GameplayStatics.h"
#include "JankenPlayerController.h"
#include "JankenNPCController.h"
#include "JankenGameState.h"

AVersusNPCMode::AVersusNPCMode()
{
	DefaultPawnClass = nullptr; // ƒvƒŒƒCƒ„[‚ÍŽ©“®¶¬‚µ‚È‚¢
    HUDClass = nullptr;
    PlayerControllerClass = AJankenPlayerController::StaticClass(); // P0
    GameStateClass = AJankenGameState::StaticClass();
}

void AVersusNPCMode::BeginPlay()
{
    Super::BeginPlay();

    FActorSpawnParameters P;
    P.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    AJankenNPCController* NPC =
        GetWorld()->SpawnActor<AJankenNPCController>(
            AJankenNPCController::StaticClass(),
            FVector::ZeroVector, FRotator::ZeroRotator, P);

    if (!NPC) {
        UE_LOG(LogTemp, Error, TEXT("Spawn NPCController failed"));
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("NPCController spawned OK"));
    }
}
