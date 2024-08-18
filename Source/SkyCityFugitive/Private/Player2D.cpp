// Fill out your copyright notice in the Description page of Project Settings.


#include "Player2D.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/GameViewportClient.h"
#include "MainGameInstance.h"
#include "EnemyBullet.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PaperFlipbookComponent.h"
#include "PaperFlipbook.h"
#include "Switch.h"
#include "RechargeStation.h"
#include "Enemy2D.h"
#include "AmmoDrops.h"
#include "Components/PostProcessComponent.h"

APlayer2D::APlayer2D()
{

	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	springArm->SetupAttachment(RootComponent);

	meleeHB = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	meleeHB->SetupAttachment(RootComponent);

	springArm->SetRelativeRotation(FRotator(springArm->GetComponentRotation().Pitch, -90, springArm->GetComponentRotation().Roll));
	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	camera->SetupAttachment(springArm);
	camera->PostProcessSettings.AutoExposureMethod = EAutoExposureMethod::AEM_Manual;

	dialogueComponent = CreateDefaultSubobject<UDialogueComponent>(TEXT("DialogueComponent"));

	gunLocation = CreateDefaultSubobject<UChildActorComponent>(TEXT("ChildActor"));
	gunLocation->SetupAttachment(RootComponent);
	hSpeed = 2;

	audioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	audioComponent->bAutoActivate = false;  // Don't play sound on start
	audioComponent->SetupAttachment(RootComponent);
}

void APlayer2D::Move(float axisValue)
{
	if (inDialogue || isDead || meleeActive || isHealing || isHurt) return;
	if (axisValue == 0)
	{
		isMoving = false;

		if (!gunActive)
		{
			if (GetSprite()->GetFlipbook() != *sprites.Find("Idle") && !isJumping) SetSprite(*sprites.Find("Idle"));
		}

		if (gunActive)
		{
			if (GetSprite()->GetFlipbook() != *sprites.Find("IdleNoArms") && !isJumping && !gun->ammoStats->IsReloading()) SetSprite(*sprites.Find("IdleNoArms"));
		}
	}
	if (axisValue < 0)
	{
		isMoving = true;

	}
	if (axisValue > 0)
	{
		isMoving = true;

	}

	if (!isSprinting && axisValue != 0)
	{
		if (gunActive)
		{
			if (GetSprite()->GetFlipbook() != *sprites.Find("WalkingNoArms") && !isJumping) SetSprite(*sprites.Find("WalkingNoArms"));
		}
		else
		{
			if (GetSprite()->GetFlipbook() != *sprites.Find("Walking") && !isJumping) SetSprite(*sprites.Find("Walking"));
		}

	}

	if (isSprinting && axisValue != 0 && !isJumping)
	{
		SetSprite(*sprites.Find("Running"));
		if (gunActive)gun->GetRenderComponent()->SetVisibility(false);
	}

	if (isSprinting && axisValue != 0 && isJumping)
	{

		if (gunActive)gun->GetRenderComponent()->SetVisibility(true);
	}

	AddMovementInput(FVector(1.0f, 0.0f, 0.0f), (axisValue / 8) * hSpeed, false);

}

void APlayer2D::PlayerJump()
{
	if (inDialogue || isDead || meleeActive || isHealing || isHurt) return;

	Jump();
	if (!gunActive)SetSprite(*sprites.Find("Jump"));
	else SetSprite(*sprites.Find("JumpNoArms"));
	isJumping = true;
	if (jumpSound)
	{
		// Create an audio component and attach the sound wave
		UAudioComponent* AudioComponent = UGameplayStatics::SpawnSoundAttached(jumpSound, GetRootComponent());
		if (AudioComponent)
		{
			// Play the audio component
			AudioComponent->VolumeMultiplier = 1;
			AudioComponent->Play();
		}
	}
}

