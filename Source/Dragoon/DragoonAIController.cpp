// Fill out your copyright notice in the Description page of Project Settings.

#include "Dragoon.h"
#include "DragoonAIController.h"

ADragoonAIController::ADragoonAIController() {
	// do super awesome AI stuff
}

ADragoonAIController::~ADragoonAIController() {
	// remove all references
	attackCircle = nullptr;
	agent = nullptr;
	game = nullptr;
}

void ADragoonAIController::AgentHasDied() {
	// remove agent from attack circle and blackboard
	if(attackCircle->GetEnemiesInCircle().Contains(agent))
		attackCircle->RemoveAgentFromCircle( agent );

	game->blackboard.RemoveAgent( agent );
}

void ADragoonAIController::Tick( float DeltaSeconds ) {
	// do stuff every frame
	Super::Tick( DeltaSeconds );
	
	/*********
	!!!!!TESTING BELOW!!!!!
	*********/

	if ( !attackCircle->GetEnemiesInCircle().Contains( agent ) ) {
		attackCircle->JoinCircle( agent );
		if ( attackCircle->GetEnemiesInCircle().Contains( agent ) ) {
			SetFocus( attackCircle->GetPlayer() );
			game->blackboard.HaveAgentJoinCombat( agent );
		}
	}

	FVector targetLoc = attackCircle->GetLocationForAgent( agent );
	FNavLocation navLoc;
	
	if ( GetWorld()->GetNavigationSystem()->ProjectPointToNavigation( targetLoc, navLoc ) && navLoc.Location.Z < agent->GetActorLocation().Z + 100  ) {
		MoveToLocation( targetLoc );
	}
	else {
		UE_LOG( LogTemp, Warning, TEXT( "Getting a new slot. Old slot was %s" ), *targetLoc.ToString() );
		attackCircle->GetNewSlotForAgent( agent );
		UE_LOG( LogTemp, Warning, TEXT( "New slot is %s" ), *attackCircle->GetLocationForAgent( agent ).ToString() );
	}

}

void ADragoonAIController::BeginPlay() {
	// call parent begin play
	Super::BeginPlay();
	// setup pointer variables
	game = ( ADragoonGameMode* )GetWorld()->GetAuthGameMode();
	agent = ( AEnemyAgent* )GetCharacter();
	attackCircle = &game->attackCircle;
	// register agent with blackboard
	game->blackboard.RegisterAgent( agent );
}
