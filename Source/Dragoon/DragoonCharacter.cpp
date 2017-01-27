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
}

//////////////////////////////////////////////////////////////////////////
// Input

void ADragoonCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &ADragoonCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ADragoonCharacter::MoveRight);

	PlayerInputComponent->BindAction( "BasicAttack", IE_Pressed, this, &ADragoonCharacter::BasicAttack );
	PlayerInputComponent->BindAction( "BasicAttack", IE_Released, this, &ADragoonCharacter::AttackDirectionChosen );
	PlayerInputComponent->BindAction( "Sheathe/UnsheatheSword", IE_Pressed, this, &ADragoonCharacter::SheatheUnsheatheSword );
	PlayerInputComponent->BindAction( "StrongAttack", IE_Pressed, this, &ADragoonCharacter::EnableStrongAttackModifier );
	PlayerInputComponent->BindAction( "StrongAttack", IE_Released, this, &ADragoonCharacter::DisableStrongAttackModifier );
	PlayerInputComponent->BindAction( "FeintAttack", IE_Pressed, this, &ADragoonCharacter::EnableFeintAttackModifier );
	PlayerInputComponent->BindAction( "FeintAttack", IE_Released, this, &ADragoonCharacter::DisableFeintAttackModifier );

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
		moveForward = Value;
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
		moveRight = Value;
	}
}

void ADragoonCharacter::BasicAttack() {
	if ( bIsGettingAttackDirection || bIsAttacking )
		return;

	UE_LOG( LogTemp, Warning, TEXT( "Basic Attack!" ) );
	UGameplayStatics::SetGlobalTimeDilation( GetWorld(), 0.5f );
	Controller->SetIgnoreMoveInput( true );
	bIsGettingAttackDirection = true;
}

void ADragoonCharacter::MyTurn( float Val ) {
	if ( bIsGettingAttackDirection ) {
		attackDirection.X += Val;
		return;
	}

	AddControllerYawInput( Val );
	if ( bIsSwordDrawn ) {
		FRotator directionToFace( 0, GetFollowCamera()->GetComponentRotation().Yaw, 0 );
		this->SetActorRotation( directionToFace );
	}
}

void ADragoonCharacter::MyLookUp( float Rate ) {
	if ( bIsGettingAttackDirection ) {
		attackDirection.Y += Rate;
		return;
	}
	
	AddControllerPitchInput( Rate );
}

void ADragoonCharacter::SheatheUnsheatheSword() {
	bIsSwordDrawn = !bIsSwordDrawn;	// set is sword drawn to opposite
	this->GetCharacterMovement()->bOrientRotationToMovement = !( this->GetCharacterMovement()->bOrientRotationToMovement );
}

void ADragoonCharacter::ResetMoveFloats() {
	moveForward = 0;
	moveRight = 0;
}

void ADragoonCharacter::EnableStrongAttackModifier() {
	bIsStrongAttack = true;
}

void ADragoonCharacter::DisableStrongAttackModifier() {
	bIsStrongAttack = false;
}

void ADragoonCharacter::EnableFeintAttackModifier() {
	bIsFeintAttack = true;
}

void ADragoonCharacter::DisableFeintAttackModifier() {
	bIsFeintAttack = false;
}

void ADragoonCharacter::AttackDirectionChosen() {
	bIsGettingAttackDirection = false;
	UE_LOG( LogTemp, Warning, TEXT( "attackDirection Vector is %s" ), *this->attackDirection.ToString() );
	attackDirection.Normalize( .1f );
	UE_LOG( LogTemp, Warning, TEXT( "attackDirection Vector is %s" ), *this->attackDirection.ToString() );
	directionOfAttack = DetermineAttackDirection( attackDirection );
	bIsAttacking = true;

	UGameplayStatics::SetGlobalTimeDilation( GetWorld(), 1 );
}

void ADragoonCharacter::FinishedAttacking() {
	bIsAttacking = false;
	attackDirection = FVector2D( 0, 0 );
	Controller->SetIgnoreMoveInput( false );
}

uint8 ADragoonCharacter::DetermineAttackDirection( FVector2D vec ) {
	EAttackHorizontal hor;
	EAttackVertical ver;
	if ( vec.X > 0.2f )
		hor = EAttackHorizontal::AH_Right;
	else if ( vec.X < -0.2f )
		hor = EAttackHorizontal::AH_Left;
	else
		hor = EAttackHorizontal::AH_Center;
	
	// UE4 calculates Y inversely. Moving the mouse upward will result in negative y values and vice versa
	if ( vec.Y > 0.2f )
		ver = EAttackVertical::AV_Down;
	else if ( vec.Y < -0.2f )
		ver = EAttackVertical::AV_Up;
	else
		ver = EAttackVertical::AV_Center;

	return AttackOrientation[ (int32)ver ][ (int32)hor ];
}
