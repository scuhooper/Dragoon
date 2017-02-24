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
	/**
	 * A number to reflect the enemie's strength when joining the attack circle of a player
	 */
	int enemyScore;

	int strongAttackScore;

	int quickAttackScore;

	int feintAttackScore;

	bool bIsInCombat = false;
	
public:
	// default c-tor needed for all UObject classes
	AEnemyAgent();

	AEnemyAgent( int score );

	/** Returns enemyScore **/
	UFUNCTION( BlueprintCallable, Category = EnemyAgent )
	FORCEINLINE int GetEnemyScore() const { return enemyScore; }

	/** Returns bIsInCombat **/
	UFUNCTION( BlueprintCallable, Category = EnemyAgent )
	FORCEINLINE bool GetIsInCombat() const { return bIsInCombat; }

	void DrawSword();

protected:
	virtual void BeginPlay();

};

// struct to represent attacks in the N-grams used by DragoonAIBlackboard
USTRUCT()
struct FAttack {
	GENERATED_BODY()

	UPROPERTY()
	EAttackDirection direction;

	UPROPERTY()
	EAttackType type;

	UPROPERTY()
	int id;

	UPROPERTY()
	AEnemyAgent* target;

	FAttack() {
		direction = EAttackDirection::AD_DownwardRightSlash;
		type = EAttackType::AT_Quick;
		id = NULL;
		target = nullptr;
	}

	FAttack( EAttackDirection dir, EAttackType atkType, AEnemyAgent* enemy ) {
		direction = dir;
		type = atkType;
		id = ( uint8 )direction + ( uint8 )type;
		target = enemy;
	}
};
