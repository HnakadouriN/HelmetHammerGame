#pragma once

#include "CoreMinimal.h"
#include "JankenBaseMode.h"
#include "VersusNPCMode.generated.h"


UCLASS()
class HELMETANDHAMMERGAME_API AVersusNPCMode : public AJankenBaseMode
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
};
