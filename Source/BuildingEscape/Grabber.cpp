// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("Grabber!!"));
	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	APlayerController* player = GetWorld()->GetFirstPlayerController();
	FVector playerLocation;
	FRotator playerRotation;
	player->GetPlayerViewPoint(playerLocation, playerRotation);

	//UE_LOG(LogTemp, Warning, TEXT("Player position: %s and orientation: %s"), *playerLocation.ToString(), *playerRotation.ToString());

	FVector lineOfSight = playerLocation + playerRotation.Vector()*Reach;
	DrawDebugLine(GetWorld(), playerLocation, lineOfSight, FColor(255, 0, 0), false, 0.f, 0.f, 10.f);

}

