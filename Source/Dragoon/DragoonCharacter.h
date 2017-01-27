// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
#include "DragoonCharacter.generated.h"

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

private:
	bool bIsSwordDrawn = false;

	bool bIsStrongAttack = false;

	bool bIsFeintAttack = false;

	bool bIsAttacking = false;

	bool bIsGettingAttackDirection = false;

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

	void MyTurn( float Val );

	void MyLookUp( float Rate );

	void SheatheUnsheatheSword();

	// Attack modifier functions
	void EnableStrongAttackModifier();

	void DisableStrongAttackModifier();

	void EnableFeintAttackModifier();

	void DisableFeintAttackModifier();

	void AttackDirectionChosen();

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
};
