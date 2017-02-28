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
	int test;

private:
	AEnemyAgent* agent;

	AttackCircle* attackCircle;

	ADragoonGameMode* game;
	
public:
	ADragoonAIController();
	~ADragoonAIController();

	void AgentHasDied();

protected:
	virtual void Tick( float DeltaSeconds ) override;

	virtual void BeginPlay() override;
};
