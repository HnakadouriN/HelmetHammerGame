// Fill out your copyright notice in the Description page of Project Settings.


#include "RuleBase.h"

int32 URuleBase::ModifyJankenResult_Implementation(int32 BaseResult, int32 /*RoundCount*/) const
{
    return BaseResult;    // ïœçXÇµÇ»Ç¢
}

void URuleBase::ModifyAttack_Implementation(bool& /*bAttackerOK*/, bool& /*bDefenderOK*/) const
{
}
