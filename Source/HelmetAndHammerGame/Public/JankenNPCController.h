// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "JankenTypes.h"
#include "JankenNPCController.generated.h"

/**
 * 
 */
UCLASS()
class HELMETANDHAMMERGAME_API AJankenNPCController : public AAIController
{
	GENERATED_BODY()
	
public:
    UPROPERTY(EditAnywhere) int32 PlayerIndex = 1;   // 1 = NPC

protected:
    void BeginPlay() override;
    void Tick(float DeltaSeconds) override;

private:
    EHand DecideHand() const;
    bool  ShouldAttack() const;
};
