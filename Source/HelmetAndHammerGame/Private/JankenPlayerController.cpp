#include "JankenPlayerController.h"
#include "JankenGameState.h"
#include "JankenTypes.h"
#include "Rule_Reverse.h"


void AJankenPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	const int32 ControllerId = GetLocalPlayer()->GetControllerId();
	UE_LOG(LogTemp, Log, TEXT("PlayerController [%d] セットアップ"), ControllerId);

	InputComponent->BindAction("Rock", IE_Pressed, this, &AJankenPlayerController::OnRock);
	InputComponent->BindAction("Scissors", IE_Pressed, this, &AJankenPlayerController::OnScissors);
	InputComponent->BindAction("Paper", IE_Pressed, this, &AJankenPlayerController::OnPaper);
	InputComponent->BindAction("Attack", IE_Pressed, this, &AJankenPlayerController::OnAttack);
	InputComponent->BindAction("Defend", IE_Pressed, this, &AJankenPlayerController::OnDefend);
}


void AJankenPlayerController::OnRock()
{
	UE_LOG(LogTemp, Log, TEXT("Input: ROCK"));
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, TEXT("ROCK"));
	SendHandAndRules(EHand::Rock);
}
void AJankenPlayerController::OnScissors()
{
	UE_LOG(LogTemp, Log, TEXT("Input: SCISSORS"));
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, TEXT("SCISSORS"));
	SendHandAndRules(EHand::Scissors);
}
void AJankenPlayerController::OnPaper()
{
	SendHandAndRules(EHand::Paper);
}
void AJankenPlayerController::OnAttack()
{
	SendAction(true);
}
void AJankenPlayerController::OnDefend()
{
	SendAction(false);
}
void AJankenPlayerController::SendHandAndRules(EHand Hand)
{
	if (AJankenGameState* GS = GetWorld()->GetGameState<AJankenGameState>())
	{
		GS->ApplyRulesAndHand(                      // ★ GameState 側の新関数
			GetLocalPlayer()->GetControllerId(),
			Hand,
			SelectedRuleIndices);

		SelectedRuleIndices.Reset();                    // 送信後クリア
	}
}
void AJankenPlayerController::SendAction(bool bAtk)
{
	if (AJankenGameState* GS = GetWorld()->GetGameState<AJankenGameState>())
		GS->SetPlayerAction(GetLocalPlayer()->GetControllerId(), bAtk);
}