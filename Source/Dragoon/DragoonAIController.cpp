// Fill out your copyright notice in the Description page of Project Settings.

#include "Dragoon.h"
#include "AttackState.h"
#include "AlertState.h"
#include "GuardState.h"
#include "PatrolState.h"
#include "DragoonAIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISenseConfig_Sight.h"

ADragoonAIController::ADragoonAIController() {
	// setup AI Perception system
	UAIPerceptionComponent* perception = CreateDefaultSubobject<UAIPerceptionComponent>( TEXT( "Perception Component" ) );
	SetPerceptionComponent( *perception );
	UAISenseConfig_Sight *sightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>( TEXT( "Sight Config" ) );

	// configure sight sense parameters
	sightConfig->SightRadius = 1000;
	sightConfig->LoseSightRadius = 1500;
	sightConfig->PeripheralVisionAngleDegrees = 90;
	sightConfig->DetectionByAffiliation.bDetectEnemies = true;
	sightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	sightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	// register sight sense with perception system
	GetAIPerceptionComponent()->ConfigureSense( *sightConfig );
	GetAIPerceptionComponent()->SetDominantSense( sightConfig->GetSenseImplementation() );
	GetAIPerceptionComponent()->OnPerceptionUpdated.AddDynamic( this, &ADragoonAIController::SenseUpdate );
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
	if ( attackCircle->GetEnemiesInCircle().Contains( agent ) )
		attackCircle->RemoveAgentFromCircle( agent );

	game->blackboard.RemoveAgent( agent );

	// stop controlling the agent and destroy this controller
	UnPossess();
	Destroy();
}

void ADragoonAIController::AttackPlayer() {
	// choose what type of attack to make
	int attackChoice = agent->ChooseAttack();
	// check with the attack circle to see if it can be performed 
	if ( attackCircle->CanAgentPerformAttack( attackChoice ) )
		agent->PerformAttack( attackChoice );
}

void ADragoonAIController::SwapState( State* newState ) {
	// if newstate exists...
	if ( newState ) {
		nextState = newState;	// update our state logic
		bIsStateChangeReady = true;	// set boolean to alert logic to begin state change
	}
}

void ADragoonAIController::ReactToIncomingAttack( int attackID, float confidenceInAttack ) {
	// check if agent is able to react to the attack
	if ( agent->IsBusy() )
		return;

	EAttackDirection attackDirection;
	EAttackType attackType;
	// use float to get whether to trust prediction
	if ( FMath::FRand() < confidenceInAttack )
	{
		// trust in the predicted attack
		// break attackID apart to get attack type and direction
		attackDirection = ( EAttackDirection )( attackID % 9 );	// modulo 9 will return a value between 0 and 8 that correlates to the direction enum
		attackType = ( EAttackType )( attackID / 9 );	// dividing by 9 will return a value between 0 and 2 that correlates to the type enum
	}
	else {
		// distrust prediction
		int newAttack = FMath::RandRange( 0, 26 );	// get random attack ID
		// break newAttack apart to get attack type and direction
		attackDirection = ( EAttackDirection )( newAttack % 9 );	// modulo 9 will return a value between 0 and 8 that correlates to the direction enum
		attackType = ( EAttackType )( newAttack / 9 );	// dividing by 9 will return a value between 0 and 2 that correlates to the type enum
	}

	// choose appropriate response based on type of attack
	if ( attackType == EAttackType::AT_Quick ) {
		// react to quick attack by parrying
		agent->ParryAttack( attackDirection );
	}
	else if ( attackType == EAttackType::AT_Strong ) {
		// react to strong attacks by dodging
		agent->DodgeAttack( attackDirection );
	}
	else if ( attackType == EAttackType::AT_Feint ) {
		// react to feints with either doing nothing or attacking the player
		float choice = FMath::FRand();
		if ( choice < .75f ) {
			// choose to do nothing
			return;
		}
		else {
			// respond to the player's feint with an attack
			AttackPlayer();
		}
	}
}

void ADragoonAIController::Tick( float DeltaSeconds ) {
	// make sure agent is alive and valid
	if ( agent->GetIsDead() )
		return;

	// do stuff every frame
	Super::Tick( DeltaSeconds );
	
	/*********
	!!!!!TESTING BELOW!!!!!
	*********/

	// if we have a state in the FSM, run its behavior
	if ( currentState )
		currentState->StateTick( agent, DeltaSeconds );

	// check if the state is attempting to move to a new state
	if ( bIsStateChangeReady && nextState )
		TransitionBetweenStates();	// move to new state
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
	if ( agent->waypoints.Num() == 0 )
		currentState = ( State* )new GuardState();
	else
		currentState = ( State* )new PatrolState();

	// make sure state is started correctly
	currentState->EnterState( agent );
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

void ADragoonAIController::SenseUpdate( TArray<AActor*> sensedActors ) {
	perceivedActors = sensedActors;	// update owned array to mimic that of actors known by perception system
}
