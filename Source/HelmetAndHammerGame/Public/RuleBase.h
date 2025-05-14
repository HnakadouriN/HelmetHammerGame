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
    /** ���s���l��M�� (-1=P0��,1=P1��,0=Draw) */
    UFUNCTION(BlueprintNativeEvent)
    int32 ModifyJankenResult(int32 BaseResult, int32 RoundCount) const;

    /** ���ǉ�: �U�h����ɉ���ł���t�b�N */
    UFUNCTION(BlueprintNativeEvent)
    void ModifyAttack(bool& bAttackerOK, bool& bDefenderOK) const;
};
