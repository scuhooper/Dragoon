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
	TSharedPtr<AttackCircle> attackCircle;

	DragoonAIBlackboard* AIBlackboard;
	
public:
	APlayerCharacter();

	virtual void BeginPlay();

protected:
	void PlayerAttack();

	virtual void SetupPlayerInputComponent( class UInputComponent* PlayerInputComponent ) override;
};
