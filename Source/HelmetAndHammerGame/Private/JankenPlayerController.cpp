// Fill out your copyright notice in the Description page of Project Settings.


#include "JankenPlayerController.h"
#include "JankenGameState.h"
#include "Rule_Reverse.h"

void AJankenPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction("Rock", IE_Pressed, this, &AJankenPlayerController::ChooseRock);
	InputComponent->BindAction("Paper", IE_Pressed, this, &AJankenPlayerController::ChoosePaper);
	InputComponent->BindAction("Scissors", IE_Pressed, this, &AJankenPlayerController::ChooseScissors);
	InputComponent->BindAction("Attack", IE_Pressed, this, &AJankenPlayerController::PressAttack);
	InputComponent->BindAction("Defend", IE_Pressed, this, &AJankenPlayerController::PressDifend);
	InputComponent->BindAction("Reverse", IE_Pressed, this, &AJankenPlayerController::ToggleReverse);
}

void AJankenPlayerController::SendHand(EHand Hand)
{
	if (AJankenGameState* GameState = GetWorld()->GetGameState<AJankenGameState>())
	{
		GameState->SetPlayerHand(Hand);
	}
}

void AJankenPlayerController::SendAction(bool bAttack)
{
	if (AJankenGameState* GameState = GetWorld()->GetGameState<AJankenGameState>())
	{
		GameState->SetPlayerAction(bAttack);
	}
}

void AJankenPlayerController::ToggleReverse()
{
	if (AJankenGameState* GameState = GetWorld()->GetGameState<AJankenGameState>())
	{
		//bool bAlreadyReversed = GameState->RoundResult.bReverse;
		if (!bActorLabelEditable)
		{
			URule_Reverse* NewRule = NewObject<URule_Reverse>();
			//GameState->SetRule(NewRule);
		}
		else
		{
			//GameState->SetRule(nullptr);
		}
	}
}
