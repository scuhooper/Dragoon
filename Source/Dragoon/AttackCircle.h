// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DragoonCharacter.h"

/**
 * Class for handling which enemies can be in combat with the player at any one given time. Controls number of enemies around the player, attacks against the player which can be happening simultaneously, and the position of enemies around the player.
 */
class DRAGOON_API AttackCircle
{
public:
	int maxEnemyScore;

	int maxAttackScore;

private:
	ADragoonCharacter* player;

	int currentEnemyScore;

	int currentAttackScore;
public:
	AttackCircle();
	~AttackCircle();
};
