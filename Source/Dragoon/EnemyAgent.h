// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DragoonCharacter.h"
#include "EnemyAgent.generated.h"

/**
 * 
 */
UCLASS()
class DRAGOON_API AEnemyAgent : public ADragoonCharacter
{
	GENERATED_BODY()
	
public:
	
private:
	int enemyScore;
	
public:
	// default c-tor needed for all UObject classes
	AEnemyAgent();

	AEnemyAgent( int score );

	/** Returns enemyScore **/
	UFUNCTION( BlueprintCallable, Category = EnemyAgent )
	FORCEINLINE int GetEnemyScore() { return enemyScore; }
};
