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

void AEnemyAgent::MyTakeDamage( int val ) {
	// exit if enemy is already dead
	if ( GetIsDead() )
		return;

	Super::MyTakeDamage( val );
	if ( GetIsDead() ) {
		ADragoonGameMode* game = (ADragoonGameMode*)GetWorld()->GetAuthGameMode();
		game->blackboard.AgentHasDied( this );
	}
}

int AEnemyAgent::ChooseAttack() {
	float rand = FMath::FRandRange( 0, 1 );
	if ( rand > .4f )
		return quickAttackScore;
	else if ( rand > .05f )
		return strongAttackScore;
	else
		return feintAttackScore;
}

void AEnemyAgent::PerformAttack( int attack ) {
	if ( IsActionInProgress() )
		return;
	if ( attack == quickAttackScore )
		BasicAttack();
	else if ( attack == strongAttackScore )
		StrongAttack();
	else if ( attack == feintAttackScore )
		FeintAttack();
	else {
		UE_LOG( LogTemp, Error, TEXT( "An invalid attack score was sent for agent %s!" ), *this->GetName() );
	}
}

void AEnemyAgent::ParryAttack( EAttackDirection dir ) {
	directionOfAttack = ( uint8 )dir;
	bIsParrying = true;
}

bool AEnemyAgent::IsBusy() {
	return IsActionInProgress();
}

void AEnemyAgent::BasicAttack() {
	AttackDirectionChosen();	// get the actual vector for attack direction
	bIsAttacking = true;	// set attacking to true for animBP
}

void AEnemyAgent::AttackDirectionChosen() {
	directionOfAttack = ( uint8 )FMath::RandRange( 0, 8 );
}

void AEnemyAgent::BeginPlay() {
	Super::BeginPlay();
}
