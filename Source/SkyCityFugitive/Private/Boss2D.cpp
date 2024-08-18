// Fill out your copyright notice in the Description page of Project Settings.



#include "Boss2D.h"
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
#include "Player2D.h"

ABoss2D::ABoss2D()
{

    stats = new CharacterStats(150, 100, 5);
    //widgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
    //widgetComponent->SetupAttachment(RootComponent);
    rayDetectionXOffset = -100;
    canAttack = true;


    aiActive = false;
    gunLocation = CreateDefaultSubobject<UChildActorComponent>(TEXT("ChildActor"));
    gunLocation->SetupAttachment(RootComponent);
    pathMarkerIndex = 0;
}

void ABoss2D::CheckForOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

    if (OtherActor && (OtherActor != this))
    {

        if (OtherActor->IsA<APlayerBullet>())
        {
            int randomNumber = FMath::RandRange(6, 15);
            stats->TakeDamage(randomNumber);
            OtherActor->Destroy();

            if (healthWidget)
            {
                healthWidget->health = stats->GetHealth();
                healthWidget->maxHealth = stats->GetMaxHealth();
            }
            if (stats->GetHealth() == 0) BossDeath();



        }

        if (OtherActor->IsA<APlayer2D>())
        {
            APlayer2D* player = Cast<APlayer2D>(OtherActor);
            player->PlayerTakeDamage(5);
            player->Hurt();
        }
    }





}

void ABoss2D::CheckForPlayerActivation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && (OtherActor != this))
    {
        if (OtherActor->IsA<APlayer2D>() && dialogueOver)
        {
          
            healthWidget = CreateWidget<UHealthWidget>(GetWorld(), healthWidgetClass);

            if (healthWidget)
            {
                healthWidget->name = name;
                healthWidget->health = stats->GetHealth();
                healthWidget->maxHealth = stats->GetMaxHealth();
                healthWidget->AddToViewport();

            }
            aiActive = true;
            dialogueOver = false;
        }
    }

}

void ABoss2D::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void ABoss2D::MoveTo(FVector dir_)
{

    AddMovementInput(dir_, 0.2f);
    // Perform a line trace upwards to check for a platform
    FVector Start = GetActorLocation() + FVector(-rayDetectionXOffset / 2, 0, 0);
    FVector End = Start + FVector(0, 0, 100); // Adjust the height as necessary

    FHitResult HitResult;
    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(this);

    bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, CollisionParams);

    if (bHit) Jump();



}


bool ABoss2D::CheckForPlatform(AActor* target_)
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


bool ABoss2D::CheckIfCanAttack(AActor* attackTarget_)
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

void ABoss2D::Attack()
{

    if (attackFunctions.Num() > 0)
    {
        int32 Index = FMath::RandRange(0, attackFunctions.Num() - 1);
        (this->*attackFunctions[Index])();
    }
    canAttack = false;


}

void ABoss2D::RandomMovement(FVector targetDir_)
{
    FVector direction = targetDir_ - GetActorLocation();
    AddMovementInput(direction, 1.f);

    // Perform a line trace upwards to check for a platform
    FVector Start = GetActorLocation() + FVector(-rayDetectionXOffset / 2, 0, 0);
    FVector End = Start + FVector(0, 0, 100); // Adjust the height as necessary

    FHitResult HitResult;
    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(this);

    bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, CollisionParams);
    //DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1, 0, 1);
    if (bHit) Jump();

   // FVector direction = targetDir_ - GetActorLocation();
    float d = sqrt(direction.X * direction.X + direction.Y * direction.Y + direction.Z * direction.Z);

    if (d < 75)
    {
        MoveEnd();
    }
}

void ABoss2D::WaitAnimation()
{


    if (!isWaiting)
    {
        if (gun) gun->SetSprite("Reload");

        SetSprite("Wait");
        waitOver = false;
        isWaiting = true;

        GetSprite()->Play();
        GetSprite()->SetLooping(false);
        GetSprite()->OnFinishedPlaying.AddDynamic(this, &ABoss2D::WaitAnimationEnd);

    }

}

