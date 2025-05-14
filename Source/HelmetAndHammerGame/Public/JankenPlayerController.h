#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "JankenTypes.h"
#include "JankenPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class HELMETANDHAMMERGAME_API AJankenPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void SetupInputComponent() override;
	
private:
	/* “ü—Íƒnƒ“ƒhƒ‰ */
	void OnRock();
	void OnScissors();
	void OnPaper();
	void OnAttack();
	void OnDefend();

	void SendHand(EHand Hand);
	void SendAction(bool bAttack);
};
