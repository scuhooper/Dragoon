// Fill out your copyright notice in the Description page of Project Settings.

#include "Dragoon.h"
#include "DragoonAIController.h"
#include "DragoonAIBlackboard.h"

DragoonAIBlackboard::DragoonAIBlackboard()
{
	agentsInCombat.Empty();
	agentsNotInCombat.Empty();
}

DragoonAIBlackboard::DragoonAIBlackboard( AttackCircle* circle ) {
	agentsInCombat.Empty();
	agentsNotInCombat.Empty();
	attackCircle = circle;
}

DragoonAIBlackboard::~DragoonAIBlackboard()
{
	attackCircle = nullptr;
}

void DragoonAIBlackboard::RegisterAgent( AEnemyAgent* agent ) {
	agentsNotInCombat.Add( agent );
}

void DragoonAIBlackboard::RemoveAgent( AEnemyAgent* agent ) {
	if ( agentsInCombat.Find( agent ) )
		agentsInCombat.Remove( agent );
	else if ( agentsNotInCombat.Find( agent ) )
		agentsNotInCombat.Remove( agent );
	else
		UE_LOG( LogTemp, Error, TEXT( "Agent %s requesting removal from AI Blackboard is not registered in the blackboard!" ), *agent->GetName() );
}

void DragoonAIBlackboard::HaveAgentJoinCombat( AEnemyAgent* agent ) {
	if ( agent->GetIsDead() || agent->GetIsInCombat() )
		return;
	else {
		RemoveAgent( agent );
		agentsInCombat.Add( agent );
	}
}

void DragoonAIBlackboard::HaveAgentFleeCombat( AEnemyAgent* agent ) {
	if ( agent->GetIsDead() || !agent->GetIsInCombat() )
		return;
	else {
		RemoveAgent( agent );
		RegisterAgent( agent );
	}
}

void DragoonAIBlackboard::RecordPlayerAttack( FAttack atk ) {
	if ( agentsInCombat.Num() == 0 )
		return;

	atk1 = atk2;
	atk2 = atk3;
	atk3 = atk.id;

	attackNGram[ atk1 ][ atk2 ][ atk3 ]++;

	if ( agentsInCombat.Contains( atk.target ) ) {
		// make target react to incoming attack
		ADragoonAIController* AIController = (ADragoonAIController*)atk.target->GetController();
		AIController->test;
	}
}
