// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "AttackCircle.h"
#include "DragoonAIBlackboard.h"
#include "GameFramework/GameModeBase.h"
#include "DragoonGameMode.generated.h"

UCLASS(minimalapi)
class ADragoonGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	// instance of attack circle
	AttackCircle attackCircle;

	// instance of blackboard
	DragoonAIBlackboard blackboard;

public:
	ADragoonGameMode();
};
