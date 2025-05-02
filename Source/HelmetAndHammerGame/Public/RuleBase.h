// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "JankenTypes.h"
#include "RuleBase.generated.h"

/**
 * 
 */
UCLASS(Abstract,Blueprintable,EditInlineNew)
class HELMETANDHAMMERGAME_API URuleBase : public UObject
{
	GENERATED_BODY()
	
public:
    virtual void   PreHand(class AJankenGameState* GS) {}
    virtual int32  ModifyJankenResult(int32 BaseResult) const { return BaseResult; }
    virtual void   OnEnterAction(class AJankenGameState* GS) {}
    virtual void   PostResolve(class AJankenGameState* GS) {}
};
