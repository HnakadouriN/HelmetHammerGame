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

public:
	UFUNCTION(BlueprintCallable)
	void SetSelectedRuleIndices(const TArray<int32>& In) {
		SelectedRuleIndices = In;
	}

protected:
	virtual void SetupInputComponent() override;
	
private:
	/* 入力ハンドラ */
	void OnRock();
	void OnScissors();
	void OnPaper();
	void OnAttack();
	void OnDefend();

	void SendHandAndRules(EHand Hand);
	void SendAction(bool bAttack);

	//void ApplyRulesAndHand(int32 PlayerId, EHand Hand, const TArray<int32>& RuleIdx);
	//にすればローカル専用だが、オンラインを想定するとどのように実装するだろうか？
	//UFUNCTION(Server,Reliable)
	//void Server_SendHandAndRules(EHand Hand,const TArray<int32>& RuleId);

	TArray<int32> SelectedRuleIndices; 
};
