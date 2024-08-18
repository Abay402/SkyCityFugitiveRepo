// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperFlipbookActor.h"
#include "PaperFlipbook.h"
#include "Components/BoxComponent.h"
#include "PaperFlipbookComponent.h"
#include "TimerManager.h"
#include "Building2D.generated.h"

/**
 * 
 */
UCLASS()
class SKYCITYFUGITIVE_API ABuilding2D : public APaperFlipbookActor
{
	GENERATED_BODY()
public: 

	ABuilding2D();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Collision)
	UBoxComponent* box;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flipbook")
	UPaperFlipbookComponent* exteriorFB;

	UFUNCTION(BlueprintCallable)
	void CheckForOverlap(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION(BlueprintCallable)
	void EndOverlap(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	FTimerHandle opacityTimer;

	void ChangeOpacity();

	bool noOpacity;

	UMaterialInstanceDynamic* opacityMaterial;

	float opacity;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void Tick(float deltaTime) override;
	
};
