// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseActor2D.h"
#include "Components/BoxComponent.h"
#include "PaperFlipbookComponent.h"
#include "Components/WidgetComponent.h"
#include "TimerManager.h"
#include "RechargeStation.generated.h"

/**
 * 
 */
UCLASS()
class SKYCITYFUGITIVE_API ARechargeStation : public ABaseActor2D
{
	GENERATED_BODY()
private:

public:
	ARechargeStation();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Collision)
	UBoxComponent* box;

	UFUNCTION(BlueprintCallable)
	void CheckForOverlap(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	void EndOverlap(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FString, UPaperFlipbook*> sprites;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UWidgetComponent* widgetComponent;

	bool playerNear;
	FTimerHandle animTimer;
	void Open();
	void Close();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAudioComponent* audioComponent;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
};
