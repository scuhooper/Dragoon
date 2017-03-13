// Fill out your copyright notice in the Description page of Project Settings.

#include "Dragoon.h"
#include "DragoonAIController.h"
#include "DragoonAIBlackboard.h"

DragoonAIBlackboard::DragoonAIBlackboard()
{
	// initialize arrays to be empty
	agentsInCombat.Empty();
	agentsNotInCombat.Empty();
}

DragoonAIBlackboard::DragoonAIBlackboard( AttackCircle* circle ) {
	// initialize arrays to be empty
	agentsInCombat.Empty();
	agentsNotInCombat.Empty();

	// set attack circle reference
	attackCircle = circle;
}

DragoonAIBlackboard::~DragoonAIBlackboard()
{
	// remove pointer reference
	attackCircle = nullptr;

	// empty arrays of references
	agentsInCombat.Empty();
	agentsNotInCombat.Empty();
}

void DragoonAIBlackboard::RegisterAgent( AEnemyAgent* agent ) {
	// check to make sure there aren't multiple references to the same agent in either array
	if ( agentsNotInCombat.Contains( agent ) || agentsInCombat.Contains( agent ) )
		return;
	else
		agentsNotInCombat.Add( agent );	// put agent in non-combat array
}

void DragoonAIBlackboard::RemoveAgent( AEnemyAgent* agent ) {
	// check through arrays for agent and remove from containing array
	if ( agentsInCombat.Find( agent ) )
		agentsInCombat.Remove( agent );
	else if ( agentsNotInCombat.Find( agent ) )
		agentsNotInCombat.Remove( agent );
	else
		UE_LOG( LogTemp, Error, TEXT( "Agent %s requesting removal from AI Blackboard is not registered in the blackboard!" ), *agent->GetName() );	// print error to log if an unregistered agent makes the request
}

void DragoonAIBlackboard::HaveAgentJoinCombat( AEnemyAgent* agent ) {
	// checks for agent to have valid state
	if ( agent->GetIsDead() || agent->GetIsInCombat() || agentsInCombat.Contains( agent ) || !agentsNotInCombat.Contains( agent ) )
		return;
	else {
		// change the agents reference to be in combat array
		RemoveAgent( agent );
		agentsInCombat.Add( agent );
		agent->JoinCombat();	// call agents function to perform any agent specific behavior needed for joining combat
	}
}

void DragoonAIBlackboard::HaveAgentFleeCombat( AEnemyAgent* agent ) {
	// checks for agent to have valid state
	if ( agent->GetIsDead() || !agent->GetIsInCombat() || !agentsInCombat.Contains( agent ) || agentsNotInCombat.Contains( agent ) )
		return;
	else {
		// swap agents reference to be in non-combat array
		RemoveAgent( agent );
		RegisterAgent( agent );
		agent->LeaveCombat();	// call agents function to perform any agent specific behavior for leaving combat
	}
}

void DragoonAIBlackboard::RecordPlayerAttack( FAttack atk ) {
	// make sure an active agent is in combat
	if ( agentsInCombat.Num() == 0 )
		return;

	// check if predicted attack is attack that was made
	if ( nextAttackPrediction == atk.id ) {
		// update weight of unknown event to be less as predictions are being correct
		weightForRandomAttack -= .05f;
		if ( weightForRandomAttack < .05f )
			weightForRandomAttack = .05f;
	}
	else {
		// increase weight for random attack as predictions are incorrect
		weightForRandomAttack += .02f;
		if ( weightForRandomAttack > .5f )
			weightForRandomAttack = .5f;
	}

	// cycle attacks forward one slot. put most recent attack in atk3. The order is important as contents are being overwritten
	atk1 = atk2;
	atk2 = atk3;
	atk3 = atk.id;

	// debug logging for testing
	UE_LOG( LogTemp, Warning, TEXT( "Player is attacking %s" ), *atk.target->GetName() );

	// update the n gram array to have another reference to attack sequence
	attackNGram[ atk1 ][ atk2 ][ atk3 ]++;

	// update history deque with new attack
	if ( !bIsHistoryFull ) {
		attackHistory.push_back( atk3 );
		// check if history is now at our max size
		if ( attackHistory.size() == maxHistorySize )
			bIsHistoryFull = true;
	}
	else {
		// remove oldest entry (will be the first entry as we add to the back), and add new attack to back of history
		attackHistory.pop_front();
		attackHistory.push_back( atk3 );
	}

	// call agent behavior to respond to attack if it is in combat
	if ( agentsInCombat.Contains( atk.target ) ) {
		// make target react to incoming attack
		ADragoonAIController* AIController = (ADragoonAIController*)atk.target->GetController();
		// testing access to ai controller. needs to be updated with actual logic for reacting to attacks
	}

	PredictNextAttack();
}

