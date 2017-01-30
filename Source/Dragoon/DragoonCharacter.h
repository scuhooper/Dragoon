// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
#include "DragoonCharacter.generated.h"

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

enum class EAttackVertical : uint8
{
	AV_Up,
	AV_Center,
	AV_Down
};

enum class EAttackHorizontal : uint8
{
	AH_Left,
	AH_Center,
	AH_Right
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

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = Movement )
	float moveForward = 0;

	UPROPERTY( VisibleAnywhere, BlueprintReadWrite, Category = Movement )
	float moveRight = 0;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = DragoonCharacter )
	AActor* sword;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Enum )
	EAttackDirection AttackDirectrionEnum;

	EAttackVertical AttackVerticalEnum;

	EAttackHorizontal AttackHorizontalEnum;

	uint8 AttackOrientation[ 3 ][ 3 ];
private:
	bool bIsSwordDrawn = false;

	bool bIsStrongAttack = false;

	bool bIsFeintAttack = false;

	bool bIsAttacking = false;

	bool bIsGettingAttackDirection = false;

	bool bIsParrying = false;

	bool bIsDodging = false;

	FVector2D attackDirection;

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

	void BasicAttack();

	void BeginAttack();

	void MyTurn( float Val );

	void MyLookUp( float Rate );

	void SheatheUnsheatheSword();

	// Attack modifier functions
	void EnableStrongAttackModifier();

	void DisableStrongAttackModifier();

	void EnableFeintAttackModifier();

	void DisableFeintAttackModifier();

	void EnableDodging();

	void DodgeKeyReleased();

	void Parry();

	void BeginParry();

	void AttackDirectionChosen();

	uint8 DetermineAttackDirection( FVector2D vec );

	uint8 directionOfAttack = ( uint8 )EAttackDirection::AD_Thrust;

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UFUNCTION( BlueprintCallable, Category=DragoonPlayer )
	FORCEINLINE bool GetIsSwordDrawn() const { return bIsSwordDrawn; }

	UFUNCTION( BlueprintCallable, Category = DragoonPlayer )
	FORCEINLINE bool GetIsAttacking() const { return bIsAttacking; }

	UFUNCTION( BlueprintCallable, Category = DragoonPlayer )
	FORCEINLINE bool GetIsStrongAttacking() const { return bIsStrongAttack; }

	UFUNCTION( BlueprintCallable, Category = DragoonPlayer )
	FORCEINLINE bool GetIsFeintAttacking() const { return bIsFeintAttack; }

	UFUNCTION( BlueprintCallable, Category = DragoonPlayer )
	void ResetMoveFloats();

	UFUNCTION( BlueprintCallable, Category = DragoonPlayer )
	void FinishedAttacking();

	UFUNCTION( BlueprintCallable, Category = DragoonPlayer )
	void FinishedDodging();

	UFUNCTION( BlueprintCallable, Category = DragoonPlayer )
	void FinishedParrying();

	UFUNCTION( BlueprintCallable, Category = DragoonPlayer )
	FORCEINLINE uint8 GetDirectionOfAttack() const { return directionOfAttack; }

	UFUNCTION( BlueprintCallable, Category = DragoonPlayer )
	FORCEINLINE bool GetIsParrying() const { return bIsParrying; }

	UFUNCTION( BlueprintCallable, Category = DragoonPlayer )
	FORCEINLINE bool GetIsDodging() const { return bIsDodging; }
};