AActor* ABoss2D::CalculateTarget()
{
    APathMarker* targetPathMarker = pathMarkers[pathMarkerIndex];
    moveOver = false;

    while (targetPathMarker == pathMarkers[pathMarkerIndex])
    {
        int totalPathMarkers = pathMarkers.Num() - 1;
        attackFinish = false;
        pathMarkerIndex = FMath::RandRange(0, totalPathMarkers);
    }
    //if (targetPathMarker != pathMarkers[pathMarkerIndex]) return pathMarkers[pathMarkerIndex];
    //else
    //{
    //    if (pathMarkerIndex == pathMarkers.Num() - 1) pathMarkerIndex--;
    //    else pathMarkerIndex++;
    //}

    return pathMarkers[pathMarkerIndex];
}

void ABoss2D::SetSprite(const char* sprite_)
{

    GetSprite()->SetFlipbook(*sprites.Find(sprite_));

}

// Called when the game starts or when spawned
void ABoss2D::BeginPlay()
{
    Super::BeginPlay();
    GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ABoss2D::CheckForOverlap);
    GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &ABoss2D::EndOverlap);
    dialogueBox->OnComponentBeginOverlap.AddDynamic(this, &ABoss2D::CheckForPlayerActivation);

    // Calculate rotation
    FRotator rotation = GetActorRotation();


    gun = GetWorld()->SpawnActor<AGun2D>(gunClass, FVector(gunLocation->GetComponentLocation().X, gunLocation->GetComponentLocation().Y, gunLocation->GetComponentLocation().Z), rotation);

    FAttachmentTransformRules transformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, true);
    gun->AttachToComponent(gunLocation, transformRules);
    gun->SetSprite("Idle");


    scale = GetSprite()->GetRelativeScale3D();
    newScale = scale;

    UMainGameInstance* gameInstance = Cast<UMainGameInstance>(GetGameInstance());

    if (gameInstance && gameInstance->eventManager)
    {
        gameInstance->eventManager->SubscribeToEvent(18, FEventManager::CreateUObject(this, &ABoss2D::Activate));
    }

    attackFunctions.Add(&ABoss2D::LightAttack);
    attackFunctions.Add(&ABoss2D::ChargeAttack);
}

void ABoss2D::Tick(float deltaTime)
{
    Super::Tick(deltaTime);


    if (aiActive)
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

        rayDetectionXOffset = -175;
        float xOff = -4;
        if (isWalking) xOff = 4;
        gunLocation->SetRelativeLocation(FVector(xOff, 1, gunLocation->GetRelativeLocation().Z));
    }
    else
    {
        FRotator currentRotation = GetActorRotation();
        currentRotation.Yaw = 180;
        SetActorRotation(currentRotation);

        rayDetectionXOffset = 175;
        float xOff = -8;
        if (isWalking) xOff = 4;
        gunLocation->SetRelativeLocation(FVector(xOff, -1, gunLocation->GetRelativeLocation().Z));
    }

 

        if (GetVelocity().X != 0 && GetSprite()->GetFlipbook() != *sprites.Find("Walk") && !isDead && !isWaiting)
        {
            SetSprite("Walk");
            isWalking = true;
        }
        else if (GetVelocity().X == 0 && GetSprite()->GetFlipbook() != *sprites.Find("Idle") && !isDead && !isWaiting)
        {
            SetSprite("Idle");
            isWalking = false;
        }

        if (isWalking)
        {

            if (gun) gun->GetRenderComponent()->SetVisibility(false);

        }
        else
        {

            if (gun) gun->GetRenderComponent()->SetVisibility(true);

        }
  
    //UHealthWidget* healthWidget = Cast<UHealthWidget>(widgetComponent->GetWidget());
    //healthWidget->health = stats->GetHealth();
    //healthWidget->maxHealth = stats->GetMaxHealth();
    GetSprite()->SetRelativeScale3D(newScale);


    //if (GetVelocity().X != 0 && GetSprite()->GetFlipbook() != *sprites.Find("Walk") && !isDead)
    //{
    //    SetSprite("Walk");
    //    isWalking = true;
    //}
    //else if (GetVelocity().X == 0 && GetSprite()->GetFlipbook() != *sprites.Find("Idle") && !isDead)
    //{
    //    SetSprite("Idle");
    //    isWalking = false;
    //}
    //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::SanitizeFloat(GetVelocity().X));
    //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, attackFinish ? TEXT("True") : TEXT("False"));
    
}

