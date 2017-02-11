// Fill out your copyright notice in the Description page of Project Settings.

#include "Dragoon.h"
#include "AttackCircle.h"

UAttackCircle::UAttackCircle() {
	availableEnemyScore = maxEnemyScore;
	availableAttackScore = maxAttackScore;
}

void UAttackCircle::JoinCircle( ADragoonCharacter* attacker ) {
	// if attacker's score <= available enemy score
		// add attacker to enemies in circle
		// reduce available score by attacker's score
}
