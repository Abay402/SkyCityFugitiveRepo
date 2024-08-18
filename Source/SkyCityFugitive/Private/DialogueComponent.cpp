 // Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueComponent.h"
#include "XmlParser.h"
#include "XmlFile.h"
#include "XmlNode.h"
#include "Misc/Paths.h"
#include "Kismet/GameplayStatics.h"
#include "Player2D.h"
#include "MainGameInstance.h"

UDialogueComponent::UDialogueComponent()
{

    nextDialogue = 1;

}



// Called when the game starts
void UDialogueComponent::BeginPlay()
{
    Super::BeginPlay();
    
    if (!FileName.IsEmpty())
    {
        dialogueWidget = CreateWidget<UDialogueWidget>(UGameplayStatics::GetPlayerController(this, 0), dialogueWidgetClass);
        dialogueWidget->responseButton1->OnClicked.AddDynamic(this, &UDialogueComponent::SelectResponse1);
        dialogueWidget->responseButton2->OnClicked.AddDynamic(this, &UDialogueComponent::SelectResponse2);
        dialogueWidget->dialogueComponent = this;
    }

}

void UDialogueComponent::DialogueBegin()
{

    LoadDialogue(nextDialogue);
    dialogueWidget->SetString(currentDialogue);
    if (npcLeft)
    {
        dialogueWidget->SetImage(npcPortrait, playerPortrait);
        dialogueWidget->SetPortraitText(npcName, playerName);

    }
    else
    {
        dialogueWidget->SetImage(playerPortrait, npcPortrait);
        dialogueWidget->SetPortraitText(playerName, npcName);

    }
    if (npcLeft) dialogueWidget->HandlePortraitVisibility(true, isPlayer);
    else dialogueWidget->HandlePortraitVisibility(false, isPlayer);
    RenderWidget(true);
   

}

void UDialogueComponent::DialogueEnd()
{
    APlayer2D* player = Cast<APlayer2D>(GetWorld()->GetFirstPlayerController()->GetPawn());
    player->inDialogue = false;
    RenderWidget(false);

    if (autoActivate) FileName.Empty();
}

void UDialogueComponent::LoadDialogue(int DialogueID)
{
    currentDialogueID = DialogueID;
       tinyxml2::XMLDocument doc;
       FString BaseDir = FPaths::ProjectContentDir();
       FString FilePath = FPaths::Combine(BaseDir, TEXT("DialogueScripts"), FileName);

       const char* FFileName = TCHAR_TO_ANSI(*FilePath);
        
       doc.LoadFile(FFileName);

       bool status = doc.Error();
       if (status) {
       	//std::cout << doc.ErrorIDToName(doc.ErrorID()) << std::endl;
       	return;
       }

       std::string dialogue = "Dialogue";
       std::string result = dialogue + std::to_string(DialogueID);


       // These are referred to as nodes. Get the top of the system
       tinyxml2::XMLElement* rootData = doc.RootElement();
       tinyxml2::XMLElement* dialogueData = rootData->FirstChildElement(result.c_str());

       const tinyxml2::XMLElement* child = rootData->FirstChildElement(result.c_str())->FirstChildElement();
     
       if (std::string(child->Name()) == "Dialogue") {

           currentDialogue = child->Attribute("string");
           isPlayer = child->BoolAttribute("speaker");
       }  


       const tinyxml2::XMLElement* secondChild = child->FirstChildElement();

       if (std::string(secondChild->Name()) == "Responses")
       {

           responseBool = secondChild->BoolAttribute("bool");

       }


       if (responseBool)
       {

           tinyxml2::XMLElement* responseChild = dialogueData->FirstChildElement("ResponseOptions")->FirstChildElement();
           response1.Add(responseChild->IntAttribute("id"), responseChild->Attribute("string"));

           responseChild = responseChild->NextSiblingElement();
           response2.Add(responseChild->IntAttribute("id"), responseChild->Attribute("string"));
        
       }

       else
       {
           tinyxml2::XMLElement* nextChild = dialogueData->FirstChildElement("NextDialogue");

           nextDialogue = nextChild->IntAttribute("id");

           nextChild = nextChild->NextSiblingElement();
           dialogueEnd = nextChild->BoolAttribute("bool");


       }

       if (npcLeft) dialogueWidget->HandlePortraitVisibility(true, isPlayer);
       else dialogueWidget->HandlePortraitVisibility(false, isPlayer);
}

void UDialogueComponent::PickResponse(int DialogueID, int responseOption)
{

    tinyxml2::XMLDocument doc;
    FString BaseDir = FPaths::ProjectContentDir();
    FString FilePath = FPaths::Combine(BaseDir, TEXT("DialogueScripts"), FileName);

    const char* FFileName = TCHAR_TO_ANSI(*FilePath);

    doc.LoadFile(FFileName);

    bool status = doc.Error();
    if (status) {
        //std::cout << doc.ErrorIDToName(doc.ErrorID()) << std::endl;
        return;
    }

    std::string dialogue = "Dialogue";
    std::string result = dialogue + std::to_string(DialogueID);


    // These are referred to as nodes. Get the top of the system
    tinyxml2::XMLElement* rootData = doc.RootElement();
    tinyxml2::XMLElement* dialogueData = rootData->FirstChildElement(result.c_str());

    if (responseBool)
    {
        tinyxml2::XMLElement* responseChild = dialogueData->FirstChildElement("ResponseOptions")->FirstChildElement();

        if (responseOption == 1)
        {

            nextDialogue = responseChild->IntAttribute("id");

        }

        if (responseOption == 2)
        {

            responseChild = responseChild->NextSiblingElement();
            nextDialogue = responseChild->IntAttribute("id");

        }



    }

}

