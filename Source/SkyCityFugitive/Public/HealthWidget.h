// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseWidgetClass.h"

#include "HealthWidget.generated.h"

/**
 * 
 */
UCLASS()
class SKYCITYFUGITIVE_API UHealthWidget : public UBaseWidgetClass
{
	GENERATED_BODY()


public:

	UHealthWidget();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float maxHealth;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString name;
};
