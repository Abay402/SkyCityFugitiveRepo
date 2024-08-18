// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseWidgetClass.h"
#include "UIPlayer.generated.h"

/**
 * 
 */
UCLASS()
class SKYCITYFUGITIVE_API UUIPlayer : public UBaseWidgetClass
{
	GENERATED_BODY()
public:
	UUIPlayer();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float maxHealth;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ammo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int totalAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int stims;
};
