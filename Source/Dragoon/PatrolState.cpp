// Fill out your copyright notice in the Description page of Project Settings.

#include "Dragoon.h"
#include "DragoonAIController.h"
#include "GuardState.h"
#include "PatrolState.h"

PatrolState::PatrolState()
{
}

PatrolState::~PatrolState()
{
}

void PatrolState::EnterState( AEnemyAgent* agent ) {
	// setup initial time to wait
	if ( !agent->bIsPatrolContinuous )
		timeToWait = FMath::FRandRange( minWaitTime, maxWaitTime );
}

void PatrolState::StateTick( AEnemyAgent* agent, float DeltaSeconds ) {
	ADragoonAIController* controller = ( ADragoonAIController* )agent->GetController();

	// swap to guard state if no waypoints are setup for agent
	if ( agent->waypoints.Num() == 0 )
		controller->SwapState( ( State* )new GuardState() );

	// check if we are at the current waypoint
	if ( agent->GetActorLocation() == agent->waypoints[ currentWaypoint ] ) {
		// agent waits and observes
		if ( !agent->bIsPatrolContinuous ) {
			// wait out timer
			if ( timeToWait > 0 )
				timeToWait -= DeltaSeconds;
			else {
				// setup next waypoint location
				UpdateWaypoint( agent );
				// set new wait timer
				timeToWait = FMath::FRandRange( minWaitTime, maxWaitTime );
			}
		}
		// agent doesn't wait at location
		else {
			// setup next waypoint location
			UpdateWaypoint( agent );
		}
	}
}

void PatrolState::ExitState( AEnemyAgent* agent ) {

}

void PatrolState::UpdateWaypoint( AEnemyAgent* agent ) {
	// grab handle for AI controller
	ADragoonAIController* controller = ( ADragoonAIController* )agent->GetController();

	// get next control point
	currentWaypoint++;
	// make sure we stay within the index bounds of the waypoint array
	if ( currentWaypoint == agent->waypoints.Num() )
		currentWaypoint = 0;
	// set new destination
	controller->targetLoc = agent->waypoints[ currentWaypoint ];
}
