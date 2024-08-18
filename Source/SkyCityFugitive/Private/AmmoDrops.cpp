// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmoDrops.h"

AAmmoDrops::AAmmoDrops()
{
	box = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	box->SetupAttachment(RootComponent);

	ammoAmount = 0;
}

void AAmmoDrops::BeginPlay()
{

	Super::BeginPlay();
	int randomNumber = FMath::RandRange(1, 10);
	ammoAmount = randomNumber;


}

void AAmmoDrops::Tick(float deltaTime)
{

	Super::Tick(deltaTime);

}