void UDialogueComponent::CheckForEventTrigger(int DialogueID)
{
    tinyxml2::XMLDocument doc;
    FString BaseDir = FPaths::ProjectContentDir();
    FString FilePath = FPaths::Combine(BaseDir, TEXT("DialogueScripts"), FileName);

    const char* FFileName = TCHAR_TO_ANSI(*FilePath);

    doc.LoadFile(FFileName);

    bool status = doc.Error();
    if (status) {
        //std::cout << doc.ErrorIDToName(doc.ErrorID()) << std::endl;
        return;
    }

    std::string dialogue = "Dialogue";
    std::string result = dialogue + std::to_string(DialogueID);


    // These are referred to as nodes. Get the top of the system
    tinyxml2::XMLElement* rootData = doc.RootElement();
    tinyxml2::XMLElement* dialogueData = rootData->FirstChildElement(result.c_str());

    tinyxml2::XMLElement* eventChild = dialogueData->FirstChildElement("EventTrigger");

    if (eventChild->BoolAttribute("bool"))
    {


        UMainGameInstance* gameInstance = Cast<UMainGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
        gameInstance->eventManager->TriggerEvent(eventChild->IntAttribute("eventID"));



    }

}

void UDialogueComponent::RenderWidget(bool toggle_)
{

    if (dialogueWidget)
    {
        if (toggle_ == true) dialogueWidget->AddToViewport();
        else dialogueWidget->RemoveFromViewport();
    }



}

void UDialogueComponent::CalculateNextDialogue()
{
    if (!responseBool)
    {
       
        CheckForEventTrigger(currentDialogueID);
        if (!dialogueEnd)
        {
            LoadDialogue(nextDialogue);
            dialogueWidget->SetString(currentDialogue);
    
        }
        else
        {

            dialogueWidget->RemoveFromViewport();
            dialogueWidget->ClearDialogue();
            DialogueEnd();
        }
    }
    else
    {

        dialogueWidget->response1Visible = true;
        dialogueWidget->response2Visible = true;
        dialogueWidget->HandleTimer(false);
        dialogueWidget->SetResponses(GetResponses(1), 1);
        dialogueWidget->SetResponses(GetResponses(2), 2);


        if (npcLeft) dialogueWidget->HandlePortraitVisibility(false, true);
        else dialogueWidget->HandlePortraitVisibility(true, true);

    }

}

void UDialogueComponent::SelectResponse1()
{
    responseOptionPicked = 1;
    InitiatiateNextDialogue();
}

void UDialogueComponent::SelectResponse2()
{
    responseOptionPicked = 2;
    InitiatiateNextDialogue();
}

FString UDialogueComponent::GetResponses(int id_)
{
    tinyxml2::XMLDocument doc;
    FString BaseDir = FPaths::ProjectContentDir();
    FString FilePath = FPaths::Combine(BaseDir, TEXT("DialogueScripts"), FileName);

    const char* FFileName = TCHAR_TO_ANSI(*FilePath);

    doc.LoadFile(FFileName);

    bool status = doc.Error();
    if (status) {
        //std::cout << doc.ErrorIDToName(doc.ErrorID()) << std::endl;
        return " ";
    }

    std::string dialogue = "Dialogue";
    std::string result = dialogue + std::to_string(currentDialogueID);


    // These are referred to as nodes. Get the top of the system
    tinyxml2::XMLElement* rootData = doc.RootElement();
    tinyxml2::XMLElement* dialogueData = rootData->FirstChildElement(result.c_str());

    if (responseBool)
    {
        tinyxml2::XMLElement* responseChild = dialogueData->FirstChildElement("ResponseOptions")->FirstChildElement();

        if (id_ == 1) return responseChild->Attribute("string");
        else if (id_ == 2)
        {

            responseChild = responseChild->NextSiblingElement();
            return responseChild->Attribute("string");
        }
        else return " ";

    }
    else return " ";

}

void UDialogueComponent::InitiatiateNextDialogue()
{
    dialogueWidget->ClearDialogue();

    PickResponse(currentDialogueID, responseOptionPicked);
    LoadDialogue(nextDialogue);
    dialogueWidget->SetString(currentDialogue);
    dialogueWidget->response1Visible = false;
    dialogueWidget->response2Visible = false;
    dialogueWidget->HandleTimer(true);

    //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, status ? TEXT("True") : TEXT("False"));
    if (npcLeft) dialogueWidget->HandlePortraitVisibility(true, isPlayer);
    else dialogueWidget->HandlePortraitVisibility(false, isPlayer);
}


