// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */

DECLARE_DELEGATE(FEventManager);

class SKYCITYFUGITIVE_API MainEventManager
{
public:
	MainEventManager();
	~MainEventManager();

    void SubscribeToEvent(int id_, FEventManager function_)
    {
        if (!events.Contains(id_)) events.Add(id_, TArray<FEventManager>());

        events.Find(id_)->Add(function_);


    }

    void TriggerEvent(int id_)
    {

        if (events.Contains(id_))
        {

            for (FEventManager functions : *events.Find(id_))
            {
                functions.ExecuteIfBound();
            }


        }


    }


	TMap<int, TArray<FEventManager>> events;

};
