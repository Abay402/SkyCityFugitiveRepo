// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FadeEffectWidget.h"
#include "OptionsWidget.h"
#include "MainPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SKYCITYFUGITIVE_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	AMainPlayerController();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<UFadeEffectWidget> fadeWidgetClass;

		float EndTransition();

	void HandleOptionsMenu();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<UOptionsWidget> optionsMenuClass;

	UOptionsWidget* optionsMenu;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
};
