#pragma once

#include "CoreMinimal.h"
#include "RuleBase.h"
#include "Rule_GuardBreak.generated.h"

/**
 * 
 */
UCLASS()
class HELMETANDHAMMERGAME_API URule_GuardBreak : public URuleBase
{
	GENERATED_BODY()
	
public:
	const float GuardBreakChance = 0.5f;   // 50% ‚Å Guard Break
	virtual void ModifyAttack_Implementation(bool& bAtk, bool& /*bDef*/) const override
	{
		if (bAtk && FMath::FRand() < GuardBreakChance)  bAtk = false;
	}
};
