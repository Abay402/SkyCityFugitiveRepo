// Fill out your copyright notice in the Description page of Project Settings.


#include "PatrolMarker.h"

// Sets default values
APatrolMarker::APatrolMarker()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APatrolMarker::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APatrolMarker::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

