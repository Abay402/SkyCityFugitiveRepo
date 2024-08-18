// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "TextWidget.h"
#include "Components/WidgetComponent.h"
#include "TextTrigger.generated.h"

UCLASS()
class SKYCITYFUGITIVE_API ATextTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATextTrigger();

	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Collision)
	UBoxComponent* box;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UTextWidget> textWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString text;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool autoActivate;

	UTextWidget* textWidget;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
