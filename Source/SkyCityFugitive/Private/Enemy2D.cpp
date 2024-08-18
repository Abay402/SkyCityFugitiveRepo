// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy2D.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include <set>
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "MainGameInstance.h"
#include "BaseEnemyAIController.h"
#include "HealthWidget.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PlayerBullet.h"
AEnemy2D::AEnemy2D()
{

    stats = new CharacterStats(50, 50, 5);
    widgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
    widgetComponent->SetupAttachment(RootComponent);
    rayDetectionXOffset = 0;
    pathMarkerIndex = 0;
    canAttack = true;
   


    aiActive = true;
    gunLocation = CreateDefaultSubobject<UChildActorComponent>(TEXT("ChildActor"));
    gunLocation->SetupAttachment(RootComponent);
}

void AEnemy2D::CheckForOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (isDead) return;
    if (OtherActor && (OtherActor != this))
    {

        if (OtherActor->IsA<APlayerBullet>())
        {
            int randomNumber = FMath::RandRange(6, 15);
            stats->TakeDamage(randomNumber);
            OtherActor->Destroy();
            UHealthWidget* healthWidget = Cast<UHealthWidget>(widgetComponent->GetWidget());
            healthWidget->health = stats->GetHealth();
            healthWidget->maxHealth = stats->GetMaxHealth();
            if (stats->GetHealth() == 0) EnemyDeath();

            FVector Direction = OtherActor->GetActorLocation() - GetActorLocation();
            Direction.Z = 0;
            FRotator NewRotation = Direction.Rotation();

            // Step 3: Apply the rotation
            if (!canSee)SetActorRotation(NewRotation);


        }
    }

    



}

void AEnemy2D::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void AEnemy2D::MoveTo(FVector dir_)
{

    AddMovementInput(GetActorForwardVector(), 0.2f);

}

bool AEnemy2D::CheckForGround()
{
    FVector start = GetActorLocation() + FVector(0, 0, 25.f);
    FVector forwardVector = GetActorForwardVector();
    FVector end = start + (forwardVector * 200.f) + FVector(rayDetectionXOffset, 0, -110.f);

    FHitResult hitResult;
    FCollisionQueryParams collisionParams;

    bool hitGround = GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECC_Visibility, collisionParams);
    //DrawDebugLine(GetWorld(), start, end, FColor::Green, false, 1, 0, 1);
    //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, hitGround ? TEXT("True") : TEXT("False"));
    return hitGround;

}

bool AEnemy2D::CheckForJump()
{

    float jumpDistance = 100.f; 
    float jumpHeight = 50.f; 
    FVector start = GetActorLocation() + FVector(0, 0, 25.f);
    FVector forwardVector = GetActorForwardVector();

    FHitResult hitResult;
    FCollisionQueryParams collisionParams;


    FVector landingPosition = start + (forwardVector * jumpDistance) + FVector(0, 0, jumpHeight);
    FVector jumpEnd = landingPosition + FVector(0, 0, -jumpHeight * 4);


    bool canJump = GetWorld()->LineTraceSingleByChannel(hitResult, landingPosition, jumpEnd, ECC_Visibility, collisionParams);
    //DrawDebugLine(GetWorld(), landingPosition, jumpEnd, FColor::Green, false, 1, 0, 1);
    //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, canJump ? TEXT("True") : TEXT("False"));
    return canJump;
    // DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1, 0, 1);
    
    //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, bCanJump ? TEXT("True") : TEXT("False"));
}

bool AEnemy2D::CheckForPlatform(AActor* target_)
{
    if (target_->GetActorLocation().Z > GetActorLocation().Z)
    {
        // Perform a line trace upwards to check for a platform
        FVector Start = GetActorLocation() + FVector(-rayDetectionXOffset / 2, 0, 0);
        FVector End = Start + FVector(0, 0, 100); // Adjust the height as necessary

        FHitResult HitResult;
        FCollisionQueryParams CollisionParams;
        CollisionParams.AddIgnoredActor(this);

        bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, CollisionParams);

        //DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1, 0, 1);
        return bHit;
    }
    else return false;
}

