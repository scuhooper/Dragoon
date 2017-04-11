// Fill out your copyright notice in the Description page of Project Settings.

#include "Dragoon.h"
#include "DragoonAIController.h"
#include "PatrolState.h"
#include "GuardState.h"
#include "AlertState.h"

GuardState::GuardState()
{
}

GuardState::~GuardState()
{
}

void GuardState::EnterState( AEnemyAgent* agent ) {
	// setup the initial variables
	ADragoonAIController* controller = ( ADragoonAIController* )agent->GetController();
	controller->targetLoc = agent->GetActorLocation();
	timeToWait = FMath::FRandRange( minWaitTime, maxWaitTime );
}

void GuardState::StateTick( AEnemyAgent* agent, float DeltaSeconds ) {
	ADragoonAIController* controller = ( ADragoonAIController* )agent->GetController();

	// is agent at the target destination?
	if ( agent->GetActorLocation() == controller->targetLoc ) {
		// see if we have been here long enough
		if ( timeToWait > 0 )
			timeToWait -= DeltaSeconds;	// update the wait timer
		else {
			// get a new destination
			FNavLocation navLoc;
			controller->navSystem->GetRandomPointInNavigableRadius( agent->guardPost, wanderRange, navLoc );	// make sure it is on the navmesh
			controller->targetLoc = navLoc.Location;
			// start moving to new destination
			controller->MoveToLocation( navLoc.Location );

			// set up new wait timer
			timeToWait = FMath::FRandRange( minWaitTime, maxWaitTime );
		}
	}

	// swap to patrol state if waypoints are setup for agent
	if ( !agent->waypoints.Num() == 0 )
		controller->SwapState( ( State* ) new PatrolState() );

	// check if the agent has sensed the player
	for ( auto& actor : controller->perceivedActors ) {
		// if we have seen the player, try to fight
		if ( actor == controller->GetAttackCircle()->GetPlayer() ) {
			controller->SwapState( ( State* )new AlertState() );
			break;
		}
		else
			continue;
	}
}

void GuardState::ExitState( AEnemyAgent* agent ) {

}
