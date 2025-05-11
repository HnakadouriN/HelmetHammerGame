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
	int32 GetId() const;

	void OnHandRock();
	void OnHandPaper();
	void OnHandScissors();
	void OnActionAttack();
	void OnActionDefend();

	void SendHand(EHand Hand);
	void SendAction(bool bAttack);
};
