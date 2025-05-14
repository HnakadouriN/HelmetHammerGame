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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GuardBreak")
	float BreakChance = 0.5f;

	void PostResolve(class AJankenGameState* GS, int32 WinnreId) override;
};
