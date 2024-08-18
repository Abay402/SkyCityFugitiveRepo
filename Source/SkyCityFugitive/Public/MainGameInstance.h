// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Containers/Map.h"
#include "RechargeStation.h"
#include "Gun2D.h"
#include "CharacterStats.h"
#include "MainEventManager.h"
#include "MainGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class SKYCITYFUGITIVE_API UMainGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UMainGameInstance();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap <FString, bool> enemyDeathData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap <int, bool> triggeredEvents;

	void RespawnEnemies();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName lastLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName lastRechargeLevel;
	bool spawnAtRechargeStation;
	bool spawnAtTrigger;
	FVector spawnRechargeLocation;
	FVector spawnLocation;
	bool playerHasDied;
	bool pickedUpGun;
	bool alarmTriggered;
	FString playerGunClass;
	AmmoStats* playerGunStats;
	CharacterStats* playerStats;
	MainEventManager* eventManager;
	FPostProcessSettings cameraSettings;
};
