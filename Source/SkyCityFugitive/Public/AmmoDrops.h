// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperFlipbookActor.h"
#include "Components/BoxComponent.h"
#include "AmmoDrops.generated.h"

/**
 * 
 */
UCLASS()
class SKYCITYFUGITIVE_API AAmmoDrops : public APaperFlipbookActor
{
	GENERATED_BODY()

public:

	AAmmoDrops();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Collision)
	UBoxComponent* box;

	int ammoAmount;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void Tick(float deltaTime) override;
};
