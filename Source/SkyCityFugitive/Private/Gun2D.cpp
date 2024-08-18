// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun2D.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "PaperFlipbook.h"
#include "MainGameInstance.h"
AGun2D::AGun2D()
{

	box = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	box->SetupAttachment(RootComponent);

	bulletExitLocation = CreateDefaultSubobject<UChildActorComponent>(TEXT("ChildActor"));
	bulletExitLocation->SetupAttachment(RootComponent);

	ammoStats = new AmmoStats(50, 50, 10);

}

void AGun2D::ShootBullet()
{
    if (bulletClass)
    {

		SetSprite("Shooting");
        FVector2D mousePosition;
        UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetMousePosition(mousePosition.X, mousePosition.Y);

        // Convert screen space to world space
        FVector worldLocation, worldDirection;
        UGameplayStatics::GetPlayerController(GetWorld(), 0)->DeprojectScreenPositionToWorld(mousePosition.X, mousePosition.Y, worldLocation, worldDirection);

        // Calculate rotation
		FRotator rotation; 
		
		rotation = GetActorRotation();
		//rotation.Normalize();
		if (newScale.X < 0)
		{
			FVector actorDirection = GetActorForwardVector();
			actorDirection.X = -actorDirection.X;
			actorDirection.Z = -actorDirection.Z;
			rotation = FRotationMatrix::MakeFromX(actorDirection).Rotator();
		}
        ABullet2D* newBullet = GetWorld()->SpawnActor<ABullet2D>(bulletClass, FVector(bulletExitLocation->GetComponentLocation().X, bulletExitLocation->GetComponentLocation().Y, bulletExitLocation->GetComponentLocation().Z - 5), rotation);
   

		float time = GetRenderComponent()->GetFlipbookLength();
		GetWorld()->GetTimerManager().SetTimer(shootingTimer, this, &AGun2D::ShootingEnd, time - 0.25f, false);
		if (!enemyGun)DecreaseCurrentMagAmmo(1);
		isShooting = true;
    }
}

void AGun2D::SetSprite(const char* sprite_)
{

	GetRenderComponent()->SetFlipbook(*sprites.Find(sprite_));

}

void AGun2D::BeginPlay()
{

	Super::BeginPlay();

	UMainGameInstance* gameInstance = Cast<UMainGameInstance>(GetGameInstance());


	if (gameInstance && gameInstance->eventManager && testBool)
	{
		gameInstance->eventManager->SubscribeToEvent(5, FEventManager::CreateUObject(this, &AGun2D::Test));
	}

	//if (gameInstance->playerGun) Destroy();


    scale = GetRenderComponent()->GetRelativeScale3D();
    newScale = scale;
}

void AGun2D::Tick(float deltaTime)
{
    Super::Tick(deltaTime);
    GetRenderComponent()->SetRelativeScale3D(newScale);

}


void AGun2D::ChargeShot()
{
	if (bulletClass)
	{

		SetSprite("Charged");

		float chargeTime = GetRenderComponent()->GetFlipbookLength();
		GetWorld()->GetTimerManager().SetTimer(chargeTimer, this, &AGun2D::ChargeEnd, chargeTime - 0.5f, false);

		float time = GetRenderComponent()->GetFlipbookLength();
		GetWorld()->GetTimerManager().SetTimer(shootingTimer, this, &AGun2D::ShootingEnd, time, false);
		if (!enemyGun)DecreaseCurrentMagAmmo(1);
		isShooting = true;
	}


}

void AGun2D::DecreaseCurrentMagAmmo(int ammoDecrease_)
{

	ammoStats->DecreaseCurrentMagAmmo(ammoDecrease_);

	
}

void AGun2D::Reload()
{

	ammoStats->SetIsReloading(true);
	SetSprite("Reloading");
	GetRenderComponent()->SetLooping(false);
	GetRenderComponent()->PlayFromStart();
	GetWorld()->GetTimerManager().SetTimer(reloadTimer, this, &AGun2D::ReloadEnd, GetRenderComponent()->GetFlipbookLength(), false);
	if (reloadSound)
	{
		// Create an audio component and attach the sound wave
		UAudioComponent* AudioComponent = UGameplayStatics::SpawnSoundAttached(reloadSound, GetRootComponent());
		if (AudioComponent)
		{
			// Play the audio component
	
			AudioComponent->Play();
		}
	}
}

