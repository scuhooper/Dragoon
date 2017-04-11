// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "State.h"

/**
 * 
 */
class DRAGOON_API AlertState : State
{
protected:
	// minimum distance to be maintained from player
	float minDistanceFromPlayer = 300;

	// maximum distance to be maintained from player
	float maxDistanceFromPlayer = 500;

	// where the agent should be standing
	FVector position;

public:
	AlertState();
	~AlertState();

	/**
	* Sets up the focus to be the player and updates the blackboard to have the agent in combat.
	* @param agent	The agent who is currently using this state for behavior.
	*/
	virtual void EnterState( AEnemyAgent* agent );

	/**
	* Checks if the agent can join the attack circle. Makes the agent stay a preferred distance away from the player.
	* @param agent	The agent who is currently using this state for behavior.
	* @param deltaSeconds	The amount of time that has passed since the last tick of the game engine
	*/
	virtual void StateTick( AEnemyAgent* agent, float deltaSeconds );

	/**
	* Clears the focus of the agent.
	* @param agent	The agent who is currently using this state for behavior.
	*/
	virtual void ExitState( AEnemyAgent* agent );
};