void DragoonAIBlackboard::AgentHasDied( AEnemyAgent* agent ) {
	// notify controller that agent has died
	ADragoonAIController* AIController = ( ADragoonAIController* )agent->GetController();
	AIController->AgentHasDied();
}

void DragoonAIBlackboard::PredictNextAttack() {
	// map to contain attack occurences from the entire playtime
	TMap<int, int> cumulativeAttackOccurences;

	if ( !bIsHistoryUsed ) {
		if ( attackHistory.size() >= 3 )
			bIsHistoryUsed = true;
	}
	else {
		for ( int i = 0; i < 27; i++ ) {
			// add any attack that has more than 0 occurences to map
			if ( attackNGram[ atk2 ][ atk3 ][ i ] > 0 ) {
				cumulativeAttackOccurences.Add( i, attackNGram[ atk2 ][ atk3 ][ i ] );

				// update most occurence indices
				CalculateHighestAttackOccurences( cumulativeAttackOccurences, i );
			}
		}

		// check history for matching patterns
		for ( int i = 0; i < attackHistory.size() - 2; i++ ) {
			if ( attackHistory[ i ] == atk2 ) {
				if ( attackHistory[ i + 1 ] == atk3 ) {
					// multiply occurence by the weight for history
					cumulativeAttackOccurences[ attackHistory[ i + 2 ] ] *= historyWeight;

					// update most occurence indices
					int index = i + 2;
					CalculateHighestAttackOccurences( cumulativeAttackOccurences, index );
				}
			}
		}

		float attack1Ratio = 0, attack2Ratio = 0, attack3Ratio = 0;
		int totalOfHighestOccurenceAttacks = 0;
		// get 3 largest values in attack occurences
		if ( cumulativeAttackOccurences.Contains( mostOccurencesIndex1 ) ) {
			totalOfHighestOccurenceAttacks += cumulativeAttackOccurences[ mostOccurencesIndex1 ];
			if ( cumulativeAttackOccurences.Contains( mostOccurencesIndex2 ) ) {
				totalOfHighestOccurenceAttacks += cumulativeAttackOccurences[ mostOccurencesIndex2 ];
				if ( cumulativeAttackOccurences.Contains( mostOccurencesIndex3 ) ) {
					totalOfHighestOccurenceAttacks += cumulativeAttackOccurences[ mostOccurencesIndex3 ];
					attack3Ratio = ( ( float )cumulativeAttackOccurences[ mostOccurencesIndex3 ] / ( float )totalOfHighestOccurenceAttacks ) * ( 1 - weightForRandomAttack );
				}
				attack2Ratio = ( ( float )cumulativeAttackOccurences[ mostOccurencesIndex2 ] / ( float )totalOfHighestOccurenceAttacks ) * ( 1 - weightForRandomAttack );
			}
			attack1Ratio = ( ( float )cumulativeAttackOccurences[ mostOccurencesIndex1 ] / ( float )totalOfHighestOccurenceAttacks ) * ( 1 - weightForRandomAttack );
		}

		// get random value to compare against weights for the predicted attack
		float prediction = FMath::FRand();

		// choose which attack for next prediction based on weights given to the attacks based on occurences
		if ( prediction < weightForRandomAttack )
			nextAttackPrediction = FMath::RandRange( 0, 26 );
		else if ( prediction < weightForRandomAttack + attack1Ratio )
			nextAttackPrediction = mostOccurencesIndex1;
		else if ( prediction < weightForRandomAttack + attack1Ratio + attack2Ratio )
			nextAttackPrediction = mostOccurencesIndex2;
		else
			nextAttackPrediction = mostOccurencesIndex3;
	}
}

void DragoonAIBlackboard::CalculateHighestAttackOccurences( TMap<int, int> &occurenceMap, int &currentIndex ) {
	// update most occurence indices
	if ( occurenceMap[ currentIndex ] > occurenceMap.FindOrAdd( mostOccurencesIndex3 ) ) {
		if ( occurenceMap[ currentIndex ] > occurenceMap.FindOrAdd( mostOccurencesIndex2 ) ) {
			if ( occurenceMap[ currentIndex ] > occurenceMap.FindOrAdd( mostOccurencesIndex1 ) ) {
				mostOccurencesIndex3 = mostOccurencesIndex2;
				mostOccurencesIndex2 = mostOccurencesIndex1;
				mostOccurencesIndex1 = currentIndex;
			}
			else {
				mostOccurencesIndex3 = mostOccurencesIndex2;
				mostOccurencesIndex2 = currentIndex;
			}
		}
		else {
			mostOccurencesIndex3 = currentIndex;
		}
	}
}
