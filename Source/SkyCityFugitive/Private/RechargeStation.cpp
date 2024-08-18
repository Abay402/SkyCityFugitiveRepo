// Fill out your copyright notice in the Description page of Project Settings.


#include "RechargeStation.h"
#include "Player2D.h"


ARechargeStation::ARechargeStation()
{

	box = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	box->SetupAttachment(RootComponent);
	widgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	widgetComponent->SetupAttachment(RootComponent);

	audioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	audioComponent->bAutoActivate = false;  // Don't play sound on start
	audioComponent->SetupAttachment(RootComponent);

}

void ARechargeStation::CheckForOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this))
	{
		if (!playerNear)
		{
			if (OtherActor->IsA<APlayer2D>())
			{
				UFadeEffectWidget* letterWidget = Cast<UFadeEffectWidget>(widgetComponent->GetWidget());
				letterWidget->PlayAnimationEffect(EUMGSequencePlayMode::Forward);
				GetRenderComponent()->SetFlipbook(*sprites.Find("Opening"));
				GetWorldTimerManager().SetTimer(animTimer, this, &ARechargeStation::Open, GetRenderComponent()->GetFlipbookLength() - 0.1f, false);
				audioComponent->Play();
				APlayer2D* player = Cast<APlayer2D>(OtherActor);
				player->nearRechargeStation = true;
				player->rechargeStation = this;
				playerNear = true;
			}
		}
	}
}

void ARechargeStation::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{


	if (OtherActor && (OtherActor != this))
	{
		if (playerNear)
		{


			if (OtherActor->IsA<APlayer2D>())
			{

				UFadeEffectWidget* letterWidget = Cast<UFadeEffectWidget>(widgetComponent->GetWidget());
				if (letterWidget) letterWidget->PlayAnimationEffect(EUMGSequencePlayMode::Reverse);
				GetRenderComponent()->SetFlipbook(*sprites.Find("Closing"));
				GetWorldTimerManager().SetTimer(animTimer, this, &ARechargeStation::Close, GetRenderComponent()->GetFlipbookLength() - 0.1f, false);
				audioComponent->FadeOut(1.0f,0.0f);

				APlayer2D* player = Cast<APlayer2D>(OtherActor);
				player->nearRechargeStation = false;
				playerNear = false;
			}
		}
	}

}

void ARechargeStation::Open()
{


	GetRenderComponent()->SetFlipbook(*sprites.Find("OpenIdle"));

}

void ARechargeStation::Close()
{

	GetRenderComponent()->SetFlipbook(*sprites.Find("CloseIdle"));

}

void ARechargeStation::BeginPlay()
{

	Super::BeginPlay();
	box->OnComponentBeginOverlap.AddDynamic(this, &ARechargeStation::CheckForOverlap);
	box->OnComponentEndOverlap.AddDynamic(this, &ARechargeStation::EndOverlap);

}