void APlayer2D::Interact()
{

	if (nearGun)
	{

		if (gun != nullptr)
		{
			UMainGameInstance* gameInstance = Cast<UMainGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

			if (UBlueprintGeneratedClass* GeneratedClass = Cast<UBlueprintGeneratedClass>(gun->GetClass()))
			{
				UObject* Outer = GeneratedClass->GetOuter();

				if (Outer)
				{
					FName PackageName = Outer->GetOutermost()->GetFName();
					gameInstance->playerGunClass = PackageName.ToString();

				}

			}
			gameInstance->pickedUpGun = true;
			AttachGunToPlayer();

			nearGun = false;
		}
	}

	if (nearRechargeStation)
	{

		ReplenishItems();
		ActivateRechargeStation();
	}

	if (nearTransitionTrigger)
	{

		UMainGameInstance* gameInstance = Cast<UMainGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
		gameInstance->spawnAtTrigger = true;
		AMainPlayerController* playerController = Cast<AMainPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		float endTransitionTimer = playerController->EndTransition();
		FTimerDelegate TimerDelegate;
		FName level = gameInstance->lastLevel;
		TimerDelegate.BindLambda([this, level]()
			{
				LoadLevel(level.ToString());
			});
		GetWorldTimerManager().SetTimer(transitionTimer, TimerDelegate, endTransitionTimer, false);
		if (transitionSound)
		{
			// Create an audio component and attach the sound wave
			UAudioComponent* AudioComponent = UGameplayStatics::SpawnSoundAttached(transitionSound, GetRootComponent());
			if (AudioComponent)
			{
				// Play the audio component
				AudioComponent->VolumeMultiplier = 1;
				AudioComponent->Play();
			}
		}
	}

	TArray<AActor*> OverlappingActors;
	GetCapsuleComponent()->GetOverlappingActors(OverlappingActors);
	for (AActor* Actor : OverlappingActors)
	{
		if (Actor && Actor != this)
		{
			if (Actor->IsA<ASwitch>())
			{


				ASwitch* switch_ = Cast<ASwitch>(Actor);
				switch_->Activate();

			}

		}
	}

}

void APlayer2D::ShootBullet()
{

	if (inDialogue || isDead || meleeActive || isHealing || isHurt) return;
	int status = isSprinting - isJumping;
	if (gunActive && status <= 0)
	{


		if (!gun->ammoStats->IsReloading() && !gun->isShooting)
		{
			if (gun->ammoStats->GetCurrentMagAmmo() == 0) Reload();
			else gun->ShootBullet();
			playerWidget->ammo = gun->ammoStats->GetCurrentMagAmmo();
			playerWidget->totalAmmo = gun->ammoStats->GetCurrentTotalAmmo();
		}
	}



}

void APlayer2D::StartSprinting()
{
	if (inDialogue || isDead || meleeActive || isHealing || isHurt) return;
	//SetSprite(*sprites.Find("Running"));
	hSpeed = 4;

	if (gunActive && !isJumping)
	{
		if (gun->ammoStats->IsReloading())
		{
			GetWorldTimerManager().ClearTimer(gun->reloadTimer);
			gun->ReloadStopAnimations();
		}
	}

	isSprinting = true;
}

void APlayer2D::StopSprinting()
{
	
	//if (!gunActive)SetSprite(*sprites.Find("Idle"));
	//if (gunActive)SetSprite(*sprites.Find("IdleNoArms"));
	hSpeed = 2;
	if (gunActive) 	gun->GetRenderComponent()->SetVisibility(true);
	isSprinting = false;
}

void APlayer2D::SwitchWeapon()
{
	if (inDialogue || isDead || meleeActive || isHealing || isHurt) return;
	if (gun)
	{
		if (gunActive)
		{
			if (gun->ammoStats->IsReloading())
			{
				GetWorldTimerManager().ClearTimer(gun->reloadTimer);
				gun->ReloadStopAnimations();
			}
			gun->GetRenderComponent()->SetVisibility(false);
			SetSprite(*sprites.Find("Idle"));
			gunActive = false;
			
		}
		else
		{
			SetSprite(*sprites.Find("IdleNoArms"));
			gun->GetRenderComponent()->SetVisibility(true);
			gunActive = true;

		}
	}
}

void APlayer2D::Reload()
{
	if (inDialogue || isDead || meleeActive || isHealing || isHurt) return;
	if (gunActive)
	{
		if (!gun->ammoStats->IsReloading() && gun->ammoStats->GetCurrentMagAmmo() != gun->ammoStats->GetMagSize())
		{
			gun->Reload();
			SetSprite(*sprites.Find("Reloading"));
		}
	}




}

