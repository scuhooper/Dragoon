// Fill out your copyright notice in the Description page of Project Settings.

#include "Dragoon.h"
#include "AttackCircle.h"

UAttackCircle::UAttackCircle() {

}

UAttackCircle::UAttackCircle( ADragoonCharacter* playerCharacter ) {
	availableEnemyScore = maxEnemyScore;
	availableAttackScore = maxAttackScore;
	player = playerCharacter;

	// establish grid based on character position and rotation
	UpdateCircleLocation();
	UpdateCircleRotation();
}

void UAttackCircle::JoinCircle( ADragoonCharacter* attacker ) {
	// if attacker's score <= available enemy score
		// add attacker to enemies in circle
		// reduce available score by attacker's score
}

void UAttackCircle::UpdateCircleLocation() {
	centerOfCircle = player->GetActorLocation();
}

void UAttackCircle::UpdateCircleRotation() {
	directionOfCircle = player->GetActorRotation();
}
