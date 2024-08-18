// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPC2D.h"
#include "Components/CapsuleComponent.h"
#include "CharacterStats.h"
#include "Components/WidgetComponent.h"
#include "PathMarker.h"
#include "Gun2D.h"
#include "HealthWidget.h"
#include "Boss2D.generated.h"

/**
 *
 */
UCLASS()
class SKYCITYFUGITIVE_API ABoss2D : public ANPC2D
{
    GENERATED_BODY()

public:

    ABoss2D();

    UFUNCTION(BlueprintCallable)
    void CheckForOverlap(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


    UFUNCTION(BlueprintCallable)
    void CheckForPlayerActivation(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


    UFUNCTION(BlueprintCallable)
    void EndOverlap(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    UFUNCTION(BlueprintCallable)
    void MoveTo(FVector dir_);

    UFUNCTION(BlueprintCallable)
    bool CheckForPlatform(AActor* target_);

    UFUNCTION(BlueprintCallable)
    bool CheckIfCanAttack(AActor* attackTarget_);

    UFUNCTION(BlueprintCallable)
    void Attack();

    UFUNCTION(BlueprintCallable)
    void RandomMovement(FVector targetDir_);

    UFUNCTION(BlueprintCallable)
    void WaitAnimation();


    UFUNCTION(BlueprintCallable)
    void WaitAnimationEnd();

    UFUNCTION(BlueprintCallable)
    AActor* CalculateTarget();

    UFUNCTION(BlueprintCallable)
    void Deactivate();

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UWidgetComponent* widgetComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<UHealthWidget> healthWidgetClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<APathMarker*> pathMarkers;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int pathMarkerIndex;


    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int deathEventTrigger;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool pathSet;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool canAttack;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool attackFinish;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool aiActive;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool isChasing;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool waitAfterAttack;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool moveOver;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool waitOver;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool calculatingStates;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<AGun2D> gunClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ChildActor)
    UChildActorComponent* gunLocation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    AGun2D* gun;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, UPaperFlipbook*> sprites;

    void SetSprite(const char* sprite_);


    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString name;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float aggroRadius;

    FTimerHandle moveTimer;
    FTimerHandle waitTimer;
protected:

    virtual void BeginPlay() override;
    virtual void Tick(float deltaTime) override;

private:
    void BossDeath();

    CharacterStats* stats;

    FString GetCurrentLevelName();

    bool isDead;

    FVector newScale;
    FVector scale;

    int lightAttackIndex;

    float rayDetectionXOffset;

    void LightAttack();
    void ChargeAttack();
    TArray<void(ABoss2D::*)()> attackFunctions;

    void AttackEnd();
    void CanAttack();

    void DeathAnimEnd();

    void MoveEnd();

    void Activate();

   

    FTimerHandle attackEndTimer;
    FTimerHandle canAttackTimer;
    FTimerHandle deathTimer;

    bool isWalking;
    bool isWaiting;
    bool dialogueOver;

    UHealthWidget* healthWidget;


    int randomVel;
};