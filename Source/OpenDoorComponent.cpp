// (c) Dorian Zgraggen 2019, dorianzgraggen.com


#include "OpenDoorComponent.h"
#include "GameFramework/Actor.h"
#include "Misc/App.h"
#include "Engine/World.h"
#include "GenericPlatform/GenericPlatformMath.h"

// Sets default values for this component's properties
UOpenDoorComponent::UOpenDoorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoorComponent::BeginPlay()
{
	Super::BeginPlay();

	GetOwner()->SetActorRotation(FRotator(0, 0, 0));
	
}


// Called every frame
void UOpenDoorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	FRotator CurrentRotation = GetOwner()->GetActorRotation();
	if (bShouldItRotate && FGenericPlatformMath::Abs(CurrentRotation.Yaw) < FGenericPlatformMath::Abs(GoalRotation.Yaw))
	{
		GetOwner()->SetActorRotation(CurrentRotation + RotationPerSecond * GetWorld()->GetDeltaSeconds());
	}
	else
	{
		bShouldItRotate = false;
	}

	// ...
}

void UOpenDoorComponent::RotateDoor()
{
	bShouldItRotate = true;
}
