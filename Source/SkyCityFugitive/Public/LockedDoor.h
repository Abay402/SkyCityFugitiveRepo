// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SwitchableActor2D.h"
#include "Components/BoxComponent.h"
#include "PaperFlipbookComponent.h"
#include "TimerManager.h"
#include "LockedDoor.generated.h"

/**
 * 
 */
UCLASS()
class SKYCITYFUGITIVE_API ALockedDoor : public ASwitchableActor2D
{
	GENERATED_BODY()
public:

	ALockedDoor();

	void Activate() override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperFlipbook* openAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperFlipbook* closeAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperFlipbook* openIdle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool noSwitch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int eventTrigger;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperFlipbook* closeIdle;

	FTimerHandle openTimer;

	FTimerHandle closeTimer;

	void OpenFinish();
	void CloseFinish();

	bool isClosed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAudioComponent* audioComponent;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void Tick(float deltaTime) override;
};
