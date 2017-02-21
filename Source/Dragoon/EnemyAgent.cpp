// Fill out your copyright notice in the Description page of Project Settings.

#include "Dragoon.h"
#include "EnemyAgent.h"

AEnemyAgent::AEnemyAgent() {
	PrimaryActorTick.bCanEverTick = true;
}

AEnemyAgent::AEnemyAgent( int score ) {
	enemyScore = score;
}
