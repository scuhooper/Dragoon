// Fill out your copyright notice in the Description page of Project Settings.

#include "Dragoon.h"
#include "AttackCircle.h"

UAttackCircle::UAttackCircle() {
	// default c-tor required by UObject
}

UAttackCircle::UAttackCircle( ADragoonCharacter* playerCharacter ) {
	// set available scores to max scores
	availableEnemyScore = maxEnemyScore;
	availableAttackScore = maxAttackScore;
	player = playerCharacter;	// set the player reference

	// set up offset with vectors
	circleSlotOffset[ EAttackCircleSlot::ACS_Front ] = FVector( 1, 0, 0 );
	circleSlotOffset[ EAttackCircleSlot::ACS_FrontRight ] = FVector( 1, 1, 0 );
	circleSlotOffset[ EAttackCircleSlot::ACS_FrontLeft ] = FVector( 1, -1, 0 );
	circleSlotOffset[ EAttackCircleSlot::ACS_Left ] = FVector( 0, -1, 0 );
	circleSlotOffset[ EAttackCircleSlot::ACS_Right ] = FVector( 0, 1, 0 );
	circleSlotOffset[ EAttackCircleSlot::ACS_Back ] = FVector( -1, 0, 0 );
	circleSlotOffset[ EAttackCircleSlot::ACS_BackRight ] = FVector( -1, 1, 0 );
	circleSlotOffset[ EAttackCircleSlot::ACS_BackLeft ] = FVector( -1, -1, 0 );

	// normalize vectors then multiply by the scale of the offset
	for ( auto& pair : circleSlotOffset ) {
		pair.Value.Normalize();
		pair.Value * offsetScale;
	}

	// establish grid based on character position
	UpdateCircleLocation();
}

void UAttackCircle::JoinCircle( ADragoonCharacter* attacker ) {
	// if attacker's score <= available enemy score
		// add attacker to enemies in circle
		// reduce available score by attacker's score
}

void UAttackCircle::UpdateCircleLocation() {
	centerOfCircle = player->GetActorLocation();	// circle is centered on wherever the player is

	/*********
	!!!!! TESTING BELOW CODE !!!!!
	*********/
	for ( auto& Location : circleSlotLocations ) {
		Location.Value = centerOfCircle + circleSlotOffset[ Location.Key ]; // update the slot location from the center of the circle and it's stored offset for particular slot
	}
}

void UAttackCircle::RemoveAgentFromCircle( ADragoonCharacter* agent ) {
	// check for which slot agent belongs to and then remove them from it
	for ( auto& slot : circleSlotOccupant ) {
		if ( slot.Value == agent ) {
			slot.Value = nullptr;
			return;
		}
	}

	// handling for agent not found
	UE_LOG( LogTemp, Error, TEXT( "Attempted to remove agent %s who is not currently in the circle!" ), *agent->GetName() );
}

FVector UAttackCircle::GetLocationForAgent( ADragoonCharacter* agent ) {
	// check for which slot agent is assigned to and then return the location of that slot
	for ( auto& Location : circleSlotOccupant ) {
		if ( Location.Value == agent ) {
			return circleSlotLocations[ Location.Key ];
		}
	}

	// handling for agent not found
	UE_LOG( LogTemp, Error, TEXT( "Agent %s is not assigned to a slot on the attack circle!" ), *agent->GetName() );
	return agent->GetActorLocation();
}

EAttackCircleSlot UAttackCircle::CheckForClosestAvailableSlot( ADragoonCharacter* requester ) {
	FVector requesterLocation = requester->GetActorLocation();
	EAttackCircleSlot bestSlot;
	TArray<EAttackCircleSlot> freeSlots;
	float bestDistance = NULL;	// start with a null distance

	// get list of empty spots
	for ( auto& Location : circleSlotOccupant ) {
		// check TMap for empty slots to add to array
		if ( Location.Value == nullptr )
			freeSlots.Add( Location.Key );
	}

	// check empty slots for shortest distance to requester
	for ( auto& slot : freeSlots ) {
		// calculate the distance squared
		float currentDistance = FVector::DistSquared( requesterLocation, circleSlotLocations[ slot ] );

		if ( bestDistance == NULL ) {	// if bestDistance is null, this is the closest spot
			bestDistance = currentDistance;
			bestSlot = slot;
		}
		else if ( currentDistance < bestDistance ) {	// update best variables if current slot is closer than best slot
			bestDistance = currentDistance;
			bestSlot = slot;
		}
	}
	
	// return the closest slot to requester
	return bestSlot;
}

void UAttackCircle::AssignAgentToSlot( ADragoonCharacter* agent, EAttackCircleSlot slot ) {
	// error check for if the slot is currently assigned to another agent
	if ( circleSlotOccupant[ slot ] != nullptr ) {
		UE_LOG( LogTemp, Error, TEXT( "Attempted to assign agent %s to an attack circle slot that was not empty" ), *agent->GetName() );
	}
	else {
		circleSlotOccupant[ slot ] = agent;	// assign agent to slot
	}
}