AActor* AEnemy2D::CalculatePathTarget()
{
    
    if (pathMarkers.Num() == 0) return nullptr;
    else
    {
        if (pathMarkerIndex == pathMarkers.Num()) pathMarkerIndex = 0;

        pathSet = true;
        targetPathMarker = pathMarkers[pathMarkerIndex];
        pathMarkerIndex++;


        return targetPathMarker;
    }

}

bool AEnemy2D::CheckIfAtPathMarker(AActor* targetPathMarker_)
{

    FVector distance = GetActorLocation() - targetPathMarker_->GetActorLocation();
    float d = sqrt(distance.X * distance.X + distance.Y * distance.Y + distance.Z * distance.Z);

    //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::SanitizeFloat(d));

    if (d < 50) pathSet = false;
    return !pathSet;
}

bool AEnemy2D::CheckIfCanAttack(AActor* attackTarget_)
{
    FVector distance = GetActorLocation() - attackTarget_->GetActorLocation();
    float d = sqrt(distance.X * distance.X + distance.Y * distance.Y + distance.Z * distance.Z);

    //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::SanitizeFloat(d));

    if (d < 200)
    {
        if (canAttack)isChasing = false;  
        return true;
    }
    else return false;
}

void AEnemy2D::Attack()
{

    if (attackFunctions.Num() > 0)
    {
        int32 Index = FMath::RandRange(0, attackFunctions.Num() - 1);
        (this->*attackFunctions[Index])();
    }
    canAttack = false;


}

void AEnemy2D::SetSprite(const char* sprite_)
{

    GetSprite()->SetFlipbook(*sprites.Find(sprite_));

}

// Called when the game starts or when spawned
void AEnemy2D::BeginPlay()
{
    Super::BeginPlay();
    GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AEnemy2D::CheckForOverlap);
    GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &AEnemy2D::EndOverlap);


    // Calculate rotation
    FRotator rotation = GetActorRotation();



    if (gunClass)
    {
        gun = GetWorld()->SpawnActor<AGun2D>(gunClass, FVector(gunLocation->GetComponentLocation().X, gunLocation->GetComponentLocation().Y, gunLocation->GetComponentLocation().Z), rotation);

        FAttachmentTransformRules transformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, true);
        gun->AttachToComponent(gunLocation, transformRules);
        gun->SetSprite("Idle");
    }
    for (const FName& tag : Tags)
    {
        FString tagString = tag.ToString();
        UMainGameInstance* gameInstance = Cast<UMainGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
        
        if (!gameInstance->enemyDeathData.Contains(tag.ToString())) gameInstance->enemyDeathData.Add(tag.ToString(), isDead);
        else
        {
            if (*gameInstance->enemyDeathData.Find(tag.ToString()))
            {
                if (gun)
                {
                    gun->Destroy();
                    gun->SetActorHiddenInGame(true);
                }
                Destroy();

            }
        }   


    }
  
    scale = GetSprite()->GetRelativeScale3D();
    newScale = scale;

    UMainGameInstance* gameInstance = Cast<UMainGameInstance>(GetGameInstance());

    if (gameInstance->alarmTriggered && !dialogueComponent->FileName.IsEmpty() && !gameInstance->lastRechargeLevel.IsNone()) Destroy();


    if (gameInstance && gameInstance->eventManager)
    {
        if (!dialogueComponent->FileName.IsEmpty()) gameInstance->eventManager->SubscribeToEvent(27, FEventManager::CreateUObject(this, &AEnemy2D::EnemyDeath));
        if (!dialogueComponent->FileName.IsEmpty()) gameInstance->eventManager->SubscribeToEvent(27, FEventManager::CreateUObject(this, &AEnemy2D::Test));
    }

    if (gun)
    {
        attackFunctions.Add(&AEnemy2D::LightAttack);
        attackFunctions.Add(&AEnemy2D::ChargeAttack);
    }
}

