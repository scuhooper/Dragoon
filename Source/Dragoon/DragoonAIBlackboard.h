// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "AttackCircle.h"
#include "EnemyAgent.h"
/**
 * 
 */
class DRAGOON_API DragoonAIBlackboard
{
public:

private:
	TArray<AEnemyAgent*> agentsNotInCombat;

	TArray<AEnemyAgent*> agentsInCombat;

	AttackCircle* attackCircle;

	int attackNGram[ 27 ][ 27 ][ 27 ];

	int atk1 = 0, atk2 = 0, atk3 = 0;

public:
	DragoonAIBlackboard();
	DragoonAIBlackboard( AttackCircle* circle );
	~DragoonAIBlackboard();
	void RegisterAgent( AEnemyAgent* agent );
	void RemoveAgent( AEnemyAgent* agent );
	void HaveAgentJoinCombat( AEnemyAgent* agent );
	void HaveAgentFleeCombat( AEnemyAgent* agent );
	void RecordPlayerAttack( FAttack atk );

private:

};
