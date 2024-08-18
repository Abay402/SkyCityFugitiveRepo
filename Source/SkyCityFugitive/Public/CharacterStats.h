// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class SKYCITYFUGITIVE_API CharacterStats
{

private:

	float health;
	float maxHealth;
	int stims;

public:
	CharacterStats();
	CharacterStats(float health_, float maxHealth_, int stims_);
	~CharacterStats();


	void SetHealth(float health_) { health = health_; }
	void SetMaxHealth(float maxHealth_) { maxHealth = maxHealth_; }
	void SetStims(int stims_) { stims = stims_; }


	float GetHealth() { return health; }
	float GetMaxHealth() { return maxHealth; }
	int GetStims() { return stims; }
	void TakeDamage(float damage_);
	void Heal(float healthIncrease_);

	
};
