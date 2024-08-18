// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemyAIController.h"
#include "Player2D.h"

ABaseEnemyAIController::ABaseEnemyAIController()
{

    PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));
    SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));

    //SetPerceptionComponent(*PerceptionComponent);

    PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ABaseEnemyAIController::OnTargetPerceptionUpdated);
  

}

void ABaseEnemyAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{

    if (Actor->IsA<APlayer2D>()) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, "Seen");


    if (Actor && Actor != GetPawn()) // Check if the actor is valid and not self
    {

        // Print whatever you want here or trigger any other action
    }
}

void ABaseEnemyAIController::BeginPlay()
{

	Super::BeginPlay();
    // Set up config for sight
    if (PerceptionComponent)
    {

        SightConfig->SightRadius = 300.0f; // Set your desired sight radius
        SightConfig->LoseSightRadius = 425.0f;
        SightConfig->DetectionByAffiliation.bDetectEnemies = true;
        SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
        SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
        
        PerceptionComponent->ConfigureSense(*SightConfig);
        PerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());
    }


}

void ABaseEnemyAIController::OnPossess(APawn* InPawn)
{

    Super::OnPossess(InPawn);

}
