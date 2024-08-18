// Fill out your copyright notice in the Description page of Project Settings.
// Forward declaration of classes

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/ChildActorComponent.h"
#include "DialogueComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/ActorComponent.h"
#include "PaperFlipbookComponent.h"
#include "Gun2D.h"
#include "Containers/Map.h"
#include "CharacterStats.h"
#include "TimerManager.h"
#include "MainPlayerController.h"
#include "RechargeStation.h"
#include "UIPlayer.h"
#include "Player2D.generated.h"

/**
 * 
 */
UCLASS()
class SKYCITYFUGITIVE_API APlayer2D : public APaperCharacter
{
	GENERATED_BODY()
public:
	// Sets default values for this component's properties
	APlayer2D();

	UFUNCTION(BlueprintCallable)
		void Move(float axisValue);

	UFUNCTION(BlueprintCallable)
		void PlayerJump();

	UFUNCTION(BlueprintCallable)
		void Interact();
	
	UFUNCTION(BlueprintCallable)
		void ShootBullet();

	UFUNCTION(BlueprintCallable)
		void StartSprinting();

	UFUNCTION(BlueprintCallable)
		void StopSprinting();
		
	UFUNCTION(BlueprintCallable)
		void SwitchWeapon();

	UFUNCTION(BlueprintCallable)
		void Reload();

	UFUNCTION(BlueprintCallable)
		void Heal();

	UFUNCTION(BlueprintCallable)
		void Melee();

	UFUNCTION(BlueprintCallable)
		void MeleeEnd();

	UFUNCTION(BlueprintCallable)
		void Hurt();

	UFUNCTION(BlueprintCallable)
		void HurtEnd();

	UFUNCTION(BlueprintCallable)
		void HealEnd();

	UFUNCTION(BlueprintCallable)
		void DeathAnimEnd();

	UFUNCTION(BlueprintCallable)
		void SetSprite(UPaperFlipbook* sprite_);

	UFUNCTION(BlueprintCallable)
		int GetHealth();

	UFUNCTION(BlueprintCallable)
		FString GetAmmo();

	UFUNCTION(BlueprintCallable)
		void CheckForOverlap(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
		void EndOverlap(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	UFUNCTION(BlueprintCallable)
		void CheckForMeleeHit(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<FString, UPaperFlipbook*> sprites;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpringArm)
		USpringArmComponent* springArm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
		UCameraComponent* camera;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DialogueComponent)
		UDialogueComponent* dialogueComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ChildActor)
		UChildActorComponent* gunLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<UUIPlayer> playerWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float hSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AActor> crosshairClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AGun2D* gun;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Collision)
		UBoxComponent* meleeHB;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UAudioComponent* audioComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USoundWave* jumpSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USoundWave* healSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USoundWave* powerUpSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USoundWave* deathSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USoundWave* transitionSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USoundWave* dialogueSound;

	bool nearGun;
	bool nearRechargeStation;
	bool nearTransitionTrigger;

	bool gunActive;

	bool meleeActive;

	FVector newScale;

	FVector scale;

	FVector initialLocation;

	bool isMoving;
	bool isSprinting;
	bool isJumping;
	bool isDead;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool inDialogue;
	bool isHealing;
	bool isHurt;

	UUIPlayer* playerWidget;
	
	void AttachGunToPlayer();
	void ReplenishItems();
	void LoadLevel(FString level_);
	void ActivateRechargeStation();
	void PlayerDeath();

	void UpdateFade();

	void PlayerTakeDamage(float damage_);


	void AdvanceDialogue();

	CharacterStats* playerStats;

	ARechargeStation* rechargeStation;

	FTimerHandle transitionTimer;


	FTimerHandle redEfectTimer;

	FTimerHandle deathTimer;
	float fallTimer;

	void LightSwitch();

	FVector initialGunLocation;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Tick(float deltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
};
