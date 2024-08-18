// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class SKYCITYFUGITIVE_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()
public:


    UPROPERTY(meta = (BindWidget))
    UButton* playButton;

    UPROPERTY(meta = (BindWidget))
    UButton* quitButton;

    UMainMenu(const FObjectInitializer& ObjectInitializer);
};
