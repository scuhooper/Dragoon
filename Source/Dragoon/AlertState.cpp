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
	controller->targetLoc = FVector( 50000, 50000, 0 );
}

void AlertState::StateTick( AEnemyAgent* agent, float DeltaSeconds ) {
	// stay near player, but keep distance further out than attack circle
	ADragoonAIController* controller = ( ADragoonAIController* )agent->GetController();
	FNavLocation navLoc;
	if ( !controller->navSystem->ProjectPointToNavigation( controller->targetLoc, navLoc ) ) {
		controller->navSystem->GetRandomReachablePointInRadius( controller->GetAttackCircle()->GetPlayer()->GetActorLocation(), 1, navLoc );
		controller->targetLoc = navLoc.Location * FMath::FRandRange( minDistanceFromPlayer, maxDistanceFromPlayer );
	}
	else
		controller->targetLoc = navLoc.Location;

	// check if we can join attack circle to attack player
	if ( controller->GetAttackCircle()->CanAgentJoinCircle( agent ) )
		controller->SwapState( ( State* )new AttackState() );	// TODO add in method for changing states, needs to delete old state and set a new state
}

void AlertState::ExitState( AEnemyAgent* agent ) {
	// clear focus
	ADragoonAIController* controller = ( ADragoonAIController* )agent->GetController();
	controller->ClearFocus( EAIFocusPriority::Gameplay );
}
