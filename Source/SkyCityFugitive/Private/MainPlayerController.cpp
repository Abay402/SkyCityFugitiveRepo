// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"

AMainPlayerController::AMainPlayerController()
{
}

float AMainPlayerController::EndTransition()
{

    UFadeEffectWidget* fadeWidget = CreateWidget<UFadeEffectWidget>(this, fadeWidgetClass);

    if (fadeWidget)
    {
        fadeWidget->AddToViewport();
        fadeWidget->PlayAnimationEffect(EUMGSequencePlayMode::Forward);
        return fadeWidget->fadeInSpeed;
    }
    
    return 0;
}

void AMainPlayerController::HandleOptionsMenu()
{

    if (!optionsMenu)
    {

        optionsMenu = CreateWidget<UOptionsWidget>(GetWorld(), optionsMenuClass);
        optionsMenu->ToggleActive();
        optionsMenu->AddToViewport();
    }
    else
    {

        optionsMenu->ToggleActive();

    }



}

void AMainPlayerController::BeginPlay()
{

	Super::BeginPlay();

    UFadeEffectWidget* fadeWidget = CreateWidget<UFadeEffectWidget>(this, fadeWidgetClass);

    if (fadeWidget)
    {
        fadeWidget->AddToViewport();
        fadeWidget->PlayAnimationEffect(EUMGSequencePlayMode::Reverse);
    }
    bShowMouseCursor = true;
    bEnableClickEvents = true;
    bEnableMouseOverEvents = true;
}

void AMainPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    if (InputComponent)
    {
        InputComponent->BindAction("Option", IE_Pressed, this, &AMainPlayerController::HandleOptionsMenu);
    }

}
