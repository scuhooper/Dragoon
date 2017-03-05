// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "EnemyAgent.h"

/**
 * Abstract class used to make other states for AI behavior
 */
class DRAGOON_API State
{
public:
	State();
	~State();

	/**
	 * Logic to run when entering current state
	 */
	virtual void EnterState( AEnemyAgent* agent ) = 0;

	/**
	* Logic to update state every frame/specified time interval
	*/
	virtual void StateTick( AEnemyAgent* agent, float deltaSeconds ) = 0;

	/**
	* Logic to run when leaving current state
	*/
	virtual void ExitState( AEnemyAgent* agent ) = 0;
};
