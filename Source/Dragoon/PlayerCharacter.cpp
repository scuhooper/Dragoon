// Fill out your copyright notice in the Description page of Project Settings.

#include "Dragoon.h"
#include "DragoonGameMode.h"
#include "PlayerCharacter.h"

APlayerCharacter::APlayerCharacter() {
}

void APlayerCharacter::BeginPlay() {
	Super::BeginPlay();

	ADragoonGameMode* game = ( ADragoonGameMode* )GetWorld()->GetAuthGameMode();
	attackCircle = game->attackCircle;
	attackCircle->SetPlayer( this );
	attackCircle->Initialize();
}
