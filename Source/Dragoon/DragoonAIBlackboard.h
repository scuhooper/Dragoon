// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Dragoon.h"
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

	TSharedPtr<AttackCircle> attackCircle;

public:
	DragoonAIBlackboard();
	DragoonAIBlackboard( TSharedPtr<AttackCircle> circle );
	~DragoonAIBlackboard();
	void RegisterAgent( AEnemyAgent* agent );
	void RemoveAgent( AEnemyAgent* agent );
	void HaveAgentJoinCombat( AEnemyAgent* agent );
	void HaveAgentFleeCombat( AEnemyAgent* agent );

private:

};
