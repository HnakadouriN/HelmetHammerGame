// Fill out your copyright notice in the Description page of Project Settings.


#include "LocalVersusMode.h"
#include "Kismet/GameplayStatics.h"
#include "JankenPlayerController.h"


void ALocalVersusMode::BeginPlay()
{
    Super::BeginPlay();

    FString Err;
    ULocalPlayer* LP = GetGameInstance()->CreateLocalPlayer(INDEX_NONE, Err, true);

    if (!LP || !LP->PlayerController)
    {
        UE_LOG(LogTemp, Error, TEXT("2P¶¬Ž¸”s: %s"), *Err);
        return;
    }

    LP->PlayerController->EnableInput(LP->PlayerController);

    UE_LOG(LogTemp, Log, TEXT("2P¶¬Š®—¹: CtrlId=%d, PC=%s"),
        LP->GetControllerId(),
        *LP->PlayerController->GetName());
}
