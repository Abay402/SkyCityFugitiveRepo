// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperFlipbookActor.h"
#include "Components/BoxComponent.h"
#include "SwitchableActor2D.generated.h"

/**
 * 
 */
UCLASS()
class SKYCITYFUGITIVE_API ASwitchableActor2D : public APaperFlipbookActor
{
	GENERATED_BODY()
public:

	ASwitchableActor2D();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Collision)
	UBoxComponent* box;

	virtual void Activate() PURE_VIRTUAL(ASwitchableActor2D::Activate, );

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void Tick(float deltaTime) override;
};
