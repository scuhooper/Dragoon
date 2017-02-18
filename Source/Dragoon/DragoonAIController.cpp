// Fill out your copyright notice in the Description page of Project Settings.

#include "DragoonAIBlackboardComponent.h"
#include "Dragoon.h"
#include "DragoonAIController.h"

ADragoonAIController::ADragoonAIController() {
	// do super awesome AI stuff
	agent = ( AEnemyAgent* )GetCharacter();

	Blackboard = NewObject<UDragoonAIBlackboardComponent>();
}
