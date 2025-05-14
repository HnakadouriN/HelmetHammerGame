#include "JankenHUD.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "JankenGameState.h"



void AJankenHUD::BeginPlay()
{
    Super::BeginPlay();

    /* ---------- Round-Info Widget ---------- */
    if (RoundWidgetClass)
    {
        RoundWidget = CreateWidget<UUserWidget>(GetWorld(), RoundWidgetClass);
        if (RoundWidget)
        {
            RoundWidget->AddToViewport();

            /* 必要な子ウィジェットをキャッシュ */
            P0Image = Cast<UImage    >(RoundWidget->GetWidgetFromName(TEXT("Image_P0Hand")));
            P1Image = Cast<UImage    >(RoundWidget->GetWidgetFromName(TEXT("Image_P1Hand")));
            WinnerText = Cast<UTextBlock>(RoundWidget->GetWidgetFromName(TEXT("Text_Winner")));
            CountText = Cast<UTextBlock>(RoundWidget->GetWidgetFromName(TEXT("TXT_Count")));
            PhaseText = Cast<UTextBlock>(RoundWidget->GetWidgetFromName(TEXT("TXT_Phase")));
            if (CountText) CountText->SetVisibility(ESlateVisibility::Hidden);
        }
    }
    
    /* ---------- Rule-Select Widget ---------- */
    if (RuleWidgetClass)
    {
        RuleWidget = CreateWidget<UUserWidget>(GetWorld(), RuleWidgetClass);
        if (RuleWidget)
        {
            RuleWidget->AddToViewport();

            CB_Custom = Cast<UCheckBox>(RuleWidget->GetWidgetFromName(TEXT("CB_Custom")));
            CB_Half = Cast<UCheckBox>(RuleWidget->GetWidgetFromName(TEXT("CB_Half")));
            BTN_Confirm = Cast<UButton  >(RuleWidget->GetWidgetFromName(TEXT("BTN_Confirm")));

            if (BTN_Confirm)
            {
                BTN_Confirm->OnClicked.AddDynamic(this, &AJankenHUD::OnConfirmRulesClicked);
            }
        }
    }

    /* ---------- GameState へバインド ---------- */
    if (AJankenGameState* GS = GetWorld()->GetGameState<AJankenGameState>())
    {
        GS->OnPhaseChanged.AddDynamic(this, &AJankenHUD::OnPhaseChanged);
        GS->OnCountdownTick.AddDynamic(this, &AJankenHUD::HandleCountdown);

        /* 起動時フェーズに合わせて UI 初期表示 */
        OnPhaseChanged(GS->Phase);
    }
}

/* ==================================================================== */
/*  ルール決定ボタン                                                     */
/* ==================================================================== */
void AJankenHUD::OnConfirmRulesClicked()
{
    if (!CB_Custom || !CB_Half) return;

    /* チェック状態からルール ID 配列を生成
       0:CustomJudge, 1:HalfAttack50 (例) */
    TArray<int32> RuleIdx;
    if (CB_Custom->IsChecked()) RuleIdx.Add(0);
    if (CB_Half->IsChecked()) RuleIdx.Add(1);

    //未選択の場合はデフォルトのじゃんけん
    //if (RuleIdx.Num() == 0) return;               // 未選択なら無視

    if (AJankenGameState* GS = GetWorld()->GetGameState<AJankenGameState>())
        GS->SetRulesAndStart(RuleIdx);
}

/* ==================================================================== */
/*  フェーズ遷移ハンドラ ? 表示切替 & 勝敗表示                          */
/* ==================================================================== */
void AJankenHUD::OnPhaseChanged(EPhase NewPhase)
{
    /* --- UI 可視制御 --- */
    if (RuleWidget)
        RuleWidget->SetVisibility(
            NewPhase == EPhase::RuleSelect ?
            ESlateVisibility::Visible : ESlateVisibility::Hidden);

    if (RoundWidget)
        RoundWidget->SetVisibility(
            NewPhase == EPhase::RuleSelect ?
            ESlateVisibility::Hidden : ESlateVisibility::HitTestInvisible);

    if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
    {
        if (NewPhase == EPhase::RuleSelect || NewPhase == EPhase::WaitingInput)
        {
            FInputModeGameAndUI Mode;

            TSharedPtr<SWidget> FocusWidget;
            if (RuleWidget)
            {
                FocusWidget = RuleWidget->TakeWidget();
            }
            Mode.SetWidgetToFocus(FocusWidget);
            Mode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

            PC->SetInputMode(Mode);
            PC->bShowMouseCursor = true;
        }
        else
        {
            PC->SetInputMode(FInputModeGameOnly());
            PC->bShowMouseCursor = false;
        }
    }

    if (PhaseText)
    {
        static const UEnum* PhaseEnum = StaticEnum<EPhase>();
        PhaseText->SetText(PhaseEnum->GetDisplayNameTextByValue(static_cast<int64>(NewPhase)));
    }
    
    /* --- ラウンド結果 --- */
    if (AJankenGameState* GS = GetWorld()->GetGameState<AJankenGameState>())
    {
        switch (NewPhase)
        {
        case EPhase::HandResult:
        case EPhase::ActionSelect:
            if (P0Image) P0Image->SetBrushFromTexture(HandToTexture(GS->Players[0].Hand));
            if (P1Image) P1Image->SetBrushFromTexture(HandToTexture(GS->Players[1].Hand));
            if (WinnerText) WinnerText->SetText(FText::GetEmpty());
            break;

        case EPhase::Resolve:
        {
            const bool bP0 = GS->Players[0].bWin;
            const bool bP1 = GS->Players[1].bWin;
            const bool bDraw = !(bP0 || bP1);

            const FText Msg = bDraw ? FText::FromString(TEXT("DRAW"))
                : bP0 ? FText::FromString(TEXT("YOU WIN!"))
                : FText::FromString(TEXT("YOU LOSE!"));

            const FLinearColor Col = bDraw ? FLinearColor::Yellow
                : bP0 ? FLinearColor::Green
                : FLinearColor::Red;

            if (WinnerText)
            {
                WinnerText->SetText(Msg);
                WinnerText->SetColorAndOpacity(FSlateColor(Col));
            }
            break;
        }
        default: break;
        }
    }
}

/* ==================================================================== */
/*  カウントダウン表示                                                   */
/* ==================================================================== */
void AJankenHUD::HandleCountdown()
{
    AJankenGameState* GS = GetWorld()->GetGameState<AJankenGameState>();
    if (!GS || !CountText) return;

    if (GS->CountdownSec > 0)
    {
        CountText->SetVisibility(ESlateVisibility::HitTestInvisible);
        CountText->SetText(FText::AsNumber(GS->CountdownSec));
    }
    else
    {
        CountText->SetVisibility(ESlateVisibility::Hidden);
    }
}

/* ==================================================================== */
/*  手 → テクスチャ変換                                                 */
/* ==================================================================== */
UTexture2D* AJankenHUD::HandToTexture(EHand H) const
{
    switch (H)
    {
    case EHand::Rock:     return RockTex;
    case EHand::Scissors: return ScissorsTex;
    case EHand::Paper:    return PaperTex;
    default:              return nullptr;
    }
}