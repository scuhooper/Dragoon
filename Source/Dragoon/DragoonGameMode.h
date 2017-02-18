// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "AttackCircle.h"
#include "GameFramework/GameModeBase.h"
#include "DragoonGameMode.generated.h"

UCLASS(minimalapi)
class ADragoonGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ADragoonGameMode();

	TSharedPtr<AttackCircle> attackCircle = MakeShareable( new AttackCircle() );
};
