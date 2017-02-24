// Fill out your copyright notice in the Description page of Project Settings.

#include "Dragoon.h"
#include "DragoonGameMode.h"
#include "DragoonAIController.h"

ADragoonAIController::ADragoonAIController() {
	// do super awesome AI stuff
}

ADragoonAIController::~ADragoonAIController() {
	attackCircle = nullptr;
}

void ADragoonAIController::Tick( float DeltaSeconds ) {
	// do stuff every frame
	Super::Tick( DeltaSeconds );
	/*if ( !agent->GetIsSwordDrawn() )
		agent->DrawSword();*/

	MoveToActor( attackCircle->GetPlayer() );
	UE_LOG( LogTemp, Warning, TEXT( "Agent's location is %s" ), *agent->GetActorLocation().ToString() );
}

void ADragoonAIController::BeginPlay() {
	Super::BeginPlay();
	ADragoonGameMode* game = ( ADragoonGameMode* )GetWorld()->GetAuthGameMode();
	agent = ( AEnemyAgent* )GetCharacter();
	attackCircle = &game->attackCircle;
	game->blackboard.RegisterAgent( agent );
}
