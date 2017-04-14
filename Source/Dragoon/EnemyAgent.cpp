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

	// run the base damage function to change health levels
	Super::MyTakeDamage( val );

	// let game know agent is dead so it can remove agent from active agents
	if ( GetIsDead() ) {
		AgentDied();
	}
}

int AEnemyAgent::ChooseAttack() {
	// randomly choose type of attack to perform
	float rand = FMath::FRandRange( 0, 1 );
	if ( rand > .4f )
		return quickAttackScore;
	else if ( rand > .05f )
		return strongAttackScore;
	else
		return feintAttackScore;
}

void AEnemyAgent::PerformAttack( int attack ) {
	// make sure agent is not busy
	if ( IsActionInProgress() )
		return;
	// call relevant attack function for agent
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
	// perform parry corresponding to direction of attack provided
	directionOfAttack = ( uint8 )dir;
	bIsParrying = true;	// set to true so animation will trigger
}

bool AEnemyAgent::IsBusy() {
	return IsActionInProgress();
}

void AEnemyAgent::DodgeAttack( EAttackDirection dir ) {
	// choose which direction to dodge based on predicted atttack direction
	if ( dir == EAttackDirection::AD_DownwardLeftSlash || dir == EAttackDirection::AD_LeftSlash || dir == EAttackDirection::AD_UpwardLeftSlash )
		moveRight = 1;
	else if ( dir == EAttackDirection::AD_DownwardRightSlash || dir == EAttackDirection::AD_RightSlash || dir == EAttackDirection::AD_UpwardRightSlash )
		moveRight = -1;
	else
		moveForward = -1;
	// set dodge to true so animation plays
	bIsDodging = true;
}

void AEnemyAgent::BasicAttack() {
	AttackDirectionChosen();	// get the actual vector for attack direction
	bIsAttacking = true;	// set attacking to true for animBP
}

void AEnemyAgent::BeginPlay() {
	Super::BeginPlay();

	// set guard location to current location if one is not set
	if ( guardPost == FVector::ZeroVector )
		guardPost = GetActorLocation();
}

void AEnemyAgent::Tick( float deltaSeconds ) {
	Super::Tick( deltaSeconds );	// call parent function to ensure continuity

	// get velocity to interpret movement when sword is drawn
	if ( GetIsSwordDrawn() ) {
		FVector velocity = GetVelocity();
		velocity.Normalize();

		// get the local value of the velocity
		velocity = GetTransform().GetRotation().UnrotateVector( velocity );

		// translate it into variables used by anim BP
		moveForward = velocity.X;
		moveRight = velocity.Y;
	}
}

void AEnemyAgent::AttackDirectionChosen() {
	// get a random direction for enemy agent's attack
	directionOfAttack = ( uint8 )FMath::RandRange( 0, 8 );
}

void AEnemyAgent::AgentDied() {
	// inform ai systems that agent has died
	ADragoonGameMode* game = ( ADragoonGameMode* )GetWorld()->GetAuthGameMode();
	game->blackboard.AgentHasDied( this );

	// disable enemy collision
	SetActorEnableCollision( false );
	// spawn the emitter at center of chest emblem
	UGameplayStatics::SpawnEmitterAttached( emitter, this->GetMesh(), TEXT( "spine_03" ), FVector( 7.5f, 10, 0 ) );
}
