// Fill out your copyright notice in the Description page of Project Settings.

#include "Dragoon.h"
#include "AttackCircle.h"

AttackCircle::AttackCircle()
{
	// set pointer to nullptr
	player = nullptr;
}

AttackCircle::~AttackCircle()
{
	// set pointers to null or remove references
	player = nullptr;
	circleSlotOccupant.Empty();
}

AttackCircle::AttackCircle( ADragoonCharacter* playerCharacter ) {
	// set player pointer and initialize the circle's arrays
	player = playerCharacter;
	Initialize();
}

void AttackCircle::JoinCircle( AEnemyAgent* attacker ) {
	// if attacker's score <= available enemy score
	if ( attacker->GetEnemyScore() <= availableEnemyScore ) {
		// add attacker to enemies in circle
		AssignAgentToSlot( attacker, CheckForClosestAvailableSlot( attacker ) );
		// reduce available score by attacker's score
		availableEnemyScore -= attacker->GetEnemyScore();
		// add enemy to list of enemy's in circle
		enemiesInCircle.Add( attacker );
		// set enemy to be in combat
		attacker->GetIsInCombat();
	}
}

void AttackCircle::UpdateCircleLocation() {
	if ( !player )	// check if player has bee set
		return;

	centerOfCircle = player->GetActorLocation();	// circle is centered on wherever the player is
}

void AttackCircle::RemoveAgentFromCircle( AEnemyAgent* agent ) {
	// check for which slot agent belongs to and then remove them from it
	for ( auto& slot : circleSlotOccupant ) {
		if ( slot.Value == agent ) {
			circleSlotOccupied[ slot.Key ] = false;	// set slot to be unoccupied
			circleSlotOccupant.Remove( slot.Key );	// remove entry from occupant map
			return;
		}
	}

	// handling for agent not found
	UE_LOG( LogTemp, Error, TEXT( "Attempted to remove agent %s who is not currently in the circle!" ), *agent->GetName() );
}

FVector AttackCircle::GetLocationForAgent( AEnemyAgent* agent ) {
	// check for which slot agent is assigned to and then return the location of that slot
	if ( ( !circleSlotOccupant.FindKey( agent ) ) )
		return agent->GetActorLocation();	// if requester is not assigned a slot, return its position

	UpdateCircleLocation();	// update circle with player's latest location

	// get which slot is assigned to requester
	EAttackCircleSlot slot = *( circleSlotOccupant.FindKey( agent ) );

	// return the center plus offset of slot
	return centerOfCircle + *circleSlotOffset.Find( slot );
}

bool AttackCircle::CanAgentPerformAttack( int attackScore ) {
	if ( attackScore <= availableAttackScore ) {	// check if attack will be valid before allowing it
		availableAttackScore -= attackScore;	// reduce available attack score
		return true;
	}
	else
		return false;
}

void AttackCircle::AgentAttackFinished( int attackScore ) {
	availableAttackScore += attackScore;	// add the finished attack's score to the available score

											// shouldn't happen, but checking just to ensure
	if ( availableAttackScore > maxAttackScore )
		availableAttackScore = maxAttackScore;
}

void AttackCircle::SetPlayer( ADragoonCharacter* newPlayer ) {
	player = newPlayer;	// update the player reference to supplied pointer
}

void AttackCircle::Initialize() {

	// set available scores to max scores
	availableEnemyScore = maxEnemyScore;
	availableAttackScore = maxAttackScore;

	// set up slots to not be occupied
	circleSlotOccupied.Add( EAttackCircleSlot::ACS_Front, false );
	circleSlotOccupied.Add( EAttackCircleSlot::ACS_FrontRight, false );
	circleSlotOccupied.Add( EAttackCircleSlot::ACS_FrontLeft, false );
	circleSlotOccupied.Add( EAttackCircleSlot::ACS_Left, false );
	circleSlotOccupied.Add( EAttackCircleSlot::ACS_Right, false );
	circleSlotOccupied.Add( EAttackCircleSlot::ACS_Back, false );
	circleSlotOccupied.Add( EAttackCircleSlot::ACS_BackRight, false );
	circleSlotOccupied.Add( EAttackCircleSlot::ACS_BackLeft, false );

	// set up offset with vectors
	circleSlotOffset.Add( EAttackCircleSlot::ACS_Front, FVector( 1, 0, 0 ) );
	circleSlotOffset.Add( EAttackCircleSlot::ACS_FrontRight, FVector( 1, 1, 0 ) );
	circleSlotOffset.Add( EAttackCircleSlot::ACS_FrontLeft, FVector( 1, -1, 0 ) );
	circleSlotOffset.Add( EAttackCircleSlot::ACS_Left, FVector( 0, -1, 0 ) );
	circleSlotOffset.Add( EAttackCircleSlot::ACS_Right, FVector( 0, 1, 0 ) );
	circleSlotOffset.Add( EAttackCircleSlot::ACS_Back, FVector( -1, 0, 0 ) );
	circleSlotOffset.Add( EAttackCircleSlot::ACS_BackRight, FVector( -1, 1, 0 ) );
	circleSlotOffset.Add( EAttackCircleSlot::ACS_BackLeft, FVector( -1, -1, 0 ) );

	// normalize vectors then multiply by the scale of the offset
	for ( auto& pair : circleSlotOffset ) {
		pair.Value.Normalize();
		pair.Value *= offsetScale;
		circleSlotOffset[ pair.Key ] = pair.Value;
	}

	// establish grid based on character position
	UpdateCircleLocation();
}

EAttackCircleSlot AttackCircle::CheckForClosestAvailableSlot( AEnemyAgent* requester ) {
	// setup local variables
	FVector requesterLocation = requester->GetActorLocation();
	EAttackCircleSlot bestSlot = EAttackCircleSlot::ACS_Front;
	TArray<EAttackCircleSlot> freeSlots;
	float bestDistance = NULL;	// start with a null distance

	// get list of empty spots
	for ( auto& Location : circleSlotOccupied ) {
		// check TMap for empty slots to add to array
		if ( !Location.Value )
			freeSlots.Add( Location.Key );
	}

	float currentDistance;
	// check empty slots for shortest distance to requester
	for ( auto& slot : freeSlots ) {
		// calculate the distance squared
		currentDistance = FVector::DistSquared( requesterLocation, centerOfCircle + circleSlotOffset[ slot ] );

		if ( bestDistance == NULL ) {	// if bestDistance is null, this is the closest spot
			bestDistance = currentDistance;
			bestSlot = slot;
		}
		else if ( currentDistance < bestDistance ) {	// update best variables if current slot is closer than best slot
			bestDistance = currentDistance;
			bestSlot = slot;
		}
	}

	freeSlots.Empty();	// destroy array when finished

	// return the closest slot to requester
	return bestSlot;
}

void AttackCircle::AssignAgentToSlot( AEnemyAgent* agent, EAttackCircleSlot slot ) {
		circleSlotOccupant.Add( slot, agent );	// assign agent to slot
		circleSlotOccupied[ slot ] = true;	// mark slot as occupied
}
