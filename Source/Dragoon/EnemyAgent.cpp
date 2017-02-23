// Fill out your copyright notice in the Description page of Project Settings.

#include "Dragoon.h"
#include "DragoonGameMode.h"
#include "EnemyAgent.h"

AEnemyAgent::AEnemyAgent() {
	PrimaryActorTick.bCanEverTick = true;
	enemyScore = 4;
	quickAttackScore = 3;
	strongAttackScore = 5;
	feintAttackScore = 8;
}

AEnemyAgent::AEnemyAgent( int score ) {
	PrimaryActorTick.bCanEverTick = true;
	enemyScore = score;
	quickAttackScore = 3;
	strongAttackScore = 5;
	feintAttackScore = 8;
}

void AEnemyAgent::BeginPlay() {
	ADragoonGameMode* game = ( ADragoonGameMode* )GetWorld()->GetAuthGameMode();
	game->blackboard.RegisterAgent( this );
	DrawSword();
}

void AEnemyAgent::DrawSword() {
	SheatheUnsheatheSword();
}
