// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueWidget.h"
#include "DialogueComponent.h"
#include "MainGameInstance.h"
#include "Components/Image.h"

void UDialogueWidget::SetResponses(FString string_, int responseID)
{

	if (responseID == 1) responseOption1 = string_;
	if (responseID == 2) responseOption2 = string_;



}

UDialogueWidget::UDialogueWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

	currentChar = 0;
	autoDialogue = false;
}

void UDialogueWidget::ClearDialogue()
{
	activeDialogue.Empty();
	inputDialogue.Empty();
	GetWorld()->GetTimerManager().PauseTimer(appendTimer);
}

void UDialogueWidget::HandleTimer(bool status_)
{

	if (status_) GetWorld()->GetTimerManager().UnPauseTimer(appendTimer);
	else GetWorld()->GetTimerManager().PauseTimer(appendTimer);

}

void UDialogueWidget::HandlePortraitVisibility(bool status_, bool isNPC)
{

	if (isNPC)
	{
		if (status_ == true)
		{
			leftPortraitVisible = true;
			rightPortraitVisible = false;
		}
		else
		{
			leftPortraitVisible = false;
			rightPortraitVisible = true;
		}
	}
	else
	{
		leftPortraitVisible = false;
		rightPortraitVisible = false;
	}


}

void UDialogueWidget::SetImage(UTexture2D* leftTexture, UTexture2D* rightTexture)
{

	leftPortraitImage->SetBrushFromTexture(leftTexture);
	rightPortraitImage->SetBrushFromTexture(rightTexture);


}

void UDialogueWidget::SetPortraitText(FString leftText, FString rightText)
{

	FText left = FText::FromString(leftText);
	leftCharacterName->SetText(left);


	FText right = FText::FromString(rightText);
	rightCharacterName->SetText(right);
}

void UDialogueWidget::NativeConstruct()
{
	Super::NativeConstruct();

	GetWorld()->GetTimerManager().SetTimer(appendTimer, this, &UDialogueWidget::AppendText, 0.085f, true);

	UMainGameInstance* gameInstance = Cast<UMainGameInstance>(GetGameInstance());

	if (gameInstance && gameInstance->eventManager)
	{
		gameInstance->eventManager->SubscribeToEvent(2, FEventManager::CreateUObject(this, &UDialogueWidget::AdvanceDialogue));
	}

}

void UDialogueWidget::AppendText()
{

	if (currentChar < inputDialogue.Len())
	{

		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, inputDialogue);
		if (inputDialogue[currentChar] != '|')
		{
			activeDialogue.AppendChar(inputDialogue[currentChar]);
			currentChar += 1;
		}
		else
		{
			if (advanceDialogue || autoDialogue)
			{
				activeDialogue.Empty();
				currentChar += 1;
				advanceDialogue = false;
			}
		}
	}

	if (currentChar == inputDialogue.Len() && (advanceDialogue || autoDialogue))
	{
		UDialogueComponent* dialogueComp = Cast<UDialogueComponent>(dialogueComponent);
		dialogueComp->CalculateNextDialogue();
		currentChar = 0;
		advanceDialogue = false;
		activeDialogue.Empty();

	}

	if (advanceDialogue)
	{

		for (currentChar; currentChar < inputDialogue.Len(); currentChar++)
		{

			if (inputDialogue[currentChar] != '|')
			{
				activeDialogue.AppendChar(inputDialogue[currentChar]);
			}
			else break;
		}

		advanceDialogue = false;
	}

}

void UDialogueWidget::AdvanceDialogue()
{
	advanceDialogue = true;
}
