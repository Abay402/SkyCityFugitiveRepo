// Fill out your copyright notice in the Description page of Project Settings.


#include "TextTrigger.h"
#include "Player2D.h"
#include "MainGameInstance.h"
#include "Kismet/GameplayStatics.h"
// Sets default values
ATextTrigger::ATextTrigger()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// Create the box component
	box = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	// Set this actor as the collision handler
	box->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	box->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	// Bind the overlap events
	box->OnComponentBeginOverlap.AddDynamic(this, &ATextTrigger::OnOverlapBegin);
	box->OnComponentEndOverlap.AddDynamic(this, &ATextTrigger::OnOverlapEnd);
	// Attach the trigger box to the root component
	RootComponent = box;
}

// Called when the game starts or when spawned
void ATextTrigger::BeginPlay()
{
	Super::BeginPlay();

	textWidget = CreateWidget<UTextWidget>(GetWorld(), textWidgetClass);

	if (textWidget)
	{
		textWidget->text = text;
		if (autoActivate) textWidget->AddToViewport();
	}

}

// Called every frame
void ATextTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATextTrigger::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (OtherActor && (OtherActor != this))
	{

		if (OtherActor->IsA<APlayer2D>())
		{
			if (textWidget)
			{
				textWidget->AddToViewport();
				//textWidget->PlayAnimationEffect(EUMGSequencePlayMode::Forward);
			}
		}

	}


}

void ATextTrigger::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

	if (OtherActor && (OtherActor != this))
	{

		if (OtherActor->IsA<APlayer2D>())
		{
			if (textWidget)
			{
				textWidget->RemoveFromViewport();
				//textWidget->PlayAnimationEffect(EUMGSequencePlayMode::Reverse);
			}

		}

	}

}

