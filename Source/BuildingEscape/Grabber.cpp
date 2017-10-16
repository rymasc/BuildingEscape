// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Components/PrimitiveComponent.h"
// Sets default values for this component's properties
UGrabber::UGrabber() {
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGrabber::BeginPlay() {
	Super::BeginPlay();
	verifyPhysicsComponent();
	setupInputComponent();
}

void UGrabber::Grab() {
	UE_LOG(LogTemp, Warning, TEXT("Grabbed"));
	auto hitResult = GetPhysicsBody();
	auto componentToGrab = hitResult.GetComponent();
	auto actorHit = hitResult.GetActor();

	if (actorHit != nullptr) {
		if (!physicsHandle) return;
		physicsHandle->GrabComponent(componentToGrab,
			NAME_None,
			componentToGrab->GetOwner()->GetActorLocation(),
			true);
	}


}

void UGrabber::Release() {
	UE_LOG(LogTemp, Warning, TEXT("Released"));
	if (!physicsHandle) return;
	physicsHandle->ReleaseComponent();
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!physicsHandle) return;
	if (physicsHandle->GrabbedComponent) {
		physicsHandle->SetTargetLocation(getEndOfReach());
	}


}

void UGrabber::verifyPhysicsComponent() {
	physicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (physicsHandle == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("%s is missing physics handle component"), *GetOwner()->GetName());
	}

}

const FHitResult UGrabber::GetPhysicsBody() {


	FHitResult hit;

	FCollisionQueryParams traceParams(FName(TEXT("")), false, GetOwner());

	GetWorld()->LineTraceSingleByObjectType(hit,
		getPlayerLocation(),
		getEndOfReach(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		traceParams
	);

	AActor* actorHit = hit.GetActor();
	if (actorHit) {
		UE_LOG(LogTemp, Warning, TEXT("Line of Sight hit: %s"), *(actorHit->GetName()));
	}
	return hit;
}

FVector UGrabber::getEndOfReach() {
	APlayerController* player = GetWorld()->GetFirstPlayerController();
	FVector playerLocation;
	FRotator playerRotation;
	player->GetPlayerViewPoint(playerLocation, playerRotation);


	return playerLocation + playerRotation.Vector()*Reach;
}

FVector UGrabber::getPlayerLocation() {
	APlayerController* player = GetWorld()->GetFirstPlayerController();
	FVector playerLocation;
	FRotator playerRotation;
	player->GetPlayerViewPoint(playerLocation, playerRotation);
	return playerLocation;
}

void UGrabber::setupInputComponent() {
	inputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (inputComponent) {
		inputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		inputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	} else {
		UE_LOG(LogTemp, Warning, TEXT("%s is missing input component"), *GetOwner()->GetName());
	}
}

