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
	// enemy is entering state to attack player, they need to join the attack circle
	ADragoonAIController* controller = ( ADragoonAIController* )agent->GetController();
	controller->GetAttackCircle()->JoinCircle( agent );
	controller->SetFocus( controller->GetAttackCircle()->GetPlayer() );

	// draw sword if not currently drawn
	if ( !agent->GetIsSwordDrawn() )
		agent->DrawSword();

	// set a random time for waiting until next attack
	timeBetweenAttacks = FMath::FRandRange( 2.0f, 3.5f );

	// setup initial spot for agent
	position = controller->GetAttackCircle()->GetLocationForAgent( agent );
	controller->MoveToLocation( position );
}

void AttackState::StateTick( AEnemyAgent* agent, float deltaSeconds ) {
	// make sure no other action is taking place for the agent
	if ( agent->IsBusy() )
		return;


	// setup the location the agent should move to, get new attack circle slot if current slot is not on the navmesh
	ADragoonAIController* controller = ( ADragoonAIController* )agent->GetController();
	// move to new destination if agent isn't in the proper place
	if ( position != controller->GetAttackCircle()->GetLocationForAgent( agent ) ) {
		position = controller->GetAttackCircle()->GetLocationForAgent( agent );
		FNavLocation navLoc;
		// make sure assigned position is on navmesh
		if ( controller->navSystem->ProjectPointToNavigation( position, navLoc ) && navLoc.Location.Z <= agent->GetActorLocation().Z + 100 )
			controller->MoveToLocation( position );
		else
			controller->GetAttackCircle()->GetNewSlotForAgent( agent );	// get new slot if current slot is not on the navmesh
	}

	// update timer
	if ( elapsedTime <= timeBetweenAttacks ) {
		elapsedTime += deltaSeconds;
		return;	// exit if timer gets updated, no other logic needed
	}
	
	// perform attack and reset timer
	controller->AttackPlayer();
	timeBetweenAttacks = FMath::FRandRange( 2.0f, 3.5f );
	elapsedTime = 0;
}

void AttackState::ExitState( AEnemyAgent* agent ) {
	// clear focus
	ADragoonAIController* controller = ( ADragoonAIController* )agent->GetController();
	controller->ClearFocus( EAIFocusPriority::Gameplay );
}
