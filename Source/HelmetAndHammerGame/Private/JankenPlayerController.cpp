// Fill out your copyright notice in the Description page of Project Settings.


#include "JankenPlayerController.h"
#include "JankenGameState.h"
#include "Rule_Reverse.h"


void AJankenPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("Rock", IE_Pressed, this, &AJankenPlayerController::OnRock);
	InputComponent->BindAction("Scissors", IE_Pressed, this, &AJankenPlayerController::OnScissors);
	InputComponent->BindAction("Paper", IE_Pressed, this, &AJankenPlayerController::OnPaper);
	InputComponent->BindAction("Attack", IE_Pressed, this, &AJankenPlayerController::OnAttack);
	InputComponent->BindAction("Defend", IE_Pressed, this, &AJankenPlayerController::OnDefend);
}


void AJankenPlayerController::OnRock()
{
	UE_LOG(LogTemp, Log, TEXT("Input: ROCK"));
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, TEXT("ROCK"));
	SendHand(EHand::Rock);
}
void AJankenPlayerController::OnScissors()
{
	UE_LOG(LogTemp, Log, TEXT("Input: SCISSORS"));
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, TEXT("SCISSORS"));
	SendHand(EHand::Scissors);
}
void AJankenPlayerController::OnPaper()
{
	SendHand(EHand::Paper);
}
void AJankenPlayerController::OnAttack()
{
	SendAction(true);
}
void AJankenPlayerController::OnDefend()
{
	SendAction(false);
}
void AJankenPlayerController::SendHand(EHand Hand)
{
	if (auto* GS = GetWorld()->GetGameState<AJankenGameState>())
		GS->SetPlayerHand(/*Idx=*/0, Hand);   // 人間は常にプレーヤー0
}
void AJankenPlayerController::SendAction(bool bAtk)
{
	if (auto* GS = GetWorld()->GetGameState<AJankenGameState>())
		GS->SetPlayerAction(/*Idx=*/0, bAtk);
}