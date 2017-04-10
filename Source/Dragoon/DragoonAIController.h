// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EnemyAgent.h"
#include "State.h"
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
	FVector targetLoc;

	UNavigationSystem* navSystem;

private:
	// reference to agent being controlled
	AEnemyAgent* agent;

	// reference to the main attack circle instance
	AttackCircle* attackCircle;

	// reference to the game mode
	ADragoonGameMode* game;

	// pointer to the current state of FSM for this controller/agent
	State* currentState;

	State* nextState;

	bool bIsStateChangeReady;
	
public:
	// default c-tor
	ADragoonAIController();

	// Destructor that sets pointers to nullptrs
	~ADragoonAIController();

	/**
	 * Update the attack circle and blackboard to remove controlled agent
	 */
	void AgentHasDied();

	void AttackPlayer();

	/**
	 * Changes state from the currentState to the supplied newState
	 * @param newState	pointer to the new state to be entered by the controller
	 */
	void SwapState( State* newState );

	/** return attack cicrle pointer **/
	FORCEINLINE AttackCircle* GetAttackCircle() const { return attackCircle; }
	/** return game mode pointer **/
	FORCEINLINE ADragoonGameMode* GetGameMode() const { return game; }

	/**
	 *
	 */
	void ReactToIncomingAttack( int attackID, float confidenceInAttack );

	/**
	*
	*/
	void QuickAttackReaction( EAttackDirection directionOfAttack );

	/**
	*
	*/
	void StrongAttackReaction( EAttackDirection directionOfAttack );

	/**
	*
	*/
	void FeintAttackReaction( EAttackDirection directionOfAttack );

protected:
	/**
	 * Updates enemy logic every frame. Used for moving AI.
	 */
	virtual void Tick( float DeltaSeconds ) override;

	/**
	 * Event runs once and sets up variables for controller. Registers the controlled agent with the blackboard.
	 */
	virtual void BeginPlay() override;

	void TransitionBetweenStates();
};
