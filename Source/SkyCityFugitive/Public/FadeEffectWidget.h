// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseWidgetClass.h"
#include "FadeEffectWidget.generated.h"

/**
 * 
 */
UCLASS()
class SKYCITYFUGITIVE_API UFadeEffectWidget : public UBaseWidgetClass
{
	GENERATED_BODY()
public:
    
    UFUNCTION(BlueprintCallable)
    void PlayAnimationEffect(EUMGSequencePlayMode::Type animationDirection_);

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        float fadeInSpeed;
protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    class UWidgetAnimation* fadeAnim;

};
