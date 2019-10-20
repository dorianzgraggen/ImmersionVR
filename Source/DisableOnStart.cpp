// (c) Dorian Zgraggen 2019, dorianzgraggen.com


#include "DisableOnStart.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UDisableOnStart::UDisableOnStart()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UDisableOnStart::BeginPlay()
{
	Super::BeginPlay();

	// ...

	GetOwner()->Destroy();
	
}


// Called every frame
void UDisableOnStart::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

