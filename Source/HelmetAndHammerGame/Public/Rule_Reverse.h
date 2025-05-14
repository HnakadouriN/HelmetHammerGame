// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RuleBase.h"
#include "Rule_Reverse.generated.h"

/**
 * 
 */
UCLASS()
class HELMETANDHAMMERGAME_API URule_Reverse : public URuleBase
{
	GENERATED_BODY()

public:
    virtual int32 ModifyJankenResult_Implementation(int32 Base, int32) const override
    {
        static const int32 Map[3] = { 2,1,0 };       
        return Map[Base + 1] - 1;                           // [-1,0,1] ‚ÉŽû‚ß‚é
    }
};
