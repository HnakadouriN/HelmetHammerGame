// Fill out your copyright notice in the Description page of Project Settings.


#include "JankenPlayerController.h"
#include "JankenGameState.h"
#include "Rule_Reverse.h"


void AJankenPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	const int32 ControllerId = GetLocalPlayer()->GetControllerId();
	UE_LOG(LogTemp, Log, TEXT("PlayerController [%d] セットアップ"), ControllerId);

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
	AJankenGameState* GS = GetWorld()->GetGameState<AJankenGameState>();
	int32 ControllerId = GetLocalPlayer()->GetControllerId();
	if (GS) GS->SetPlayerHand(ControllerId, Hand);
}
void AJankenPlayerController::SendAction(bool bAtk)
{
	AJankenGameState* GS = GetWorld()->GetGameState<AJankenGameState>();
	int32 ControllerId = GetLocalPlayer()->GetControllerId();
	if (GS) GS->SetPlayerAction(ControllerId, bAtk);
}