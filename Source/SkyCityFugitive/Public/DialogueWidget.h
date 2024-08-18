// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TimerManager.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "DialogueWidget.generated.h"

/**
 * 
 */

UCLASS()
class SKYCITYFUGITIVE_API UDialogueWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

    UFUNCTION(BlueprintCallable)
    void SetString(FString string_) { inputDialogue = string_; }

    UFUNCTION(BlueprintCallable)
    void SetResponses(FString string_, int responseID);

    UPROPERTY(meta = (BindWidget))
    UButton* responseButton1;

    UPROPERTY(meta = (BindWidget))
    UButton* responseButton2;

    UPROPERTY(meta = (BindWidget))
    class UImage* leftPortraitImage;

    UPROPERTY(meta = (BindWidget))
    class UImage* rightPortraitImage;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* leftCharacterName;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* rightCharacterName;

    UDialogueWidget(const FObjectInitializer& ObjectInitializer);
    
    UActorComponent* dialogueComponent;


    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool response1Visible;


    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool response2Visible;


    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool leftPortraitVisible;


    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool rightPortraitVisible;

    void ClearDialogue();

    void HandleTimer(bool status_);

    void HandlePortraitVisibility(bool status_, bool isNPC);

    void SetImage(UTexture2D* leftTexture, UTexture2D* rightTexture);
    void SetPortraitText(FString leftText, FString rightText);

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString inputDialogue;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString activeDialogue;


    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString responseOption1;


    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString responseOption2;



    virtual void NativeConstruct() override;

    void AppendText();

    void AdvanceDialogue();

    FTimerHandle appendTimer;

    int currentChar;

    bool advanceDialogue;
    bool autoDialogue;
};