void APlayer2D::Heal()
{
	if (inDialogue || isDead || meleeActive || isHurt) return;
	SwitchWeapon();
	isHealing = true;
	SetSprite(*sprites.Find("Heal"));
	GetSprite()->Play();
	GetSprite()->SetLooping(false);
	GetSprite()->OnFinishedPlaying.AddDynamic(this, &APlayer2D::HealEnd);



}

void APlayer2D::Melee()
{
	if (inDialogue || isDead || isHurt) return;

	if (!meleeActive)
	{
		meleeActive = true;
		SetSprite(*sprites.Find("Melee"));
		GetSprite()->Play();
		GetSprite()->SetLooping(false);
		GetSprite()->OnFinishedPlaying.AddDynamic(this, &APlayer2D::MeleeEnd);
		meleeHB->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
}

void APlayer2D::SetSprite(UPaperFlipbook* sprite_)
{

	GetSprite()->SetFlipbook(sprite_);

}


int APlayer2D::GetHealth()
{
	return playerStats->GetHealth();
}

FString APlayer2D::GetAmmo()
{
	if (gunActive) return FString::FromInt(gun->ammoStats->GetCurrentMagAmmo()) + FString(" / ") + FString::FromInt(gun->ammoStats->GetCurrentTotalAmmo());
	else return FString(" ");
}

void APlayer2D::CheckForOverlap(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{


	if (OtherActor && (OtherActor != this))
	{
	


		if (OtherActor->IsA<AGun2D>())
		{

			nearGun = true;
			if (!gun) gun = Cast<AGun2D>(OtherActor);

		}

		if (OtherActor->IsA<AEnemyBullet>() && OtherActor)
		{
			if (!isDead)
			{
				int randomNumber = FMath::RandRange(4, 13);
				PlayerTakeDamage(randomNumber);
				OtherActor->Destroy();
				//if (playerStats->GetHealth() == 0) PlayerDeath();
			}
		}


		if (OtherActor->IsA<AAmmoDrops>() && OtherActor)
		{

			AAmmoDrops* ammo = Cast<AAmmoDrops>(OtherActor);
			if (gun)
			{
				gun->ammoStats->IncreaseCurrentTotalAmmo(ammo->ammoAmount);
				playerWidget->ammo = gun->ammoStats->GetCurrentMagAmmo();
				playerWidget->totalAmmo = gun->ammoStats->GetCurrentTotalAmmo();
			}

			if (powerUpSound)
			{
				// Create an audio component and attach the sound wave
				UAudioComponent* AudioComponent = UGameplayStatics::SpawnSoundAttached(powerUpSound, GetOwner()->GetRootComponent());
				if (AudioComponent)
				{
					// Play the audio component
					AudioComponent->VolumeMultiplier = 1;
					AudioComponent->Play();
				}
			}
			OtherActor->Destroy();
		}
	}

	if (OtherComp)
	{



	}


}

void APlayer2D::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{


	if (OtherActor && (OtherActor != this))
	{
		if (nearGun)
		{
			nearGun = false;
			if (!gunActive) gun = nullptr;
		}

	}

}

void APlayer2D::CheckForMeleeHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (OtherActor && OtherActor != this)
	{

			if (OtherActor->IsA<AEnemy2D>())
			{
			
				AEnemy2D* enemy = Cast<AEnemy2D>(OtherActor);
				enemy->stats->TakeDamage(5);
			}



	}





}

void APlayer2D::AttachGunToPlayer()
{

	FAttachmentTransformRules transformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, true);
	gun->AttachToComponent(gunLocation, transformRules);
	gun->SetSprite("IdleWithArms");


	gunActive = true;
	SetSprite(*sprites.Find("IdleNoArms"));

	if (playerWidget)
	{
		playerWidget->ammo = gun->ammoStats->GetCurrentMagAmmo();
		playerWidget->totalAmmo = gun->ammoStats->GetCurrentTotalAmmo();
	}
}

void APlayer2D::ReplenishItems()
{


	UMainGameInstance* gameInstance = Cast<UMainGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	gameInstance->RespawnEnemies();
	playerStats->SetHealth(playerStats->GetMaxHealth());
	if (gun)
	{
		gun->ammoStats->SetCurrentTotalAmmo(gun->ammoStats->GetMaxAmmo());
		gun->ammoStats->SetCurrentMagAmmo(gun->ammoStats->GetMagSize());
	}


}

