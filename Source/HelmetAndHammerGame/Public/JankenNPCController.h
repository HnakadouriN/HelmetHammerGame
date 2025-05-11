// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "JankenTypes.h"
#include "JankenNPCController.generated.h"

/**
 * 
 */
UCLASS()
class HELMETANDHAMMERGAME_API AJankenNPCController : public AAIController
{
	GENERATED_BODY()
	
public:
    /* NPC が操作するプレイヤースロット (0 or 1) ― ローカル対戦では 1 固定 */
    UPROPERTY(EditAnywhere) int32 PlayerIndex = 1;

protected:
    void BeginPlay() override;       /* Tick を開始 */
    void Tick(float DeltaSeconds) override;

private:
    /* フェーズ別処理 */
    void HandleWaitingInput(class AJankenGameState*);
    void HandleActionSelect(class AJankenGameState*);
    void HandleResolve(class AJankenGameState*);

    /* 汎用ヘルパ */
    void Delay(float Sec, TFunction<void()> Fn);
    class AJankenGameState* GetGS() const;
    EHand DecideHand()  const;       // 0.4 s 後に出す手
    bool  ShouldAttack() const;      // 自分が攻撃側か？

    /* 調整パラメータ */
    UPROPERTY(EditAnywhere) float HandDelay = 0.4f;   // 手を出すまで
    UPROPERTY(EditAnywhere) float ActionDelay = 3.0f;   // 攻防を押すまで

    /* タイマーはラウンドにつき 1 本ずつ */
    FTimerHandle HandTimer;
    FTimerHandle ActionTimer;
};
