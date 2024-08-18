// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperSpriteActor.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "TimerManager.h"
#include "Sound/SoundBase.h"
#include "Components/AudioComponent.h"
#include "Bullet2D.generated.h"

/**
 * 
 */
UCLASS()
class SKYCITYFUGITIVE_API ABullet2D : public APaperSpriteActor
{
	GENERATED_BODY()
	
public:

	ABullet2D();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Collision)
		UBoxComponent* box;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ProjectileMovement)
		UProjectileMovementComponent* projectileMovement;
		
	UFUNCTION()
		void OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USoundWave* bulletSound;

	FTimerHandle deathTimer;

	void Despawn();


protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void Tick(float deltaTime) override;
};
