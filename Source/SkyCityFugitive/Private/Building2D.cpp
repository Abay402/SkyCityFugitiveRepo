// Fill out your copyright notice in the Description page of Project Settings.


#include "Building2D.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Player2D.h"

ABuilding2D::ABuilding2D()
{
	PrimaryActorTick.bCanEverTick = true;

	box = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	box->SetupAttachment(RootComponent);

	exteriorFB = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("FlipbookComponent"));
	exteriorFB->SetupAttachment(RootComponent);
	opacity = 1.f;
}

void ABuilding2D::CheckForOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (OtherActor && (OtherActor != this))
	{
		if (OtherActor->IsA<APlayer2D>())
		{
			GetWorld()->GetTimerManager().ClearTimer(opacityTimer);
			noOpacity = true;
			GetWorld()->GetTimerManager().SetTimer(opacityTimer, this, &ABuilding2D::ChangeOpacity, 0.05f, true);
		}
	}
}

void ABuilding2D::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && (OtherActor != this))
	{
		if (OtherActor->IsA<APlayer2D>())
		{
			GetWorld()->GetTimerManager().ClearTimer(opacityTimer);
			noOpacity = false;
			GetWorld()->GetTimerManager().SetTimer(opacityTimer, this, &ABuilding2D::ChangeOpacity, 0.05f, true);
		}
	}
}

void ABuilding2D::ChangeOpacity()
{

	if (noOpacity)
	{

		if (opacity == 0.0f)
		{

			GetWorld()->GetTimerManager().ClearTimer(opacityTimer);
			return;
		}

		opacity -= 0.1f;
		//opacityMaterial->SetScalarParameterValue(TEXT("Opacity"), opacity);
	}
	if (!noOpacity)
	{

		if (opacity == 1.0f)
		{

			GetWorld()->GetTimerManager().ClearTimer(opacityTimer);
			return;
		}

		opacity += 0.1f;
		//opacityMaterial->SetScalarParameterValue(TEXT("Opacity"), opacity);
	}


}

void ABuilding2D::BeginPlay()
{
	Super::BeginPlay();

	box->OnComponentBeginOverlap.AddDynamic(this, &ABuilding2D::CheckForOverlap);
	box->OnComponentEndOverlap.AddDynamic(this, &ABuilding2D::EndOverlap);

	UMaterialInterface* Material = exteriorFB->GetMaterial(0);
	opacityMaterial = UMaterialInstanceDynamic::Create(Material, this);
	exteriorFB->SetMaterial(0, opacityMaterial);

}

void ABuilding2D::Tick(float deltaTime)
{

	Super::Tick(deltaTime);
	opacityMaterial->SetScalarParameterValue(TEXT("Opacity"), opacity);
}
