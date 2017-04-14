// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "State.h"
#include "EnemyAgent.h"

/**
 * 
 */
class DRAGOON_API PatrolState : State
{
protected:
	// keep current indexed waypoint
	int currentWaypoint = 0;

	// time to wait before next move
	float timeToWait;

	// minimum time to wait before moving again
	float minWaitTime = 4;

	// maximum time to wait before moving again
	float maxWaitTime = 8;

public:
	PatrolState();
	~PatrolState();

	/**
	* Sets initial wait timer if agent's patrol is set to wait at patrol points
	* @param agent	The agent who is currently using this state for behavior.
	*/
	virtual void EnterState( AEnemyAgent* agent );

	/**
	* Checks if agent has arrived at a patrol point. 
	* Will update to the next patrol point either immediately or after a wait period depending on if the agent's patrol is continuous.
	* @param agent	The agent who is currently using this state for behavior.
	* @param deltaSeconds	The amount of time that has passed since the last tick of the game engine
	*/
	virtual void StateTick( AEnemyAgent* agent, float deltaSeconds );

	/**
	* 
	* @param agent	The agent who is currently using this state for behavior.
	*/
	virtual void ExitState( AEnemyAgent* agent );

protected:
	/**
	 * Gets the next waypoint from the agent's waypoint array.
	 * Will get first point if index would exceed array bounds.
	 * @param agent	The agent who is currently using this state for behavior.
	 */
	void UpdateWaypoint( AEnemyAgent* agent );
};
