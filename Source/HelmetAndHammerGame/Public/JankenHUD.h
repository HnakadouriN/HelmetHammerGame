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
	/* ==== テクスチャ割り当て ==== */
	UPROPERTY(EditDefaultsOnly, Category = "Janken|Textures") UTexture2D* RockTex;
	UPROPERTY(EditDefaultsOnly, Category = "Janken|Textures") UTexture2D* ScissorsTex;
	UPROPERTY(EditDefaultsOnly, Category = "Janken|Textures") UTexture2D* PaperTex;

	/* ==== UI ウィジェットクラス ==== */
	UPROPERTY(EditDefaultsOnly, Category = "Janken|UI") TSubclassOf<UUserWidget> RoundWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category = "Janken|UI") TSubclassOf<UUserWidget> RuleWidgetClass;

protected:
	void BeginPlay() override;
private:
	/* ================== 生成したウィジェット ================== */
	UUserWidget* RoundWidget = nullptr;   // == WBP_RoundInfo
	UUserWidget* RuleWidget = nullptr;   // == WBP_RuleSelect

	/* ---- RoundInfo 内部部品 ---- */
	UImage* P0Image = nullptr;
	UImage* P1Image = nullptr;
	UTextBlock* WinnerText = nullptr;
	UTextBlock* CountText = nullptr;
	UTextBlock* PhaseText = nullptr;

	/* ---- RuleSelect 内部部品 ---- */
	UCheckBox* CB_Custom = nullptr; 
	UCheckBox* CB_Half = nullptr;   
	UButton* BTN_Confirm = nullptr;   

	/* ================== コールバック ================== */
	/** フェーズ変更時：UI の可視切替とラウンド情報更新 */
	UFUNCTION() void OnPhaseChanged(EPhase NewPhase);

	/** GameState がカウントダウンをブロードキャスト */
	UFUNCTION() void HandleCountdown();

	/** ルール OK ボタン */
	UFUNCTION() void OnConfirmRulesClicked();

	/* ================== ヘルパ ================== */
	UTexture2D* HandToTexture(EHand H) const;
};
