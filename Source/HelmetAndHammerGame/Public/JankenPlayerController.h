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
	/* ���̓n���h�� */
	void OnRock();
	void OnScissors();
	void OnPaper();
	void OnAttack();
	void OnDefend();

	void SendHandAndRules(EHand Hand);
	void SendAction(bool bAttack);

	//void ApplyRulesAndHand(int32 PlayerId, EHand Hand, const TArray<int32>& RuleIdx);
	//�ɂ���΃��[�J����p�����A�I�����C����z�肷��Ƃǂ̂悤�Ɏ������邾�낤���H
	//UFUNCTION(Server,Reliable)
	//void Server_SendHandAndRules(EHand Hand,const TArray<int32>& RuleId);

	TArray<int32> SelectedRuleIndices; 
};
