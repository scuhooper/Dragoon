// Fill out your copyright notice in the Description page of Project Settings.

#include "Dragoon.h"
#include "DragoonAIController.h"
#include "DragoonGameMode.h"
#include "AttackState.h"
#include "AlertState.h"

AlertState::AlertState()
{
}

AlertState::~AlertState()
{
}

void AlertState::EnterState( AEnemyAgent* agent ) {
	// have agent look at player
	ADragoonAIController* controller = ( ADragoonAIController* )agent->GetController();
	controller->SetFocus( controller->GetAttackCircle()->GetPlayer() );

	// have agent join the blackboard list for agents ready for combat
	controller->GetGameMode()->blackboard.HaveAgentJoinCombat( agent );

	// initialize position
	position = agent->GetActorLocation();
}

void AlertState::StateTick( AEnemyAgent* agent, float DeltaSeconds ) {
	if ( agent->IsBusy() )
		return;

	// stay near player, but keep distance further out than attack circle
	ADragoonAIController* controller = ( ADragoonAIController* )agent->GetController();
	// subtract agent from player to get the vector for agent going away from the player. (player - agent = vector towards player from agent)
	FVector vectorAwayFromPlayer = position - controller->GetAttackCircle()->GetPlayer()->GetActorLocation();

	// get current distance from player and normalize the vector
	float currentDistanceAway = vectorAwayFromPlayer.Size();
	vectorAwayFromPlayer.Normalize();

	// move to a new position if agent isn't inside the buffer zone
	if ( currentDistanceAway < minDistanceFromPlayer || currentDistanceAway > maxDistanceFromPlayer ) {
		// figure out a point away from the agent to get the preferred distance away from the player
		position = agent->GetActorLocation() + ( vectorAwayFromPlayer * ( FMath::FRandRange( minDistanceFromPlayer, maxDistanceFromPlayer ) - currentDistanceAway ) );
		controller->MoveToLocation( position );
	}

	// check if we can join attack circle to attack player
	if ( controller->GetAttackCircle()->CanAgentJoinCircle( agent ) )
		controller->SwapState( ( State* )new AttackState() );
}

void AlertState::ExitState( AEnemyAgent* agent ) {
	// clear focus
	ADragoonAIController* controller = ( ADragoonAIController* )agent->GetController();
	controller->ClearFocus( EAIFocusPriority::Gameplay );
}
