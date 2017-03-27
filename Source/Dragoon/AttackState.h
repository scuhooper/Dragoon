// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "State.h"

/**
 * 
 */
class DRAGOON_API AttackState : State
{
protected:
	float timeBetweenAttacks = 0;
	float elapsedTime = 0;
public:
	AttackState();
	~AttackState();

	/**
	*
	*/
	virtual void EnterState( AEnemyAgent* agent );

	/**
	*
	*/
	virtual void StateTick( AEnemyAgent* agent, float deltaSeconds );

	/**
	*
	*/
	virtual void ExitState( AEnemyAgent* agent );
};
