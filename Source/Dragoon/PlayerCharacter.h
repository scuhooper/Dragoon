// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AttackCircle.h"
#include "DragoonAIBlackboard.h"
#include "DragoonCharacter.h"
#include "PlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class DRAGOON_API APlayerCharacter : public ADragoonCharacter
{
	GENERATED_BODY()
	
private:
	// reference to existing attack circle instance
	AttackCircle* attackCircle;
	// reference to blackboard instance
	DragoonAIBlackboard* AIBlackboard = nullptr;
	
public:
	APlayerCharacter();
	~APlayerCharacter();

	// sets up the variables for class
	virtual void BeginPlay() override;

protected:
	/**
	 * Calls the did new attack occur function. Determines attack specifics and updates blackboard if an enemy is being attacked.
	 */
	void PlayerAttack();

	// setup control mappings
	virtual void SetupPlayerInputComponent( class UInputComponent* PlayerInputComponent ) override;

	/**
	 * Checks if an action is in progress. If action in progress, returns false. Else, calls begin attack funciton and returns true.
	 */
	bool DidNewAttackOccur();
};