void AEnemy2D::Tick(float deltaTime)
{
    Super::Tick(deltaTime);

    if (!isDead && stats->GetHealth() == 0) EnemyDeath();

    if (isChasing)
    {
        FVector PlayerLocation = UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation();

        // Get the gun's location
        FVector GunLocation = gunLocation->GetComponentLocation();

        // Calculate the direction vector from the gun to the player
        FVector Direction = PlayerLocation - GunLocation;
        Direction.Y = 0; // Ignore the Z component to keep the rotation in the XY plane

        // Create a rotation from this direction
        FRotator GunRotation = Direction.Rotation();

        // Set the gun's rotation
        gunLocation->SetWorldRotation(GunRotation);
    }

    FVector ActorForwardVector = GetActorForwardVector();

    // Define the direction you want to compare against, e.g., forward in the world (X axis)
    FVector TargetDirection = FVector::ForwardVector;

    // Calculate the dot product
    float DotProduct = FVector::DotProduct(ActorForwardVector, TargetDirection);

    // You can use a threshold value to check if the actor is facing the direction
    float Threshold = 0.707f; // approximately cos(45 degrees)

    if (DotProduct > Threshold)
    {

        FRotator currentRotation = GetActorRotation();
        currentRotation.Yaw = 0;
        SetActorRotation(currentRotation);

        FRotator widgetRotation = widgetComponent->GetComponentRotation();
        widgetRotation.Yaw = 90;
        widgetComponent->SetWorldRotation(widgetRotation);
        rayDetectionXOffset = -175;
        float xOff = -10;
        if (isWalking) xOff = 4;
        gunLocation->SetRelativeLocation(FVector(xOff, 2.5, gunLocation->GetRelativeLocation().Z));
    }
    else
    {
        FRotator currentRotation = GetActorRotation();
        currentRotation.Yaw = 180;
        SetActorRotation(currentRotation);

        FRotator widgetRotation = widgetComponent->GetComponentRotation();
        widgetRotation.Yaw = 90;
        widgetComponent->SetWorldRotation(widgetRotation);
        rayDetectionXOffset = 175;
        float xOff = -4; 
        if (isWalking) xOff = 4;
        gunLocation->SetRelativeLocation(FVector(xOff, -2.5, gunLocation->GetRelativeLocation().Z));
    }

    UHealthWidget* healthWidget = Cast<UHealthWidget>(widgetComponent->GetWidget());
    healthWidget->health = stats->GetHealth();
    healthWidget->maxHealth = stats->GetMaxHealth();
    GetSprite()->SetRelativeScale3D(newScale);
    if (pathSet)CheckIfAtPathMarker(targetPathMarker);

    if (GetVelocity().X != 0 && GetSprite()->GetFlipbook() != *sprites.Find("Walk") && !isDead)
    {
        SetSprite("Walk");
        isWalking = true;
    }
    else if (GetVelocity().X == 0 && GetSprite()->GetFlipbook() != *sprites.Find("Idle") && !isDead)
    {
        SetSprite("Idle");
        isWalking = false;
    }
    //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::SanitizeFloat(GetVelocity().X));
    //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, isChasing ? TEXT("True") : TEXT("False"));
}

void AEnemy2D::EnemyDeath()
{
    for (const FName& tag : Tags)
    {
        FString tagString = tag.ToString();
        UMainGameInstance* gameInstance = Cast<UMainGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
        *gameInstance->enemyDeathData.Find(tag.ToString()) = true;
    }
    isDead = true;
    aiActive = false;
    SetSprite("Death");
    if (gun)
    {
        gun->Destroy();
        gun->SetActorHiddenInGame(true);
    }
    GetWorldTimerManager().SetTimer(deathTimer, this, &AEnemy2D::DeathAnimEnd, GetSprite()->GetFlipbookLength() - 0.1f, false);
    if (deathSound)
    {
        // Create an audio component and attach the sound wave
        UAudioComponent* AudioComponent = UGameplayStatics::SpawnSoundAttached(deathSound, GetRootComponent());
        if (AudioComponent)
        {
            // Play the audio component
            AudioComponent->VolumeMultiplier = 1;
            AudioComponent->Play();
        }
    }
}


