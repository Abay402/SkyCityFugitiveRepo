// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseWidgetClass.h"
#include "TextWidget.generated.h"

/**
 * 
 */
UCLASS()
class SKYCITYFUGITIVE_API UTextWidget : public UBaseWidgetClass
{
	GENERATED_BODY()
public:



    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString text;





};
