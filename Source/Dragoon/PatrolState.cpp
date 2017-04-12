// Fill out your copyright notice in the Description page of Project Settings.

#include "Dragoon.h"
#include "DragoonAIController.h"
#include "GuardState.h"
#include "PatrolState.h"
#include "AlertState.h"

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

	// get the first waypoint for agent
	UpdateWaypoint( agent );

	// set walk speed to look like normal marching
	agent->GetCharacterMovement()->MaxWalkSpeed = 300;
}

void PatrolState::StateTick( AEnemyAgent* agent, float DeltaSeconds ) {
	ADragoonAIController* controller = ( ADragoonAIController* )agent->GetController();

	// swap to guard state if no waypoints are setup for agent
	if ( agent->waypoints.Num() == 0 )
		controller->SwapState( ( State* )new GuardState() );

	// check if we are at the current waypoint
	if ( FVector::PointsAreNear( agent->GetActorLocation(), agent->waypoints[ currentWaypoint ], 50 ) ) {
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

void PatrolState::ExitState( AEnemyAgent* agent ) {
	// reset walking speed to normal value
	agent->GetCharacterMovement()->MaxWalkSpeed = 600;
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
	controller->MoveToLocation( agent->waypoints[ currentWaypoint ] );
}
