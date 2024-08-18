// Fill out your copyright notice in the Description page of Project Settings.


#include "FadeEffectWidget.h"

void UFadeEffectWidget::PlayAnimationEffect(EUMGSequencePlayMode::Type animationDirection_)
{

    if (IsValid(fadeAnim))
    {
        PlayAnimation(fadeAnim, 0.0f, 1, animationDirection_, fadeInSpeed);
    }


}
