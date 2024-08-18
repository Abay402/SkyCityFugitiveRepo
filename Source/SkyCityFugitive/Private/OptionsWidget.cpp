// Fill out your copyright notice in the Description page of Project Settings.


#include "OptionsWidget.h"

UOptionsWidget::UOptionsWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UOptionsWidget::ToggleActive()
{
	if (active) active = false;
	else active = true;



}
