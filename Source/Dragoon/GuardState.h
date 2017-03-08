// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "State.h"
#include "EnemyAgent.h"

/**
 * 
 */
class DRAGOON_API GuardState : State
{
protected:
	// distance that a guard can wander from their post
	float wanderRange = 100;

	// time to wait before next move
	float timeToWait;

	// minimum time to wait before moving again
	float minWaitTime = 4;

	// maximum time to wait before moving again
	float maxWaitTime = 8;

public:
	GuardState();
	~GuardState();

	/**
	* Initializes the wait timer to random value and move to location to be the agent's location.
	* @param agent	The agent who is currently using this state for behavior.
	*/
	virtual void EnterState( AEnemyAgent* agent );

	/**
	* Checks if agent has arrived at last randomly chosen location, and generates new location if it has.
	* @param agent	The agent who is currently using this state for behavior.
	* @param deltaSeconds	The amount of time that has passed since the last tick of the game engine
	*/
	virtual void StateTick( AEnemyAgent* agent, float deltaSeconds );

	/**
	* 
	* @param agent	The agent who is currently using this state for behavior.
	*/
	virtual void ExitState( AEnemyAgent* agent );
};
