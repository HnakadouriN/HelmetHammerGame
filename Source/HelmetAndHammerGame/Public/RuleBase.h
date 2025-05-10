#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "JankenTypes.h"
#include "RuleBase.generated.h"

/**
 * ���[�����N���X
 */
UCLASS(Abstract,Blueprintable,EditInlineNew)
class HELMETANDHAMMERGAME_API URuleBase : public UObject
{
	GENERATED_BODY()
	
public:
    virtual void   PreHand(class AJankenGameState* GS) {}
    //���s�̕␳
    virtual int32  ModifyJankenResult(int32 BaseResult,int32) const { return BaseResult; }
	//�A�N�V�����I�����̕␳
    virtual void   OnEnterAction(class AJankenGameState* GS) {}
	//�A�N�V�����������̕␳
    virtual void   PostResolve(class AJankenGameState* GS,int32) {}
};
