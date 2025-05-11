// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "VersusNPCMode.generated.h"

/**
 * 
 */
UCLASS()
class HELMETANDHAMMERGAME_API AVersusNPCMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AVersusNPCMode();
protected:
	virtual void BeginPlay() override;
};
