// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"

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

	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UPhysicsHandleComponent * PhysicsHandle = GetPhysicsHandle();	
	if(PhysicsHandle == nullptr)
	{
		return;
	}


	if(PhysicsHandle->GetGrabbedComponent() != nullptr)
	{
		FVector TargetLocation = GetComponentLocation() + GetForwardVector()*HoldDistance;
		PhysicsHandle->SetTargetLocationAndRotation(TargetLocation, GetComponentRotation());
	}

	
}

void UGrabber::Release()
{
	UPhysicsHandleComponent * PhysicsHandle = GetPhysicsHandle();	

	if(PhysicsHandle == nullptr)
	{
		return;
	}

	if(PhysicsHandle->GetGrabbedComponent() != nullptr)
	{
		PhysicsHandle->GetGrabbedComponent()->GetOwner()->Tags.Remove("Grabbed");
		PhysicsHandle->ReleaseComponent();
	}


}

void UGrabber::Grab()
{
	UPhysicsHandleComponent * PhysicsHandle = GetPhysicsHandle();	

	FHitResult OutHit;
	bool HasHit = GetGrabbableInReach(OutHit);
	
	if(HasHit)
	{	
		AActor* Actor = OutHit.GetActor();
		UPrimitiveComponent* HitComponent = OutHit.GetComponent();
		HitComponent->SetSimulatePhysics(true);
		HitComponent->WakeAllRigidBodies();
		Actor->Tags.Add("Grabbed");
		Actor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		PhysicsHandle->GrabComponentAtLocationWithRotation(OutHit.GetComponent(), NAME_None, OutHit.ImpactPoint, GetComponentRotation());
	}

}

UPhysicsHandleComponent *UGrabber::GetPhysicsHandle()
{
	UPhysicsHandleComponent * PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

    return PhysicsHandle;
}

bool UGrabber::GetGrabbableInReach(FHitResult &OutHit) const
{
	FVector Start = GetComponentLocation();
	FVector End = Start + GetForwardVector() * MaxDistance;
	DrawDebugLine(GetWorld(), Start, End, FColor::Red);
	DrawDebugSphere(GetWorld(), End, 10, 10, FColor::Blue, false, 5);

	FCollisionShape Sphere = FCollisionShape::MakeSphere(GrabberRadius);

	bool HasHit = GetWorld()->SweepSingleByChannel(OutHit, Start, End, FQuat::Identity, ECC_GameTraceChannel2, Sphere);

    return HasHit;
}
