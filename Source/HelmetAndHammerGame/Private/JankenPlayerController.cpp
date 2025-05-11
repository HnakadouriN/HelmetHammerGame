// Fill out your copyright notice in the Description page of Project Settings.


#include "JankenPlayerController.h"
#include "JankenGameState.h"
#include "Rule_Reverse.h"


int32 AJankenPlayerController::GetId() const
{
	return GetLocalPlayer()->GetControllerId();
}

void AJankenPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	const FString Prefix = FString::Printf(TEXT("Player%d_"), GetId());
	auto B = [this, &Prefix](const TCHAR* Name,auto Method)
	{
		InputComponent->BindAction(*FString(Prefix + Name), IE_Pressed, this, Method);
	};

	B(TEXT("Rock"), &AJankenPlayerController::OnHandRock);
	B(TEXT("Paper"), &AJankenPlayerController::OnHandPaper);
	B(TEXT("Scissors"), &AJankenPlayerController::OnHandScissors);
	B(TEXT("Attack"), &AJankenPlayerController::OnActionAttack);
	B(TEXT("Defend"), &AJankenPlayerController::OnActionDefend);
}

void AJankenPlayerController::OnHandRock()
{
	SendHand(EHand::Rock);
}
void AJankenPlayerController::OnHandPaper()
{
	SendHand(EHand::Paper);
}
void AJankenPlayerController::OnHandScissors()
{
	SendHand(EHand::Scissors);
}
void AJankenPlayerController::OnActionAttack()
{
	SendAction(true);
}
void AJankenPlayerController::OnActionDefend()
{
	SendAction(false);
}

void AJankenPlayerController::SendHand(EHand Hand)
{
	UE_LOG(LogTemp, Log, TEXT("PC%d SendHand %d"), GetId(), (int32)Hand);
	if (AJankenGameState* GameState = GetWorld()->GetGameState<AJankenGameState>())
	{
		GameState->SetPlayerHand(GetId(), Hand);
	}
}

void AJankenPlayerController::SendAction(bool bAttack)
{
	if (AJankenGameState* GameState = GetWorld()->GetGameState<AJankenGameState>())
	{
		GameState->SetPlayerAction(GetId(), bAttack);
	}
}

