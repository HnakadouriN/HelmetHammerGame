// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LocalVersusMode.generated.h"

/**
 * 
 */
UCLASS()
class HELMETANDHAMMERGAME_API ALocalVersusMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ALocalVersusMode();
	
protected:
	virtual void BeginPlay() override;
};
