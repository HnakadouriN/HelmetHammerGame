// Fill out your copyright notice in the Description page of Project Settings.


#include "JankenBaseMode.h"
#include "JankenGameState.h"
#include "JankenPlayerController.h"
#include "JankenHUD.h"

AJankenBaseMode::AJankenBaseMode()
{
	DefaultPawnClass = nullptr; // プレイヤーは自動生成しない
	HUDClass = AJankenHUD::StaticClass(); // HUDを使用する
	PlayerControllerClass = AJankenPlayerController::StaticClass(); // P0
	GameStateClass = AJankenGameState::StaticClass();
}

