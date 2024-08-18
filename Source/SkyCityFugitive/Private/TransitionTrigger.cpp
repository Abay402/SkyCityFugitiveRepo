// Fill out your copyright notice in the Description page of Project Settings.


#include "TransitionTrigger.h"
#include "Player2D.h"
#include "MainGameInstance.h"
#include "Kismet/GameplayStatics.h"
// Sets default values
ATransitionTrigger::ATransitionTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    // Create the box component
    box = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
    // Set this actor as the collision handler
    box->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    box->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
    // Bind the overlap events
    box->OnComponentBeginOverlap.AddDynamic(this, &ATransitionTrigger::OnOverlapBegin);
    box->OnComponentEndOverlap.AddDynamic(this, &ATransitionTrigger::OnOverlapEnd);
    // Attach the trigger box to the root component
    RootComponent = box;
	widgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	widgetComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ATransitionTrigger::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATransitionTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATransitionTrigger::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (OtherActor && (OtherActor != this))
	{
		
		if (OtherActor->IsA<APlayer2D>())
		{
			UFadeEffectWidget* letterWidget = Cast<UFadeEffectWidget>(widgetComponent->GetWidget());
			letterWidget->PlayAnimationEffect(EUMGSequencePlayMode::Forward);
			APlayer2D* player = Cast<APlayer2D>(OtherActor);
			player->nearTransitionTrigger = true;
			UMainGameInstance* gameInstance = Cast<UMainGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
			gameInstance->lastLevel = targetLevel;
			gameInstance->spawnLocation = spawnLocation;
			
		}
		
	}


}

void ATransitionTrigger::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

	if (OtherActor && (OtherActor != this))
	{
	
		if (OtherActor->IsA<APlayer2D>())
		{
			UFadeEffectWidget* letterWidget = Cast<UFadeEffectWidget>(widgetComponent->GetWidget());
			if (letterWidget) letterWidget->PlayAnimationEffect(EUMGSequencePlayMode::Reverse);
			APlayer2D* player = Cast<APlayer2D>(OtherActor);
			player->nearTransitionTrigger = false;
		
		}
	
	}

}

