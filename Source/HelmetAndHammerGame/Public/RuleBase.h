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
    /** 勝敗数値を弄る (-1=P0勝,1=P1勝,0=Draw) */
    UFUNCTION(BlueprintNativeEvent)
    int32 ModifyJankenResult(int32 BaseResult, int32 RoundCount) const;

    /** ★追加: 攻防判定に介入できるフック */
    UFUNCTION(BlueprintNativeEvent)
    void ModifyAttack(bool& bAttackerOK, bool& bDefenderOK) const;
};
