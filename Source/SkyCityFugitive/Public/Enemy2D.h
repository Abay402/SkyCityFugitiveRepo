// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPC2D.h"
#include "Components/CapsuleComponent.h"
#include "CharacterStats.h"
#include "Components/WidgetComponent.h"
#include "PathMarker.h"
#include "Gun2D.h"
#include "AmmoDrops.h"
#include "Enemy2D.generated.h"

/**
 * 
 */
UCLASS()
class SKYCITYFUGITIVE_API AEnemy2D : public ANPC2D
{
	GENERATED_BODY()
	
public:

    AEnemy2D();

    UFUNCTION(BlueprintCallable)
    void CheckForOverlap(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION(BlueprintCallable)
    void EndOverlap(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    UFUNCTION(BlueprintCallable)
    void MoveTo(FVector dir_);

    UFUNCTION(BlueprintCallable)
    bool CheckForGround();

    UFUNCTION(BlueprintCallable)
    bool CheckForJump();

    UFUNCTION(BlueprintCallable)
    bool CheckForPlatform(AActor* target_);

    UFUNCTION(BlueprintCallable)
    AActor* CalculatePathTarget();

    UFUNCTION(BlueprintCallable)
    bool CheckIfAtPathMarker(AActor* targetPathMarker_);


    UFUNCTION(BlueprintCallable)
    bool CheckIfCanAttack(AActor* attackTarget_);

    UFUNCTION(BlueprintCallable)
    void Attack();


    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<AAmmoDrops> ammoDropClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UWidgetComponent* widgetComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<APathMarker*> pathMarkers;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int pathMarkerIndex;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool pathSet;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool canAttack;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool canSee;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool attackFinish;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool aiActive;


    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool isChasing;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    APathMarker* targetPathMarker;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<AGun2D> gunClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<AGun2D> playerGunClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ChildActor)
    UChildActorComponent* gunLocation;


    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    AGun2D* gun;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    USoundWave* deathSound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, UPaperFlipbook*> sprites;

    void SetSprite(const char* sprite_);

    CharacterStats* stats;
protected:

    virtual void BeginPlay() override;
    virtual void Tick(float deltaTime) override;
#if WITH_EDITOR

    virtual void OnConstruction(const FTransform& transform_) override;
#endif

private:
    void EnemyDeath();

    bool bTagAndLevelWritten;

    FString GenerateRandomTag();

    
    void SetActorTag(const FString& newTag_);
    FString GetCurrentLevelName();

    bool isDead;

    void Test();
    FVector newScale;

    FVector scale;
    int lightAttackIndex;

    float rayDetectionXOffset;

    void LightAttack();
    void ChargeAttack();
    TArray<void(AEnemy2D::*)()> attackFunctions;

    void AttackEnd();
    void CanAttack();

    void DeathAnimEnd();

    FTimerHandle attackEndTimer;
    FTimerHandle canAttackTimer;
    FTimerHandle deathTimer;
    bool isWalking;
};