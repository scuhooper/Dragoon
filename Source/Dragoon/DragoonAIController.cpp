// Fill out your copyright notice in the Description page of Project Settings.

#include "Dragoon.h"
#include "AttackState.h"
#include "AlertState.h"
#include "DragoonAIController.h"

ADragoonAIController::ADragoonAIController() {
	// do super awesome AI stuff
}

ADragoonAIController::~ADragoonAIController() {
	// remove all references
	attackCircle = nullptr;
	agent = nullptr;
	game = nullptr;
	delete currentState;
	currentState = nullptr;
}

void ADragoonAIController::AgentHasDied() {
	// remove agent from attack circle and blackboard
	if(attackCircle->GetEnemiesInCircle().Contains(agent))
		attackCircle->RemoveAgentFromCircle( agent );

	game->blackboard.RemoveAgent( agent );

	UnPossess();
	Destroy();
}

void ADragoonAIController::AttackPlayer() {
	int attackChoice = agent->ChooseAttack();
	if ( attackCircle->CanAgentPerformAttack( attackChoice ) )
		agent->PerformAttack( attackChoice );
}

void ADragoonAIController::SwapState( State* newState ) {
	if ( newState ) {
		nextState = newState;
		bIsStateChangeReady = true;
	}
}

void ADragoonAIController::Tick( float DeltaSeconds ) {
	if ( agent->GetIsDead() )
		return;

	// do stuff every frame
	Super::Tick( DeltaSeconds );
	
	/*********
	!!!!!TESTING BELOW!!!!!
	*********/

	if ( currentState )
		currentState->StateTick( agent, DeltaSeconds );

	if ( bIsStateChangeReady && nextState )
		TransitionBetweenStates();

	MoveToLocation( targetLoc );
}

void ADragoonAIController::BeginPlay() {
	// call parent begin play
	Super::BeginPlay();

	// setup pointer variables
	game = ( ADragoonGameMode* )GetWorld()->GetAuthGameMode();
	agent = ( AEnemyAgent* )GetCharacter();
	attackCircle = &game->attackCircle;
	navSystem = GetWorld()->GetNavigationSystem();

	// register agent with blackboard
	game->blackboard.RegisterAgent( agent );

	// setup initial state for agent
	currentState = ( State* )new AlertState();
	nextState = ( State* )new AlertState();
	bIsStateChangeReady = true;
}

void ADragoonAIController::TransitionBetweenStates() {
	// begin transition
	currentState->ExitState( agent );

	// update pointers to the new states
	State* oldState = currentState;
	currentState = nextState;
	
	// reset next state and remove the old state object
	nextState = nullptr;
	delete oldState;

	// start up the new state
	if ( currentState )
		currentState->EnterState( agent );
	
	// state change has completed
	bIsStateChangeReady = false;
}
