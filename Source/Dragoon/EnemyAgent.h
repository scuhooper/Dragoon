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

	// Number to reflect the attack strength of a strong attack
	int strongAttackScore;
	// Number to reflect the attack strength of a quick attack
	int quickAttackScore;
	// Number to reflect the attack strength of a feint attack
	int feintAttackScore;
	// Whether agent is in combat/attack circle
	bool bIsInCombat = false;
	
public:
	// default c-tor needed for all UObject classes
	AEnemyAgent();
	/**
	 * Sets up agent to have specified enemy score
	 * @param score	Value to to which enemy score should be set
	 */
	AEnemyAgent( int score );

	/** Returns enemyScore **/
	UFUNCTION( BlueprintCallable, Category = EnemyAgent )
	FORCEINLINE int GetEnemyScore() const { return enemyScore; }

	/** Returns bIsInCombat **/
	UFUNCTION( BlueprintCallable, Category = EnemyAgent )
	FORCEINLINE bool GetIsInCombat() const { return bIsInCombat; }

	/**
	 * Wrapper function for equipping/unequipping sword
	 */
	void DrawSword();

	/**
	* Sets bIsInCombat to true and draws sword
	*/
	void JoinCombat();

	/**
	* Sets bIsInCombat to false and sheathes sword
	*/
	void LeaveCombat();

	/**
	 * Enemy agents overriden take damage function
	 */
	virtual void MyTakeDamage( int val ) override;

protected:
	/**
	* Currently just calls parent's begin play event
	*/
	virtual void BeginPlay();

};

// struct to represent attacks in the N-grams used by DragoonAIBlackboard
USTRUCT()
struct FAttack {
	GENERATED_BODY()

	UPROPERTY()
	EAttackDirection direction;	// holds the attack's direction

	UPROPERTY()
	EAttackType type;	// holds what kind of attack is being performed

	UPROPERTY()
	int id;	// id is derived from direction and type. Serves to uniquely identify each of the 27 possible attacks

	UPROPERTY()
	AEnemyAgent* target;	// enemy that is being attacked

	// default c-tor. needed for USTRUCT
	FAttack() {
		direction = EAttackDirection::AD_DownwardRightSlash;
		type = EAttackType::AT_Quick;
		id = NULL;
		target = nullptr;
	}

	/**
	 * Constructor to be used when creating attacks
	 * @param dir		the direction of the attack being performed
	 * @param atkType	the type of attack being performed (strong, quick, feint)
	 * @param enemy		the target of the attack being performed
	 */
	FAttack( EAttackDirection dir, EAttackType atkType, AEnemyAgent* enemy ) {
		direction = dir;
		type = atkType;
		id = ( uint8 )direction + ( uint8 )type;
		target = enemy;
	}
};
