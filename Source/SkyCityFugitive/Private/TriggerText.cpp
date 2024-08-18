// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerText.h"
#include "Player2D.h"
#include "MainGameInstance.h"
#include "Kismet/GameplayStatics.h"
// Sets default values
ATriggerText::ATriggerText()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// Create the box component
	box = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	// Set this actor as the collision handler
	box->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	box->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	// Bind the overlap events
	box->OnComponentBeginOverlap.AddDynamic(this, &ATriggerText::OnOverlapBegin);
	box->OnComponentEndOverlap.AddDynamic(this, &ATriggerText::OnOverlapEnd);
	// Attach the trigger box to the root component
	RootComponent = box;

	widgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	widgetComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ATriggerText::BeginPlay()
{
	Super::BeginPlay();

	UTextWidget* letterWidget = Cast<UTextWidget>(widgetComponent->GetWidget());
	letterWidget->text = text;
	if (autoActivate)
	{
		letterWidget->AddToViewport();
	}
}

// Called every frame
void ATriggerText::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATriggerText::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (OtherActor && (OtherActor != this))
	{

		if (OtherActor->IsA<APlayer2D>())
		{
			UTextWidget* letterWidget = Cast<UTextWidget>(widgetComponent->GetWidget());
			letterWidget->AddToViewport();
		}

	}


}

void ATriggerText::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

	if (OtherActor && OtherActor != this)
	{

		if (OtherActor->IsA<APlayer2D>())
		{
			UTextWidget* letterWidget = Cast<UTextWidget>(widgetComponent->GetWidget());
			if (letterWidget)letterWidget->RemoveFromViewport();

		}

	}

}