// Fill out your copyright notice in the Description page of Project Settings.


#include "Switch.h"
#include "MainGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "SwitchableActor2D.h"
ASwitch::ASwitch()
{

	box = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	box->SetupAttachment(RootComponent);


}

void ASwitch::Activate()
{

	if (eventTrigger == 0)
	{

		if (parent)
		{
			ASwitchableActor2D* switchableActor = Cast<ASwitchableActor2D>(parent);
			switchableActor->Activate();
		
		}
	}
	else
	{


		UMainGameInstance* gameInstance = Cast<UMainGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
		gameInstance->eventManager->TriggerEvent(eventTrigger);
		if (activateOnce) Destroy();

	}



}

void ASwitch::BeginPlay()
{
	Super::BeginPlay();

	if (GetAttachParentActor() != nullptr) parent = GetAttachParentActor();

}

void ASwitch::Tick(float deltaTime)
{
	Super::Tick(deltaTime);
}