// Called when the actor is being constructed
#if WITH_EDITOR
void AEnemy2D::OnConstruction(const FTransform& transform_)
{
    Super::OnConstruction(transform_);

    // Set a random tag during construction in the editor
    if (!bTagAndLevelWritten)
    {
        if (GetWorld() && GetWorld()->IsEditorWorld())
        {
            //SetActorTag(GenerateRandomTag());
        }
    }
}
#endif

FString AEnemy2D::GenerateRandomTag()
{
    static std::set<FString> usedTags;

    FString newTag;
    do
    {
        newTag = FString::Printf(TEXT("RandomTag_%d"), FMath::Rand());
    } while (usedTags.find(newTag) != usedTags.end()); 

    usedTags.insert(newTag);

    FString filePath = FPaths::ProjectSavedDir() + TEXT("EnemyTagsWithLevelLocation.txt");
    FString levelName = GetCurrentLevelName();
    FString content = FString::Printf(TEXT("\"%s\" : \"%s\"\n"), *newTag, *levelName);

    FFileHelper::SaveStringToFile(content, *filePath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), FILEWRITE_Append);

    bTagAndLevelWritten = true;
    return newTag;
}

void AEnemy2D::SetActorTag(const FString& newTag_)
{
    Tags.Empty();
    Tags.Add(*newTag_);
}

FString AEnemy2D::GetCurrentLevelName()
{
    UWorld* world = GetWorld();
    if (world)
    {
        return FPaths::GetBaseFilename(world->GetMapName(), true);
    }
    return FString("");
}

void AEnemy2D::Test()
{
    AGun2D* newGun = GetWorld()->SpawnActor<AGun2D>(playerGunClass, GetActorLocation(), GetActorRotation());

}

void AEnemy2D::LightAttack()
{
    if (isDead) return;
    if (lightAttackIndex < 3)
    {

        gun->ShootBullet();
        GetWorld()->GetTimerManager().SetTimer(attackEndTimer, this, &AEnemy2D::LightAttack, gun->GetRenderComponent()->GetFlipbookLength(), false);
        lightAttackIndex += 1;
    }
    if (lightAttackIndex == 3)
    {
        GetWorld()->GetTimerManager().ClearTimer(attackEndTimer);
        GetWorld()->GetTimerManager().SetTimer(attackEndTimer, this, &AEnemy2D::AttackEnd, gun->GetRenderComponent()->GetFlipbookLength(), false);
        canAttack = false;
        lightAttackIndex = 0;
    }
    //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, "Light Attack");
}

void AEnemy2D::ChargeAttack()
{
    if (isDead) return;
    gun->ChargeShot();
    GetWorld()->GetTimerManager().SetTimer(attackEndTimer, this, &AEnemy2D::AttackEnd, gun->GetRenderComponent()->GetFlipbookLength(), false);
    canAttack = false;

}

void AEnemy2D::AttackEnd()
{
    attackFinish = true;
    GetWorld()->GetTimerManager().SetTimer(canAttackTimer, this, &AEnemy2D::CanAttack, 2, false);
}

void AEnemy2D::CanAttack()
{
    attackFinish = false;
    canAttack = true;

}

void AEnemy2D::DeathAnimEnd()
{

    Destroy();


    if (ammoDropClass)
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
  
        AAmmoDrops* newBullet = GetWorld()->SpawnActor<AAmmoDrops>(ammoDropClass, FVector(GetActorLocation().X, 60.f, GetActorLocation().Z), GetActorRotation());
    }

}


