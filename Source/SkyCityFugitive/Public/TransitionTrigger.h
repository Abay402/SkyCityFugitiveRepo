// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "TransitionTrigger.generated.h"

UCLASS()
class SKYCITYFUGITIVE_API ATransitionTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATransitionTrigger();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Collision)
		UBoxComponent* box;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector spawnLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName targetLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UWidgetComponent* widgetComponent;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
