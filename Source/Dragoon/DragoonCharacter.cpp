// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "Dragoon.h"
#include "Kismet/HeadMountedDisplayFunctionLibrary.h"
#include "DragoonCharacter.h"

//////////////////////////////////////////////////////////////////////////
// ADragoonCharacter

ADragoonCharacter::ADragoonCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	// Setup the combat system grid
	AttackOrientation[ ( int32 )EAttackVertical::AV_Up ][ ( int32 )EAttackHorizontal::AH_Left ] = ( uint8 )EAttackDirection::AD_UpwardLeftSlash;
	AttackOrientation[ ( int32 )EAttackVertical::AV_Up ][ ( int32 )EAttackHorizontal::AH_Center ] = ( uint8 )EAttackDirection::AD_UpwardSlash;
	AttackOrientation[ ( int32 )EAttackVertical::AV_Up ][ ( int32 )EAttackHorizontal::AH_Right ] = ( uint8 )EAttackDirection::AD_UpwardRightSlash;
	AttackOrientation[ ( int32 )EAttackVertical::AV_Center ][ ( int32 )EAttackHorizontal::AH_Left ] = ( uint8 )EAttackDirection::AD_LeftSlash;
	AttackOrientation[ ( int32 )EAttackVertical::AV_Center ][ ( int32 )EAttackHorizontal::AH_Center ] = ( uint8 )EAttackDirection::AD_Thrust;
	AttackOrientation[ ( int32 )EAttackVertical::AV_Center ][ ( int32 )EAttackHorizontal::AH_Right ] = ( uint8 )EAttackDirection::AD_RightSlash;
	AttackOrientation[ ( int32 )EAttackVertical::AV_Down ][ ( int32 )EAttackHorizontal::AH_Left ] = ( uint8 )EAttackDirection::AD_DownwardLeftSlash;
	AttackOrientation[ ( int32 )EAttackVertical::AV_Down ][ ( int32 )EAttackHorizontal::AH_Center ] = ( uint8 )EAttackDirection::AD_DownwardSlash;
	AttackOrientation[ ( int32 )EAttackVertical::AV_Down ][ ( int32 )EAttackHorizontal::AH_Right ] = ( uint8 )EAttackDirection::AD_DownwardRightSlash;

	// Set health to equal MaxHealth
	health = maxHealth;
}

//////////////////////////////////////////////////////////////////////////
// Input

void ADragoonCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction( "Jump/Dodge", IE_Pressed, this, &ADragoonCharacter::EnableDodging );
	PlayerInputComponent->BindAction( "Jump/Dodge", IE_Released, this, &ADragoonCharacter::DodgeKeyReleased );

	PlayerInputComponent->BindAxis( "MoveForward", this, &ADragoonCharacter::MoveForward );
	PlayerInputComponent->BindAxis( "MoveRight", this, &ADragoonCharacter::MoveRight );

	PlayerInputComponent->BindAction( "BasicAttack", IE_Pressed, this, &ADragoonCharacter::BasicAttack );
	PlayerInputComponent->BindAction( "BasicAttack", IE_Released, this, &ADragoonCharacter::BeginAttack );
	PlayerInputComponent->BindAction( "Parry", IE_Pressed, this, &ADragoonCharacter::Parry );
	PlayerInputComponent->BindAction( "Parry", IE_Released, this, &ADragoonCharacter::BeginParry );
	PlayerInputComponent->BindAction( "Sheathe/UnsheatheSword", IE_Pressed, this, &ADragoonCharacter::SheatheUnsheatheSword );
	PlayerInputComponent->BindAction( "StrongAttack", IE_Pressed, this, &ADragoonCharacter::StrongAttack );
	PlayerInputComponent->BindAction( "FeintAttack", IE_Pressed, this, &ADragoonCharacter::FeintAttack );

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &ADragoonCharacter::MyTurn);
	PlayerInputComponent->BindAxis("TurnRate", this, &ADragoonCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &ADragoonCharacter::MyLookUp);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ADragoonCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ADragoonCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ADragoonCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ADragoonCharacter::OnResetVR);
}


void ADragoonCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ADragoonCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void ADragoonCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void ADragoonCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ADragoonCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ADragoonCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
		moveForward = Value;	// set float to equal value moved for use in animation BP when sword is drawn
	}
}

void ADragoonCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
		moveRight = Value;	// set float to equal value moved for use in animation BP when sword is drawn
	}
}

void ADragoonCharacter::BasicAttack() {
	if ( IsActionInProgress() )	// check for other actions currently happening
		return;

	// draw sword if not currently drawn
	if ( !bIsSwordDrawn ) {
		this->SheatheUnsheatheSword();
		return;
	}

	UGameplayStatics::SetGlobalTimeDilation( GetWorld(), 0.5f );	// sets slow-mo effect
	Controller->SetIgnoreMoveInput( true );	// stop getting move input
	bIsGettingAttackDirection = true;	// used to stop controlling camera and instead make the attack direction vector
}

void ADragoonCharacter::BeginAttack() {
	if ( !bIsGettingAttackDirection )	// if we failed to get attack direction, exit
		return;

	AttackDirectionChosen();	// get the actual vector for attack direction
	bIsAttacking = true;	// set attacking to true for animBP
}

void ADragoonCharacter::MyTurn( float Val ) {
	// if we are determing attack direction
	if ( bIsGettingAttackDirection ) {
		attackDirection.X += Val;	// add X movement of mouse to vector
		return;
	}

	AddControllerYawInput( Val );	// called in original Turn
}

