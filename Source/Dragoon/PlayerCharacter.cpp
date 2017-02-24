// Fill out your copyright notice in the Description page of Project Settings.

#include "Dragoon.h"
#include "DragoonAIBlackboard.h"
#include "DragoonGameMode.h"
#include "PlayerCharacter.h"

APlayerCharacter::APlayerCharacter() {
}

APlayerCharacter::~APlayerCharacter() {
	attackCircle = nullptr;
}

void APlayerCharacter::BeginPlay() {
	Super::BeginPlay();

	ADragoonGameMode* game = ( ADragoonGameMode* )GetWorld()->GetAuthGameMode();
	attackCircle = &game->attackCircle;
	attackCircle->SetPlayer( this );
	attackCircle->Initialize();

	AIBlackboard = &game->blackboard;
}

void APlayerCharacter::PlayerAttack() {
	BeginAttack();

	EAttackType type;
	if ( GetIsStrongAttacking() )
		type = EAttackType::AT_Strong;
	else if ( GetIsFeintAttacking() )
		type = EAttackType::AT_Feint;
	else
		type = EAttackType::AT_Quick;

	// AIBlackboard->RecordPlayerAttack( FAttack( ( EAttackDirection )directionOfAttack, type, GetActorForwardVector() ) );

	FHitResult hit;
	FCollisionQueryParams params( FName( TEXT( "Attack Target Trace" ) ), true, this );
	ActorLineTraceSingle( hit, GetActorLocation(), GetActorForwardVector() * 500, ECollisionChannel::ECC_Pawn, params );

	if ( ( AEnemyAgent* )hit.Actor.Get() ) {
		AIBlackboard->RecordPlayerAttack( FAttack( ( EAttackDirection )directionOfAttack, type, ( AEnemyAgent* )hit.Actor.Get() ) );
	}
}

void APlayerCharacter::SetupPlayerInputComponent( UInputComponent* PlayerInputComponent ) {
	// Set up gameplay key bindings
	check( PlayerInputComponent );
	PlayerInputComponent->BindAction( "Jump/Dodge", IE_Pressed, this, &APlayerCharacter::EnableDodging );
	PlayerInputComponent->BindAction( "Jump/Dodge", IE_Released, this, &APlayerCharacter::DodgeKeyReleased );

	PlayerInputComponent->BindAxis( "MoveForward", this, &APlayerCharacter::MoveForward );
	PlayerInputComponent->BindAxis( "MoveRight", this, &APlayerCharacter::MoveRight );

	PlayerInputComponent->BindAction( "BasicAttack", IE_Pressed, this, &APlayerCharacter::BasicAttack );
	PlayerInputComponent->BindAction( "BasicAttack", IE_Released, this, &APlayerCharacter::PlayerAttack );
	PlayerInputComponent->BindAction( "Parry", IE_Pressed, this, &APlayerCharacter::Parry );
	PlayerInputComponent->BindAction( "Parry", IE_Released, this, &APlayerCharacter::BeginParry );
	PlayerInputComponent->BindAction( "Sheathe/UnsheatheSword", IE_Pressed, this, &APlayerCharacter::SheatheUnsheatheSword );
	PlayerInputComponent->BindAction( "StrongAttack", IE_Pressed, this, &APlayerCharacter::StrongAttack );
	PlayerInputComponent->BindAction( "FeintAttack", IE_Pressed, this, &APlayerCharacter::FeintAttack );

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis( "Turn", this, &APlayerCharacter::MyTurn );
	PlayerInputComponent->BindAxis( "TurnRate", this, &APlayerCharacter::TurnAtRate );
	PlayerInputComponent->BindAxis( "LookUp", this, &APlayerCharacter::MyLookUp );
	PlayerInputComponent->BindAxis( "LookUpRate", this, &APlayerCharacter::LookUpAtRate );

	// handle touch devices
	PlayerInputComponent->BindTouch( IE_Pressed, this, &APlayerCharacter::TouchStarted );
	PlayerInputComponent->BindTouch( IE_Released, this, &APlayerCharacter::TouchStopped );

	// VR headset functionality
	PlayerInputComponent->BindAction( "ResetVR", IE_Pressed, this, &APlayerCharacter::OnResetVR );
}