void AGun2D::Test()
{

	
}

void AGun2D::ReloadStopAnimations()
{
	ammoStats->SetIsReloading(false);
	SetSprite("IdleWithArms");
	GetRenderComponent()->SetLooping(true);
	GetRenderComponent()->PlayFromStart();
}

void AGun2D::ReloadEnd()
{
	//GetWorldTimerManager().ClearTimer(reloadTimer);
	ammoStats->InitializeAmmo();
	ReloadStopAnimations();
	if (gunCock)
	{
		// Create an audio component and attach the sound wave
		UAudioComponent* AudioComponent = UGameplayStatics::SpawnSoundAttached(gunCock, GetRootComponent());
		if (AudioComponent)
		{
			// Play the audio component

			AudioComponent->Play();
		}
	}
}

void AGun2D::ShootingEnd()
{
	if (!ammoStats->IsReloading())SetSprite("IdleWithArms");
	isShooting = false;
}

void AGun2D::ChargeEnd()
{

	FVector2D mousePosition;
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetMousePosition(mousePosition.X, mousePosition.Y);

	// Convert screen space to world space
	FVector worldLocation, worldDirection;
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->DeprojectScreenPositionToWorld(mousePosition.X, mousePosition.Y, worldLocation, worldDirection);

	// Calculate rotation
	FRotator rotation;

	rotation = GetActorRotation();
	//rotation.Normalize();
	if (newScale.X < 0)
	{
		FVector actorDirection = GetActorForwardVector();
		actorDirection.X = -actorDirection.X;
		actorDirection.Z = -actorDirection.Z;
		rotation = FRotationMatrix::MakeFromX(actorDirection).Rotator();
	}

	ABullet2D* newBullet = GetWorld()->SpawnActor<ABullet2D>(bulletClass, FVector(bulletExitLocation->GetComponentLocation().X, bulletExitLocation->GetComponentLocation().Y, bulletExitLocation->GetComponentLocation().Z - 5), rotation);



}



AmmoStats::AmmoStats()
{

	maxAmmo = 0;
	currentTotalAmmo = 0;
	currentMagAmmo = 0;
	isReloading = false;
	magSize = 0;
}

AmmoStats::AmmoStats(int maxAmmo_, int currentTotalAmmo_, int magSize_)
{
	maxAmmo = maxAmmo_;
	currentTotalAmmo = currentTotalAmmo_;
	isReloading = false;
	magSize = magSize_;
	currentMagAmmo = 0;
	InitializeAmmo();

}

void AmmoStats::SetCurrentTotalAmmo(int currentTotalAmmo_)
{

	currentTotalAmmo = currentTotalAmmo_;
	if (currentTotalAmmo > maxAmmo) currentTotalAmmo = maxAmmo;
}
void AmmoStats::SetCurrentMagAmmo(int currentMagAmmo_)
{

	currentMagAmmo = currentMagAmmo_;
	if (currentMagAmmo > magSize) currentMagAmmo = magSize;
}
void AmmoStats::DecreaseCurrentMagAmmo(int currentMagAmmo_)
{

	currentMagAmmo -= currentMagAmmo_;
	if (currentMagAmmo < 0) currentMagAmmo = 0;
}

void AmmoStats::IncreaseCurrentTotalAmmo(int ammoIncrease_)
{

	currentTotalAmmo += ammoIncrease_;

	if (currentTotalAmmo > maxAmmo) currentTotalAmmo = maxAmmo;

}


void AmmoStats::InitializeAmmo()
{

	if (currentTotalAmmo > magSize)
	{
		if (currentMagAmmo == 0)
		{
			currentMagAmmo = magSize;
			currentTotalAmmo -= magSize;
		}
		else
		{

			int diff = magSize - currentMagAmmo;

			currentMagAmmo = magSize;

			currentTotalAmmo -= diff;
		}
	}
	else
	{
		if (currentMagAmmo == 0)
		{
			currentMagAmmo = currentTotalAmmo;
			currentTotalAmmo = 0;
		}
		else
		{

			int diff = magSize - currentMagAmmo;
			if (diff < currentTotalAmmo) currentMagAmmo += diff;
			else currentMagAmmo += currentTotalAmmo;
			currentTotalAmmo -= diff;
			if (currentTotalAmmo < 0) currentTotalAmmo = 0;
		}

	}

}