void ABoss2D::BossDeath()
{

    isDead = true;
    aiActive = false;
    SetSprite("Death");
    gun->Destroy();
    gun->SetActorHiddenInGame(true);
    GetWorldTimerManager().SetTimer(deathTimer, this, &ABoss2D::DeathAnimEnd, GetSprite()->GetFlipbookLength() - 0.2f, false);

    UMainGameInstance* gameInstance = Cast<UMainGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
    gameInstance->eventManager->TriggerEvent(333);
}


FString ABoss2D::GetCurrentLevelName()
{
    UWorld* world = GetWorld();
    if (world)
    {
        return FPaths::GetBaseFilename(world->GetMapName(), true);
    }
    return FString("");
}



void ABoss2D::LightAttack()
{
    if (lightAttackIndex < 4)
    {

        GetWorld()->GetTimerManager().SetTimer(attackEndTimer, this, &ABoss2D::LightAttack, gun->GetRenderComponent()->GetFlipbookLength(), false);

        lightAttackIndex += 1;
    }
    if (lightAttackIndex > 0)   gun->ShootBullet();
    if (lightAttackIndex == 4)
    {
        GetWorld()->GetTimerManager().ClearTimer(attackEndTimer);
        GetWorld()->GetTimerManager().SetTimer(attackEndTimer, this, &ABoss2D::AttackEnd, gun->GetRenderComponent()->GetFlipbookLength(), false);
        canAttack = false;
        lightAttackIndex = 0;
    }
    //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, "Light Attack");
    waitAfterAttack = true;
    waitOver = false;
}

void ABoss2D::ChargeAttack()
{

    gun->ChargeShot();
    GetWorld()->GetTimerManager().SetTimer(attackEndTimer, this, &ABoss2D::AttackEnd, gun->GetRenderComponent()->GetFlipbookLength(), false);
    canAttack = false;

    waitAfterAttack = false;
}

void ABoss2D::AttackEnd()
{
    attackFinish = true;
    if (!waitAfterAttack)GetWorld()->GetTimerManager().SetTimer(canAttackTimer, this, &ABoss2D::CanAttack, 3, false);
}

void ABoss2D::CanAttack()
{
    attackFinish = false;
    canAttack = true;

}

void ABoss2D::DeathAnimEnd()
{

    Destroy();
}

void ABoss2D::MoveEnd()
{
    moveOver = true;

}

void ABoss2D::Activate()
{

    if (aiActive == false) aiActive = true;
    healthWidget = CreateWidget<UHealthWidget>(GetWorld(), healthWidgetClass);

    if (healthWidget)
    {
        healthWidget->name = name;
        healthWidget->health = stats->GetHealth();
        healthWidget->maxHealth = stats->GetMaxHealth();
        healthWidget->AddToViewport();

    }
    dialogueOver = true;

}

void ABoss2D::Deactivate()
{

    if (healthWidget)
    {
        healthWidget->RemoveFromViewport();
        healthWidget->RemoveFromParent();
    }
    aiActive = false;
}

void ABoss2D::WaitAnimationEnd()
{
    if (gun) gun->SetSprite("IdleWithArms");
    waitOver = true;
    isWaiting = false;
    SetSprite("Idle");
    GetSprite()->OnFinishedPlaying.Clear();
    GetSprite()->SetLooping(true);
    GetSprite()->PlayFromStart();
    attackFinish = false;
    canAttack = true;


}

