// Fill out your copyright notice in the Description page of Project Settings.

#include "Dragoon.h"
#include "LevelExitTrigger.h"


// Sets default values for this component's properties
ULevelExitTrigger::ULevelExitTrigger()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void ULevelExitTrigger::BeginPlay()
{
	Super::BeginPlay();

	// ...
	GetOwner()->OnActorBeginOverlap.AddDynamic( this, &ULevelExitTrigger::OnOverlapStart );
}


// Called every frame
void ULevelExitTrigger::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// ...
}

void ULevelExitTrigger::OnOverlapStart( AActor* thisActor, AActor* otherActor ) {
	UGameplayStatics::OpenLevel( this, levelName );
}
