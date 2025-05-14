#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Engine/Texture2D.h"
#include "Components/CheckBox.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"
#include "JankenTypes.h"
#include "JankenHUD.generated.h"
/**
 * 
 */
UCLASS()
class HELMETANDHAMMERGAME_API AJankenHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	/* ==== �e�N�X�`�����蓖�� ==== */
	UPROPERTY(EditDefaultsOnly, Category = "Janken|Textures") UTexture2D* RockTex;
	UPROPERTY(EditDefaultsOnly, Category = "Janken|Textures") UTexture2D* ScissorsTex;
	UPROPERTY(EditDefaultsOnly, Category = "Janken|Textures") UTexture2D* PaperTex;

	/* ==== UI �E�B�W�F�b�g�N���X ==== */
	UPROPERTY(EditDefaultsOnly, Category = "Janken|UI") TSubclassOf<UUserWidget> RoundWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category = "Janken|UI") TSubclassOf<UUserWidget> RuleWidgetClass;

protected:
	void BeginPlay() override;
private:
	/* ================== ���������E�B�W�F�b�g ================== */
	UUserWidget* RoundWidget = nullptr;   // == WBP_RoundInfo
	UUserWidget* RuleWidget = nullptr;   // == WBP_RuleSelect

	/* ---- RoundInfo �������i ---- */
	UImage* P0Image = nullptr;
	UImage* P1Image = nullptr;
	UTextBlock* WinnerText = nullptr;
	UTextBlock* CountText = nullptr;
	UTextBlock* PhaseText = nullptr;

	/* ---- RuleSelect �������i ---- */
	UCheckBox* CB_Custom = nullptr; 
	UCheckBox* CB_Half = nullptr;   
	UButton* BTN_Confirm = nullptr;   

	/* ================== �R�[���o�b�N ================== */
	/** �t�F�[�Y�ύX���FUI �̉��ؑւƃ��E���h���X�V */
	UFUNCTION() void OnPhaseChanged(EPhase NewPhase);

	/** GameState ���J�E���g�_�E�����u���[�h�L���X�g */
	UFUNCTION() void HandleCountdown();

	/** ���[�� OK �{�^�� */
	UFUNCTION() void OnConfirmRulesClicked();

	/* ================== �w���p ================== */
	UTexture2D* HandToTexture(EHand H) const;
};
