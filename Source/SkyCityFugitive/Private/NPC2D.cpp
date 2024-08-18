// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC2D.h"
#include "Kismet/GameplayStatics.h"
#include "Enemy2D.h"
#include "Boss2D.h"
#include "Player2D.h"
ANPC2D::ANPC2D()
{

	dialogueBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	dialogueBox->SetupAttachment(RootComponent);
	//box->SetCollisionProfileName(UCollisionProfile::);
	dialogueComponent = CreateDefaultSubobject<UDialogueComponent>(TEXT("DialogueComponent"));

}

void ANPC2D::InitializeDialogue()
{
}

void ANPC2D::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this))
	{
		if (!dialogueComponent->FileName.IsEmpty())
		{
			if (OtherActor->IsA<APlayer2D>())
			{


				if (dialogueComponent->autoActivate)
				{
					if (this->IsA<AEnemy2D>())
					{
						AEnemy2D* enemy = Cast<AEnemy2D>(this);
						enemy->aiActive = false;
						if (OtherActor->GetActorLocation().X > GetActorLocation().X) dialogueComponent->npcLeft = true;
						else dialogueComponent->npcLeft = false;
						dialogueComponent->DialogueBegin();
					}

					if (this->IsA<ABoss2D>())
					{
						ABoss2D* boss = Cast<ABoss2D>(this);
						boss->aiActive = false;
						if (OtherActor->GetActorLocation().X > GetActorLocation().X) dialogueComponent->npcLeft = true;
						else dialogueComponent->npcLeft = false;
						dialogueComponent->DialogueBegin();
					}


					APlayer2D* player = Cast<APlayer2D>(OtherActor);
					player->inDialogue = true;
					player->SetSprite(*player->sprites.Find("Idle"));
				}
			}
		}
	}
}


void ANPC2D::BeginPlay()
{
	Super::BeginPlay();

	if (!dialogueComponent->FileName.IsEmpty()) dialogueBox->OnComponentBeginOverlap.AddDynamic(this, &ANPC2D::OverlapBegin);

}