void APlayer2D::LoadLevel(FString level_)
{

	UGameplayStatics::OpenLevel(GetWorld(), FName(*level_));

}

void APlayer2D::ActivateRechargeStation()
{
	UMainGameInstance* gameInstance = Cast<UMainGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	gameInstance->spawnRechargeLocation = rechargeStation->GetActorLocation();
	gameInstance->spawnAtRechargeStation = true;
	AMainPlayerController* playerController = Cast<AMainPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	float endTransitionTimer = playerController->EndTransition();
	FString levelName = rechargeStation->GetLevel()->GetOutermost()->GetName();
	gameInstance->lastRechargeLevel = FName(levelName);
	playerStats->SetStims(5);
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindLambda([this]()
		{
			LoadLevel("RechargeLevel");
		});
	GetWorldTimerManager().SetTimer(transitionTimer, TimerDelegate, endTransitionTimer, false);
}

void APlayer2D::PlayerDeath()
{
	SetSprite(*sprites.Find("Death"));
	isDead = true;
	if (gun) gun->Destroy();
	GetSprite()->Play();
	GetSprite()->SetLooping(false);
	GetSprite()->OnFinishedPlaying.AddDynamic(this, &APlayer2D::DeathAnimEnd);
	if (deathSound)
	{
		// Create an audio component and attach the sound wave
		UAudioComponent* AudioComponent = UGameplayStatics::SpawnSoundAttached(deathSound, GetOwner()->GetRootComponent());
		if (AudioComponent)
		{
			// Play the audio component
			AudioComponent->VolumeMultiplier = 1;
			AudioComponent->Play();
		}
	}
}

void APlayer2D::DeathAnimEnd()
{

	ReplenishItems();

	UMainGameInstance* gameInstance = Cast<UMainGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	gameInstance->spawnAtRechargeStation = true;
	AMainPlayerController* playerController = Cast<AMainPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	float endTransitionTimer = playerController->EndTransition();

	if (gameInstance->lastRechargeLevel.IsNone())
	{
		gameInstance->lastRechargeLevel = FName(GetLevel()->GetOutermost()->GetName());
		gameInstance->spawnRechargeLocation = initialLocation;
	}
	playerStats->SetStims(5);
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindLambda([this]()
		{
			LoadLevel("DeathLevel");
		});
	GetWorldTimerManager().SetTimer(transitionTimer, TimerDelegate, endTransitionTimer, false);


}

void APlayer2D::UpdateFade()
{
	UMainGameInstance* gameInstance = Cast<UMainGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (camera)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, "Alarm");
		// Calculate the alpha value based on time
		float ElapsedTime = GetWorld()->GetTimeSeconds();
		float Alpha = (FMath::Sin(ElapsedTime * (PI / 1.0f)) + 1.0f) / 2.0f;

		// Interpolate between the normal and red gamma values
		FVector4 NormalGamma = FVector4(1.0f, 1.0f, 1.0f, 1.0f);
		FVector4 RedGamma = FVector4(1.0f, 0.3f, 0.3f, 1.0f);

		FVector4 CurrentGamma = FMath::Lerp(NormalGamma, RedGamma, Alpha);

		// Apply the interpolated gamma to the post-process settings
		FPostProcessSettings& PostProcessSettings = camera->PostProcessSettings;
		camera->PostProcessSettings.bOverride_ColorGamma = true;
		camera->PostProcessSettings.ColorGamma = CurrentGamma;
	}




}

void APlayer2D::PlayerTakeDamage(float damage_)
{

	playerStats->TakeDamage(damage_);
	playerWidget->health = playerStats->GetHealth();
	playerWidget->maxHealth = playerStats->GetMaxHealth();
}

void APlayer2D::MeleeEnd()
{

	meleeActive = false;
	SetSprite(*sprites.Find("Idle"));
	GetSprite()->OnFinishedPlaying.Clear();
	GetSprite()->SetLooping(true);
	meleeHB->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetSprite()->PlayFromStart();
}

