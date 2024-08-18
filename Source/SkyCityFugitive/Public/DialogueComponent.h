// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "tinyxml2.h"
#include "DialogueWidget.h"
#include "DialogueComponent.generated.h"


UCLASS(BlueprintType, Blueprintable)
class SKYCITYFUGITIVE_API UDialogueComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UDialogueComponent();

	UFUNCTION(BlueprintCallable)
	void LoadDialogue(int DialogueID);

	UFUNCTION(BlueprintCallable)
	void PickResponse(int DialogueID, int responseOption);

	UFUNCTION(BlueprintCallable)
	void CheckForEventTrigger(int DialogueID);

	void RenderWidget(bool toggle_);

	void CalculateNextDialogue();
	
	UFUNCTION(BlueprintCallable)
	void SelectResponse1();
	UFUNCTION(BlueprintCallable)
	void SelectResponse2();

	FString GetResponses(int id_);

	void InitiatiateNextDialogue();
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString FileName;

	UPROPERTY(BlueprintReadWrite)
	FString currentDialogue;

	UPROPERTY(BlueprintReadWrite)
	int nextDialogue;

	UPROPERTY(BlueprintReadWrite)
	TMap<int, FString> response1;

	UPROPERTY(BlueprintReadWrite)
	TMap<int, FString> response2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool autoActivate;

	UPROPERTY(BlueprintReadWrite)
	bool responseBool;

	UPROPERTY(BlueprintReadWrite)
	bool isPlayer;

	UPROPERTY(BlueprintReadWrite)
	bool dialogueEnd;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UDialogueWidget> dialogueWidgetClass;

	UPROPERTY()
	UDialogueWidget* dialogueWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* npcPortrait;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* playerPortrait;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString npcName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString playerName;

	int currentDialogueID;
	
	int responseOptionPicked;

	void DialogueBegin();
	void DialogueEnd();
	
	bool npcLeft;




};
