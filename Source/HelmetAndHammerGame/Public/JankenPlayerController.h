// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "JankenTypes.h"
#include "JankenPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class HELMETANDHAMMERGAME_API AJankenPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void SetupInputComponent() override;
	
private:
	void ChooseRock() { SendHand(EHand::Rock); }
	void ChoosePaper() { SendHand(EHand::Paper); }
	void ChooseScissors() { SendHand(EHand::Scissors); }

	void PressAttack() { SendAction(true); }
	void PressDifend() { SendAction(false); }

	void ToggleReverse();

	void SendHand(EHand Hand);
	void SendAction(bool bAttack);
};
