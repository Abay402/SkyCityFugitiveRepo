// Fill out your copyright notice in the Description page of Project Settings.


#include "SwitchableActor2D.h"

ASwitchableActor2D::ASwitchableActor2D()
{
	box = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	box->SetupAttachment(RootComponent);

}

void ASwitchableActor2D::BeginPlay()
{

	Super::BeginPlay();
}

void ASwitchableActor2D::Tick(float deltaTime)
{

	Super::Tick(deltaTime);

}
