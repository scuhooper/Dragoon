// Fill out your copyright notice in the Description page of Project Settings.

#include "Dragoon.h"
#include "DragoonAIController.h"

ADragoonAIController::ADragoonAIController() {
	// do super awesome AI stuff
}

ADragoonAIController::~ADragoonAIController() {
	// AgentHasDied();
	attackCircle = nullptr;
	agent = nullptr;
	game = nullptr;
}

void ADragoonAIController::AgentHasDied() {
	attackCircle->RemoveAgentFromCircle( agent );
	game->blackboard.RemoveAgent( agent );
}

void ADragoonAIController::Tick( float DeltaSeconds ) {
	// do stuff every frame
	Super::Tick( DeltaSeconds );
	
	if ( !attackCircle->GetEnemiesInCircle().Contains( agent ) ) {
		attackCircle->JoinCircle( agent );
		if ( attackCircle->GetEnemiesInCircle().Contains( agent ) ) {
			SetFocus( attackCircle->GetPlayer() );
			agent->JoinCombat();
			game->blackboard.HaveAgentJoinCombat( agent );
		}
	}

	MoveToLocation( attackCircle->GetLocationForAgent( agent ) );
}

void ADragoonAIController::BeginPlay() {
	Super::BeginPlay();
	game = ( ADragoonGameMode* )GetWorld()->GetAuthGameMode();
	agent = ( AEnemyAgent* )GetCharacter();
	attackCircle = &game->attackCircle;
	game->blackboard.RegisterAgent( agent );
}
