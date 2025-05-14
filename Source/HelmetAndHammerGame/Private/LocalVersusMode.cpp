#include "LocalVersusMode.h"
#include "EngineUtils.h"
#include "GameFramework/PlayerStart.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "JankenPlayerController.h"


void ALocalVersusMode::BeginPlay()
{
    Super::BeginPlay();

    // 1 人しか居ない時だけ 2P を生成（PIE“NumPlayers=2”時は不要）
    if (GetGameInstance()->GetNumLocalPlayers() < 2)
    {
		UGameplayStatics::CreatePlayer(this, /*ControllerId=*/1, /*bSpawnPawn=*/true);
    }
}
void ALocalVersusMode::PreLogin(const FString& Opt, const FString& Addr,
    const FUniqueNetIdRepl& Id, FString& ErrMsg)
{
	Super::PreLogin(Opt, Addr, Id, ErrMsg);
    if (GetNumPlayers() >= 2)
        ErrMsg = TEXT("LocalVersus: only 2 players allowed");
}
AActor* ALocalVersusMode::ChoosePlayerStart_Implementation(AController* NewPlayer)
{
	if (APlayerController* PC = Cast<APlayerController>(NewPlayer))
	{
		if (ULocalPlayer* LP = PC->GetLocalPlayer())
		{
            const int32 Cid = LP->GetControllerId(); // 0 or 1
            const FName Desired = (Cid == 0) ? TEXT("P1Start") : TEXT("P2Start");

			for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
			{
				if (It->PlayerStartTag == Desired)
					return *It;
			}
		}
	}
        return Super::ChoosePlayerStart_Implementation(NewPlayer); // フォールバック
}