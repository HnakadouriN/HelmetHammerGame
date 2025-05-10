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
	virtual int32 ModifyJankenResult(int32 BaseResult,int32) const override
	{
		return -BaseResult;
	}
};
