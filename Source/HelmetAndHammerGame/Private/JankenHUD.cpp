#include "JankenHUD.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "JankenGameState.h"


void AJankenHUD::BeginPlay()
{
    Super::BeginPlay();

    /* ------- Widget インスタンス生成 ------- */
    if (RoundWidgetClass)
    {
        RoundWidget = CreateWidget(GetWorld(), RoundWidgetClass);
        if (RoundWidget)
        {
            RoundWidget->AddToViewport();

            P0Image = Cast<UImage>(RoundWidget->GetWidgetFromName(TEXT("Image_P0Hand")));
            P1Image = Cast<UImage>(RoundWidget->GetWidgetFromName(TEXT("Image_P1Hand")));
            WinnerText = Cast<UTextBlock>(RoundWidget->GetWidgetFromName(TEXT("Text_Winner")));
            CountText = Cast<UTextBlock>(RoundWidget->GetWidgetFromName(TEXT("TXT_Count")));
            if (CountText)  CountText->SetVisibility(ESlateVisibility::Hidden);
        }
    }

    /* ------- GameState のイベント購読 ------- */
    if (auto* GS = GetWorld()->GetGameState<AJankenGameState>())
    {
        GS->OnPhaseChanged.AddDynamic(this, &AJankenHUD::OnPhaseChanged);

        /* CountdownTick は C++ delegate なので手動バインド */
        GS->OnCountdownTick.AddDynamic(this, &AJankenHUD::HandleCountdown);
    }
}

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

/* ------- フェーズごとに UI 更新 ------- */
void AJankenHUD::OnPhaseChanged(EPhase NewPhase)
{
    AJankenGameState* GS = GetWorld()->GetGameState<AJankenGameState>();
    if (!GS || !RoundWidget) return;

    if (NewPhase == EPhase::CountingDown)
    {
        HandleCountdown();            // ★ここを追加
    }

    switch (NewPhase)
    {
    case EPhase::HandResult:
    case EPhase::ActionSelect:
        /* 手アイコンだけ更新、バナーは空 */
        if (P0Image) P0Image->SetBrushFromTexture(HandToTexture(GS->Players[0].Hand));
        if (P1Image) P1Image->SetBrushFromTexture(HandToTexture(GS->Players[1].Hand));
        if (WinnerText) WinnerText->SetText(FText::GetEmpty());
        break;

    case EPhase::Resolve:
    {
        const bool  bP0Win = GS->Players[0].bWin;
        const bool  bP1Win = GS->Players[1].bWin;
        const bool  bDraw = !(bP0Win || bP1Win);

        FText Msg = bDraw ? FText::FromString("DRAW")
            : bP0Win ? FText::FromString("YOU WIN!")
            : FText::FromString("YOU LOSE!");
        FLinearColor Col = bDraw ? FLinearColor::Yellow
            : bP0Win ? FLinearColor::Green
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

/* ------- 手 Enum → テクスチャ ------- */
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
