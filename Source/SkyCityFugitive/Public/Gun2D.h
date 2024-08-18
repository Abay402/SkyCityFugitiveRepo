// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperFlipbookActor.h"
#include "Components/ChildActorComponent.h"
#include "Components/BoxComponent.h"
#include "PaperFlipbookComponent.h"
#include "Bullet2D.h"
#include "Gun2D.generated.h"

/**
 * 
 */
UCLASS()
class SKYCITYFUGITIVE_API AGun2D : public APaperFlipbookActor
{
	GENERATED_BODY()

public:

	AGun2D();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Collision)
		UBoxComponent* box;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ChildActor)
		UChildActorComponent* bulletExitLocation;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<ABullet2D> bulletClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ChildActor)
		bool testBool;
	void ShootBullet();

	void ChargeShot();
	void DecreaseCurrentMagAmmo(int ammoDecrease_);

	void Reload();

	UFUNCTION(BlueprintCallable)
		void Test();
	void ReloadStopAnimations();
	void ReloadEnd();
	void ShootingEnd();
	void ChargeEnd();


	struct AmmoStats* ammoStats;
	FVector newScale;
	FVector scale;
	FTimerHandle reloadTimer;
	FTimerHandle shootingTimer;
	FTimerHandle chargeTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FString, UPaperFlipbook*> sprites;
	
	bool isShooting;

	void SetSprite(const char* sprite_);


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool enemyGun;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundWave* reloadSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundWave* gunCock;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void Tick(float deltaTime) override;
};

struct AmmoStats
{
private:
	int maxAmmo;
	int currentMagAmmo;
	int currentTotalAmmo;
	int magSize;
	bool isReloading;

public:

	AmmoStats();
	AmmoStats(int maxAmmo_, int currentTotalAmmo_, int magSize_);

	//Setters for ship ammo stats
	void SetMaxAmmo(int maxAmmo_) { maxAmmo = maxAmmo_; }
	void SetCurrentTotalAmmo(int currentTotalAmmo_);
	void DecreaseCurrentMagAmmo(int currentMagAmmo_);
	void SetMagSize(int magSize_) { magSize = magSize_; }
	void SetCurrentMagAmmo(int currentMagAmmo_);
	//Check if gun is reloading
	bool IsReloading() { return isReloading; }
	void SetIsReloading(bool isReloading_) { isReloading = isReloading_; }
	//Getters for ammo stats
	int GetMaxAmmo() { return maxAmmo; }
	int GetMagSize() { return magSize; }
	int GetCurrentTotalAmmo() { return currentTotalAmmo; }
	int GetCurrentMagAmmo() { return currentMagAmmo; }

	//Increase total ammo
	void IncreaseCurrentTotalAmmo(int ammoIncrease_);


	//Initialize the ammo and calculate mag size 
	void InitializeAmmo();

};