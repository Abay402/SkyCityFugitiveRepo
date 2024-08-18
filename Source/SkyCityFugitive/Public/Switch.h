// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperFlipbookActor.h"
#include "Components/BoxComponent.h"
#include "Switch.generated.h"

/**
 * 
 */
UCLASS()
class SKYCITYFUGITIVE_API ASwitch : public APaperFlipbookActor
{
	GENERATED_BODY()
public:

	ASwitch();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Collision)
	UBoxComponent* box;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool activateOnce;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int eventTrigger;

	AActor* parent;

	void Activate();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void Tick(float deltaTime) override;
};
