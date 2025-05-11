// Fill out your copyright notice in the Description page of Project Settings.


#include "LocalVersusMode.h"
#include "Engine/LocalPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "JankenGameState.h"
#include "JankenPlayerController.h"

ALocalVersusMode::ALocalVersusMode() {
	DefaultPawnClass = nullptr;
	HUDClass = nullptr;
	PlayerControllerClass = AJankenPlayerController::StaticClass();
	GameStateClass = AJankenGameState::StaticClass();
}

void ALocalVersusMode::BeginPlay()
{
	Super::BeginPlay();

	auto* LocalPlayer = UGameplayStatics::CreatePlayer(GetWorld(), 1, true);
	if (LocalPlayer == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("Failed to create LocalPlayer"));
	}
}

