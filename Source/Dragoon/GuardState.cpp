// Fill out your copyright notice in the Description page of Project Settings.

#include "Dragoon.h"
#include "DragoonAIController.h"
#include "PatrolState.h"
#include "GuardState.h"

GuardState::GuardState()
{
}

GuardState::~GuardState()
{
}

void GuardState::EnterState( AEnemyAgent* agent ) {
	ADragoonAIController* controller = ( ADragoonAIController* )agent->GetController();
	controller->targetLoc = agent->GetActorLocation();
	timeToWait = FMath::FRandRange( minWaitTime, maxWaitTime );
}

void GuardState::StateTick( AEnemyAgent* agent, float DeltaSeconds ) {
	ADragoonAIController* controller = ( ADragoonAIController* )agent->GetController();

	if ( agent->GetActorLocation() == controller->targetLoc ) {
		if ( timeToWait > 0 )
			timeToWait -= DeltaSeconds;
		else {
			FNavLocation navLoc;
			controller->navSystem->GetRandomPointInNavigableRadius( agent->guardPost, wanderRange, navLoc );
			controller->targetLoc = navLoc.Location;
			timeToWait = FMath::FRandRange( minWaitTime, maxWaitTime );
		}
	}

	// swap to patrol state if waypoints are setup for agent
	if ( !agent->waypoints.Num() == 0 )
		controller->SwapState( ( State* ) new PatrolState() );
}

void GuardState::ExitState( AEnemyAgent* agent ) {

}
