// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "State.h"
#include "EnemyAgent.h"

/**
 * 
 */
class DRAGOON_API PatrolState : State
{
public:
	PatrolState();
	~PatrolState();

	/**
	* Sets up the focus to be the player and puts the agent into the attack circle.
	* @param agent	The agent who is currently using this state for behavior.
	*/
	virtual void EnterState( AEnemyAgent* agent );

	/**
	* Checks if the agent can attack, and chooses an attack if possible
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
