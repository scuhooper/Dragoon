// Fill out your copyright notice in the Description page of Project Settings.

#include "Dragoon.h"
#include "DragoonAIController.h"
#include "AttackState.h"

AttackState::AttackState()
{
}

AttackState::~AttackState()
{
}

void AttackState::EnterState( AEnemyAgent* agent ) {
	// draw sword if not currently drawn
	if ( !agent->GetIsSwordDrawn() )
		agent->DrawSword();

	timeBetweenAttacks = FMath::FRandRange( 3.0f, 5.0f );
}

void AttackState::StateTick( AEnemyAgent* agent, float deltaSeconds ) {
	if ( elapsedTime <= timeBetweenAttacks ) {
		elapsedTime += deltaSeconds;
		return;
	}

	ADragoonAIController* controller = ( ADragoonAIController* )agent->GetController();
	controller->AttackPlayer();
	timeBetweenAttacks = FMath::FRandRange( 3.0f, 5.0f );
	elapsedTime = 0;
}

void AttackState::ExitState( AEnemyAgent* agent ) {

}
