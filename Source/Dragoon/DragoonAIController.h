// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EnemyAgent.h"
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

	TSharedPtr<AttackCircle> attackCircle;
	
public:
	ADragoonAIController();

protected:
	virtual void Tick( float DeltaSeconds ) override;

	virtual void BeginPlay() override;
};
