// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "Dragoon.h"
#include "DragoonGameMode.h"
#include "DragoonCharacter.h"

ADragoonGameMode::ADragoonGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// create objects for use by AI systems
	attackCircle = AttackCircle();
	blackboard = DragoonAIBlackboard( &attackCircle );	// uses the attack circle made on previous line
}