void ADragoonCharacter::MyLookUp( float Rate ) {
	// if we are determining attack direction
	if ( bIsGettingAttackDirection ) {
		attackDirection.Y += Rate;	// add Y movement of mouse to vector
		return;
	}
	
	AddControllerPitchInput( Rate );	// called in original LookUp
}

void ADragoonCharacter::SheatheUnsheatheSword() {
	bIsSwordDrawn = !bIsSwordDrawn;	// set is sword drawn to opposite
	this->GetCharacterMovement()->bOrientRotationToMovement = !( this->GetCharacterMovement()->bOrientRotationToMovement );	// toggle orienting rotation with movement
	bUseControllerRotationYaw = !bUseControllerRotationYaw;	// toggle whether to move character yaw with camera yaw
}

void ADragoonCharacter::ResetMoveFloats() {
	moveForward = 0;
	moveRight = 0;
}

void ADragoonCharacter::StrongAttack() {
	bIsStrongAttack = true;
	BasicAttack();
}

void ADragoonCharacter::FeintAttack() {
	bIsFeintAttack = true;
	BasicAttack();
}

void ADragoonCharacter::EnableDodging() {
	// jump if sword isn't out
	if ( !bIsSwordDrawn ) {
		Jump();
		return;
	}

	// check for other actions occurring
	if ( IsActionInProgress() )
		return;

	// set animBP bool and turn off movement input
	bIsDodging = true;
	Controller->SetIgnoreMoveInput( true );
}

void ADragoonCharacter::DodgeKeyReleased() {
	// call this engine function if sword is not out
	if ( !bIsSwordDrawn ) {
		StopJumping();
		return;
	}
}

void ADragoonCharacter::Parry() {
	// check for other actions occurring
	if ( IsActionInProgress() )
		return;

	// draw sword if it is not out already
	if ( !bIsSwordDrawn ) {
		this->SheatheUnsheatheSword();
		return;
	}

	UGameplayStatics::SetGlobalTimeDilation( GetWorld(), 0.5f );	// start slow-mo effect
	Controller->SetIgnoreMoveInput( true );	// ignore movement while deciding attack direction
	bIsGettingAttackDirection = true;	// true to get attack direction vector
}

void ADragoonCharacter::BeginParry() {
	// if we don't have attack direction, exit
	if ( !bIsGettingAttackDirection )
		return;

	AttackDirectionChosen();	// convert vector to direction from enum array
	bIsParrying = true;	// set animBP bool
}

void ADragoonCharacter::AttackDirectionChosen() {
	bIsGettingAttackDirection = false;	// stop calculating vector from mouse movement
	attackDirection.Normalize( .1f );	// normalize vector, but if squared length is less than .1f set it to 0
	directionOfAttack = DetermineAttackDirection( attackDirection );	// get the enum array spot from vector
	UGameplayStatics::SetGlobalTimeDilation( GetWorld(), 1 );	// disable slow-mo effect
}

void ADragoonCharacter::FinishedAttacking() {
	bIsAttacking = false;	// set animBPs bool to false
	bIsStrongAttack = false;
	bIsFeintAttack = false;
	attackDirection = FVector2D( 0, 0 );	// reset attack direction vector
	Controller->SetIgnoreMoveInput( false );	// enable movement input
}

void ADragoonCharacter::FinishedDodging() {
	bIsDodging = false;	// set animBP bool to false
	Controller->SetIgnoreMoveInput( false );	// enable movement input
}

void ADragoonCharacter::FinishedParrying() {
	bIsParrying = false;	// set animBP bool to false
	attackDirection = FVector2D( 0, 0 );	// reset attack direction vector
	Controller->SetIgnoreMoveInput( false );	// enable movement input
}

uint8 ADragoonCharacter::DetermineAttackDirection( FVector2D vec ) {
	// enum variables for row/column
	EAttackHorizontal hor;
	EAttackVertical ver;

	// determine which column based on X value
	if ( vec.X > 0.2f )
		hor = EAttackHorizontal::AH_Right;
	else if ( vec.X < -0.2f )
		hor = EAttackHorizontal::AH_Left;
	else
		hor = EAttackHorizontal::AH_Center;
	
	// UE4 calculates Y inversely. Moving the mouse upward will result in negative y values and vice versa
	// determine which row based on Y value
	if ( vec.Y > 0.2f )
		ver = EAttackVertical::AV_Down;
	else if ( vec.Y < -0.2f )
		ver = EAttackVertical::AV_Up;
	else
		ver = EAttackVertical::AV_Center;

	// return the corresponding entry from array
	return AttackOrientation[ (int32)ver ][ (int32)hor ];
}

bool ADragoonCharacter::IsActionInProgress() {
	if ( bIsAttacking || bIsDodging || bIsDead || bIsHurt || bIsRecovering || bIsGettingAttackDirection || bIsParrying )
		return true;
	else
		return false;
}

void ADragoonCharacter::TakeDamage( int Val ) {
	if ( bIsHurt )
		return;

	health -= Val;
	Controller->SetIgnoreMoveInput( true );

	if ( health <= 0 )
		Dead();
	else
		bIsHurt = true;
}

void ADragoonCharacter::Dead() {
	bIsDead = true;
}

void ADragoonCharacter::RecoveredFromHit() {
	Controller->SetIgnoreMoveInput( false );
	bIsHurt = false;
}

void ADragoonCharacter::FinishedRecovering() {
	Controller->SetIgnoreMoveInput( false );
	bIsRecovering = false;
}

void ADragoonCharacter::AttackWasParried() {
	this->FinishedAttacking();
	Controller->SetIgnoreMoveInput( true );
	bIsRecovering = true;
}
