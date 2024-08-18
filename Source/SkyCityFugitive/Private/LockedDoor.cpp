// Fill out your copyright notice in the Description page of Project Settings.


#include "LockedDoor.h"
#include "MainGameInstance.h"
ALockedDoor::ALockedDoor()
{
	isClosed = true;
	audioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	audioComponent->bAutoActivate = false;  // Don't play sound on start
	audioComponent->SetupAttachment(RootComponent);
}

void ALockedDoor::Activate()
{

	if (isClosed)
	{
		GetRenderComponent()->SetFlipbook(openAnim);
		GetWorld()->GetTimerManager().SetTimer(openTimer, this, &ALockedDoor::OpenFinish, GetRenderComponent()->GetFlipbookLength() - 0.1f, false);
	}
	else
	{

		GetRenderComponent()->SetFlipbook(closeAnim);
		GetWorld()->GetTimerManager().SetTimer(closeTimer, this, &ALockedDoor::CloseFinish, GetRenderComponent()->GetFlipbookLength() - 0.1f, false);

	}

	audioComponent->Play();
	if (noSwitch)
	{

		UMainGameInstance* gameInstance = Cast<UMainGameInstance>(GetGameInstance());
		gameInstance->triggeredEvents.Add(eventTrigger, true);

	}
}

void ALockedDoor::OpenFinish()
{


	GetRenderComponent()->SetFlipbook(openIdle);
	isClosed = false;
	box->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	audioComponent->Stop();
}

void ALockedDoor::CloseFinish()
{

	GetRenderComponent()->SetFlipbook(closeIdle);
	isClosed = true;
	box->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	audioComponent->Stop();
}

void ALockedDoor::BeginPlay()
{
	Super::BeginPlay();

	if (noSwitch)
	{

		UMainGameInstance* gameInstance = Cast<UMainGameInstance>(GetGameInstance());
		if (gameInstance && gameInstance->eventManager)
		{
			gameInstance->eventManager->SubscribeToEvent(eventTrigger, FEventManager::CreateUObject(this, &ALockedDoor::Activate));
		}


		if (gameInstance->triggeredEvents.Contains(eventTrigger))
		{

			if (*gameInstance->triggeredEvents.Find(eventTrigger) == true)
			{

				Activate();

			}

		}


	}
}

void ALockedDoor::Tick(float deltaTime)
{

	Super::Tick(deltaTime);

}
