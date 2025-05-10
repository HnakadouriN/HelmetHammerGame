#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "JankenTypes.h"
#include "RuleBase.generated.h"

/**
 * ルール基底クラス
 */
UCLASS(Abstract,Blueprintable,EditInlineNew)
class HELMETANDHAMMERGAME_API URuleBase : public UObject
{
	GENERATED_BODY()
	
public:
    virtual void   PreHand(class AJankenGameState* GS) {}
    //勝敗の補正
    virtual int32  ModifyJankenResult(int32 BaseResult,int32) const { return BaseResult; }
	//アクション選択時の補正
    virtual void   OnEnterAction(class AJankenGameState* GS) {}
	//アクション解決時の補正
    virtual void   PostResolve(class AJankenGameState* GS,int32) {}
};
