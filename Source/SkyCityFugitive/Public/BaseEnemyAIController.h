// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "BaseEnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class SKYCITYFUGITIVE_API ABaseEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
    ABaseEnemyAIController();


    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        UAISenseConfig_Sight* SightConfig;

    UFUNCTION()
        void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

protected:
    virtual void BeginPlay() override;
    virtual void OnPossess(APawn* InPawn) override;

};
