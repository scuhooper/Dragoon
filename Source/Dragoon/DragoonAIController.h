// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EnemyAgent.h"
#include "DragoonGameMode.h"
#include "AttackCircle.h"
#include "AIController.h"
#include "DragoonAIController.generated.h"

/**
 * 
 */
UCLASS()
class DRAGOON_API ADragoonAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	// testing..
	int test;

private:
	// reference to agent being controlled
	AEnemyAgent* agent;

	// reference to the main attack circle instance
	AttackCircle* attackCircle;

	// reference to the game mode
	ADragoonGameMode* game;
	
public:
	// default c-tor
	ADragoonAIController();

	// Destructor that sets pointers to nullptrs
	~ADragoonAIController();

	/**
	 * Update the attack circle and blackboard to remove controlled agent
	 */
	void AgentHasDied();

protected:
	/**
	 * Updates enemy logic every frame. Used for moving AI.
	 */
	virtual void Tick( float DeltaSeconds ) override;

	/**
	 * Event runs once and sets up variables for controller. Registers the controlled agent with the blackboard.
	 */
	virtual void BeginPlay() override;
};
