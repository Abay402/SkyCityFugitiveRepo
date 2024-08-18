// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "OptionsWidget.generated.h"

/**
 * 
 */
UCLASS()
class SKYCITYFUGITIVE_API UOptionsWidget : public UUserWidget
{
	GENERATED_BODY()
public:


    UPROPERTY(meta = (BindWidget))
    UButton* resumeButton;

    UPROPERTY(meta = (BindWidget))
    UButton* quitButton;

    UOptionsWidget(const FObjectInitializer& ObjectInitializer);


    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool active;

    void ToggleActive();


};
