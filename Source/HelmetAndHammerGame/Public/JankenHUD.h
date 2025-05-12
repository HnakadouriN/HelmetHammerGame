// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Engine/Texture2D.h"
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
	/* テクスチャを Class Defaults で割り当ててください */
	UPROPERTY(EditDefaultsOnly, Category = "Janken|Textures") UTexture2D* RockTex;
	UPROPERTY(EditDefaultsOnly, Category = "Janken|Textures") UTexture2D* ScissorsTex;
	UPROPERTY(EditDefaultsOnly, Category = "Janken|Textures") UTexture2D* PaperTex;

	/* RoundInfo ウィジェット（WBP_RoundInfo）を指定 */
	UPROPERTY(EditDefaultsOnly, Category = "Janken|UI") TSubclassOf<UUserWidget> RoundWidgetClass;
protected:
	void BeginPlay() override;
private:
	UUserWidget* RoundWidget = nullptr;
	class UImage* P0Image = nullptr;
	class UImage* P1Image = nullptr;
	class UTextBlock* WinnerText = nullptr;

	UFUNCTION() void OnPhaseChanged(EPhase NewPhase);
	UTexture2D* HandToTexture(EHand H) const;
};
