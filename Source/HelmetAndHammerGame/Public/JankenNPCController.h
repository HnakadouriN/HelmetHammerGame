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
    /* NPC �����삷��v���C���[�X���b�g (0 or 1) �\ ���[�J���ΐ�ł� 1 �Œ� */
    UPROPERTY(EditAnywhere) int32 PlayerIndex = 1;

protected:
    void BeginPlay() override;       /* Tick ���J�n */
    void Tick(float DeltaSeconds) override;

private:
    /* �t�F�[�Y�ʏ��� */
    void HandleWaitingInput(class AJankenGameState*);
    void HandleActionSelect(class AJankenGameState*);
    void HandleResolve(class AJankenGameState*);

    /* �ėp�w���p */
    void Delay(float Sec, TFunction<void()> Fn);
    class AJankenGameState* GetGS() const;
    EHand DecideHand()  const;       // 0.4 s ��ɏo����
    bool  ShouldAttack() const;      // �������U�������H

    /* �����p�����[�^ */
    UPROPERTY(EditAnywhere) float HandDelay = 0.4f;   // ����o���܂�
    UPROPERTY(EditAnywhere) float ActionDelay = 3.0f;   // �U�h�������܂�

    /* �^�C�}�[�̓��E���h�ɂ� 1 �{���� */
    FTimerHandle HandTimer;
    FTimerHandle ActionTimer;
};
