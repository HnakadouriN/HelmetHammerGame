// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JankenBaseMode.h"
#include "LocalVersusMode.generated.h"

/**
 * 
 */
UCLASS()
class HELMETANDHAMMERGAME_API ALocalVersusMode : public AJankenBaseMode
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
};
