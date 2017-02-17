// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AttackCircle.h"
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
	AttackCircle* attackCircle = nullptr;
	
public:
	APlayerCharacter();

	virtual void BeginPlay();
};
