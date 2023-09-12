// Fill out your copyright notice in the Description page of Project Settings.


#include "BoxTrigger.h"


UBoxTrigger::UBoxTrigger()
{
    PrimaryComponentTick.bCanEverTick = true;

}

void UBoxTrigger::BeginPlay()
{
	Super::BeginPlay();

}

void UBoxTrigger::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	AActor* Actor = AcceptKey();
	if(Actor != nullptr)
	{
		UPrimitiveComponent* Component = Cast<UPrimitiveComponent>(Actor->GetRootComponent());
		if(Component != nullptr)
		{
			Component->SetSimulatePhysics(false);
		}
		Actor->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);
		Mover->SetShouldMove(true);
	}
	else
	{
		Mover->SetShouldMove(false);

	}

}

void UBoxTrigger::SetMover(UMover *NewMover)
{
	Mover = NewMover;
}

AActor *UBoxTrigger::AcceptKey() const
{
	
	TArray<AActor*> Actors;
	
	GetOverlappingActors(Actors);

	if(Actors.Num() > 0)
	{
		for(AActor* Actor : Actors)
		{
			bool HasKey = Actor->ActorHasTag(Key);
			bool IsGrabbed = Actor->ActorHasTag("Grabbed");
			if(HasKey && !IsGrabbed)
			{
				return Actor;
			}
		}

	}

    return nullptr;
}
