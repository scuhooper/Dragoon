// Fill out your copyright notice in the Description page of Project Settings.

#include "Dragoon.h"
#include "DragoonGameMode.h"
#include "EnemyAgent.h"

AEnemyAgent::AEnemyAgent() {
	// make actor able to tick
	PrimaryActorTick.bCanEverTick = true;
	// setup scores for attacks and enemy score
	enemyScore = 4;
	quickAttackScore = 3;
	strongAttackScore = 5;
	feintAttackScore = 8;
}

AEnemyAgent::AEnemyAgent( int score ) {
	// make actor able to tick
	PrimaryActorTick.bCanEverTick = true;

	// setup scores for attacks and enemy score
	enemyScore = score;
	quickAttackScore = 3;
	strongAttackScore = 5;
	feintAttackScore = 8;
}

void AEnemyAgent::BeginPlay() {
	Super::BeginPlay();
}

void AEnemyAgent::DrawSword() {
	SheatheUnsheatheSword(); // equip/unequip sword
}

void AEnemyAgent::JoinCombat() {
	// set bool and draw sword
	bIsInCombat = true;
	DrawSword();
}

void AEnemyAgent::LeaveCombat() {
	// set bool and sheathe sword
	bIsInCombat = false;
	DrawSword();
}
