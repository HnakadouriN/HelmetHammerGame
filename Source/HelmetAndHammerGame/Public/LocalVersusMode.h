#pragma once

#include "CoreMinimal.h"
#include "JankenBaseMode.h"
#include "LocalVersusMode.generated.h"

/**
 * 
 */
UCLASS()
class ALocalVersusMode : public AJankenBaseMode   // Å©ã§í äÓíÍÇåpè≥
{
    GENERATED_BODY()
protected:
    virtual void BeginPlay() override;
    virtual void PreLogin(const FString& Options, const FString& Address,
        const FUniqueNetIdRepl& UniqueId, FString& ErrorMsg) override;
    virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
};