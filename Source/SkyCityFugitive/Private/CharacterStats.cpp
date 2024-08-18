// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStats.h"

CharacterStats::CharacterStats()
{

	health = 0;
	maxHealth = 0;
	stims = 0;


}

CharacterStats::CharacterStats(float health_, float maxHealth_, int stims_)
{

	health = health_;
	maxHealth = maxHealth_;
	stims = stims_;

}

CharacterStats::~CharacterStats()
{




}
void CharacterStats::TakeDamage(float damage_)
{

	health -= damage_;

	if (health < 0) health = 0;


}

void CharacterStats::Heal(float healthIncrease_)
{
	if (stims != 0)
	{
		health += healthIncrease_;

		if (health > maxHealth) health = maxHealth;
	}

	stims--;
	if (stims < 0) stims = 0;
}
