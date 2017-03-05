// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
#include "DragoonCharacter.generated.h"

// enum for particular attack direction based off of attack grid
UENUM( BlueprintType )
enum class EAttackDirection : uint8
{
	AD_DownwardRightSlash	UMETA( DisplayName == "DownwardRightSlash" ),
	AD_DownwardSlash		UMETA( DisplayName == "DownwardSlash" ),
	AD_DownwardLeftSlash	UMETA( DisplayName == "DownwardLeftSlash" ),
	AD_RightSlash			UMETA( DisplayName == "RightSlash" ),
	AD_Thrust				UMETA( DisplayName == "Thrust" ),
	AD_LeftSlash			UMETA( DisplayName == "LeftSlash" ),
	AD_UpwardRightSlash		UMETA( DisplayName == "UpwardRightSlash" ),
	AD_UpwardSlash			UMETA( DisplayName == "UpwardSlash" ),
	AD_UpwardLeftSlash		UMETA( DisplayName == "UpwardLeftSlash" )
};

// enum to store whether the attack should go upwards, downwards, or through the middle
enum class EAttackVertical : uint8
{
	AV_Up,
	AV_Center,
	AV_Down
};

// enum to store whether the attack should go towards the left, right, or center
enum class EAttackHorizontal : uint8
{
	AH_Left,
	AH_Center,
	AH_Right
};

// enum to represent the type of attack
UENUM()
enum class EAttackType : uint8 {
	AT_Quick = 0,
	AT_Strong = 9,
	AT_Feint = 18
};

UCLASS(config=Game)
class ADragoonCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

public:
	ADragoonCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	// float to hold movement speed in x direction for animation blueprint while sword is drawn
	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = Movement )
	float moveForward = 0;

	// float to hold movement speed in y direction for animation blueprint while sword is drawn
	UPROPERTY( VisibleAnywhere, BlueprintReadWrite, Category = Movement )
	float moveRight = 0;

	// reference for attaching sword in derived BP class
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = DragoonCharacter )
	AActor* sword;

	// variable of attack direction enum so it can be accessed in blueprints
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Enum )
	EAttackDirection AttackDirectrionEnum;

	// enums for storing x and y components of mouse movement when deciding which attack/parry to use
	EAttackVertical AttackVerticalEnum;
	EAttackHorizontal AttackHorizontalEnum;

	// 2D array for storing y and x components of attack direction, respectively
	uint8 AttackOrientation[ 3 ][ 3 ];

	// stores which direction was chosen from the AttackOrientation array
	uint8 directionOfAttack = ( uint8 )EAttackDirection::AD_Thrust;

	// used to store the direction from which the character is hit
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Combat )
	EAttackDirection hitDirection;

	// no property because UPROPERTY does not support const
	const int maxHealth = 100;

	// how much damage character does
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Combat )
	int damage;

	// damage multiplier for StrongAttack
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Combat )
	float strongAttackDamageMultiplier;

protected:
	/// character state booleans
	bool bIsSwordDrawn = false;
	bool bIsStrongAttack = false;
	bool bIsFeintAttack = false;
	bool bIsAttacking = false;
	bool bIsGettingAttackDirection = false;
	bool bIsParrying = false;
	bool bIsDodging = false;
	bool bIsHurt = false;
	bool bIsDead = false;
	bool bIsRecovering = false;

	// 2D Vector to store the mouse movement for choosing an attack/parry's direction
	FVector2D attackDirection;

	int health;

