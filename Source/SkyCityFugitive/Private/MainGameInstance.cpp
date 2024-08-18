// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameInstance.h"

UMainGameInstance::UMainGameInstance()
{
    playerStats = new CharacterStats(50, 50,5);
    eventManager = new MainEventManager();
}

void UMainGameInstance::RespawnEnemies()
{


    for (auto it = enemyDeathData.CreateIterator(); it; ++it)
    {
        it.Value() = false;
    }

}