void APlayer2D::Hurt()
{

	if (inDialogue || isDead) return;

	if (!isHurt)
	{
		SwitchWeapon();
		SetSprite(*sprites.Find("Hurt"));
		GetSprite()->Play();
		GetSprite()->SetLooping(false);
		GetSprite()->OnFinishedPlaying.AddDynamic(this, &APlayer2D::HurtEnd);
		isHurt = true;
	}


}

void APlayer2D::HurtEnd()
{
	isHurt = false;
	SetSprite(*sprites.Find("Idle"));
	GetSprite()->OnFinishedPlaying.Clear();
	GetSprite()->SetLooping(true);
	GetSprite()->PlayFromStart();
	SwitchWeapon();


}

void APlayer2D::HealEnd()
{
	isHealing = false;
	SetSprite(*sprites.Find("Idle"));
	GetSprite()->OnFinishedPlaying.Clear();
	GetSprite()->SetLooping(true);
	GetSprite()->PlayFromStart();
	playerStats->Heal(15);
	SwitchWeapon();
	if (healSound)
	{
		// Create an audio component and attach the sound wave
		UAudioComponent* AudioComponent = UGameplayStatics::SpawnSoundAttached(healSound, GetRootComponent());
		if (AudioComponent)
		{
			// Play the audio component
			AudioComponent->VolumeMultiplier = 1;
			AudioComponent->Play();
		}
	}
}

void APlayer2D::AdvanceDialogue()
{
	if (inDialogue)
	{
		UMainGameInstance* gameInstance = Cast<UMainGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
		gameInstance->eventManager->TriggerEvent(2);
			
		if (dialogueSound)
		{
			// Create an audio component and attach the sound wave
			UAudioComponent* AudioComponent = UGameplayStatics::SpawnSoundAttached(dialogueSound, GetOwner()->GetRootComponent());
			if (AudioComponent)
			{
				// Play the audio component
				AudioComponent->VolumeMultiplier = 1;
				AudioComponent->Play();
			}
		}
	}


}

void APlayer2D::LightSwitch()
{
	UMainGameInstance* gameInstance = Cast<UMainGameInstance>(GetGameInstance());

	if (!gameInstance->alarmTriggered)
	{
		camera->PostProcessSettings.AutoExposureBias = -2;
		GetWorldTimerManager().SetTimer(redEfectTimer, this, &APlayer2D::UpdateFade, GetWorld()->GetDeltaSeconds(), true);
		audioComponent->Play();
		gameInstance->alarmTriggered = true;
	}
	else
	{

		camera->PostProcessSettings.AutoExposureBias = -0.5;
		FVector4 NormalGamma = FVector4(1.0f, 1.0f, 1.0f, 1.0f);
		camera->PostProcessSettings.ColorGamma = NormalGamma;
		GetWorldTimerManager().ClearTimer(redEfectTimer);
		audioComponent->Stop();
		gameInstance->alarmTriggered = false;
	}

}

