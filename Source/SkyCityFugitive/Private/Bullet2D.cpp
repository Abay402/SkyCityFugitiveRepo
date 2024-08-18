// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet2D.h"
#include "Engine/Engine.h"
#include "Player2D.h"
#include "Gun2D.h"
#include "Building2D.h"
#include "AmmoDrops.h"
#include "RechargeStation.h"
#include "Kismet/GameplayStatics.h"
ABullet2D::ABullet2D()
{


	box = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	box->SetupAttachment(RootComponent);

	projectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	projectileMovement->SetPlaneConstraintEnabled(true);
	projectileMovement->SetPlaneConstraintNormal(FVector(0.0f, 1.0f, 0.0f));
	projectileMovement->InitialSpeed = 4000.f;
	projectileMovement->MaxSpeed = 4000.f;
	projectileMovement->bRotationFollowsVelocity = true;


}

void ABullet2D::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	// Check if the OtherActor is valid and not this actor
	if (OtherActor != nullptr && OtherActor != this)
	{

		// Check if the OtherActor is a wall or ground (without a specific tag)
		if (OtherActor->ActorHasTag("Wall"))
		{
			// Destroy the projectile
			Destroy();
		}
		// Else, it's a platform, so do nothing and let the projectile pass through
	}


}

void ABullet2D::Despawn()
{

	this->Destroy();

}

void ABullet2D::BeginPlay()
{
	Super::BeginPlay();

	box->OnComponentBeginOverlap.AddDynamic(this, &ABullet2D::OverlapBegin);

	GetWorld()->GetTimerManager().SetTimer(deathTimer, this, &ABullet2D::Despawn, 4.0f, false);
	if (bulletSound)
	{
		// Create an audio component and attach the sound wave
		UAudioComponent* AudioComponent = UGameplayStatics::SpawnSoundAttached(bulletSound, GetRootComponent());
		if (AudioComponent)
		{
			// Play the audio component
			AudioComponent->VolumeMultiplier = 1;
			AudioComponent->Play();
		}
	}



}

void ABullet2D::Tick(float deltaTime)
{
	Super::Tick(deltaTime);


}
