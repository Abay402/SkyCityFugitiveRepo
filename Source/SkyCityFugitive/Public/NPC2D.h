// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "DialogueComponent.h"
#include "Components/BoxComponent.h"
#include "NPC2D.generated.h"

/**
 * 
 */
UCLASS()
class SKYCITYFUGITIVE_API ANPC2D : public APaperCharacter
{
	GENERATED_BODY()

public:


	ANPC2D();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DialogueComponent)
	UDialogueComponent* dialogueComponent;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Collision)
	UBoxComponent* dialogueBox;

	void InitializeDialogue();

	UFUNCTION(BlueprintCallable)
	void OverlapBegin(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);



protected:
	// Called when the game starts
	virtual void BeginPlay() override;

};