void APlayer2D::BeginPlay()
{

	Super::BeginPlay();
	
	initialGunLocation = gunLocation->GetRelativeLocation();
	scale = GetSprite()->GetRelativeScale3D();
	newScale = scale;
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &APlayer2D::CheckForOverlap);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &APlayer2D::EndOverlap);
	meleeHB->OnComponentBeginOverlap.AddDynamic(this, &APlayer2D::CheckForMeleeHit);
	meleeHB->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	UGameViewportClient* viewportClient = GetWorld()->GetGameViewport();

	FVector2D viewportSize;
	viewportClient->GetViewportSize(viewportSize);


	FVector2D screenCenter(viewportSize.X / 2, viewportSize.Y / 2);


	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetMouseLocation(screenCenter.X, screenCenter.Y);

	UMainGameInstance* gameInstance = Cast<UMainGameInstance>(GetGameInstance());

	initialLocation = GetActorLocation();
	if (gameInstance)
	{

		if (gameInstance->spawnAtRechargeStation)
		{
			FVector newLocation = gameInstance->spawnRechargeLocation;
			newLocation.Y = GetActorLocation().Y;
			SetActorLocation(newLocation);
			gunLocation->SetRelativeLocation(initialGunLocation);
			springArm->CameraLagSpeed = 0;
			camera->PostProcessSettings = gameInstance->cameraSettings;
			inDialogue = false;
			gameInstance->spawnAtRechargeStation = false;
		}


		if (gameInstance->spawnAtTrigger)
		{
			FVector newLocation = gameInstance->spawnLocation;
			newLocation.Y = GetActorLocation().Y;
			SetActorLocation(newLocation);
			gunLocation->SetRelativeLocation(initialGunLocation);
			springArm->CameraLagSpeed = 0;
			camera->PostProcessSettings = gameInstance->cameraSettings;
			inDialogue = false;
			gameInstance->spawnAtTrigger = false;
		}

		if (gameInstance->pickedUpGun)
		{
			UBlueprint* Blueprint = LoadObject<UBlueprint>(nullptr, *gameInstance->playerGunClass);

			if (Blueprint)
			{

				UClass* GeneratedClass = Blueprint->GeneratedClass;

				if (GeneratedClass)
				{

					AActor* NewActor = GetWorld()->SpawnActor<AActor>(GeneratedClass, GetActorLocation(), GetActorRotation());
					gun = Cast<AGun2D>(NewActor);
					gunLocation->SetRelativeLocation(initialGunLocation);
					AttachGunToPlayer();
					//gun->bulletExitLocation->SetWorldLocation(FVector(gun->bulletExitLocation->GetComponentLocation().X, gun->bulletExitLocation->GetComponentLocation().Y, gun->bulletExitLocation->GetComponentLocation().Z + 35));
					gun->ammoStats = gameInstance->playerGunStats;
				}
			}

		}

		if (!gameInstance->playerHasDied) playerStats = gameInstance->playerStats;
		else playerStats = new CharacterStats(50, 50, 5);
	}

	if (gameInstance && gameInstance->eventManager)
	{
		gameInstance->eventManager->SubscribeToEvent(14, FEventManager::CreateUObject(this, &APlayer2D::LightSwitch));
	}


	if (gameInstance->alarmTriggered)
	{
		GetWorld()->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateLambda([this]()
			{
				// Your code to execute after 1 second
				GetWorldTimerManager().SetTimer(redEfectTimer, this, &APlayer2D::UpdateFade, GetWorld()->GetDeltaSeconds(), true);
				audioComponent->Play();
			}));
	}

	playerWidget = CreateWidget<UUIPlayer>(GetWorld(), playerWidgetClass);

	if (playerWidget)
	{
		playerWidget->health = playerStats->GetHealth();
		playerWidget->maxHealth = playerStats->GetMaxHealth();

		playerWidget->AddToViewport();

	}


}


void APlayer2D::EndPlay(const EEndPlayReason::Type EndPlayReason)
{

	Super::EndPlay(EndPlayReason);
	UMainGameInstance* gameInstance = Cast<UMainGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (!isDead) gameInstance->playerStats = playerStats;
	if (gun) gameInstance->playerGunStats = gun->ammoStats;
	gameInstance->playerHasDied = isDead;
	gameInstance->cameraSettings = camera->PostProcessSettings;
}

