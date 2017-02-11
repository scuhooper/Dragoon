// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DragoonCharacter.h"
#include "UObject/NoExportTypes.h"
#include "AttackCircle.generated.h"

/**
 * 
 */

UCLASS()
class DRAGOON_API UAttackCircle : public UObject
{
	GENERATED_BODY()

public:
	// Holds the maximum enemy score that can be within the circle at any given moment
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = AttackCircle )
	int maxEnemyScore;

	// Holds the maximum enemy attack score that can be taking place within the circle at any given moment
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = AttackCircle )
	int maxAttackScore;

private:
	// The score available to place new enemies in the circle
	UPROPERTY( VisibleAnywhere,  Category = AttackCirlce )
	int availableEnemyScore;

	// The score available to start new attacks in the circle
	UPROPERTY( VisibleAnywhere, Category = AttackCircle )
	int availableAttackScore;

	// Pointer to the player
	UPROPERTY( VisibleAnywhere, Category = AttackCircle )
	ADragoonCharacter* player;

	// An array of all enemies currently within the circle
	UPROPERTY( VisibleAnywhere, Category = AttackCircle )
	TArray<ADragoonCharacter*> enemiesInCircle;

public:
	UAttackCircle();

	/** Returns availableAttackScore **/
	UFUNCTION( BlueprintCallable, Category = AttackCircle )
	FORCEINLINE int GetAvailableAttackScore() const { return availableAttackScore; }
	/** Returns availableEnemyScore **/
	UFUNCTION( BlueprintCallable, Category = AttackCircle )
	FORCEINLINE int GetAvailableEnemyScore() const { return availableEnemyScore; }
	/** Returns player **/
	UFUNCTION( BlueprintCallable, Category = AttackCircle )
	FORCEINLINE ADragoonCharacter* GetPlayer() const { return player; }
	/** Returns enemiesInCircle **/
	UFUNCTION( BlueprintCallable, Category = AttackCircle )
	FORCEINLINE TArray<ADragoonCharacter*> GetEnemiesInCircle() const { return enemiesInCircle; }

	/**
	 * Handles request from an attacker to join the attack circle
	 */
	UFUNCTION( BlueprintCallable, Category = AttackCircle )
	void JoinCircle( ADragoonCharacter* attacker );
private:

};
