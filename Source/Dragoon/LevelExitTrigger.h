// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "LevelExitTrigger.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DRAGOON_API ULevelExitTrigger : public UActorComponent
{
	GENERATED_BODY()

public:
	// name of level to be loaded when trigger is activated
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Level )
	FName levelName = TEXT( "Level1_TheHub" );

public:	
	// Sets default values for this component's properties
	ULevelExitTrigger();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

protected:
	// overlap funciton delegate
	UFUNCTION( BlueprintCallable, Category = Level )
	void OnOverlapStart( AActor* thisActor, AActor* otherActor );
	
};