void APlayer2D::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	UMainGameInstance* gameInstance = Cast<UMainGameInstance>(GetGameInstance());
	FVector2D mousePosition;
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetMousePosition(mousePosition.X, mousePosition.Y);
	FVector2D viewportSize;
	GetWorld()->GetGameViewport()->GetViewportSize(viewportSize);
	FVector2D offsetFromCenter = mousePosition - FVector2D(viewportSize.X / 2, viewportSize.Y / 2);


	FVector worldLocation, worldDirection;
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->DeprojectScreenPositionToWorld(mousePosition.X, mousePosition.Y, worldLocation, worldDirection);
	worldDirection.Y = 0;
	FRotator rotation;

	if (!isDead)
	{

		if (playerStats->GetHealth() == 0) PlayerDeath();

		if (gun)
		{

			playerWidget->ammo = gun->ammoStats->GetCurrentMagAmmo();
			playerWidget->totalAmmo = gun->ammoStats->GetCurrentTotalAmmo();

		}


		if (offsetFromCenter.X > 0)
		{
			newScale.X = scale.X;
			FVector gunPos = initialGunLocation;
			gunPos.X = -10.25;
			if (isMoving) gunPos.X = -5.25;
			if (isJumping)
			{
				gunPos.Z = 17;
				gunPos.X = -1;
			}
			else gunPos.Z = initialGunLocation.Z;
			gunLocation->SetRelativeLocation(gunPos);
			if (gunActive)
			{
				rotation = FRotationMatrix::MakeFromX(worldDirection).Rotator();
				gun->newScale.X = gun->scale.X;
				gunLocation->SetWorldRotation(rotation);
			}

			meleeHB->SetRelativeLocation(FVector(30.f, meleeHB->GetRelativeLocation().Y, meleeHB->GetRelativeLocation().Z));

		}

		if (offsetFromCenter.X < 0)
		{
			newScale.X = -scale.X;
			FVector gunPos = initialGunLocation;
			gunPos.X = 10.25;
			if (isMoving) gunPos.X = 5.25;
			if (isJumping)
			{
				gunPos.Z = 17;
				gunPos.X = 1;
			}
			else gunPos.Z = initialGunLocation.Z;
			gunLocation->SetRelativeLocation(gunPos);
			if (gunActive)
			{

				gun->newScale.X = -gun->scale.X;
				worldDirection.X = -worldDirection.X;
				worldDirection.Z = -worldDirection.Z;
				rotation = FRotationMatrix::MakeFromX(worldDirection).Rotator();
				gunLocation->SetWorldRotation(rotation);
			}
			meleeHB->SetRelativeLocation(FVector(-30.f, meleeHB->GetRelativeLocation().Y, meleeHB->GetRelativeLocation().Z));
		}

		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, meleeActive ? TEXT("True") : TEXT("False"));


		GetSprite()->SetRelativeScale3D(newScale);

		if (isJumping && GetCharacterMovement()->IsMovingOnGround() && !isHurt)
		{
			GetSprite()->PlayFromStart();
			isJumping = false;


		}
		if (isJumping && !isHurt)
		{

			if (GetSprite()->GetPlaybackPosition() > GetSprite()->GetFlipbookLength() - 0.1f)
			{

				if (!gunActive)SetSprite(*sprites.Find("Fall"));
				else SetSprite(*sprites.Find("FallNoArms"));
			}

			if (GetSprite()->GetFlipbook() == *sprites.Find("Fall") || GetSprite()->GetFlipbook() == *sprites.Find("FallNoArms"))
			{

				if (GetSprite()->GetPlaybackPosition() > GetSprite()->GetFlipbookLength() - 0.1f)
				{

					GetSprite()->Stop();

				}

			}


		}

		if (isDead)
		{


			if (GetSprite()->GetPlaybackPosition() > GetSprite()->GetFlipbookLength() - 0.1f)
			{
				GetSprite()->Stop();

			}

		}


		if (!GetCharacterMovement()->IsMovingOnGround()) fallTimer += deltaTime;

		if (fallTimer > 0 && GetCharacterMovement()->IsMovingOnGround())
		{

			if (fallTimer * 4 > 6) playerStats->TakeDamage(fallTimer * 10);



			fallTimer = 0;
		}
		if (playerWidget)
		{
			playerWidget->health = playerStats->GetHealth();
			playerWidget->maxHealth = playerStats->GetMaxHealth();
			playerWidget->stims = playerStats->GetStims();
			if (!playerWidget->IsInViewport()) playerWidget->AddToViewport();

		}
	
	}

	

}

void APlayer2D::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Binding an action input
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APlayer2D::PlayerJump);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &APlayer2D::Interact);

	PlayerInputComponent->BindAction("WeaponLeftClick", IE_Pressed, this, &APlayer2D::ShootBullet);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &APlayer2D::StartSprinting);

	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &APlayer2D::StopSprinting);

	PlayerInputComponent->BindAction("SwitchWeapon", IE_Pressed, this, &APlayer2D::SwitchWeapon);

	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &APlayer2D::Reload);

	PlayerInputComponent->BindAction("Heal", IE_Pressed, this, &APlayer2D::Heal);

	PlayerInputComponent->BindAction("AdvanceDialogue", IE_Pressed, this, &APlayer2D::AdvanceDialogue);

	PlayerInputComponent->BindAction("Melee", IE_Pressed, this, &APlayer2D::Melee);

	// Binding an axis input
	PlayerInputComponent->BindAxis("MoveHorizontally", this, &APlayer2D::Move);




}
