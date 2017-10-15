// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"


// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	player = GetWorld()->GetFirstPlayerController()->GetPawn();
	door = GetOwner();
}

void UOpenDoor::OpenDoor()
{
	FRotator newRotation = FRotator(0.0f, openAngle, 0.0f);
	door->SetActorRotation(newRotation);
	doorOpen = true;
}

void UOpenDoor::CloseDoor()
{
	
	FRotator newRotation = FRotator(0.0f, 0, 0.0f);
	door->SetActorRotation(newRotation);
	doorOpen = false;
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (pressurePlate->IsOverlappingActor(player)) {
		OpenDoor();
		lastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}

	if (doorOpen && GetWorld()->GetTimeSeconds() - lastDoorOpenTime > DoorCloseDelay) {
		CloseDoor();
	}

	
}

