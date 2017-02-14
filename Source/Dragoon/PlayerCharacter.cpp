// Fill out your copyright notice in the Description page of Project Settings.

#include "Dragoon.h"
#include "PlayerCharacter.h"

APlayerCharacter::APlayerCharacter() {
	attackCircle = NewObject<UAttackCircle>();
	attackCircle->SetPlayer( this );
	attackCircle->Initialize();
}


