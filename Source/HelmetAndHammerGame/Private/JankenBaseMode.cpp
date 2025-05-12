// Fill out your copyright notice in the Description page of Project Settings.


#include "JankenBaseMode.h"
#include "JankenGameState.h"
#include "JankenPlayerController.h"
#include "JankenHUD.h"

AJankenBaseMode::AJankenBaseMode()
{
	DefaultPawnClass = nullptr; // �v���C���[�͎����������Ȃ�
	HUDClass = AJankenHUD::StaticClass(); // HUD���g�p����
	PlayerControllerClass = AJankenPlayerController::StaticClass(); // P0
	GameStateClass = AJankenGameState::StaticClass();
}