protected:

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

	/** 
	 * Handler for attack button pressed.
	 * Sets slow-motion effect, disables movement input, and toggles getting attack direction input
	 */
	virtual void BasicAttack();

	/**
	 * Handler for attack button released
	 * Sets the final attack direction and toggles on IsAttacking
	 */
	void BeginAttack();

	/**
	* Called via input to turn at a given rate. Sets attack direction X component if attack or parry button is pressed.
	* If sword is drawn, it adjusts the character's rotation to match that of the camera. This results in the character looking at where the camera is pointed.
	* @param Val	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	*/
	void MyTurn( float Val );

	/**
	* Called via input to turn look up/down at a given rate. Sets attack direction Y component if attack or parry button is pressed.
	* @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	*/
	void MyLookUp( float Rate );

	/**
	 * Toggles SwordDrawn and whether to orient roation to movement direction
	 */
	void SheatheUnsheatheSword();

	/**
	 * Handler for StrongAttack button pressed. 
	 * Sets bIsStrongAttack to true and calls BasicAttack
	 */
	void StrongAttack();

	/**
	* Handler for FeintAttack button pressed.
	* Sets bIsFeintAttack to true and calls BasicAttack
	*/
	void FeintAttack();

	/**
	* Handler for Jump/Dodge button pressed.
	* Calls Jump if sword is not drawn.
	* If sword is drawn, sets bIsDodging to true and disables movement input.
	*/
	void EnableDodging();

	/**
	* Handler for Jump/Dodge button released.
	* Calls StopJumping if sword is not drawn.
	*/
	void DodgeKeyReleased();

	/**
	* Handler for Parry button pressed.
	* Equips sword if sword is not currently equipped.
	* Sets slow-motion effect, stops movement input, and toggles get attack direction on.
	*/
	void Parry();

	/**
	* Handler for Parry button released.
	* Submits mouse movement to be turned into a 2D vector. Sets IsParrying to true.
	*/
	void BeginParry();

	/**
	 * Toggles off the bool for getting attack direction.
	 * Normalizes the 2D vector gotten for attack direction or sets it to zero.
	 * Call DetermineAttackDirection with the normalized 2D vector.
	 * Turns slow-motion effect off.
	 */
	virtual void AttackDirectionChosen();

	/**
	 * Converts X and Y value of vec to their respective enum equivalents.
	 * &Param vec	2D Vector that has been normalized to lie within a unit circle
	 * &Return		Value from AttackOrientation array which corresponds to Y and X components of vec
	 */
	uint8 DetermineAttackDirection( FVector2D vec );

	/**
	 * Checks state booleans and returns true if any are true.
	 */
	bool IsActionInProgress();

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	/** Returns bIsSwordDrawn **/
	UFUNCTION( BlueprintCallable, Category = DragoonPlayer )
	FORCEINLINE bool GetIsSwordDrawn() const { return bIsSwordDrawn; }
	/** Returns bIsAttacking **/
	UFUNCTION( BlueprintCallable, Category = Combat )
	FORCEINLINE bool GetIsAttacking() const { return bIsAttacking; }
	/** Returns bIsStrongAttack **/
	UFUNCTION( BlueprintCallable, Category = Combat )
	FORCEINLINE bool GetIsStrongAttacking() const { return bIsStrongAttack; }
	/** Returns bIsFeintAttack **/
	UFUNCTION( BlueprintCallable, Category = Combat )
	FORCEINLINE bool GetIsFeintAttacking() const { return bIsFeintAttack; }
	/** Returns directionOfAttack **/
	UFUNCTION( BlueprintCallable, Category = Combat )
	FORCEINLINE uint8 GetDirectionOfAttack() const { return directionOfAttack; }
	/** Returns bIsParrying **/
	UFUNCTION( BlueprintCallable, Category = Combat )
	FORCEINLINE bool GetIsParrying() const { return bIsParrying; }
	/** Returns bIsDodging **/
	UFUNCTION( BlueprintCallable, Category = Combat )
	FORCEINLINE bool GetIsDodging() const { return bIsDodging; }
	/** Returns bIsHurt **/
	UFUNCTION( BlueprintCallable, Category = Combat )
	FORCEINLINE bool GetIsHurt() const { return bIsHurt; }
	/** Returns bIsDead **/
	UFUNCTION( BlueprintCallable, Category = Combat )
	FORCEINLINE bool GetIsDead() const { return bIsDead; }
	/** Returns bIsRecovering **/
	UFUNCTION( BlueprintCallable, Category = Combat )
	FORCEINLINE bool GetIsRecovering() const { return bIsRecovering; }

	/**
	 * Resets moveForward and moveRight to 0.
	 */
	UFUNCTION( BlueprintCallable, Category = DragoonPlayer )
	void ResetMoveFloats();

	/**
	 * Sets IsAttacking to false.
	 * Resets attackDirection vector to zero.
	 * Enables movement input.
	 */
	UFUNCTION( BlueprintCallable, Category = Combat )
	void FinishedAttacking();

	/**
	 * Sets IsDodging to false.
	 * Enables movement input
	 */
	UFUNCTION( BlueprintCallable, Category = Combat )
	void FinishedDodging();

	/**  
	 * Sets IsParrying to false.
	 * Enables movement input.
	 */
	UFUNCTION( BlueprintCallable, Category = Combat )
	void FinishedParrying();

	/**
	 * Subtracts Val from character's health.
	 * Disables movement input.
	 * Will check to see if character has died and will call Dead().
	 */
	UFUNCTION( BlueprintCallable, Category = Combat )
	virtual void MyTakeDamage( int Val );

	/**
	 * Sets bIsDead to true.
	 */
	UFUNCTION( BlueprintCallable, Category = Combat )
	void Dead();

	/**
	 * Enables movement input and sets bIsHurt to false.
	 */
	UFUNCTION( BlueprintCallable, Category = Combat )
	void RecoveredFromHit();

	/**
	 * Enables movement input and sets bIsRecovering to false;
	 */
	UFUNCTION( BlueprintCallable, Category = Combat )
	void FinishedRecovering();

	/**
	 * Calls FinishedAttacking to leave attacking states, disables movement, and sets was parried to true
	 */
	UFUNCTION( BlueprintCallable, Category = Combat )
	void AttackWasParried();

	/**
	 * Calculates damage when overlapped by sword.
	 */
	UFUNCTION( Category = Combat )
	void OnOverlapStart( AActor* thisActor, AActor* otherActor );
};
